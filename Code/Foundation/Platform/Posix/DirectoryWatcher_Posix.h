#include <Foundation/FoundationInternal.h>
NS_FOUNDATION_INTERNAL_HEADER

#include <Foundation/IO/DirectoryWatcher.h>

#if __has_include(<sys/inotify.h>)
#  include <fcntl.h>
#  include <poll.h>
#  include <sys/inotify.h>

#  include <Foundation/IO/Implementation/FileSystemMirror.h>
#  include <Foundation/IO/OSFile.h>
#  include <Foundation/Logging/Log.h>

#  if NS_DISABLED(NS_SUPPORTS_FILE_ITERATORS)
#    error The directory watcher implementation needs file iterators
#  endif

// Comment in to get verbose output on the function of the directory watcher
// #  define DEBUG_FILE_WATCHER

#  ifdef DEBUG_FILE_WATCHER
#    define DEBUG_LOG(...) nsLog::Debug(__VA_ARGS__)
#  else
#    define DEBUG_LOG(...)
#  endif

namespace
{
  bool IsFile(uint32_t mask)
  {
    return (mask & IN_ISDIR) == 0;
  }

  bool IsDirectory(uint32_t mask)
  {
    return mask & IN_ISDIR;
  }

  struct MoveEvent
  {
    nsString path;
    bool isDirectory = false;
    uint32_t cookie = 0; // Two related events have the same cookie

    void Clear()
    {
      path.Clear();
      cookie = 0;
    }

    bool IsEmpty()
    {
      return path.IsEmpty();
    }
  };

  struct RenamedDirectory
  {
    nsString path;
    int wd;
  };

  using nsFileSystemMirrorType = nsFileSystemMirror<bool>;
} // namespace

#  ifndef _NS_DEFINED_POLLFD_POD
#    define _NS_DEFINED_POLLFD_POD
NS_DEFINE_AS_POD_TYPE(struct pollfd);
#  endif

struct nsDirectoryWatcherImpl
{
  nsHashTable<int, nsString> m_wdToPath;
  nsMap<nsString, int> m_pathToWd;
  nsString m_topLevelPath;

  int m_inotifyFd = -1;
  uint32_t m_inotifyWatchMask = 0;
  nsBitflags<nsDirectoryWatcher::Watch> m_whatToWatch;
  nsDynamicArray<nsUInt8> m_buffer;

  nsUniquePtr<nsFileSystemMirrorType> m_fileSystemMirror;
  // List of directories we could not watch yet.
  // This might happen if the directory was moved / renamed before we could add a watch to it.
  nsHashSet<nsString> m_pendingDirectories;

  void WatchNewDirectory(const nsStringBuilder& path, nsDirectoryWatcher::EnumerateChangesFunction& func, bool fireEvent)
  {
    nsStringBuilder tmpPath = path;
    EnsureTrailingSlash(tmpPath);

    bool directoryAlreadyExists = false;
    if (m_fileSystemMirror)
    {
      m_fileSystemMirror->AddDirectory(tmpPath, &directoryAlreadyExists).AssertSuccess();
    }

    if (m_whatToWatch.IsSet(nsDirectoryWatcher::Watch::Creates) && !directoryAlreadyExists && fireEvent)
    {
      RemoveTrailingSlash(tmpPath);
      func(tmpPath, nsDirectoryWatcherAction::Added, nsDirectoryWatcherType::Directory);
      EnsureTrailingSlash(tmpPath);
    }

    if (!m_whatToWatch.IsSet(nsDirectoryWatcher::Watch::Subdirectories))
    {
      return;
    }

    if (m_pathToWd.Contains(tmpPath))
    {
      // Already watching, skip
      return;
    }

    RemoveTrailingSlash(tmpPath);
    int wd = inotify_add_watch(m_inotifyFd, tmpPath.GetData(), m_inotifyWatchMask);
    if (wd >= 0)
    {
      DEBUG_LOG("Now watching {}", tmpPath);
      EnsureTrailingSlash(tmpPath);
      m_pathToWd.Insert(tmpPath, wd);
      m_wdToPath.Insert(wd, tmpPath);

      // Whenever we add a directory we might be "to late" to see changes inside it.
      // So iterate the file system and make sure we track all files / subdirectories
      nsFileSystemIterator subdirIt;

      subdirIt.StartSearch(tmpPath.GetData(),
        m_whatToWatch.IsSet(nsDirectoryWatcher::Watch::Subdirectories)
          ? nsFileSystemIteratorFlags::ReportFilesAndFoldersRecursive
          : nsFileSystemIteratorFlags::ReportFiles);

      nsStringBuilder tmpPath2;
      for (; subdirIt.IsValid(); subdirIt.Next())
      {
        const nsFileStats& stats = subdirIt.GetStats();
        stats.GetFullPath(tmpPath2);
        if (stats.m_bIsDirectory)
        {
          directoryAlreadyExists = false;
          if (m_fileSystemMirror)
          {
            m_fileSystemMirror->AddDirectory(tmpPath2, &directoryAlreadyExists).AssertSuccess();
          }

          if (m_whatToWatch.IsSet(nsDirectoryWatcher::Watch::Creates) && !directoryAlreadyExists)
          {
            func(tmpPath2, nsDirectoryWatcherAction::Added, nsDirectoryWatcherType::Directory);
          }

          EnsureTrailingSlash(tmpPath2);
          if (!m_pathToWd.Contains(tmpPath2))
          {
            RemoveTrailingSlash(tmpPath2);
            int wd = inotify_add_watch(m_inotifyFd, tmpPath2.GetData(), m_inotifyWatchMask);
            if (wd >= 0)
            {
              DEBUG_LOG("Now watching {}", tmpPath2);
              EnsureTrailingSlash(tmpPath2);
              m_pathToWd.Insert(tmpPath2, wd);
              m_wdToPath.Insert(wd, tmpPath2);
            }
            else
            {
              m_pendingDirectories.Insert(tmpPath2);
            }
          }
        }
        else
        {
          bool fileExistsAlready = false;
          if (m_fileSystemMirror)
          {
            m_fileSystemMirror->AddFile(tmpPath2, false, &fileExistsAlready, nullptr).AssertSuccess();
          }
          if (m_whatToWatch.IsSet(nsDirectoryWatcher::Watch::Creates) && !fileExistsAlready)
          {
            func(tmpPath2, nsDirectoryWatcherAction::Added, nsDirectoryWatcherType::File);
          }
        }
      }
    }
    else
    {
      m_pendingDirectories.Insert(tmpPath);
    }
  }
};

nsDirectoryWatcher::nsDirectoryWatcher()
  : m_pImpl(NS_DEFAULT_NEW(nsDirectoryWatcherImpl))
{
  m_pImpl->m_buffer.SetCountUninitialized(4 * 1024);
}

nsDirectoryWatcher::~nsDirectoryWatcher()
{
  CloseDirectory();
  NS_DEFAULT_DELETE(m_pImpl);
}

nsResult nsDirectoryWatcher::OpenDirectory(nsStringView sAbsolutePath, nsBitflags<Watch> whatToWatch)
{
  if (m_pImpl->m_inotifyFd >= 0)
  {
    return NS_FAILURE; // already open
  }

  m_pImpl->m_inotifyFd = inotify_init();
  if (m_pImpl->m_inotifyFd < 0)
  {
    return NS_FAILURE; // init failure
  }

  // Configure the file descriptor to be non-blocking
  int flags = fcntl(m_pImpl->m_inotifyFd, F_GETFL, 0);
  if (fcntl(m_pImpl->m_inotifyFd, F_SETFL, flags | O_NONBLOCK) != 0)
  {
    close(m_pImpl->m_inotifyFd);
    m_pImpl->m_inotifyFd = -1;
  }

  nsStringBuilder folder = sAbsolutePath;
  folder.MakeCleanPath();
  EnsureTrailingSlash(folder);

  m_pImpl->m_topLevelPath = folder;

  const int inotifyFd = m_pImpl->m_inotifyFd;

  uint32_t watchMask = IN_MOVE_SELF;
  // TODO add IN_MOVE_SELF handling


  // If we need to watch subdirectories, we always need to be notified if a subdirectory was created or deleted
  if (whatToWatch.IsSet(Watch::Subdirectories))
  {
    watchMask |= IN_CREATE | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO;
  }
  if (whatToWatch.IsSet(Watch::Creates))
  {
    watchMask |= IN_CREATE;
  }
  if (whatToWatch.IsSet(Watch::Renames))
  {
    watchMask |= IN_MOVED_FROM | IN_MOVED_TO;
  }
  if (whatToWatch.IsSet(Watch::Writes))
  {
    watchMask |= IN_CLOSE_WRITE | IN_CREATE;
  }
  if (whatToWatch.IsSet(Watch::Deletes))
  {
    watchMask |= IN_DELETE;
  }

  int wd = inotify_add_watch(inotifyFd, folder.GetData(), watchMask);
  if (wd < 0)
  {
    close(m_pImpl->m_inotifyFd);
    m_pImpl->m_inotifyFd = -1;
    return NS_FAILURE;
  }

  if ((whatToWatch.IsSet(Watch::Deletes) && whatToWatch.IsSet(Watch::Subdirectories)) || whatToWatch.IsSet(Watch::Writes))
  {
    // When a sub-folder is moved out of view. We need to trigger delete events for all files inside it.
    // Thus we need to keep a in memory copy of the file system.
    m_pImpl->m_fileSystemMirror = NS_DEFAULT_NEW(nsFileSystemMirrorType);
    m_pImpl->m_fileSystemMirror->AddDirectory(folder.GetData()).AssertSuccess();
  }

  m_pImpl->m_whatToWatch = whatToWatch;
  m_pImpl->m_inotifyWatchMask = watchMask;

  DEBUG_LOG("Now watching {}", folder);
  m_pImpl->m_wdToPath.Insert(wd, folder);
  m_pImpl->m_pathToWd.Insert(folder, wd);

  nsFileSystemIterator dirIt;
  dirIt.StartSearch(folder.GetData(), nsFileSystemIteratorFlags::ReportFoldersRecursive);
  nsStringBuilder subFolderPath;
  while (dirIt.IsValid())
  {
    dirIt.GetStats().GetFullPath(subFolderPath);
    wd = inotify_add_watch(inotifyFd, subFolderPath.GetData(), watchMask);
    if (wd >= 0)
    {
      EnsureTrailingSlash(subFolderPath);
      DEBUG_LOG("Now watching {}", subFolderPath);
      m_pImpl->m_wdToPath.Insert(wd, subFolderPath);
      m_pImpl->m_pathToWd.Insert(subFolderPath, wd);
    }
    dirIt.Next();
  }

  return NS_SUCCESS;
}

void nsDirectoryWatcher::CloseDirectory()
{
  const int inotifyFd = m_pImpl->m_inotifyFd;
  if (inotifyFd >= 0)
  {
    for (auto& paths : m_pImpl->m_wdToPath)
    {
      inotify_rm_watch(inotifyFd, paths.Key());
    }
    m_pImpl->m_inotifyFd = -1;
    close(inotifyFd);
  }
  m_pImpl->m_wdToPath.Clear();
  m_pImpl->m_pathToWd.Clear();
  m_pImpl->m_fileSystemMirror = nullptr;
}


void nsDirectoryWatcher::EnumerateChanges(EnumerateChangesFunction func, nsTime waitUpTo)
{
  const int inotifyFd = m_pImpl->m_inotifyFd;
  uint8_t* const buffer = m_pImpl->m_buffer.GetData();
  const size_t bufferSize = m_pImpl->m_buffer.GetCount();

  nsStringBuilder tmpPath;

  const nsBitflags<Watch> whatToWatch = m_pImpl->m_whatToWatch;
  nsFileSystemMirrorType* mirror = m_pImpl->m_fileSystemMirror.Borrow();

  MoveEvent lastMoveFrom;

  // An orpahend move from is when we see a move from, but no move to
  // This means the file was moved outside of our view of the file system
  // tread this as a delete.
  auto processOrphanedMoveFrom = [&](MoveEvent& moveFrom)
  {
    if (whatToWatch.IsSet(Watch::Deletes))
    {
      if (!moveFrom.isDirectory)
      {
        func(moveFrom.path.GetData(), nsDirectoryWatcherAction::Removed, nsDirectoryWatcherType::File);
        mirror->RemoveFile(moveFrom.path).AssertSuccess();
        m_pImpl->m_pendingDirectories.Remove(moveFrom.path);
      }
      else
      {
        nsStringBuilder dirPath;
        mirror->Enumerate(moveFrom.path, [&](nsStringView sPath, typename nsFileSystemMirrorType::Type type)
                {
                  if (type == nsFileSystemMirrorType::Type::File)
                  {
                    func(sPath, nsDirectoryWatcherAction::Removed, nsDirectoryWatcherType::File);
                  }
                  else
                  {
                    func(sPath, nsDirectoryWatcherAction::Removed, nsDirectoryWatcherType::Directory);
                    dirPath = sPath;
                    EnsureTrailingSlash(dirPath);
                    auto it = m_pImpl->m_pathToWd.Find(dirPath);
                    if (it.IsValid())
                    {
                      DEBUG_LOG("No longer watching {}", it.Key());
                      inotify_rm_watch(inotifyFd, it.Value());
                      m_pImpl->m_wdToPath.Remove(it.Value());
                      m_pImpl->m_pathToWd.Remove(it);
                    }
                  }
                  //
                })
          .AssertSuccess();
        mirror->RemoveDirectory(moveFrom.path).AssertSuccess();

        func(moveFrom.path, nsDirectoryWatcherAction::Removed, nsDirectoryWatcherType::Directory);

        dirPath = moveFrom.path;
        EnsureTrailingSlash(dirPath);
        auto it = m_pImpl->m_pathToWd.Find(dirPath);
        NS_ASSERT_DEBUG(it.IsValid(), "path should exist");
        if (it.IsValid())
        {
          DEBUG_LOG("No longer watching {}", it.Key());
          m_pImpl->m_wdToPath.Remove(it.Value());
          m_pImpl->m_pathToWd.Remove(it);
        }
      }
    }
    moveFrom.Clear();
  };

  if (inotifyFd >= 0)
  {
    int timeout = static_cast<int>(waitUpTo.GetMilliseconds());
    if (timeout > 0)
    {
      struct pollfd pollFor = {inotifyFd, POLLIN, 0};
      int pollResult = poll(&pollFor, 1, timeout);
      if (pollResult < 0)
      {
        // Error, stop
        nsLog::Error("Unexpected result from poll when watching directory {}", m_sDirectoryPath);
        CloseDirectory();
        return;
      }
      else if (pollResult == 0)
      {
        return; // timeout, no results
      }
    }

    ssize_t numBytesRead = 0;
    while ((numBytesRead = read(inotifyFd, buffer, bufferSize)) > 0)
    {
      size_t curPos = 0;
      while (curPos < numBytesRead)
      {
        const struct inotify_event* event = (struct inotify_event*)(buffer + curPos);

        auto it = m_pImpl->m_wdToPath.Find(event->wd);
        if (it.IsValid() && event->len > 0)
        {
          tmpPath = it.Value();
          tmpPath.AppendPath(event->name);

          const char* type = "file";
          NS_IGNORE_UNUSED(type);
          if (IsDirectory(event->mask))
          {
            type = "folder";
          }

          if (event->mask & IN_CREATE)
          {
            DEBUG_LOG("IN_CREATE {} {} {} {}", type, tmpPath, event->cookie, event->mask);

            if (IsDirectory(event->mask))
            {
              m_pImpl->WatchNewDirectory(tmpPath, func, true);
            }
            else
            {
              bool fileExistsAlready = false;
              if (mirror != nullptr)
              {
                mirror->AddFile(tmpPath, true, &fileExistsAlready, nullptr).AssertSuccess();
              }

              if (whatToWatch.IsSet(Watch::Creates) && !fileExistsAlready)
              {
                func(tmpPath.GetData(), nsDirectoryWatcherAction::Added, nsDirectoryWatcherType::File);
              }
            }
          }
          else if (event->mask & IN_CLOSE_WRITE)
          {
            DEBUG_LOG("IN_CLOSE_WRITE {} {} {} {}", type, tmpPath, event->cookie, event->mask);
            if (whatToWatch.IsSet(Watch::Writes) && IsFile(event->mask))
            {
              bool addPending = false;
              if (mirror)
              {
                mirror->AddFile(tmpPath, false, nullptr, &addPending).AssertSuccess();
              }
              if (!addPending)
              {
                func(tmpPath.GetData(), nsDirectoryWatcherAction::Modified, nsDirectoryWatcherType::File);
              }
            }
          }
          else if (event->mask & IN_DELETE)
          {
            DEBUG_LOG("IN_DELETE {} {} {}", type, tmpPath, event->cookie);

            if (IsDirectory(event->mask))
            {
              m_pImpl->m_pendingDirectories.Remove(tmpPath);

              if (mirror)
              {
                mirror->RemoveDirectory(tmpPath).AssertSuccess();
              }

              if (whatToWatch.IsSet(Watch::Deletes))
              {
                func(tmpPath.GetData(), nsDirectoryWatcherAction::Removed, nsDirectoryWatcherType::Directory);
              }

              if (whatToWatch.IsSet(Watch::Subdirectories))
              {
                EnsureTrailingSlash(tmpPath);
                auto deletedDirIt = m_pImpl->m_pathToWd.Find(tmpPath);
                if (deletedDirIt.IsValid())
                {
                  int deletedWd = deletedDirIt.Value();
                  deletedDirIt = m_pImpl->m_pathToWd.Remove(deletedDirIt);
                  inotify_rm_watch(inotifyFd, deletedWd);
                  m_pImpl->m_wdToPath.Remove(deletedWd);
                  DEBUG_LOG("No longer watching {}", tmpPath);
                }
              }
            }
            else
            {
              if (mirror)
              {
                mirror->RemoveFile(tmpPath).AssertSuccess();
              }

              if (whatToWatch.IsSet(Watch::Deletes))
              {
                func(tmpPath.GetData(), nsDirectoryWatcherAction::Removed, nsDirectoryWatcherType::File);
              }
            }
          }
          else if (event->mask & IN_MOVED_FROM)
          {
            DEBUG_LOG("IN_MOVED_FROM {} {} {}", type, tmpPath, event->cookie);

            if (!lastMoveFrom.IsEmpty())
            {
              processOrphanedMoveFrom(lastMoveFrom);
            }

            lastMoveFrom.path = tmpPath;
            lastMoveFrom.cookie = event->cookie;
            lastMoveFrom.isDirectory = IsDirectory(event->mask);
          }
          else if (event->mask & IN_MOVED_TO)
          {
            DEBUG_LOG("IN_MOVED_TO {} {} {}", type, tmpPath, event->cookie);

            if (!lastMoveFrom.IsEmpty() && lastMoveFrom.cookie != event->cookie)
            {
              // orphaned move from
              processOrphanedMoveFrom(lastMoveFrom);
            }

            if (lastMoveFrom.IsEmpty())
            {
              // Orphaned move to, treat as add
              if (IsFile(event->mask))
              {
                bool fileAlreadyExists = false;
                if (mirror)
                {
                  mirror->AddFile(tmpPath, false, &fileAlreadyExists, nullptr).AssertSuccess();
                }

                if (whatToWatch.IsSet(Watch::Creates) && !fileAlreadyExists)
                {
                  func(tmpPath.GetData(), nsDirectoryWatcherAction::Added, nsDirectoryWatcherType::File);
                }
              }
              else
              {
                m_pImpl->WatchNewDirectory(tmpPath, func, true);
              }
            }
            else
            {
              // regular move
              if (whatToWatch.IsSet(Watch::Renames))
              {
                func(lastMoveFrom.path.GetData(), nsDirectoryWatcherAction::RenamedOldName, IsFile(event->mask) ? nsDirectoryWatcherType::File : nsDirectoryWatcherType::Directory);
                func(tmpPath.GetData(), nsDirectoryWatcherAction::RenamedNewName, IsFile(event->mask) ? nsDirectoryWatcherType::File : nsDirectoryWatcherType::Directory);
              }

              if (IsDirectory(event->mask))
              {
                if (m_pImpl->m_pendingDirectories.Contains(lastMoveFrom.path))
                {
                  m_pImpl->m_pendingDirectories.Remove(lastMoveFrom.path);
                  m_pImpl->WatchNewDirectory(tmpPath, func, false);
                }
                else
                {
                  nsStringBuilder moveFromPath = lastMoveFrom.path;
                  EnsureTrailingSlash(moveFromPath);
                  EnsureTrailingSlash(tmpPath);

                  nsDynamicArray<RenamedDirectory> renamedDirectories;
                  for (auto it = m_pImpl->m_pathToWd.GetIterator(); it.IsValid();)
                  {
                    if (it.Key().StartsWith(moveFromPath))
                    {
                      m_pImpl->m_wdToPath.Remove(it.Value());
                      nsStringBuilder fixedPath = it.Key();
                      fixedPath.Shrink(moveFromPath.GetCharacterCount(), 0);
                      fixedPath.Prepend(tmpPath);
                      renamedDirectories.PushBack({fixedPath, it.Value()});
                      it = m_pImpl->m_pathToWd.Remove(it);
                    }
                    else
                    {
                      it.Next();
                    }
                  }

                  for (auto& renamedDirectory : renamedDirectories)
                  {
                    m_pImpl->m_pathToWd.Insert(renamedDirectory.path, renamedDirectory.wd);
                    m_pImpl->m_wdToPath.Insert(renamedDirectory.wd, std::move(renamedDirectory.path));
                  }
                }
              }

              if (mirror)
              {
                if (IsFile(event->mask))
                {
                  mirror->RemoveFile(lastMoveFrom.path).AssertSuccess();
                  mirror->AddFile(tmpPath, false, nullptr, nullptr).AssertSuccess();
                }
                else
                {
                  mirror->MoveDirectory(lastMoveFrom.path, tmpPath).AssertSuccess();
                }
              }
              lastMoveFrom.Clear();
            }
          }
        }
        else
        {
          DEBUG_LOG("UNKNOWN {}", event->mask);
        }

        curPos += sizeof(struct inotify_event) + event->len;
      }
    }
  }

  if (!lastMoveFrom.IsEmpty())
  {
    processOrphanedMoveFrom(lastMoveFrom);
  }
}

void nsDirectoryWatcher::EnumerateChanges(nsArrayPtr<nsDirectoryWatcher*> watchers, EnumerateChangesFunction func, nsTime waitUpTo)
{
  int timeout = static_cast<int>(waitUpTo.GetMilliseconds());
  if (timeout > 0)
  {
    nsHybridArray<struct pollfd, 16> pollFor;
    pollFor.SetCount(watchers.GetCount());

    for (nsUInt32 i = 0; i < watchers.GetCount(); ++i)
    {
      nsDirectoryWatcher* curWatcher = watchers[i];
      pollFor[i] = {curWatcher->m_pImpl->m_inotifyFd, POLLIN, 0};
    }


    int pollResult = poll(pollFor.GetData(), pollFor.GetCount(), timeout);
    if (pollResult < 0)
    {
      // Error, stop
      nsLog::Error("Unexpected result from poll enumerating multiple watchers");
      return;
    }
    else if (pollResult == 0)
    {
      return; // timeout, no results
    }
  }

  for (nsDirectoryWatcher* watcher : watchers)
  {
    watcher->EnumerateChanges(func);
  }
}

#  undef DEBUG_LOG

#else // <sys/inotify.h> is missing
struct nsDirectoryWatcherImpl
{
};

nsDirectoryWatcher::nsDirectoryWatcher()
  : m_pImpl(nullptr)
{
}

nsResult nsDirectoryWatcher::OpenDirectory(const nsString& path, nsBitflags<Watch> whatToWatch)
{
  return NS_FAILURE;
}

void nsDirectoryWatcher::CloseDirectory()
{
}

nsDirectoryWatcher::~nsDirectoryWatcher()
{
}

void nsDirectoryWatcher::EnumerateChanges(EnumerateChangesFunction func, nsTime waitUpTo)
{
  nsLog::Warning("nsDirectoryWatcher not supported on this Linux system");
}

void nsDirectoryWatcher::EnumerateChanges(nsArrayPtr<nsDirectoryWatcher*> watchers, EnumerateChangesFunction func, nsTime waitUpTo)
{
  nsLog::Warning("nsDirectoryWatcher not supported on this Linux system");
}

#endif

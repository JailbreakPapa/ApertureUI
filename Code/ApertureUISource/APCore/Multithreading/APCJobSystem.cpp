#include <APCore/Multithreading/APCJobSystem.h>

#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <utility>

namespace aperture::core::threading
{
  void APCJobSystem::InitializeJobSystem(const APCJobSystemConfig& p_config)
  {
    m_MaxThreads = p_config.m_Composition_threadcount + p_config.m_Script_threadcount +
                   p_config.m_Rendering_threadcount + p_config.m_Parsing_threadcount;

    m_ActiveThreads = 0;

    // Initialize specific thread types
    CreateTypeThread(core::Runtype::FreeThread_Composition, p_config.m_Composition_threadcount);
    CreateTypeThread(core::Runtype::FreeThread_Scripting, p_config.m_Script_threadcount);
    CreateTypeThread(core::Runtype::FreeThread_Rendering, p_config.m_Rendering_threadcount);
    CreateTypeThread(core::Runtype::FreeThread_Layout, p_config.m_Parsing_threadcount);

    if (p_config.m_allowCreationOfNewThreadsOnOverfill)
    {
      nsLog::Info("Dynamic thread creation enabled.");
      m_bAllowCreationOfNewThreadsOnOverfill = true;
    }
  }

  void APCJobSystem::JoinAllThreads()
  {
    // Wait for all threads to complete
    Wait();
    nsLog::Info("All threads joined.");
  }

  nsResult APCJobSystem::CancelJob(nsUuid p_jobid, const IAPCCommandQueue& p_queue)
  {
    // Cancel a specific job within a queue
    if (p_queue.m_mutex.IsLocked())
    {
      // #34: (Mikael A.): Add Unique ID Names to Queues, would help with debugging.
      nsLog::Error("Cannot cancel job {0} in queue with Type: {1}. Queue is locked.", p_jobid.ToString(), CommandTypeToString(p_queue.GetType()));
      return NS_FAILURE;
    }
    for (auto& job : m_CompositionQueues)
    {
      if (job.first == p_jobid && job.second == &p_queue)
      {
        if (job.second->m_mutex.IsLocked())
        {
          nsLog::Error("Cannot cancel job {0} in queue of Type: {1}. Job is locked.", p_jobid.ToString(), CommandTypeToString(p_queue.GetType()));
          return NS_FAILURE;
        }
        m_CompositionQueues.RemoveAndSwap(job);
        nsLog::Info("Sub Job {0} canceled.", p_jobid);
      }
    }
    nsLog::Info("Job Queue {0} canceled.", p_jobid);
    return NS_SUCCESS;
  }

  nsResult APCJobSystem::CancelJobGroup(const CommandGroup& p_group)
  {
    // Cancel all jobs in a CommandGroup
    for (auto queue : p_group.m_CommandQueues)
    {
      for (auto& job : m_CompositionQueues)
      {
        if (job.second == queue)
        {
          return CancelJob(job.first, *queue);
        }
      }
    }
    nsLog::Info("All jobs in group {0} canceled.", p_group.m_sGroupName);
    return NS_SUCCESS;
  }

  nsResult APCJobSystem::CancelAllJobs()
  {
    // Cancel all jobs in the system
    m_CompositionQueues.Clear();
    m_ScriptQueues.Clear();
    m_RenderingQueues.Clear();
    m_ParsingQueues.Clear();
    nsLog::Dev("All jobs canceled.");
    return NS_SUCCESS;
  }

  void APCJobSystem::CreateTypeThread(const core::Runtype& p_runtype, nsUInt8 p_threadcount)
  {
    switch (p_runtype)
    {
      case core::Runtype::FreeThread_Composition:
        for (nsUInt8 i = 0; i < p_threadcount; ++i)
        {
          std::thread m_r_CompositionThread([&]()
            {
            m_ActiveThreads++;
            m_ActiveCompositionThreads++;
            while (true)
            {
              if (m_CompositionQueues.IsEmpty() != true)
              {
                for (auto& job : m_CompositionQueues)
                {
                  if (job.second->Execute() == NS_FAILURE)
                  {
                    nsLog::Error("Job of Type: {0} failed to execute.", CommandTypeToString(job.second->GetType()));
                    continue;
                  }
                  m_CompositionQueues.PopBack();
                }
              }
              else
              {
                std::unique_lock<std::mutex> lock(wakeMutex);
                wakeCondition.wait(lock);
              }
            } });
          m_CompositionThreads.PushBack(&m_r_CompositionThread);
          m_r_CompositionThread.detach();
        }
        break;
      case core::Runtype::FreeThread_Scripting:
        for (nsUInt8 i = 0; i < p_threadcount; ++i)
        {
          std::thread m_r_ScriptThread([&]()
            {
            m_ActiveThreads++;
            m_ActiveScriptThreads++;
            while (true)
            {
              if (m_ScriptQueues.IsEmpty() != true)
              {
                for (auto& job : m_ScriptQueues)
                {
                  if (job.second->Execute() == NS_FAILURE)
                  {
                    nsLog::Error("Job of Type: {0} failed to execute.", CommandTypeToString(job.second->GetType()));
                    continue;
                  }
                  m_ScriptQueues.PopBack();
                }
              }
              else
              {
                std::unique_lock<std::mutex> lock(wakeMutex);
                wakeCondition.wait(lock);
              }
            } });
          m_ScriptThreads.PushBack(&m_r_ScriptThread);
          m_r_ScriptThread.detach();
        }
        break;
      case core::Runtype::FreeThread_Rendering:
        for (nsUInt8 i = 0; i < p_threadcount; ++i)
        {
          std::thread m_r_RenderingThread([&]()
            {
            m_ActiveThreads++;
            m_ActiveRenderingThreads++;
            while (true)
            {
              if (m_RenderingQueues.IsEmpty() != true)
              {
                for (auto& job : m_RenderingQueues)
                {
                  if (job.second->Execute() == NS_FAILURE)
                  {
                    nsLog::Error("Job of Type: {0} failed to execute.", CommandTypeToString(job.second->GetType()));
                    continue;
                  }
                  m_RenderingQueues.PopBack();
                }
              }
              else
              {
                std::unique_lock<std::mutex> lock(wakeMutex);
                wakeCondition.wait(lock);
              }
            } });
          m_RenderingThreads.PushBack(&m_r_RenderingThread);
          m_r_RenderingThread.detach();
        }
        break;
      case core::Runtype::FreeThread_Layout:
        for (nsUInt8 i = 0; i < p_threadcount; ++i)
        {
          std::thread m_r_ParsingThread([&]()
            {
            m_ActiveThreads++;
            m_ActiveParsingThreads++;
            while (true)
            {
              if (m_ParsingQueues.IsEmpty() != true)
              {
                for (auto& job : m_ParsingQueues)
                {
                  if (job.second->Execute() == NS_FAILURE)
                  {
                    nsLog::Error("Job of Type: {0} failed to execute.", CommandTypeToString(job.second->GetType()));
                    continue;
                  }
                  m_ParsingQueues.PopBack();
                }
              }
              else
              {
                std::unique_lock<std::mutex> lock(wakeMutex);
                wakeCondition.wait(lock);
              }
            } });
          std::unique_ptr<std::thread> a = std::make_unique<std::thread>();
          m_ParsingThreads.PushBack(a.get());
          m_r_ParsingThread.detach();
        }
        break;
      default:
        nsLog::Error("Invalid runtype: {0}.", static_cast<int>(p_runtype));
        break;
    }
  }

  bool APCJobSystem::IsJobRunning(nsUuid p_jobid, const IAPCCommandQueue& p_queue)
  {
    // Check if a specific job is running
    for (const auto& job : m_CompositionQueues)
    {
      if (job.first == p_jobid && job.second == &p_queue)
      {
        return true;
      }
    }
    return false;
  }

  bool APCJobSystem::IsJobGroupRunning(const CommandGroup& p_group)
  {
    // Check if any job in the group is running
    for (auto queue : p_group.m_CommandQueues)
    {
      for (const auto& job : m_CompositionQueues)
      {
        if (job.second == queue)
        {
          return true;
        }
      }
    }
    return false;
  }

  void APCJobSystem::Wait()
  {
    while (this->m_ActiveThreads.load() > 0)
    {
      SafePoll();
    }
  }

  void APCJobSystem::Shutdown()
  {
    // Terminate all threads
    NS_ASSERT_DEBUG(CancelAllJobs() == NS_FAILURE, "APCJobSystem: Failed to shutdown a job. forcing advance.");
    m_MaxThreads = 0;
    nsLog::Info("Job system shut down.");
  }

  void APCJobSystem::SafePoll()
  {
    wakeCondition.notify_one(); // wake one worker thread
    std::this_thread::yield();  // allow this thread to be rescheduled
  }

  nsUInt8 APCJobSystem::ActiveThreads() const
  {
    return m_ActiveThreads.load();
  }

  nsUInt8 APCJobSystem::MaxThreads() const
  {
    return m_MaxThreads.load();
  }

  nsUInt8 APCJobSystem::QueuedJobs() const
  {
    return m_CompositionQueues.GetCount() + m_ScriptQueues.GetCount() +
           m_RenderingQueues.GetCount() + m_ParsingQueues.GetCount();
  }

  nsUInt8 APCJobSystem::ActiveCompositionThreads() const
  {
    return m_ActiveCompositionThreads.load();
  }

  nsUInt8 APCJobSystem::ActiveScriptThreads() const
  {
    return m_ActiveScriptThreads.load();
  }

  nsUInt8 APCJobSystem::ActiveRenderingThreads() const
  {
    return m_ActiveRenderingThreads.load();
  }

  nsUInt8 APCJobSystem::ActiveParsingThreads() const
  {
    return m_ActiveParsingThreads.load();
  }

  void APCJobSystem::AddCommandGroup(const CommandGroup& p_group)
  {
    m_CommandGroups.PushBack(p_group);
    nsLog::Info("Added command group: {0}", p_group.m_sGroupName);
  }
  void APCJobSystem::RunGeneral()
  {
    // Run the job system
    for (auto& job : m_CompositionQueues)
    {
      if (job.second->Execute() == NS_FAILURE)
      {
        nsLog::Error("Job of Type: {0} failed to execute.", CommandTypeToString(job.second->GetType()));
        continue;
      }
    }
    for (auto& job : m_ScriptQueues)
    {
      if (job.second->Execute() == NS_FAILURE)
      {
        nsLog::Error("Job of Type: {0} failed to execute.", CommandTypeToString(job.second->GetType()));
        continue;
      }
    }
    for (auto& job : m_RenderingQueues)
    {
      if (job.second->Execute() == NS_FAILURE)
      {
        nsLog::Error("Job of Type: {0} failed to execute.", CommandTypeToString(job.second->GetType()));
        continue;
      }
    }
    for (auto& job : m_ParsingQueues)
    {
      if (job.second->Execute() == NS_FAILURE)
      {
        nsLog::Error("Job of Type: {0} failed to execute. continuing.", CommandTypeToString(job.second->GetType()));
        continue;
      }
    }
    nsLog::Info("Job system running.");
  }
  void APCJobSystem::RunThreadsOfType(const core::CommandType& p_runtype)
  {
    nsHybridArray<std::thread*, 1> m_r_TypeThreads;
    if (m_bAllowCreationOfNewThreadsOnOverfill)
    {
      switch (p_runtype)
      {
        case core::CommandType::Composition:
          m_r_TypeThreads = m_CompositionThreads;
          break;
        case core::CommandType::Scripting:
          m_r_TypeThreads = m_ScriptThreads;
          break;
        case core::CommandType::Rendering:
          m_r_TypeThreads = m_RenderingThreads;
          break;
        case core::CommandType::Layout:
          m_r_TypeThreads = m_ParsingThreads;
          break;
        default:
          nsLog::Error("Invalid command type: {0}.", static_cast<int>(p_runtype));
          return;
      }
      for (auto& thread : m_r_TypeThreads)
      {
        if (thread->joinable())
        {
          thread->join();
        }
        else
        {
          nsLog::Error("Local Thread not joinable.");
          continue;
        }
        std::thread m_r_OverfillThread([&]()
          {
          nsLog::BroadcastLoggingEvent(nullptr, nsLogMsgType::WarningMsg, "ALERT: Overfill thread Created! Attempting to run overflowed tasks!");
          RunGeneral(); });
      }
    }
    else
    {
      nsLog::Warning("Dynamic thread creation disabled. Running All Jobs To Complete Overruled ones");
    }
  }

  void APCJobSystem::RunThreadsOfType(const core::Runtype& p_runtype)
  {
    nsHybridArray<std::thread*, 1> m_r_TypeThreads;
    if (m_bAllowCreationOfNewThreadsOnOverfill)
    {
      switch (p_runtype)
      {
        case core::Runtype::FreeThread_Composition:
          m_r_TypeThreads = m_CompositionThreads;
        case core::Runtype::FreeThread_Scripting:
          m_r_TypeThreads = m_ScriptThreads;
        case core::Runtype::FreeThread_Rendering:
          m_r_TypeThreads = m_RenderingThreads;
        case core::Runtype::FreeThread_Layout:
          m_r_TypeThreads = m_ParsingThreads;
        default:
          for (auto& thread : m_r_TypeThreads)
          {
            if (thread->joinable())
            {
              thread->join();
            }
            else
            {
              nsLog::Error("Local Thread not joinable.");
              continue;
            }
            std::thread m_r_OverfillThread([&]()
              {
                nsLog::BroadcastLoggingEvent(nullptr, nsLogMsgType::WarningMsg, "ALERT: Overfill thread Created! Attempting to run overflowed tasks!");
                RunGeneral(); });
          }
          break;
      }
    }
    else
    {
      nsLog::Warning("Dynamic thread creation disabled. Running All Jobs To Complete Overruled ones");
    }
  }
  void APCJobSystem::AddJob(const IAPCCommandQueue& p_uJob)
  {
    // Add a job to the system
    if (p_uJob.m_mutex.IsLocked())
    {
      nsLog::Error("Cannot add job to queue with Type: {0}. Queue is locked.", CommandTypeToString(p_uJob.GetType()));
      return;
    }
    nsUuid sUuid = sUuid.MakeUuid();
    switch (p_uJob.GetType())
    {
      case CommandType::Composition:
        m_CompositionQueues.PushBack(std::make_pair(sUuid, (IAPCCommandQueue*)&p_uJob));
        break;
      case CommandType::Scripting:
        m_ScriptQueues.PushBack(std::make_pair(sUuid, (IAPCCommandQueue*)&p_uJob));
        break;
      case CommandType::Rendering:
        m_RenderingQueues.PushBack(std::make_pair(sUuid, (IAPCCommandQueue*)&p_uJob));
        break;
      case aperture::core::CommandType::Layout:
        m_ParsingQueues.PushBack(std::make_pair(sUuid, (IAPCCommandQueue*)&p_uJob));
        break;
      default:
        nsLog::Error("Cannot add job to queue with Type: {0}. Queue is locked.", CommandTypeToString(p_uJob.GetType()));
        break;
    }
    nsLog::Info("Job added to queue with Type: {0}.", CommandTypeToString(p_uJob.GetType()));
  }
} // namespace aperture::core::threading

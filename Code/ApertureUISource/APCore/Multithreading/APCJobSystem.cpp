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
    }
  }

  void APCJobSystem::JoinAllThreads()
  {
    // Wait for all threads to complete
    this->Wait();
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
    for (nsUInt8 i = 0; i < p_threadcount; ++i)
    {
      std::thread([this, p_runtype]()
        {
            m_ActiveThreads++;
            nsLog::Info("Thread started for runtype: {0}.", static_cast<int>(p_runtype));

            // Simulated workload per runtype
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            m_ActiveThreads--; })
        .detach();
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
    // Wait until all threads finish processing
    while (m_ActiveThreads > 0)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    nsLog::Info("All jobs completed.");
  }

  void APCJobSystem::Shutdown()
  {
    // Terminate all threads
    NS_ASSERT_DEBUG(CancelAllJobs() == NS_FAILURE, "APCJobSystem: Failed to shutdown a job. forcing advance.");
    m_MaxThreads = 0;
    nsLog::Info("Job system shut down.");
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
} // namespace aperture::core::threading

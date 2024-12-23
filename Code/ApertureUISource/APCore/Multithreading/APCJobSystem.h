/*
This code is part of Aperture UI - A HTML/CSS/JS UI Middleware

Copyright (c) 2020-2024 WD Studios L.L.C. and/or its licensors. All
rights reserved in all media.

The coded instructions, statements, computer programs, and/or related
material (collectively the "Data") in these files contain confidential
and unpublished information proprietary WD Studios and/or its
licensors, which is protected by United States of America federal
copyright law and by international treaties.

This software or source code is supplied under the terms of a license
agreement and nondisclosure agreement with WD Studios L.L.C. and may
not be copied, disclosed, or exploited except in accordance with the
terms of that agreement. The Data may not be disclosed or distributed to
third parties, in whole or in part, without the prior written consent of
WD Studios L.L.C..

WD STUDIOS MAKES NO REPRESENTATION ABOUT THE SUITABILITY OF THIS
SOURCE CODE FOR ANY PURPOSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER, ITS AFFILIATES,
PARENT COMPANIES, LICENSORS, SUPPLIERS, OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OR PERFORMANCE OF THIS SOFTWARE OR SOURCE CODE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include <APCore/CommandExecutor/IAPCCommandQueue.h>
#include <APHTML/APEngineCommonIncludes.h>

namespace aperture::core::threading
{
  /**
   * @brief Represents a group of commands to be executed by the job system.
   *
   * The CommandGroup structure is used to group multiple command queues under a single name and runtype.
   * This allows for better organization and management of related command queues.
   *
   * @note This is usally automatically managed by the APCJobSystem class, but this can be used for manual management.
   * @note An CommandGroup is great for batched/massive operations that need to be executed, like script compilation & caching on first load, a View that has alot of elements that need to be updated, etc.
   */
  struct NS_COREAPENGINE_DLL CommandGroup final
  {
    /// @brief Is this CommandGroup required to run at startup?
    bool m_bRequired = false;
    nsString m_sGroupName;
    core::Runtype m_runtype = core::Runtype::AnyThread;
    nsHybridArray<IAPCCommandQueue*, 1> m_CommandQueues;
    nsUuid m_GroupID;

    CommandGroup() {
        m_GroupID = m_GroupID.MakeUuid();
    }

    bool operator==(const CommandGroup& p_other) const
    {
      return m_sGroupName == p_other.m_sGroupName && m_runtype == p_other.m_runtype && m_CommandQueues == p_other.m_CommandQueues && m_bRequired == p_other.m_bRequired && m_GroupID == p_other.m_GroupID;
    }
    bool operator!=(const CommandGroup& p_other) const
    {
      return !(*this == p_other);
    }
    bool operator==(CommandGroup* p_other) const
    {
      return m_sGroupName == p_other->m_sGroupName && m_runtype == p_other->m_runtype && m_CommandQueues == p_other->m_CommandQueues && m_bRequired == p_other->m_bRequired;
    }
  };

  struct NS_COREAPENGINE_DLL APCJobSystemConfig
  {
    nsUInt8 m_Composition_threadcount = 0;
    nsUInt8 m_Script_threadcount = 0;
    nsUInt8 m_Rendering_threadcount = 0;
    nsUInt8 m_Parsing_threadcount = 0;
    bool m_allowCreationOfNewThreadsOnOverfill = false;
  };
  /*
   * @class APCJobSystem
   * @brief An custom made Job System for the Aperture SDK.
   * @note The Job System can take both CommandQueues, and Raw Functions if needed.
   * It is also possible for Queued Job to be executed by the User Directly.
   */
  class NS_COREAPENGINE_DLL APCJobSystem
  {
  public:
    /// @brief Initializes the Job System with a set amount of threads.
    /// @param p_threadcount The amount of threads to use for the Job System.
    /// @param p_allowCreationOfNewThreadsOnOverfill If the Job System should create new threads if the Job Queue is full.
    void InitializeJobSystem(const APCJobSystemConfig& p_config);

    /// @brief Runs All Jobs in the Job System. This is meant for FreeThreads, that are not locked to a specific type of work.
		void RunGeneral();

    void RunThreadsOfType(const core::CommandType& p_runtype);
    void RunThreadsOfType(const core::Runtype& p_runtype);
    /**
     * @brief Waits for all threads to complete their tasks.
     */
    void JoinAllThreads();

    /**
     * @brief Cancels a specific job.
     * @param p_jobid The ID of the job to cancel.
     * @param p_queue The command queue where the job is located.
     * @return Result of the cancellation operation.
     */
    nsResult CancelJob(nsUuid p_jobid, const IAPCCommandQueue& p_queue);

    /**
     * @brief Cancels all jobs in a specific command group.
     * @param p_group The command group whose jobs should be canceled.
     * @return Result of the cancellation operation.
     */
    nsResult CancelJobGroup(const CommandGroup& p_group);

    /**
     * @brief Cancels all jobs in the job system.
     * @return Result of the cancellation operation.
     */
    nsResult CancelAllJobs();

    /**
     * @brief Adds an object to the lifetime management system.
     *
     * This function adds an object to the internal list of objects whose lifetime
     * is managed by the job system. The object will be kept alive as long as the
     * thread (or set of threads) is running.
     *
     * @tparam T The type of the object to add.
     * @param p_object Pointer to the object to add.
     * @warning Lifetime objects are not managed by the job system for thread-safety. Make sure to lock the object as needed.
     * @note Lifetime objects are useful for events like creating an v8 Isolate per Script thread, to make Multithreading work with the engine easier.
     */
    template <typename T>
    void AddLifetimeObject(T* p_object)
    {
      std::scoped_lock<std::mutex> lock(wakeMutex);
      m_LifetimeObjects.PushBack(p_object);
    }

    /**
     * @brief Creates a specific type of thread.
     * @param p_runtype The type of runtime for the thread.
     * @param p_threadcount The number of threads to create.
     */
    void CreateTypeThread(const core::Runtype& p_runtype, nsUInt8 p_threadcount);

    /**
     * @brief Checks if a specific job is currently running.
     * @param p_jobid The ID of the job to check.
     * @param p_queue The command queue where the job is located.
     * @return True if the job is running, false otherwise.
     */
    bool IsJobRunning(nsUuid p_jobid, const IAPCCommandQueue& p_queue);

    /**
     * @brief Checks if any job in a specific command group is currently running.
     * @param p_group The command group to check.
     * @return True if any job in the group is running, false otherwise.
     */
    bool IsJobGroupRunning(const CommandGroup& p_group);

    /**
     * @brief Waits for all jobs to complete.
     */
    void Wait();

    /**
     * @brief Shuts down the job system, terminating all threads.
     */
    void Shutdown();
    /// @brief Safely Polls the Job System. This Prevents Deadlocks.
    void SafePoll();
    /**
     * @brief Gets the number of active threads.
     * @return The number of active threads.
     */
    nsUInt8 ActiveThreads() const;

    /**
     * @brief Gets the maximum number of threads allowed.
     * @return The maximum number of threads.
     */
    nsUInt8 MaxThreads() const;

    /**
     * @brief Gets the number of jobs currently queued.
     * @return The number of queued jobs.
     */
    nsUInt8 QueuedJobs() const;

    /**
     * @brief Gets the number of active composition threads.
     * @return The number of active composition threads.
     */
    nsUInt8 ActiveCompositionThreads() const;

    /**
     * @brief Gets the number of active script threads.
     * @return The number of active script threads.
     */
    nsUInt8 ActiveScriptThreads() const;

    /**
     * @brief Gets the number of active rendering threads.
     * @return The number of active rendering threads.
     */
    nsUInt8 ActiveRenderingThreads() const;

    /**
     * @brief Gets the number of active parsing threads.
     * @return The number of active parsing threads.
     */
    nsUInt8 ActiveParsingThreads() const;

    /**
     * @brief Adds a command group to the job system.
     * @param p_group The command group to add.
     */
    void AddCommandGroup(const CommandGroup& p_group);

		void AddJob(const IAPCCommandQueue& p_uJob);
  protected:
    /// @brief The list of GENERAL lifetime objects managed by the job system.
    template <typename T>
    static nsHybridArray<T, 1> m_LifetimeObjects;

    template <typename T>
    static nsHybridArray<T, 1> m_ScriptLifetimeObjects;

    template <typename T>
    static nsHybridArray<T, 1> m_RenderingLifetimeObjects;

    template <typename T>
    static nsHybridArray<T, 1> m_ParsingLifetimeObjects;

    std::mutex wakeMutex;
    std::condition_variable wakeCondition;
    nsDeque<CommandGroup> m_CommandGroups;
    nsDeque<std::pair<nsUuid, IAPCCommandQueue*>> m_CompositionQueues;
    nsDeque<std::pair<nsUuid, IAPCCommandQueue*>> m_ScriptQueues;
    nsDeque<std::pair<nsUuid, IAPCCommandQueue*>> m_RenderingQueues;
    nsDeque<std::pair<nsUuid, IAPCCommandQueue*>> m_ParsingQueues;

    nsHybridArray<std::thread*, 1> m_CompositionThreads;
    nsHybridArray<std::thread*, 1> m_ScriptThreads;
    nsHybridArray<std::thread*, 1> m_RenderingThreads;
    nsHybridArray<std::thread*, 1> m_ParsingThreads;

    bool m_bAllowCreationOfNewThreadsOnOverfill = false;
    std::atomic<nsUInt8> m_ActiveThreads;
    std::atomic<nsUInt8> m_MaxThreads;
    std::atomic<nsUInt8> m_ActiveCompositionThreads;
    std::atomic<nsUInt8> m_ActiveScriptThreads;
    std::atomic<nsUInt8> m_ActiveRenderingThreads;
    std::atomic<nsUInt8> m_ActiveParsingThreads;
  };
} // namespace aperture::core::threading

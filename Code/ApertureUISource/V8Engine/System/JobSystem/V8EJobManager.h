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

#include <APCore/Multithreading/APCJobSystem.h>
#include <V8Engine/System/Utils/Multithreading/V8EThreadSafeIsolate.h>
#include <V8Engine/V8EngineDLL.h>
#include <libplatform/libplatform.h>

namespace aperture::v8::jobsystem
{
  static NS_ALWAYS_INLINE std::function<void()> CreateFunctionFromTask(::v8::Task* task)
  {
    return [task = std::move(task)]()
    { task->Run(); };
  }
  class V8EWorkerTaskRunner : public ::v8::TaskRunner
  {
  public:
    using TimeFunction = nsTime (*)();
    V8EWorkerTaskRunner(nsUInt32 p_iThreadPoolSize, TimeFunction p_uTimeFunction);
    /**
     * Returns true if idle tasks are enabled for this TaskRunner.
     */
    virtual bool IdleTasksEnabled() override;

    /**
     * Returns true if non-nestable tasks are enabled for this TaskRunner.
     */
    virtual bool NonNestableTasksEnabled() const override;

    /**
     * Returns true if non-nestable delayed tasks are enabled for this TaskRunner.
     */
    virtual bool NonNestableDelayedTasksEnabled() const override;

    ///============= Post Task Impl =============
    virtual void PostTaskImpl(std::unique_ptr<::v8::Task> task,
      const ::v8::SourceLocation& location) override;
    virtual void PostNonNestableTaskImpl(std::unique_ptr<::v8::Task> task,
      const ::v8::SourceLocation& location) override;
    virtual void PostDelayedTaskImpl(std::unique_ptr<::v8::Task> task,
      double delay_in_seconds,
      const ::v8::SourceLocation& location) override;
    virtual void PostNonNestableDelayedTaskImpl(std::unique_ptr<::v8::Task> task,
      double delay_in_seconds,
      const ::v8::SourceLocation& location) override;
    virtual void PostIdleTaskImpl(std::unique_ptr<::v8::IdleTask> task,
      const ::v8::SourceLocation& location) override;

  private:
    class V8EWorkerThread
    {
      NS_DISALLOW_COPY_AND_ASSIGN(V8EWorkerThread);

    public:
      explicit V8EWorkerThread(V8EWorkerTaskRunner* runner);
      ~V8EWorkerThread();
      // This thread attempts to get tasks in a loop from |runner_| and run them.
      void Run();

      void RenameWorkerThread(const char* p_iWorkerThread);

      const char* GetWorkerThreadName();

    private:
      nsString m_sThreadName;
      V8EWorkerTaskRunner* runner_;
    };
    nsHybridArray<nsUniquePtr<V8EWorkerThread>, 2> thread_pool;
  };
  class V8EPlatform : public ::v8::Platform
  {
  public:
    explicit V8EPlatform(int thread_pool_size, ::v8::platform::IdleTaskSupport idle_task_support, ::v8::platform::InProcessStackDumping in_process_stack_dumping, std::unique_ptr<::v8::TracingController> tracing_controller, ::v8::platform::PriorityMode priority_mode);

  private:
    nsMap<V8EThreadSafeIsolate*, std::shared_ptr<V8EWorkerTaskRunner>> m_workerTaskRunners;
    nsUniquePtr<V8EWorkerTaskRunner> m_workerTaskRunner;
    std::atomic<nsUInt8> workerthreads;

    // Inherited via Platform
    ::v8::PageAllocator* GetPageAllocator() override;
    int NumberOfWorkerThreads() override;
    double MonotonicallyIncreasingTime() override;
    double CurrentClockTimeMillis() override;
    ::v8::TracingController* GetTracingController() override;
    std::unique_ptr<::v8::JobHandle> CreateJobImpl(::v8::TaskPriority priority, std::unique_ptr<::v8::JobTask> job_task, const ::v8::SourceLocation& location) override;
    void PostTaskOnWorkerThreadImpl(::v8::TaskPriority priority, std::unique_ptr<::v8::Task> task, const ::v8::SourceLocation& location) override;
    void PostDelayedTaskOnWorkerThreadImpl(::v8::TaskPriority priority, std::unique_ptr<::v8::Task> task, double delay_in_seconds, const ::v8::SourceLocation& location) override;
  };
  /*
   * @brief The Job Manager for the V8 Engine.
   * This class is responsible for managing the Jobs that are executed on the V8 Engine
   * by: Getting scripts, compiling them, Passing & Creating IAPCCommands out of them, and sending them to the APCJobSystem.
   */
  class NS_V8ENGINE_DLL V8EJobManager
  {
  public:
    bool Initialize();
    void Shutdown();

    void PostJob(std::unique_ptr<::v8::Task> task, const ::v8::SourceLocation& location);
    void PostBatchedJob(std::unique_ptr<::v8::Task> task, const ::v8::SourceLocation& location);

    nsUInt32 GetFrameCount() const;

  protected:
    core::IAPCCommandQueue* CreateQueueFromJobs(const nsString& p_sJobName, const nsHybridArray<::v8::Task*, 1>& p_aJobs);

  private:
    V8EPlatform* m_pPlatform;
    nsUniquePtr<aperture::core::threading::APCJobSystem> m_pJobSystem;
    std::atomic<nsUInt32> m_iV8EJobManagerFrameCount;
  };
} // namespace aperture::v8::jobsystem

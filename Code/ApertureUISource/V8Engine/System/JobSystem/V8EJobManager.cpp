#include <APCore/APEngine.h>
#include <APCore/CommandExecutor/IAPCCommand.h>
#include <APCore/CommandExecutor/IAPCCommandList.h>
#include <APCore/CommandExecutor/IAPCCommandQueue.h>
#include <Foundation/Profiling/Profiling.h>
#include <V8Engine/System/JobSystem/V8EJobManager.h>
#include <Foundation/Time/Clock.h>
void aperture::v8::jobsystem::V8EWorkerTaskRunner::SetJobManager(V8EJobManager* jobManager)
{
  m_pJobManager = jobManager;
}
aperture::v8::jobsystem::V8EWorkerTaskRunner::V8EWorkerTaskRunner(nsUInt32 p_iThreadPoolSize, TimeFunction p_uTimeFunction)
{
  if (p_iThreadPoolSize > ApertureSDK::GetScriptThreadCount())
  {
    nsLog::Error("V8EJobManager(V8EWorkerTaskRunner): The ThreadPoolSize provided, goes over the listed amount in ApertureSDK::GetScriptThreadCount(). it is being set to the max allowed value.");
    p_iThreadPoolSize = ApertureSDK::GetScriptThreadCount();
  }
}

bool aperture::v8::jobsystem::V8EWorkerTaskRunner::IdleTasksEnabled()
{
  return true;
}
bool aperture::v8::jobsystem::V8EWorkerTaskRunner::NonNestableTasksEnabled() const
{
  return true;
}
bool aperture::v8::jobsystem::V8EWorkerTaskRunner::NonNestableDelayedTasksEnabled() const
{
  return true;
}
void aperture::v8::jobsystem::V8EWorkerTaskRunner::PostTaskImpl(std::unique_ptr<::v8::Task> task, const ::v8::SourceLocation& location)
{
  nsLog::Debug("V8EWorkerTaskRunner::PostTask: Posting Task from File: {0}", location.FileName());
  m_pJobManager->PostBatchedJob(std::move(task), location);
}

void aperture::v8::jobsystem::V8EWorkerTaskRunner::PostNonNestableTaskImpl(std::unique_ptr<::v8::Task> task, const ::v8::SourceLocation& location)
{
  nsLog::Debug("V8EWorkerTaskRunner::PostNonNestableTask: Posting Non Nestable Task from File: {0}", location.FileName());
  m_pJobManager->PostBatchedJob(std::move(task), location);
}

void aperture::v8::jobsystem::V8EWorkerTaskRunner::PostDelayedTaskImpl(std::unique_ptr<::v8::Task> task, double delay_in_seconds, const ::v8::SourceLocation& location)
{
  nsLog::Debug("V8EWorkerTaskRunner::PostDelayedTask: Posting Delayed Task from File: {0}", location.FileName());
  m_pJobManager->PostBatchedJob(std::move(task), location);
}

void aperture::v8::jobsystem::V8EWorkerTaskRunner::PostNonNestableDelayedTaskImpl(std::unique_ptr<::v8::Task> task, double delay_in_seconds, const ::v8::SourceLocation& location)
{
  nsLog::Debug("V8EWorkerTaskRunner::PostNonNestableDelayedTask: Posting Non Nestable Delayed Task from File: {0}", location.FileName());
  m_pJobManager->PostBatchedJob(std::move(task), location);
}

void aperture::v8::jobsystem::V8EWorkerTaskRunner::PostIdleTaskImpl(std::unique_ptr<::v8::IdleTask> task, const ::v8::SourceLocation& location)
{

}


aperture::v8::jobsystem::V8EPlatform::V8EPlatform(int thread_pool_size, ::v8::platform::IdleTaskSupport idle_task_support, ::v8::platform::InProcessStackDumping in_process_stack_dumping, std::unique_ptr<::v8::TracingController> tracing_controller, ::v8::platform::PriorityMode priority_mode)
{
  if (thread_pool_size > ApertureSDK::GetScriptThreadCount())
  {
    nsLog::Error("V8EJobManager(V8EPlatform): The ThreadPoolSize provided, goes over the listed amount in ApertureSDK::GetScriptThreadCount(). it is being set to the max allowed value.");
    thread_pool_size = ApertureSDK::GetScriptThreadCount();
  }
}

void aperture::v8::jobsystem::V8EPlatform::SetJobManager(V8EJobManager* jobManager)
{
}

::v8::PageAllocator* aperture::v8::jobsystem::V8EPlatform::GetPageAllocator()
{
  return nullptr;
}

int aperture::v8::jobsystem::V8EPlatform::NumberOfWorkerThreads()
{
  return this->workerthreads.load();
}

double aperture::v8::jobsystem::V8EPlatform::MonotonicallyIncreasingTime()
{
  return nsTime::Now().GetSeconds();
}

double aperture::v8::jobsystem::V8EPlatform::CurrentClockTimeMillis()
{
  return nsTime::Now().GetMilliseconds();
}

::v8::TracingController* aperture::v8::jobsystem::V8EPlatform::GetTracingController()
{
  return nullptr;
}

std::unique_ptr<::v8::JobHandle> aperture::v8::jobsystem::V8EPlatform::CreateJobImpl(::v8::TaskPriority priority, std::unique_ptr<::v8::JobTask> job_task, const ::v8::SourceLocation& location)
{
  return std::unique_ptr<::v8::JobHandle>();
}

void aperture::v8::jobsystem::V8EPlatform::PostTaskOnWorkerThreadImpl(::v8::TaskPriority priority, std::unique_ptr<::v8::Task> task, const ::v8::SourceLocation& location)
{
  nsLog::Debug("V8EWorkerTaskRunner::PostTaskOnWorkerThreadImpl: Posting Idle Task from File: {0}", location.FileName());
  nsHybridArray<::v8::Task*, 1> taskArray;
  taskArray.PushBack(task.get()); // Pass the raw pointer
  m_pJobManager->PostBatchedJob(std::move(task), location);
}

void aperture::v8::jobsystem::V8EPlatform::PostDelayedTaskOnWorkerThreadImpl(::v8::TaskPriority priority, std::unique_ptr<::v8::Task> task, double delay_in_seconds, const ::v8::SourceLocation& location)
{
  nsLog::Debug("V8EWorkerTaskRunner::PostDelayedTaskOnWorkerThreadImpl: Posting Idle Task from File: {0}", location.FileName());
  nsHybridArray<::v8::Task*, 1> taskArray;
  taskArray.PushBack(task.get()); // Pass the raw pointer
  m_pJobManager->PostBatchedJob(std::move(task), location);
}

void aperture::v8::jobsystem::V8EJobManager::PostJob(std::unique_ptr<::v8::Task> task, const ::v8::SourceLocation& location)
{
  NS_PROFILE_SCOPE("V8EJobManager::PostJob");
  nsLog::Debug("V8EJobManager::PostJob: Posting Task from File: {0}", location.FileName());
  nsHybridArray<::v8::Task*, 1> taskArray;
  taskArray.PushBack(task.get()); // Pass the raw pointer
  m_pJobSystem->AddJob(*CreateQueueFromJobs("V8EJobManager::PostJob", taskArray));
}



bool aperture::v8::jobsystem::V8EJobManager::Initialize()
{
  NS_PROFILE_SCOPE("V8EJobManager::Initialize");
  m_iV8EJobManagerFrameCount.store(0);
  if (m_pJobSystem == nullptr)
  {
    m_pJobSystem = nsMakeUnique<core::threading::APCJobSystem>();
  }
  m_pJobSystem->InitializeJobSystem(core::threading::APCJobSystemConfig{
    0, ApertureSDK::GetScriptThreadCount(),
    0, 0, false});
  if (m_pJobSystem->ActiveScriptThreads() == ApertureSDK::GetScriptThreadCount())
  {
    return true;
  }
  return false;
}

void aperture::v8::jobsystem::V8EJobManager::Shutdown()
{
  NS_PROFILE_SCOPE("V8EJobManager::Shutdown");
  this->m_pJobSystem->Shutdown();
  nsLog::Debug("V8EJobManager: Shutting down.");
}


void aperture::v8::jobsystem::V8EJobManager::PostBatchedJob(std::unique_ptr<::v8::Task> task, const ::v8::SourceLocation& location)
{
  NS_PROFILE_SCOPE("V8EJobManager::PostBatchedJob");
  nsLog::Debug("V8EJobManager::PostJob: Posting Batched Task File: {0}", location.FileName());
}

nsUInt32 aperture::v8::jobsystem::V8EJobManager::GetFrameCount() const
{
  return m_iV8EJobManagerFrameCount.load();
}

void aperture::v8::jobsystem::V8EJobManager::SetPlatform(V8EPlatform* platform)
{
  m_pPlatform = (platform);
  platform->SetJobManager(this);
}

aperture::core::IAPCCommandQueue* aperture::v8::jobsystem::V8EJobManager::CreateQueueFromJobs(const nsString& p_sJobName, const nsHybridArray<::v8::Task*, 1>& p_aJobs)
{
  auto pQueue = nsMakeUnique<core::IAPCCommandQueue>();
  auto pList = nsMakeUnique<core::IAPCCommandList>();
  for (auto& job : p_aJobs)
  {
    core::IAPCCommand command(aperture::core::CommandType::Scripting, aperture::core::Runtype::FreeThread_Scripting);
    command.SetFunction(CreateFunctionFromTask(job));
    pList->AddCommand(command);
  }
  pQueue->AddCommandList(*pList);
  return pQueue.Release();
}

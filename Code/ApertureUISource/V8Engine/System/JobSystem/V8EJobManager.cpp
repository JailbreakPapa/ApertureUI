#include <V8Engine/System/JobSystem/V8EJobManager.h>
#include "V8EJobManager.h"


bool aperture::v8::jobsystem::V8EWorkerTaskRunner::IdleTasksEnabled()
{
  return false;
}
bool aperture::v8::jobsystem::V8EWorkerTaskRunner::NonNestableTasksEnabled() const
{
  return false;
}
bool aperture::v8::jobsystem::V8EWorkerTaskRunner::NonNestableDelayedTasksEnabled() const
{
  return false;
}
void aperture::v8::jobsystem::V8EWorkerTaskRunner::PostTaskImpl(std::unique_ptr<::v8::Task> task, const ::v8::SourceLocation& location)
{
}
void aperture::v8::jobsystem::V8EWorkerTaskRunner::PostNonNestableTaskImpl(std::unique_ptr<::v8::Task> task, const ::v8::SourceLocation& location)
{
}

void aperture::v8::jobsystem::V8EWorkerTaskRunner::PostDelayedTaskImpl(std::unique_ptr<::v8::Task> task, double delay_in_seconds, const ::v8::SourceLocation& location)
{
}

void aperture::v8::jobsystem::V8EWorkerTaskRunner::PostNonNestableDelayedTaskImpl(std::unique_ptr<::v8::Task> task, double delay_in_seconds, const ::v8::SourceLocation& location)
{
}

void aperture::v8::jobsystem::V8EWorkerTaskRunner::PostIdleTaskImpl(std::unique_ptr<::v8::IdleTask> task, const ::v8::SourceLocation& location)
{
}

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

#include <V8Engine/V8EngineDLL.h>
#include <libplatform/libplatform.h>

namespace aperture::v8::jobsystem
{
  class NS_V8ENGINE_DLL V8EWorkerTaskRunner : public ::v8::TaskRunner
  {
  public:
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
  };
  class NS_V8ENGINE_DLL V8EPlatform : public ::v8::Platform
  {
  public:
    explicit V8EPlatform(int thread_pool_size, ::v8::platform::IdleTaskSupport idle_task_support, ::v8::platform::InProcessStackDumping in_process_stack_dumping, std::unique_ptr<::v8::TracingController> tracing_controller, ::v8::platform::PriorityMode priority_mode);

  private:
    nsUniquePtr<V8EWorkerTaskRunner> m_workerTaskRunner;
  };
  /*
   * @brief The Job Manager for the V8 Engine.
   * This class is responsible for managing the Jobs that are executed on the V8 Engine
   * by: Getting scripts, compiling them, Passing & Creating IAPCCommands out of them, and sending them to the APCJobSystem.
   */
  class NS_V8ENGINE_DLL V8EJobManager
  {
  public:
    
  private:
  };
} // namespace aperture::v8::jobsystem
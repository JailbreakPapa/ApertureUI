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

#include <APHTML/APEngineCommonIncludes.h>
#include <APHTML/V8Engine/System/Internal/V8Helpers.h>
#include <APHTML/V8Engine/Core/V8EngineMain.h>
#include <APHTML/V8Engine/V8EngineDLL.h>
#include <Foundation/Threading/TaskSystem.h>
#include <v8-isolate.h>

namespace aperture::v8
{
  /**
   * @class V8EEngineRuntime
   * @brief Manages the V8 Engine runtime environment as a singleton.
   *
   * The V8EEngineRuntime class is responsible for initializing, managing, and shutting down the V8 Engine runtime.
   * It provides functionalities to compile and execute scripts, manage isolates, snapshots, and cache bound objects.
   * This class ensures thread-safe operations within a multithreaded environment by handling task groups and isolates.
   *
   * @details
   * - **Singleton Pattern:** Ensures a single instance of the runtime throughout the application.
   * - **Initialization & Shutdown:** Methods to initialize and gracefully shutdown the runtime.
   * - **Script Management:** Compiles and executes scripts, with options to cache bound objects.
   * - **Snapshot Handling:** Manages snapshots for faster startup and execution of scripts.
   * - **Task Management:** Handles task groups and isolates to maintain thread safety.
   *
   * @example
   * ```cpp
   * V8EEngineRuntime::Instance().InitializeRuntime(engineMain);
   * V8EEngineRuntime::Instance().RequestScriptToBeCompiledAndRan(scriptBuffer);
   * V8EEngineRuntime::Instance().ShutdownRuntime();
   * ```
   * @warning **ASSURE** Deletion of the EngineRuntime when used as a Singleton(It should also be used sparingly).
   * @note This class interacts closely with V8 Engine components and requires proper setup of V8 isolates and contexts.
   */
  class NS_APERTURE_DLL V8EEngineRuntime
  {
  public:
    NS_DECLARE_SINGLETON(V8EEngineRuntime);
    class V8QuickScriptTask : public nsTask
    {
      friend class V8EEngineRuntime;

    public:
      V8QuickScriptTask(const core::CoreBuffer<nsUInt8>& in_Script, const char* in_pScriptName, V8EEngineRuntime* in_pRuntime)
        : m_Script(in_Script)
        , m_ScriptName(in_pScriptName)
        , m_pRuntime(in_pRuntime)
      {
      }
      virtual ~V8QuickScriptTask();
      virtual void Execute() override;
      nsTaskGroupID GetTaskGroupID() const;
      ::v8::Isolate* GetIsolate() const;

    private:
      ::v8::Isolate* m_Isolate;
      V8EEngineRuntime* m_pRuntime;
      nsTaskGroupID m_TaskGroupID;
      core::CoreBuffer<nsUInt8> m_Script;
      const char* m_ScriptName;
    };

  public:
    friend class V8QuickScriptTask;
    /// @brief Constructor and Destructor for Singleton Passing.
    V8EEngineRuntime()
      : m_SingletonRegistrar(this)
    {
    }

    ~V8EEngineRuntime();

    void InitializeRuntime(V8EEngineMain* pEngineMain);

    void SetScriptPath(const char* in_pScriptPath, aperture::core::IAPCFileSystem::EFileType in_eFileType = aperture::core::IAPCFileSystem::EFileType::VFS);

    V8EEngineMain* GetV8EngineMain();

    void ShutdownRuntime();

    nsResult GetRuntimeStatus() const;

    static nsResult CacheBoundObjects(::v8::Isolate* in_isolate, ::v8::Local<::v8::FunctionTemplate> in_FuncTemplate, const char* in_pCacheName = "default_apui_js_cache", bool in_bWriteImmediately = false);

    static nsResult CacheBoundObjects(::v8::Isolate* in_isolate, ::v8::Local<::v8::ObjectTemplate> in_ObjTemplate, const char* in_pCacheName = "default_apui_js_cache", bool in_bWriteImmediately = false);

    static nsResult RequestScriptToBeCompiledAndRan(const core::CoreBuffer<nsUInt8>& in_Script, bool in_bRunImmediately = false, const char* in_pScriptName = "default_apui_js_script");

    nsHybridArray<std::pair<::v8::SnapshotCreator*, ::v8::StartupData>, 1> GetSnapshots() const;

    nsResult ProcessAndTickEngine();

    void WDTaskSystemV8Callback(nsTaskGroupID in_TaskGroupID);

  private:
    /*
     * @brief Prepares the runtime for execution by checking for any snapshots.
     * If there are no snapshots, then it will cache all of the bound functions and objects, and store them in the snapshot, then continue as normal.
     * If there are snapshots, then it will load them and continue as normal.
     */
    void PrepareRuntime();

    nsResult CompileAndRunScripts();

    void Cleanup_TaskGroups();

    void Cleanup_Isolates();

    void Cleanup_Snapshots();

    void Cleanup_Scripts();

  private:
  
    bool m_bPopLatestIsolate = false;
    nsString m_ScriptPath;
    nsResult im_RuntimeStatus = NS_FAILURE;
    nsHybridArray<core::CoreBuffer<nsUInt8>, 1> m_ScriptsToCompileAndRun;
    nsHybridArray<nsTaskGroupID, 1> m_TaskGroups;
    nsHybridArray<::v8::Isolate*, 1> m_IsolatesToCache;
    nsHybridArray<::v8::Isolate*, 1> m_IsolatesToDestroy;
    nsHybridArray<::v8::Isolate*, 1> m_Isolates;
    nsHybridArray<std::pair<::v8::SnapshotCreator*, ::v8::StartupData>, 1> m_Snapshots;
    nsHybridArray<core::CoreBuffer<nsUInt8>, 1> m_SnapshotData;
    V8EEngineMain* m_pV8EngineMain;
  };

  static void V8ErrorCallback(const char* location, const char* message);

  
} // namespace aperture::v8

/**
 * @def BIND_FUNCTION_TO_JS
 * @brief Macro to bind a function to JavaScript.
 *
 * @warning This macro isn't thread-safe (see V8EEngineRuntime for that reason), and can fail, so handle failures accordingly.
 * @param name Name of the function to bind.
 * @param func Pointer to the function to bind.
 */
#define CACHE_FUNCTION_FROM_JS(name, func)                                      \
  if (aperture::v8::V8EEngineRuntime::CacheBoundObjects(nullptr, func, name, true) == NS_FAILURE) \
  {                                                                             \
    nsLog::SeriousWarning("Unable to Cache Object: {0}", name);                 \
  }


/**
 * @def BIND_OBJECT_TO_JS
 * @brief Macro to bind an object to JavaScript.
 *
 * @warning This macro isn't thread-safe (see V8EEngineRuntime for that reason), and can fail, so handle failures accordingly.
 * @param name Name of the object to bind.
 * @param obj Pointer to the object to bind.
 */
#define CACHE_OBJECT_FROM_JS(name, obj)                                        \
  if (aperture::v8::V8EEngineRuntime::CacheBoundObjects(nullptr, obj, name, true) == NS_FAILURE) \
  {                                                                            \
    nsLog::SeriousWarning("Unable to Cache Object: {0}", name);                \
  }

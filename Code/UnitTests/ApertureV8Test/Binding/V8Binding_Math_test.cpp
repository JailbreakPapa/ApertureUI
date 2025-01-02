#include <FoundationTest/FoundationTestPCH.h>

#include <Foundation/Configuration/Startup.h>
#include <Foundation/IO/FileSystem/DataDirTypeFolder.h>
#include <Foundation/Logging/ConsoleWriter.h>
#include <Foundation/Logging/HTMLWriter.h>
#include <Foundation/Logging/Log.h>
#include <Foundation/Logging/VisualStudioWriter.h>
#include <Foundation/Threading/Thread.h>

#include <functional>
#include <APHTML/APEngine.h>
#include <V8Engine/Core/V8EngineMain.h>
#include <V8Engine/Binding/V8EBinder.h>

NS_CREATE_SIMPLE_TEST_GROUP(Binding);
NS_CREATE_SIMPLE_TEST(Binding, V8EBinder)
{
  NS_TEST_BLOCK(nsTestBlock::Enabled, "Setup SDK")
  {
    aperture::ApertureSDK::SetScriptThreadCount(2);
    aperture::ApertureSDK::Initialize();
  }
  NS_TEST_BLOCK(nsTestBlock::Enabled, "Setup Platform")
  {
    aperture::core::IAPCPlatform platform;
    aperture::core::DefaultLoggingSystem loggingSystem;
    aperture::core::IAPCMemoryAllocator memoryAllocator;
    platform.SetLoggingSystem(loggingSystem);
    platform.SetMemoryAllocator(memoryAllocator);
    NS_TEST_BOOL(aperture::core::IAPCPlatform::InitializePlatform("") == true);
  }
  NS_TEST_BLOCK(nsTestBlock::Enabled, "Setup V8Engine and Run Script.")
  {
    aperture::v8::V8EEngineMain engineMain;
    NS_TEST_BOOL(engineMain.InitializeV8Engine() == true);
    engineMain.GetV8EJobManager()->Initialize();
    engineMain.GetV8EJobManager()->SetPlatform(engineMain.GetV8EEnginePlatform());

    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator =
      v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    ::v8::Isolate* isolate = ::v8::Isolate::New(create_params);
    std::function<void()> testFunction = []() { nsLog::Info("Hello from testFunction");  };
    aperture::v8::binding::V8EBinder binder(isolate);
    binder.BindFunctionToJS("testFunction", testFunction);
  }
}

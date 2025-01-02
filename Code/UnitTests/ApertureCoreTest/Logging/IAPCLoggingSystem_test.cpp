#include <FoundationTest/FoundationTestPCH.h>

#include <Foundation/Configuration/Startup.h>
#include <Foundation/IO/FileSystem/DataDirTypeFolder.h>
#include <Foundation/Logging/ConsoleWriter.h>
#include <Foundation/Logging/HTMLWriter.h>
#include <Foundation/Logging/Log.h>
#include <Foundation/Logging/VisualStudioWriter.h>
#include <Foundation/Threading/Thread.h>

#include <APHTML/Interfaces/APCPlatform.h>
NS_CREATE_SIMPLE_TEST_GROUP(Logging);

NS_CREATE_SIMPLE_TEST(Logging, IAPCLogging)
{
  NS_TEST_BLOCK(nsTestBlock::Enabled, "Info")
  {
    {
      aperture::core::IAPCPlatform platform;
      aperture::core::DefaultLoggingSystem loggingSystem;
      platform.SetLoggingSystem(loggingSystem);
      // TODO: Re-route all nsGlobal Log messages to the logging system.
      platform.GetLoggingSystem()->LogInfo("This is a test.");
      NS_TEST_STRING(loggingSystem.GetMessageLog().front().c_str(), "[INFO] This is a test.");
    } 
  }
  NS_TEST_BLOCK(nsTestBlock::Enabled, "Warning")
  {
    {
      aperture::core::IAPCPlatform platform;
      aperture::core::DefaultLoggingSystem loggingSystem;
      platform.SetLoggingSystem(loggingSystem);
      // TODO: Re-route all nsGlobal Log messages to the logging system.
      platform.GetLoggingSystem()->LogWarning("This is a Warning test.");
      platform.GetLoggingSystem()->LogInfo("This is a test.");
      NS_TEST_STRING(loggingSystem.GetMessageLog().front().c_str(), "[WARNING] This is a Warning test.");
    }
  }
  NS_TEST_BLOCK(nsTestBlock::Enabled, "Error")
  {
    {
      aperture::core::IAPCPlatform platform;
      aperture::core::DefaultLoggingSystem loggingSystem;
      platform.SetLoggingSystem(loggingSystem);
      // TODO: Re-route all nsGlobal Log messages to the logging system.
      platform.GetLoggingSystem()->LogError("This is a Error test.");
      NS_TEST_STRING(loggingSystem.GetMessageLog().front().c_str(), "[ERROR] This is a Error test.");
    }
  }
  NS_TEST_BLOCK(nsTestBlock::Enabled, "FatalError")
  {
    aperture::core::IAPCPlatform platform;
    aperture::core::DefaultLoggingSystem loggingSystem;
    platform.SetLoggingSystem(loggingSystem);
    // TODO: Re-route all nsGlobal Log messages to the logging system.
    platform.GetLoggingSystem()->LogFatalError("This is a FatalError test.");
    NS_TEST_STRING(loggingSystem.GetMessageLog().front().c_str(), "[FATAL] This is a FatalError test.");
  }
}

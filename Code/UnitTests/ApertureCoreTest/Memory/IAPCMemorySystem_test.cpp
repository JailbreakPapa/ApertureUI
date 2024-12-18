#include <FoundationTest/FoundationTestPCH.h>

#include <Foundation/Configuration/Startup.h>
#include <Foundation/IO/FileSystem/DataDirTypeFolder.h>
#include <Foundation/Logging/ConsoleWriter.h>
#include <Foundation/Logging/HTMLWriter.h>
#include <Foundation/Logging/Log.h>
#include <Foundation/Logging/VisualStudioWriter.h>
#include <Foundation/Threading/Thread.h>

#include <APCore/Interfaces/APCPlatform.h>

NS_CREATE_SIMPLE_TEST_GROUP(Memory);

NS_CREATE_SIMPLE_TEST(Memory, IAPCMemory_WDAlloc)
{
  // NOTE(Mikael A.) This test block uses the standard memory allocator by WDS.
  NS_TEST_BLOCK(nsTestBlock::Enabled, "Alloc")
  {
    aperture::core::IAPCPlatform platform;
    aperture::core::DefaultLoggingSystem loggingSystem;
    aperture::core::IAPCMemoryAllocator memoryAllocator;
    platform.SetLoggingSystem(loggingSystem);
    platform.SetMemoryAllocator(memoryAllocator);
    NS_TEST_BOOL(aperture::core::IAPCPlatform::InitializePlatform("") == true);
    void* pMemory = platform.GetAllocatorSystem()->Alloc(1024, 16);
    nsLog::Info("Allocated memory at address: {0}", pMemory);
    nsLog::Info("Allocated memory of size: {0}", sizeof(pMemory));
    NS_TEST_BOOL(pMemory != nullptr);
    NS_TEST_BOOL(sizeof(pMemory) == 1024);
  }
  
  NS_TEST_BLOCK(nsTestBlock::Enabled, "Realloc")
  {
  }
  NS_TEST_BLOCK(nsTestBlock::Enabled, "Free")
  {
  }
}
// NOTE(Mikael A.) This test block uses MiMalloc memory allocator.
NS_CREATE_SIMPLE_TEST(Memory, IAPCMemory_MiMalloc)
{

}
// NOTE(Mikael A.) This test block uses simulated custom memory allocator, meant to simulate a custom memory allocator provided by the user.
NS_CREATE_SIMPLE_TEST(Memory, IAPCMemory_CustomOverride)
{
}

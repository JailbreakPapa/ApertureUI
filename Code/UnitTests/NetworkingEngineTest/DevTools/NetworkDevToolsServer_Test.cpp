#include <NetworkingEngineTest/ApertureNetworkTestPCH.h>

#include <Foundation/IO/FileSystem/FileReader.h>
#include <Foundation/IO/FileSystem/FileSystem.h>
#include <Foundation/IO/FileSystem/FileWriter.h>
#include <Foundation/Logging/Log.h>
#include <Foundation/System/Process.h>
#include <Foundation/Utilities/CommandLineUtils.h>

#include <NetworkEngine/DevTools/NETDTServer.h>

NS_CREATE_SIMPLE_TEST_GROUP(DevTools);
NS_CREATE_SIMPLE_TEST(DevTools, DevToolsServer)
{
  if (nsFileSystem::DetectSdkRootDirectory("Data/UnitTests") != NS_SUCCESS)
    return;

  nsStringBuilder sSDKPath = nsFileSystem::GetSdkRootDirectory();
  nsStringBuilder sOutputFolder = nsTestFramework::GetInstance()->GetRelTestDataPath();
  sOutputFolder.MakeCleanPath();

  sSDKPath.AppendPath(sOutputFolder);
  sSDKPath.MakeCleanPath();
  sSDKPath.AppendPath("Packaged_DevTools");

  if (nsOSFile::ExistsDirectory(sSDKPath) == false)
  {
    nsLog::Error("StartLocalHostTest: Cant Find Deployed DevTools Directory! Please make sure you have built devtools, and placed it in the 'Packaged_DevTools' Directory In the test folder!");
    return;
  }
  NS_TEST_BLOCK(nsTestBlock::Enabled, "StartAndListen")
  {
    aperture::networking::NetworkDevToolsServer server(3228);
    NS_TEST_BOOL_MSG(server.StartLocalHostServer() == true, "Failed to Start Local Host Server!");
  }
}


NS_FORCE_INLINE void SetupUTDatapath()
{
  nsStringBuilder temp;
  auto a = nsOSFile::GetApplicationDirectory() + "../../../";
  nsFileSystem::SetSdkRootDirectory(a.ConvertTo<nsStringView>());
  nsFileSystem::GetSdkRootDirectory().GetData(temp);
  temp.Append(nsTestFramework::GetInstance()->GetRelTestDataPath());
  /// <summary>
  /// Add the data directory to the file system as a data directory, so we dont have to have a inline variable.
  /// </summary>
  nsFileSystem::AddDataDirectory(temp);
}

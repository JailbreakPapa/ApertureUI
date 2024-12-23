#include <APCore/APEngine.h>

void aperture::ApertureSDK::SetScriptThreadCount(nsUInt8 p_threadcount)
{
  if (IsSDKActive())
  {
    nsLog::SeriousWarning("ApertureSDK: Cannot change the amount of Script Threads after the SDK has been initialized.");
    return;
  }
  {
    NS_LOCK(m_SDKLocker);
    m_pScriptThreadCount = p_threadcount;
  }
}

nsUInt8 aperture::ApertureSDK::GetScriptThreadCount()
{
  return m_pScriptThreadCount;
}

inline void aperture::ApertureSDK::SetRenderThreadCount(nsUInt8 p_threadcount)
{
  if (IsSDKActive())
  {
    nsLog::SeriousWarning("ApertureSDK: Cannot change the amount of Render Threads after the SDK has been initialized.");
    return;
  }
  {
    NS_LOCK(m_SDKLocker);
    m_pRenderThreadCount = p_threadcount;
  }
}

nsUInt8 aperture::ApertureSDK::GetRenderThreadCount()
{
  return m_pRenderThreadCount;
}

inline void aperture::ApertureSDK::SetCompositionThreadCount(nsUInt8 p_threadcount)
{
  if (IsSDKActive())
  {
    nsLog::SeriousWarning("ApertureSDK: Cannot change the amount of Composition Threads after the SDK has been initialized.");
    return;
  }
  {
    NS_LOCK(m_SDKLocker);
    m_pCompositionThreadCount = p_threadcount;
  }
}

nsUInt8 aperture::ApertureSDK::GetCompositionThreadCount()
{
  return m_pCompositionThreadCount;
}

void aperture::ApertureSDK::SetPlatformName(const nsString& p_platformname)
{
  if (IsSDKActive())
  {
    nsLog::SeriousWarning("ApertureSDK: Cannot change the Platform Name after the SDK has been initialized.");
    return;
  }
  {
    NS_LOCK(m_SDKLocker);
    m_sPlatformName = p_platformname;
  }
}

nsString aperture::ApertureSDK::GetPlatformName()
{
  return m_sPlatformName;
}

inline void aperture::ApertureSDK::SetPlatformVersion(const nsString& p_platformversion)
{
  if (IsSDKActive())
  {
    nsLog::SeriousWarning("ApertureSDK: Cannot change the Platform Version after the SDK has been initialized.");
    return;
  }
  {
    NS_LOCK(m_SDKLocker);
    m_sPlatformVersion = p_platformversion;
  }
}

nsString aperture::ApertureSDK::GetPlatformVersion()
{
  return m_sPlatformVersion;
}

inline bool aperture::ApertureSDK::IsSDKActive()
{
  return m_bSDKActive;
}

void aperture::ApertureSDK::SetSDKTextEncoding(ESDKTextEncoding p_textencoding)
{
  if (IsSDKActive())
  {
    nsLog::SeriousWarning("ApertureSDK: Cannot change the Text Encoding after the SDK has been initialized. Please reinitialize your platform, then retry.");
    return;
  }
  {
    NS_LOCK(m_SDKLocker);
    m_eTextEncoding = p_textencoding;
  }
}

aperture::ApertureSDK::ESDKTextEncoding aperture::ApertureSDK::GetSDKTextEncoding()
{
  return m_eTextEncoding;
}

void aperture::ApertureSDK::SetSDKActive(bool p_active)
{
  {
    NS_LOCK(m_SDKLocker);
    m_bSDKActive = p_active;
  }
}

bool aperture::ApertureSDK::IsNDAPlatform()
{
  return m_bIsNDAPlatform;
}


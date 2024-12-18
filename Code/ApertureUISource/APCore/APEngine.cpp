#include <APCore/APEngine.h>

void aperture::ApertureSDK::SetSDKActive(bool p_active)
{
  {
    NS_LOCK(m_SDKLocker);
    m_bSDKActive = p_active;
  }
}

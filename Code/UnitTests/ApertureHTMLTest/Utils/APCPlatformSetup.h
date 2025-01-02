#include <APHTML/Interfaces/APCPlatform.h>

class APHTMLTEST_APCFileSystem
  : public aperture::core::IAPCFileSystem
{
public:
  
  APHTMLTEST_APCFileSystem() = default;
  virtual aperture::core::CoreBuffer<nsUInt8> GetFileData(const char* in_filepath) override;

  virtual bool FileExists(const nsString& in_filepath) override;

  virtual nsString FileCharset(const nsString& in_filepath) override;

};

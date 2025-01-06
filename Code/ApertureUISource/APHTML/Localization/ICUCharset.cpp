#include "ICUCharset.h"
#include <APHTML/Localization/ICUCharset.h>
#include <unicode/ucsdet.h>
#include <unicode/unistr.h>

const char* aperture::localization::ICUCharset::GetCharset(std::vector<char>& in_buffer)
{
  UErrorCode status = U_ZERO_ERROR;
  UCharsetDetector* detector = ucsdet_open(&status);
  if (U_FAILURE(status))
  {
    nsLog::SeriousWarning("Failed to open ICU charset detector!");
    return "";
  }

  ucsdet_setText(detector, in_buffer.data(), in_buffer.size(), &status);
  if (U_FAILURE(status))
  {
    nsLog::SeriousWarning("Failed to set text for ICU charset detector!");
    ucsdet_close(detector);
    return "";
  }

  const UCharsetMatch* match = ucsdet_detect(detector, &status);
  if (U_FAILURE(status))
  {
    nsLog::SeriousWarning("Failed to detect charset with ICU charset detector!");
    ucsdet_close(detector);
    return "";
  }

  const char* charset = ucsdet_getName(match, &status);
  if (U_FAILURE(status))
  {
    nsLog::SeriousWarning("Failed to get charset name from ICU charset detector!");
    ucsdet_close(detector);
    return "";
  }

  ucsdet_close(detector);
  return charset;
}

const char* aperture::localization::ICUCharset::GetCharset(core::CoreBuffer<char>& in_buffer)
{
    std::vector<char> in_vbuffer;
    for(auto& i : in_buffer)
    {
      in_vbuffer.push_back(i);
    }
    return GetCharset(in_vbuffer);
}


const char* aperture::localization::ICUCharset::GetCharset(core::CoreBuffer<nsUInt8>& in_buffer)
{
    std::vector<char> in_vbuffer;
    for(auto& i : in_buffer)
    {
      char l = static_cast<char>(i);
      in_vbuffer.push_back(l);
    }
    return GetCharset(in_vbuffer);
}

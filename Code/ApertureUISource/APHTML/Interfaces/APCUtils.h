#pragma once
namespace aperture::core
{
  template <class T>
  inline void SafeDelete(T*& pVal)
  {
    if (pVal)
    {
      delete pVal;
      pVal = nullptr;
    }
  }

  template <class T>
  inline void SafeDeleteArray(T*& pVal)
  {
    if (pVal)
    {
      delete[] pVal;
      pVal = nullptr;
    }
  }
  /// @brief Utility class to format a string quickly!
  template <typename... ARGS>
  inline nsString Format(nsStringView sFormat, ARGS&&... args)
  {
    nsStringBuilder contain;
    return nsFormatStringImpl<ARGS...>(sFormat, std::forward<ARGS>(args)...).GetTextCStr(contain);
  }
} // namespace aperture::core

#pragma warning(push)
#pragma warning(disable : 4091)
/*
This code is part of Aperture UI - A HTML/CSS/JS UI Middleware

Copyright (c) 2020-2024 WD Studios L.L.C. and/or its licensors. All
rights reserved in all media.

The coded instructions, statements, computer programs, and/or related
material (collectively the "Data") in these files contain confidential
and unpublished information proprietary WD Studios and/or its
licensors, which is protected by United States of America federal
copyright law and by international treaties.

This software or source code is supplied under the terms of a license
agreement and nondisclosure agreement with WD Studios L.L.C. and may
not be copied, disclosed, or exploited except in accordance with the
terms of that agreement. The Data may not be disclosed or distributed to
third parties, in whole or in part, without the prior written consent of
WD Studios L.L.C..

WD STUDIOS MAKES NO REPRESENTATION ABOUT THE SUITABILITY OF THIS
SOURCE CODE FOR ANY PURPOSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER, ITS AFFILIATES,
PARENT COMPANIES, LICENSORS, SUPPLIERS, OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OR PERFORMANCE OF THIS SOFTWARE OR SOURCE CODE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#pragma once

#include <APHTML/APEngineDLL.h>
#include <APHTML/Interfaces/APCPlatform.h>
#include <APHTML/APEngineCommonIncludes.h>
namespace aperture::core
{
  class IAPCCommand;
}
namespace aperture
{
  /// @brief Static Interface for setting global values for the Aperture SDK.
  /// @warning This class is not meant for initialization, but for setting global values. please set values before using the SDK.
  static class NS_APERTURE_DLL ApertureSDK
  {
  public:
    /// @brief The Text Encoding that the SDK will use.
    enum class ESDKTextEncoding : nsUInt8
    {
      UTF8 = 1,
      UTF16 = 2,
      UTF16LE = 3,
    };

  public:
    /// @brief Initializes the SDK. This should be called before using the SDK, and always succeeds, but will assert on values that are incorrect.
    static void Initialize();
    // @note We scope lock write access to these values for protection.
    // @note Also, we do not allow for the values to be changed after the SDK is active.
    static void SetScriptThreadCount(nsUInt8 p_threadcount);

    static nsUInt8 GetScriptThreadCount();

    static void SetRenderThreadCount(nsUInt8 p_threadcount);

    static nsUInt8 GetRenderThreadCount();

    static void SetCompositionThreadCount(nsUInt8 p_threadcount);

    static nsUInt8 GetCompositionThreadCount();

    static void SetPlatformName(const nsString& p_platformname);

    static nsString GetPlatformName();

    static void SetPlatformVersion(const nsString& p_platformversion);

    static nsString GetPlatformVersion();

    static bool IsSDKActive();

    static void SetSDKTextEncoding(ESDKTextEncoding p_textencoding);

    static ESDKTextEncoding GetSDKTextEncoding();

    static bool IsNDAPlatform();

    static void SetIsCachingAllowed(bool p_allowcaching);

    static bool IsCachingAllowed();
  private:
    static void SetSDKActive(bool p_active);
  
  private:
    /// @brief Is Serialization Allowed for the SDK? This is a core feature about Aperture that allows for Performance improvements, e.g. Script Compilation, Binding Classes to V8, etc.
    static inline bool m_bIsSerializationAllowedforSDK = false;
    /// @brief Is the SDK in NDA Mode?/Is the Platform NDA Protected? We need to know this for certain features.
    static inline bool m_bIsNDAPlatform = false;
    /// @brief The Text Encoding that the SDK will use.
    static inline ESDKTextEncoding m_eTextEncoding;
    /// @brief Sets the amount of threads that aperture can create/use for Script Compiling/Execution.
    static inline nsUInt8 m_pScriptThreadCount;
    /// @brief Sets the amount of threads that aperture can create/use for Rendering.
    static inline nsUInt8 m_pRenderThreadCount;
    /// @brief Sets the amount of threads that aperture can create/use for Composition (HTML, DOM, CSS, etc).
    static inline nsUInt8 m_pCompositionThreadCount;
    /// @brief The Name of the current platform that the SDK is running on.
    static inline nsString m_sPlatformName;
    /// @brief The Version of the current platform that the SDK is running on.
    /// @note This is not the version of the SDK, but the version of the platform that the SDK is running on.
    static inline nsString m_sPlatformVersion;
    /// @brief The Version of the SDK that is currently being used.
    static inline nsString m_sSDKVersion;
    /// @brief Is the SDK Active? (Has it been initialized?)
    /// @note This would be set by the APCPlatform on initialization.
    static inline bool m_bSDKActive;
    /// @brief Are We in Trial Mode?
    /// @note This is used to determine if the SDK is in Trial Mode. This should ONLY be used for feedback to the user about their license. the SDK should never use the value outside of that.
    static inline bool m_bSDKTrial;
    static inline nsMutex m_SDKLocker;
  };
} // namespace aperture

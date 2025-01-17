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
#include <APHTML/Interfaces/APCFileSystem.h>
#include <APHTML/Interfaces/APCHarrlowInterface.h>
#include <APHTML/Interfaces/APCLoggingSystem.h>
#include <APHTML/Interfaces/APCMemoryAllocator.h>
#include <Foundation/Configuration/Singleton.h>
#include <Foundation/Time/Stopwatch.h>
#include <Foundation/Utilities/EnumerableClass.h>

namespace aperture::core
{
  /// @brief Interface for applications to provide a interfaces to APUI. Also Represents the Lifetime of The SDK.
  class NS_APERTURE_DLL IAPCPlatform : public nsEnumerable<IAPCPlatform>
  {
    friend class IAPCFileSystem;
    friend class IAPCHarrlowInterface;
    friend class IAPCLoggingSystem;
    friend class IAPCMemoryAllocator;

    NS_DECLARE_ENUMERABLE_CLASS(IAPCPlatform);
    NS_DECLARE_SINGLETON(IAPCPlatform);

  public:
    IAPCPlatform()
      : m_SingletonRegistrar(this)
    {
      m_Stopwatch.StopAndReset();
    }

    virtual ~IAPCPlatform()
    {
      KillPlatform();
    }
    
    /// @brief Initializes the Underlying platform implementation(memory handler, filesystem, etc).
    static bool InitializePlatform(const char* licensekey);
    void StartPlatform();
    /// @brief Sets the filesystem for APUI to use.
    /// @param in_filesystem filesystem to use.
    void SetFileSystem(const IAPCFileSystem& in_filesystem);

    // TODO: Functions to listen in on log events.
    void SetMemoryAllocator(const IAPCMemoryAllocator& in_memorysystem);

    void SetLoggingSystem(const IAPCLoggingSystem& in_loggingsystem);

    void KillPlatform();

    IAPCFileSystem* GetFileSystem()
    {
      return m_filesystem;
    }

    IAPCMemoryAllocator* GetAllocatorSystem()
    {
      return m_memoryallocator;
    }

    IAPCLoggingSystem* GetLoggingSystem()
    {
      return m_loggingsystem;
    }

    nsTime GetTimeSinceStart()
    {
      return m_Stopwatch.GetRunningTotal();
    }

    static bool IsEngineReady();

  private:
    nsStopwatch m_Stopwatch;
    static inline bool m_bEngineStatus;

    nsMutex lockermt;

    IAPCFileSystem* m_filesystem = nullptr;

    IAPCMemoryAllocator* m_memoryallocator = nullptr;

    IAPCLoggingSystem* m_loggingsystem = nullptr;
  };

} // namespace aperture::core

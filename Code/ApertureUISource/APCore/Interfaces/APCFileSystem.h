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
#include <APCore/COREAPEngineDLL.h>
#include <APCore/Interfaces/Internal/APCBuffer.h>
#include <Foundation/IO/OSFile.h>
#include <Foundation/Types/UniquePtr.h>
namespace aperture::core
{
  /// @brief Interface for applications to provide a filesystem to APUI. Is used by APCCoreLibrary.
  class NS_COREAPENGINE_DLL IAPCFileSystem
  {
  public:
    /// @brief Default constructor for the IAPCFileSystem class.
    IAPCFileSystem() = default;

    /// @brief Constructor for the IAPCFileSystem class that initializes the FileSystem with the FULL path of the UI Resources.
    /// @param uiresources Path to the UI resources.
    explicit IAPCFileSystem(const char* uiresources)
      : m_uiresources(uiresources)
    {
    }

    /// @brief Gets the buffer of the file's data.
    /// @param in_filepath Path to the file.
    /// @return Buffer of the file's data.
    virtual core::CoreBuffer<nsUInt8> GetFileData(const char* in_filepath)
    {
      return core::CoreBuffer<nsUInt8>();
    }
    virtual const char* GetFileMimeType(const char* in_filepath);
    /// @brief Checks if a file exists.
    /// @param in_filepath Path to the file.
    /// @return True if the file exists, false otherwise.
    virtual bool FileExists(const nsString& in_filepath) { return false; }

    /// @brief Returns the file's charset.
    /// @param in_filepath Path to the file.
    /// @return The file's charset.
    virtual nsString FileCharset(const nsString& in_filepath);

    /// @brief Sets the path to the UI resources.
    /// @param uiresources Path to the UI resources.
    void SetUIResourcesPath(const char* uiresources) { m_uiresources = uiresources; }

    /// @brief Gets the path to the UI resources.
    /// @return The path to the UI resources.
    const char* GetUIResourcesPath() { return m_uiresources; }

    /// @brief Gets the full file path based on the given URI subpath.
    /// @param urisubpath The URI subpath.
    /// @return The full file path.
    const char* GetFullFilePath(const char* urisubpath);

    /// @brief Gets the raw file handle for the specified file. TODO: Currently only works for Windows.
    /// @param in_filepath Path to the file.
    /// @param mode The file mode.
    /// @return The raw file handle.
    nsOSFileData GetRawFileHandle(const char* in_filepath, const char* mode);

    // Do Relative Path, etc...

  protected:
    const char* m_uiresources; ///< Path to the UI resources.
  };
} // namespace aperture::core

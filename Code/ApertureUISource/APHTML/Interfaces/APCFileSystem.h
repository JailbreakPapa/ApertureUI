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
#include <APHTML/APEngineCommonIncludes.h>
#include <APHTML/Interfaces/Internal/APCBuffer.h>
#include <Foundation/IO/OSFile.h>
#include <Foundation/Types/UniquePtr.h>
namespace aperture::core
{
  /// @brief Interface for applications to provide a filesystem to APUI. Is used by APCCoreLibrary.
  class NS_APERTURE_DLL IAPCFileSystem
  {
  public:
    enum class EFileType
    {
      OSDependant,
      VFS,
    };
    enum class ERequestType
    {
      Script_JS,
      CSS,
      XML,
      HTML,
      JSON,
      ImageOrResource,
      OtherBinary
    };
    enum class ERequestAmmount
    {
      Single = 0x001,
      GlobAll = 0x002
    };

  public:
    /// @brief Default constructor for the IAPCFileSystem class.
    IAPCFileSystem() = default;

    /// @brief Constructor for the IAPCFileSystem class that initializes the FileSystem with the FULL path of the UI Resources.
    /// @param uiresources Path to the UI resources.
    explicit IAPCFileSystem(const char* uiresources)
      : m_uiresources(uiresources)
    {
    }

    /// @brief Required URI Resolve function to be implemented by the user. this is needed for the engine to get the data from managed place, whether it be VFS (PAK Based System) or OS.
    /// @param in_uri Path to the file
    /// @param out_filedatafiles vector of the file data
    /// @param type what type of files are we retrieving
    /// @param in_patharchivepath If we are using VFS, what is the path the the VFS Archive File?
    /// @return If The Resolve Was Successful.
    /// @warning Handling a Bad Resolve is weird to work with, as we rely on the user to handle the data. if the function returns false, we will not be able to get the data (e.g. parsing a CSS file will fail). **MAKE SURE THIS IS ALWAYS RETURNING TRUE.**
    virtual bool RequestURIResolve(const char* in_uri, const char* in_patharchivepath, std::vector<core::CoreBuffer<nsUInt8>>& out_filedatafiles, EFileType type = EFileType::OSDependant, ERequestType filetype = ERequestType::Script_JS, ERequestAmmount ammount = ERequestAmmount::Single);
    /// @brief Gets the buffer of the file's data.
    /// @param in_filepath Path to the file.
    /// @return Buffer of the file's data.
    virtual core::CoreBuffer<nsUInt8> GetFileData(const char* in_filepath, EFileType type = EFileType::OSDependant);

    virtual core::CoreBuffer<const char*> GetFileDataChar(const char* in_filepath, EFileType type = EFileType::OSDependant);

    virtual bool RequestCreateFile(const char* in_filepath, core::CoreBuffer<nsUInt8> out_filedata, EFileType type = EFileType::OSDependant);

    virtual bool RequestCreateFile(const char* in_filepath, core::CoreBuffer<const char*> out_filedata, EFileType type = EFileType::OSDependant);
    
    virtual const char* GetFileMimeType(const char* in_filepath);
    /// @brief Checks if a file exists.
    /// @param in_filepath Path to the file.
    /// @return True if the file exists, false otherwise.
    virtual bool FileExists(const nsString& in_filepath) { return false; }

    /// @brief Returns the file's charset.
    /// @param in_filepath Path to the file.
    /// @return The file's charset.
    virtual const char* FileCharset(const nsString& in_filepath);

    /// @brief Sets the path to the UI resources.
    /// @param uiresources Path to the UI resources.
    void SetUIResourcesPath(const char* uiresources) { m_uiresources = uiresources; }

    /// @brief Gets the path to the UI resources.
    /// @return The path to the UI resources.
    /// @note If We are working from VFS, then its really dependant on the user to resolve paths, and get us the needed data.
    const char* GetUIResourcesPath() { return m_uiresources; }

    /// @brief Gets the full file path based on the given URI subpath.
    /// @param urisubpath The URI subpath.
    /// @return The full file path.
    virtual const char* GetFullFilePath(const char* urisubpath);

    /// @brief Gets the raw file handle for the specified file. TODO: Currently only works for Windows.
    /// @param in_filepath Path to the file.
    /// @param mode The file mode.
    /// @return The raw file handle.
    nsOSFileData GetRawFileHandle(const char* in_filepath, const char* mode);

    // TODO: Do Relative Path, etc...
  private:
    bool EvaluateVFSInternal(const char* in_uri, const char* in_patharchivepath, std::vector<core::CoreBuffer<nsUInt8>>& out_filedatafiles, EFileType type, ERequestType filetype, ERequestAmmount ammount);

  protected:
    const char* m_uiresources; ///< Path to the UI resources.
  };
} // namespace aperture::core

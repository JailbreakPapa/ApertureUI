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
#include <uriparser/Uri.h>

namespace aperture::uri
{
  class NS_APERTURE_DLL URIBank
  {
  public:
    URIBank() = default;

    explicit URIBank(const char* in_uribankname);
    void SetURIBankName(const char* in_uribankname);
    const char* GetURIBankName();

    UriUriA* FindRawURI(const nsString& in_uri);
    const char* FindStringURI(UriUriA* in_uri);

    void AddURI(const nsString& in_uri, UriUriA* in_rawuri);
    void AddURI(UriUriA* in_uri, nsUInt8 in_accessType = 0 /* 0 = OSDependant, 1 = VFS */);
    void AddURI(const nsString& in_uri, nsUInt8 in_accessType = 0 /* 0 = OSDependant, 1 = VFS */);

    void RemoveURI(const nsString& in_uri);

    const char* FindResolvedPath(const nsString& in_uri);
    const char* FindOSPath(const nsString& in_uri);
    const char* FindOSPath(UriUriA* in_uri);

    const char* GetRequestedFileName(const nsString& in_uri);
    const char* GetRequestedFileName(UriUriA* in_uri);

    const char* GetRequestedFileExtension(const nsString& in_uri);
    const char* GetRequestedFileExtension(UriUriA* in_uri);

    bool IsContentURI(const nsString& in_uri);
    bool IsContentURI(UriUriA* in_uri);
    // Helper for nsMap
    static nsStringView UriObjToStringView(UriUriA* in_uri);
  private:
    nsString m_uribankname;
    nsMap<nsString, UriUriA*> m_uriBank;
  };
} // namespace aperture::uri

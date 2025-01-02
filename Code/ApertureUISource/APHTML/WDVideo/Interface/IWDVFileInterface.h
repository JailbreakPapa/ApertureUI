/*
 *   Copyright (c) 2023 Watch Dogs LLC
 *   All rights reserved.
 *   You are only allowed access to this code, if given WRITTEN permission by WD Studios L.L.C.
 */

#pragma once

#include <Foundation/IO/OSFile.h>
#include <APHTML/APEngineDLL.h>
#include <libwebm/mkvparser/mkvparser.h>
// NOTE: This is a workaround for the warning C4275, we could make webm_parser a DLL, but it's not necessary.
#pragma warning(push)
#pragma warning(disable : 4275)

namespace wdvideo
{
  class NS_APERTURE_DLL IWDVFileInterface : public mkvparser::IMkvReader
  {
  private:
    long long inlength;
    FILE* infile;

    bool is_preloaded;
    unsigned char* inrawdata;

  public:
    IWDVFileInterface();
    virtual ~IWDVFileInterface();

    int open(const char* filename, bool preload);
    bool getFileSize();
    virtual int Read(long long position, long length, unsigned char* buffer);
    virtual int Length(long long* total, long long* available);
  };
} // namespace wdvideo

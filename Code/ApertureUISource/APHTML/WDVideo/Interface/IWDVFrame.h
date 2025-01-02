/*
 *   Copyright (c) 2023 Watch Dogs LLC
 *   All rights reserved.
 *   You are only allowed access to this code, if given WRITTEN permission by WD Studios L.L.C.
 */

#pragma once

#include <APHTML/APEngineDLL.h>
#include <libsimplewebm/libwebm/mkvparser/mkvparser.h>

namespace wdvideo
{
  class NS_APERTURE_DLL IWDVFrame
  {
  private:
    unsigned char* m_y;
    unsigned char* m_u;
    unsigned char* m_v;

    size_t m_ySize;
    size_t m_uvSize;

    size_t m_width;
    size_t m_height;
    size_t m_displayWidth;
    size_t m_displayHeight;

    double m_time;

  public:
    IWDVFrame(size_t width, size_t height);
    ~IWDVFrame();

    void copy(IWDVFrame* dst);

    unsigned char* y() const;
    unsigned char* u() const;
    unsigned char* v() const;

    size_t ySize() const;
    size_t uvSize() const;

    size_t yPitch() const;
    size_t uvPitch() const;

    size_t width() const;
    size_t height() const;
    size_t displayWidth() const;
    size_t displayHeight() const;

    void setTime(double time);
    double time() const;
  };
} // namespace wdvideo

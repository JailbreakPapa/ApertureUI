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
  class NS_APERTURE_DLL IWDVPacket
  {
  public:
    enum class Type
    {
      Video,
      Audio
    };

  protected:
    const mkvparser::Block* m_block;
    Type m_type;
    double m_time;

  public:
    IWDVPacket(const mkvparser::Block* block, Type type, double time);
    ~IWDVPacket();

    Type type() const;
    const mkvparser::Block* block() const;
    double time() const;
  };
} // namespace wdvideo

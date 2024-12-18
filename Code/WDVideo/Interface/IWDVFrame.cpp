/*
 *   Copyright (c) 2023 Watch Dogs LLC
 *   All rights reserved.
 *   You are only allowed access to this code, if given WRITTEN permission by WD Studios L.L.C.
 */

#include <APCore/Interfaces/APCUtils.h>
#include <WDVideo/Interface/IWDVFrame.h>

namespace wdvideo
{
    IWDVFrame::IWDVFrame(size_t width, size_t height) :
        m_width(0), m_height(0),
        m_displayWidth(width), m_displayHeight(height),
        m_time(0.0)
    {
        // vpx size is +64 aligned to 16
        m_width = (((width + 64) - 1) & ~15) + 16;
        m_height = (((height + 64) - 1) & ~15) + 16;

        m_ySize = m_width * m_height;
        m_uvSize = (m_width / 2) * (m_height / 2);

        m_y = new unsigned char[m_ySize];
        m_u = new unsigned char[m_uvSize];
        m_v = new unsigned char[m_uvSize];

        // - initially black
        std::memset(m_y, 0, m_ySize);
        std::memset(m_u, 128, m_uvSize);
        std::memset(m_v, 128, m_uvSize);
    }

    IWDVFrame::~IWDVFrame()
    {
        aperture::core::SafeDeleteArray<unsigned char>(m_y);
        aperture::core::SafeDeleteArray<unsigned char>(m_u);
        aperture::core::SafeDeleteArray<unsigned char>(m_v);
    }

    unsigned char *IWDVFrame::y() const
    {
        return m_y;
    }

    unsigned char *IWDVFrame::u() const
    {
        return m_u;
    }

    unsigned char *IWDVFrame::v() const
    {
        return m_v;
    }

    size_t IWDVFrame::ySize() const
    {
        return m_ySize;
    }

    size_t IWDVFrame::uvSize() const
    {
        return m_uvSize;
    }

    size_t IWDVFrame::yPitch() const
    {
        return m_width;
    }

    size_t IWDVFrame::uvPitch() const
    {
        return m_width / 2;
    }

    size_t IWDVFrame::width() const
    {
        return m_width;
    }

    size_t IWDVFrame::height() const
    {
        return m_height;
    }

    size_t IWDVFrame::displayWidth() const
    {
        return m_displayWidth;
    }

    size_t IWDVFrame::displayHeight() const
    {
        return m_displayHeight;
    }

    void  IWDVFrame::setTime(double time)
    {
        m_time = time;
    }

    double  IWDVFrame::time() const
    {
        return m_time;
    }

    void IWDVFrame::copy(IWDVFrame *dst)
    {
        std::memcpy(dst->y(), m_y, m_ySize);
        std::memcpy(dst->u(), m_u, m_uvSize);
        std::memcpy(dst->v(), m_v, m_uvSize);
    }
}
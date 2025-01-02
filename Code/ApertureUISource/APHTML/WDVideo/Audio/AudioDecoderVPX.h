/*
 *   Copyright (c) 2023 Watch Dogs LLC
 *   All rights reserved.
 *   You are only allowed access to this code, if given WRITTEN permission by WD Studios L.L.C.
 */

#pragma once

#include <Foundation/Containers/DynamicArray.h>
#include <vorbis/codec.h>
#include <APHTML/Interfaces/Internal/APCBuffer.h>


namespace wdvideo
{
  class VideoPlayerWebm;
  class NS_APERTURE_DLL AudioDataVPX
  {
  protected:
        float   *mValues;
        size_t  mSize;

    public:
        AudioDataVPX(float *values, size_t size) :
            mValues(values),
            mSize(size)
        {
        }

        ~AudioDataVPX()
        {
            delete[] mValues;
        }

        const float *values() { return mValues; }
        size_t  size() { return mSize; }
  };

  class NS_APERTURE_DLL AudioDecoderVPX
  {
  protected:
    vorbis_info m_vorbisInfo;
    vorbis_comment m_vorbisComment;
    vorbis_dsp_state m_vorbisDsp;
    vorbis_block m_vorbisBlock;
    int64_t m_packetCount;
    VideoPlayerWebm* m_parent;
    uint64_t m_framesDecoded;
    aperture::core::CoreBuffer<float>* m_buffer;

  public:
    AudioDecoderVPX(VideoPlayerWebm* ParentPlayer, size_t* BufferSize);
    ~AudioDecoderVPX();

    void init();
    bool initHeader(unsigned char* data, size_t size);
    bool postInit();

    void resetDecode();
    bool decodeHeader(const unsigned char* data, size_t length);
    int decode(const unsigned char* aData, size_t aLength,
      int64_t aOffset, uint64_t aTstampUsecs, uint64_t aTstampDuration,
      int64_t aDiscardPadding, int32_t* aTotalFrames);
    long rate();
    int channels();

    float decodedTime();
    size_t bufferSize() const;
  };
} // namespace wdvideo

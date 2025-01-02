#include "APHTML/WDVideo/Audio/AudioDecoderVPX.h"
#include "APHTML/WDVideo/Video/VideoPlayerWebm.h"

#include <APHTML/Interfaces/APCUtils.h>
#include <APHTML/Interfaces/APCPlatform.h>


#include <algorithm>
#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>

namespace wdvideo
{
  static uint64_t ne_xiph_lace_value(unsigned char** np)
  {
    uint64_t lace;
    uint64_t value;
    unsigned char* p = *np;

    lace = *p++;
    value = lace;
    while (lace == 255)
    {
      lace = *p++;
      value += lace;
    }

    *np = p;

    return value;
  }

  ogg_packet InitOggPacket(const unsigned char* aData, size_t aLength,
    bool aBOS, bool aEOS,
    int64_t aGranulepos, int64_t aPacketNo)
  {
    ogg_packet packet;
    packet.packet = const_cast<unsigned char*>(aData);
    packet.bytes = (long)aLength;
    packet.b_o_s = aBOS;
    packet.e_o_s = aEOS;
    packet.granulepos = aGranulepos;
    packet.packetno = aPacketNo;
    return packet;
  }

  AudioDecoderVPX::AudioDecoderVPX(VideoPlayerWebm* ParentPlayer, size_t* BufferSize)
    : m_packetCount(0)
    , m_framesDecoded(0)
  {
    memset(&m_vorbisBlock, 0, sizeof(m_vorbisBlock));
    memset(&m_vorbisDsp, 0, sizeof(m_vorbisDsp));
    memset(&m_vorbisInfo, 0, sizeof(m_vorbisInfo));
    memset(&m_vorbisComment, 0, sizeof(m_vorbisComment));

    m_buffer = new aperture::core::CoreBuffer<float>(bufferSize());
  }

  AudioDecoderVPX::~AudioDecoderVPX()
  {
    aperture::core::SafeDelete<aperture::core::CoreBuffer<float>>(m_buffer);

    vorbis_block_clear(&m_vorbisBlock);
    vorbis_dsp_clear(&m_vorbisDsp);
    vorbis_info_clear(&m_vorbisInfo);
    vorbis_comment_clear(&m_vorbisComment);
  }

  void AudioDecoderVPX::init()
  {
    vorbis_info_init(&m_vorbisInfo);
    vorbis_comment_init(&m_vorbisComment);
    memset(&m_vorbisBlock, 0, sizeof(m_vorbisBlock));
    memset(&m_vorbisDsp, 0, sizeof(m_vorbisDsp));
  }

  bool AudioDecoderVPX::initHeader(unsigned char* data, size_t size)
  {
    if (size < 1)
      return false;

    nsLog::Error("using vorbis: {0}", vorbis_version_string());

    m_packetCount = 0;

#if 1
    unsigned char c = data[0] + 1;

    if (c > 3)
      return false;

    nsLog::Info("vorbis header packets: %d", c);

    unsigned char sizes[8] = {0};
    unsigned int totalSize = 0;

    for (int i = 0; i < c; i++)
    {
      unsigned char _size = data[1 + i];
      sizes[i] = _size;
      totalSize += _size;
      nsLog::Info("vorbis header packet %d size: %d", i, _size);
    }

    data += c;

    unsigned char type = data[0];

    nsLog::Info("vorbis header packet 0 type: %d (%c %c %c %c %c %c)", type, data[1], data[2], data[3], data[4], data[5], data[6]);
    if (!decodeHeader(data, sizes[0]))
      return false;

    data += sizes[0];
    type = data[0];

    nsLog::Info("vorbis header packet 1 type: %d (%c %c %c %c %c %c)", type, data[1], data[2], data[3], data[4], data[5], data[6]);
    if (!decodeHeader(data, sizes[1]))
      return false;

    data += sizes[1];
    type = data[0];

    int size3 = size - totalSize;
    nsLog::Info("vorbis header packet 2 size: %d", size3);

    nsLog::Info("vorbis header packet 2 type: %d (%c %c %c %c %c %c)", type, data[1], data[2], data[3], data[4], data[5], data[6]);
    if (!decodeHeader(data, size3))
      return false;
#else
    for (int i = 0; i < 3; i++)
    {
      while ((size - 6) && memcmp(data, "vorbis", 6))
      {
        data++;
        size--;
      }

      nsLog::Info("vorbis header packet: %d size: %d", i, size);

      unsigned char* oggPacket = data;
      oggPacket--;

      if (i == 2)
        size = size + 1;

      if (!decodeHeader(oggPacket, size))
        return false;

      data++;
    }
#endif

    return true;
  }

  bool AudioDecoderVPX::postInit()
  {
    int r = vorbis_synthesis_init(&m_vorbisDsp, &m_vorbisInfo);
    if (r)
    {
      return false;
    }

    r = vorbis_block_init(&m_vorbisDsp, &m_vorbisBlock);
    if (r)
    {
      return false;
    }

    return true;
  }

  void AudioDecoderVPX::resetDecode()
  {
    // vorbis_synthesis_restart(&mVorbisDsp);

    m_framesDecoded = 0;
  }

  bool AudioDecoderVPX::decodeHeader(const unsigned char* data, size_t length)
  {
    bool bos = m_packetCount == 0;
    bool eos = false;
    int r = 0;

    ogg_packet pkt = InitOggPacket(data, length, bos, eos, 0, m_packetCount);
    r = vorbis_synthesis_headerin(&m_vorbisInfo, &m_vorbisComment, &pkt);

    nsLog::Info("%d: %d (%s)", m_packetCount, r, m_vorbisComment.vendor);

    m_packetCount++;

    return r == 0;
  }

  long AudioDecoderVPX::rate()
  {
    return m_vorbisInfo.rate;
  }

  int AudioDecoderVPX::channels()
  {
    return m_vorbisInfo.channels;
  }

  int AudioDecoderVPX::decode(const unsigned char* aData, size_t aLength,
    int64_t aOffset, uint64_t aTstampUsecs, uint64_t aTstampDuration,
    int64_t aDiscardPadding, int32_t* aTotalFrames)
  {
    if (m_packetCount < 3)
      return 1;

    ogg_packet pkt = InitOggPacket(aData, aLength, false, false, -1, m_packetCount++);
    bool first_packet = m_packetCount == 4;

    int vs_ret = vorbis_synthesis(&m_vorbisBlock, &pkt);
    if (vs_ret)
    {
      if (vs_ret == OV_ENOTAUDIO)
        return 2;
      else if (vs_ret == OV_EBADPACKET)
        return 3;
      else
        return -1;
    }

    if (vorbis_synthesis_blockin(&m_vorbisDsp,
          &m_vorbisBlock))
    {
      return 4;
    }

    float** pcm = 0;
    int32_t frames = vorbis_synthesis_pcmout(&m_vorbisDsp, &pcm);

    if (frames == 0 && first_packet)
    {
    }

    while (frames > 0)
    {
      uint32_t channels = m_vorbisDsp.vi->channels;

      size_t bufferSize = frames * channels;
      float* buffer = m_buffer->get(bufferSize);

      for (uint32_t i = 0; i < channels; i++)
      {
        float* ptr = buffer + i;
        float* mono = pcm[i];

        for (int j = 0; j < frames; j++)
        {
          float val = mono[j];

          *ptr = val;
          ptr += channels;
        }
      }

      *aTotalFrames += frames;
      m_framesDecoded += frames;

      // call callback
      m_parent->addAudioData(buffer, bufferSize);

      if (vorbis_synthesis_read(&m_vorbisDsp, frames))
      {
        return 5;
      }

      frames = vorbis_synthesis_pcmout(&m_vorbisDsp, &pcm);
    }

    return 0;
  }

  float AudioDecoderVPX::decodedTime()
  {
    return (float)m_framesDecoded / (float)m_vorbisInfo.rate;
  }

  size_t AudioDecoderVPX::bufferSize() const
  {
    return m_buffer->size();
  }
} // namespace wdvideo

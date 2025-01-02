/*
 *   Copyright (c) 2023 Watch Dogs LLC
 *   All rights reserved.
 *   You are only allowed access to this code, if given WRITTEN permission by WD Studios L.L.C.
 */

#pragma once


#include <vpx/vp8dx.h>
#include <vpx/vpx_decoder.h>

#include <APHTML/Interfaces/APCFileSystem.h>
#include <APHTML/Interfaces/Internal/APCObjectPool.h>
#include <Foundation/Time/Stopwatch.h>
#include <APHTML/WDVideo/Audio/AudioDecoderVPX.h>
#include <APHTML/WDVideo/Interface/IWDVFileInterface.h>
#include <APHTML/WDVideo/Interface/IWDVFrameBuffer.h>
#include <APHTML/WDVideo/Interface/IWDVPacket.h>
#include <APHTML/WDVideo/Interface/IWDVPacketQueue.h>
#include <APHTML/WDVideo/Interface/IWDVideoPlayer.h>
#include <libsimplewebm/libwebm/mkvparser/mkvparser.h>
#include <mutex>
#include <thread>
#include <APHTML/WDVideo/WDVEngineDLL.h>

namespace wdvideo
{
  static const double NS_PER_S = 1e9;

  class NS_APERTURE_DLL VideoPlayerWebm : public IWDVVideoPlayer
  {
    typedef mkvparser::Segment seg_t;

    struct VpxData
    {
      vpx_codec_ctx_t codec;
      vpx_codec_iter_t iter;
      vpx_image_t* img;
      vpx_codec_iface_t* iface;
      vpx_codec_flags_t flags;
      bool initialized;
    };

  private:
    bool m_initialized;

    OnAudioDataDecoded m_onAudioDataDecoded;
    void* m_onAudioDataDecodedUserPtr;
    OnVideoFinished m_onVideoFinished;

    void reset();
    void destroy();
    void updateYUVData(double time);

    // decoding
    VpxData m_decoderData;
    wdvideo::IWDVFileInterface* m_reader;
    mkvparser::EBMLHeader m_header;
    std::unique_ptr<seg_t> m_segment;
    const mkvparser::Cluster* m_cluster;
    const mkvparser::Tracks* m_tracks;
    AudioDecoderVPX* m_audioDecoder;
    aperture::core::CoreBuffer<unsigned char>* m_decodeBuffer;
    IWDVFrameBuffer* m_frameBuffer;
    const mkvparser::BlockEntry* m_blockEntry;
    long m_audioTrackIdx;
    bool m_hasVideo;
    bool m_hasAudio;
    std::string m_fileRoot;
    std::atomic<size_t> m_framesDecoded;
    nsStopwatch m_timer;

    std::atomic<double> m_playTime;

    // threading
    std::thread m_thread;
    std::mutex m_updateMutex;
    std::atomic<bool> m_threadRunning;
    char m_threadErrorDesc[UVPX_THREAD_ERROR_DESC_SIZE];

    std::thread startDecodingThread();
    void stopDecodingThread();
    void decodingThread();
    void threadError(int errorState, const char* format, ...);

    IWDVPacketQueue m_videoQueue;
    IWDVPacketQueue m_audioQueue;

    IWDVPacket* demuxPacket();
    IWDVPacket* getPacket(IWDVPacket::Type type);
    void decodePacket(IWDVPacket* p);

    aperture::core::ObjectPool<IWDVPacket>* m_packetPool;

  public:
    enum class State
    {
      Uninitialized,
      Initialized,
      Buffering,
      Playing,
      Paused,
      Stopped,
      Finished
    };
    std::atomic<State> m_state;

    IWDVVideoPlayer::VideoInfo m_info;

    IWDVVideoPlayer::Config m_config;

  public:
    VideoPlayerWebm(const IWDVVideoPlayer::Config& cfg);
    ~VideoPlayerWebm();

    virtual IWDVVideoPlayer::LoadResult load(const char* fileName, int audioTrack, bool preloadFile) override;
    virtual bool update(float dt);

    virtual IWDVVideoPlayer::VideoInfo info() const override;
    IWDVFrameBuffer* frameBuffer();

    virtual void setOnAudioData(OnAudioDataDecoded func, void* userPtr) override;
    virtual void setOnVideoFinished(OnVideoFinished func) override;

    virtual double playTime() override;
    virtual float duration() override;

    virtual void play() override;
    virtual void pause() override;
    virtual void stop() override;
    virtual bool isStopped() override;
    virtual bool isPaused() override;
    virtual bool isPlaying() override;

    void addAudioData(float* values, size_t count);

    static const IWDVVideoPlayer::Config& defaultConfig();
    virtual bool readStats(IWDVVideoPlayer::Statistics* dst) override;
  };
} // namespace wdvideo

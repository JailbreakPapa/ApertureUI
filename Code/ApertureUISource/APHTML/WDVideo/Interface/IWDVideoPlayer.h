/*
 *   Copyright (c) 2023 Watch Dogs LLC
 *   All rights reserved.
 *   You are only allowed access to this code, if given WRITTEN permission by WD Studios L.L.C.
 */

#pragma once
#include <APHTML/APEngineDLL.h>
#include <APHTML/WDVideo/Interface/IWDVFrame.h>

namespace wdvideo
{
  typedef void (*OnAudioDataDecoded)(void* userPtr, float* values, size_t count);
  typedef void (*OnVideoFinished)();
  typedef void (*DebugLogFuncPtr)(const char* msg);
  class VideoPlayer;
  class IWDVVideoPlayer
  {
  protected:
    VideoPlayer* m_videoPlayer;

  public:
    struct Config
    {
      const char* fileRoot;
      int decodeThreadsCount;
      int videoDecodeBufferSize;
      int audioDecodeBufferSize;
      int frameBufferCount;

      Config()
        : fileRoot(nullptr)
        , decodeThreadsCount(1)
        , videoDecodeBufferSize(2 * 1024 * 1024)
        , audioDecodeBufferSize(4 * 1024)
        , frameBufferCount(4)
      {
      }
    };

    struct Statistics
    {
      int framesDecoded;
      int videoBufferSize;
      int audioBufferSize;
    };

    struct VideoInfo
    {
      int width;
      int height;
      float duration;
      float frameRate;
      int hasAudio;
      int audioChannels;
      int audioFrequency;
      int audioSamples;
      int decodeThreadsCount;
    };

    struct TextureInfo
    {
      int width;
      int height;
      void* texture;
    };

    enum class LoadResult
    {
      Success = 0,
      FileNotExists,
      FailedParseHeader,
      FailedCreateInstance,
      FailedLoadSegment,
      FailedGetSegmentInfo,
      FailedInitializeVideoDecoder,
      FailedDecodeAudioHeader,
      FailedInitializeAudioDecoder,
      UnsupportedVideoCodec,
      NotInitialized,
      InternalError
    };

  public:
    IWDVVideoPlayer() = default;
    IWDVVideoPlayer(const Config& cfg);
    ~IWDVVideoPlayer();

    virtual LoadResult load(const char* fileName, int audioTrack, bool preloadFile);
    virtual bool update(float dt);

    virtual VideoInfo info() const;

    virtual IWDVFrame* lockRead();
    virtual void unlockRead();

    virtual void setOnAudioData(OnAudioDataDecoded func, void* userPtr);
    virtual void setOnVideoFinished(OnVideoFinished func);

    virtual double playTime();
    virtual float duration();

    virtual void play();
    virtual void pause();
    virtual void stop();
    virtual bool isStopped();
    virtual bool isPaused();
    virtual bool isPlaying();

    static const Config& defaultConfig();
    virtual bool readStats(Statistics* dst);

    static void setDebugLog(DebugLogFuncPtr func);
  };
} // namespace wdvideo

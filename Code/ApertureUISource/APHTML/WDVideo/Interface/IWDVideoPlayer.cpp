#include "IWDVideoPlayer.h"

wdvideo::IWDVVideoPlayer::IWDVVideoPlayer(const Config& cfg)
{
}

wdvideo::IWDVVideoPlayer::~IWDVVideoPlayer()
{
}

wdvideo::IWDVVideoPlayer::LoadResult wdvideo::IWDVVideoPlayer::load(const char* fileName, int audioTrack, bool preloadFile)
{
  return LoadResult();
}

bool wdvideo::IWDVVideoPlayer::update(float dt)
{
  return false;
}

wdvideo::IWDVVideoPlayer::VideoInfo wdvideo::IWDVVideoPlayer::info() const
{
  return VideoInfo();
}

wdvideo::IWDVFrame* wdvideo::IWDVVideoPlayer::lockRead()
{
  return nullptr;
}

void wdvideo::IWDVVideoPlayer::unlockRead()
{
}

void wdvideo::IWDVVideoPlayer::setOnAudioData(OnAudioDataDecoded func, void* userPtr)
{
}

void wdvideo::IWDVVideoPlayer::setOnVideoFinished(OnVideoFinished func)
{
}

double wdvideo::IWDVVideoPlayer::playTime()
{
  return 0.0;
}

float wdvideo::IWDVVideoPlayer::duration()
{
  return 0.0f;
}

void wdvideo::IWDVVideoPlayer::play()
{
}

void wdvideo::IWDVVideoPlayer::pause()
{
}

void wdvideo::IWDVVideoPlayer::stop()
{
}

bool wdvideo::IWDVVideoPlayer::isStopped()
{
  return false;
}

bool wdvideo::IWDVVideoPlayer::isPaused()
{
  return false;
}

bool wdvideo::IWDVVideoPlayer::isPlaying()
{
  return false;
}

const wdvideo::IWDVVideoPlayer::Config& wdvideo::IWDVVideoPlayer::defaultConfig()
{
  return Config();
}

bool wdvideo::IWDVVideoPlayer::readStats(Statistics* dst)
{
  return false;
}

void wdvideo::IWDVVideoPlayer::setDebugLog(DebugLogFuncPtr func)
{
}

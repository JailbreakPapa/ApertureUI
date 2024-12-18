#include "IWDVPacket.h"

wdvideo::IWDVPacket::IWDVPacket(const mkvparser::Block* block, Type type, double time)
{
}

wdvideo::IWDVPacket::~IWDVPacket()
{
}

wdvideo::IWDVPacket::Type wdvideo::IWDVPacket::type() const
{
  return Type();
}

const mkvparser::Block* wdvideo::IWDVPacket::block() const
{
  return nullptr;
}

double wdvideo::IWDVPacket::time() const
{
  return 0.0;
}

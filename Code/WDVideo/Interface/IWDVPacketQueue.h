/*
 *   Copyright (c) 2023 Watch Dogs LLC
 *   All rights reserved.
 *   You are only allowed access to this code, if given WRITTEN permission by WD Studios L.L.C.
 */

#include <queue>
#include <WDVideo/Interface/IWDVPacket.h>
namespace wdvideo
{
  class IWDVPacketQueue
  {
  protected:
    std::queue<IWDVPacket*> m_queue;

  public:
    IWDVPacketQueue() {}
    ~IWDVPacketQueue() {}

    void enqueue(IWDVPacket* val)
    {
      m_queue.push(val);
    }

    void pop()
    {
      m_queue.pop();
    }

    void destroy()
    {
      while (!m_queue.empty())
        m_queue.pop();
    }

    size_t size() const
    {
      return m_queue.size();
    }

    bool empty() const
    {
      return size() == 0;
    }

    IWDVPacket* first()
    {
      if (m_queue.size() > 0)
        return m_queue.front();

      return nullptr;
    }
  };
} // namespace wdvideo

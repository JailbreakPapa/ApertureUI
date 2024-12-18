/*
 *   Copyright (c) 2023 Watch Dogs LLC
 *   All rights reserved.
 *   You are only allowed access to this code, if given WRITTEN permission by WD Studios L.L.C.
 */

#pragma once
#include <Foundation/Communication/IpcChannel.h>
#include <Foundation/Communication/RemoteInterface.h>
#include <NetworkEngine/NETEngineDLL.h>


namespace aperture::networking
{
  class nsRemoteInterface;
  class nsRemoteMessage;
  class NS_NETENGINE_DLL nsIpcChannelHTTP : public nsIpcChannel
  {
    nsIpcChannelHTTP(nsStringView sAddress, Mode::Enum mode);
    ~nsIpcChannelHTTP();

  protected:
    virtual void InternalConnect() override;
    virtual void InternalDisconnect() override;
    virtual void InternalSend() override;
    virtual bool NeedWakeup() const override;
    virtual bool RequiresRegularTick() override { return true; }
    virtual void Tick() override;
    void NetworkMessageHandler(nsRemoteMessage& msg);
    void HTTPEventHandler(const nsRemoteEvent& e);

    nsString m_sAddress;
    nsString m_sLastAddress;
    nsTime m_LastConnectAttempt;
    nsUniquePtr<nsRemoteInterface> m_pNetwork;
  };
} // namespace aperture::networking
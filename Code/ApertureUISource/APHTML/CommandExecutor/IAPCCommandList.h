/*
 *   Copyright (c) 2024 WD Studios L.L.C.
 *   All rights reserved.
 *   You are only allowed access to this code, if given WRITTEN permission by WD Studios L.L.C.
 */
#pragma once

#include <APHTML/APEngineDLL.h>
#include <APHTML/CommandExecutor/IAPCCommandCommon.h>
#include <APHTML/APEngineCommonIncludes.h>
#include <Foundation/Types/UniquePtr.h>
namespace aperture::core
{
  class IAPCCommand;
  class IAPCCommandQueue;
  class NS_APERTURE_DLL IAPCCommandList
  {
  public:
    IAPCCommandList() = default;
    virtual ~IAPCCommandList() = default;

    IAPCCommandList(IAPCCommandQueue& in_queue)
      : m_queue(&in_queue)
    {
    }

    virtual bool VerifyAndCommitCommands();

    IAPCCommandQueue* GetQueue() const { return m_queue; }
    
    void AddCommand(IAPCCommand& command)
    {
      m_commands.PushBack(std::move(&command));
    }

    CommandType GetType() const { return m_type; }
    void SetType(CommandType type) { m_type = type; }

    Runtype GetRunType() const { return m_runtype; }
    void SetRunType(Runtype runtype) { m_runtype = runtype; }

    nsHybridArray<IAPCCommand*, 1>& GetCommands() { return m_commands; }

  private:
    core::CommandType m_type;
    core::Runtype m_runtype;
    nsHybridArray<IAPCCommand*, 1> m_commands;
    IAPCCommandQueue* m_queue;
  };
} // namespace aperture::core

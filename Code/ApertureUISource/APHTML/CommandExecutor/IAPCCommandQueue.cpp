#include <APHTML/CommandExecutor/IAPCCommand.h>
#include <APHTML/CommandExecutor/IAPCCommandList.h>
#include <APHTML/CommandExecutor/IAPCCommandQueue.h>

void aperture::core::IAPCCommandQueue::AddLoneCommandList(IAPCCommandList& commandList)
{
  if (commandList.VerifyAndCommitCommands() && commandList.GetRunType() == core::Runtype::AnyThread)
  {
    ExecuteLoneCommandList(commandList);
    m_commandLists.PushBack(&commandList);
  }
  else
  {
    nsLog::Error("CommandList of Type: {0}, Has a bad command or is locked somehow.", CommandTypeToString(commandList.GetType()));
    // Break here, so we can debug.
    NS_DEBUG_BREAK;
  }
}
void aperture::core::IAPCCommandQueue::AddCommandList(IAPCCommandList& commandList)
{
  if (commandList.VerifyAndCommitCommands() && commandList.GetRunType() == GetRunType())
  {
    ExecuteLoneCommandList(commandList);
    m_commandLists.PushBack(&commandList);
  }
  else
  {
    nsLog::Error("CommandList of Type: {0}, Has a bad command, is locked somehow or has a bad Run Type.", CommandTypeToString(commandList.GetType()));
    // Break here, so we can debug.
    NS_DEBUG_BREAK;
  }
}
void aperture::core::IAPCCommandQueue::ExecuteLoneCommandList(IAPCCommandList& commandList)
{
  for (auto& command : commandList.GetCommands())
  {
    if (command && command->GetCommandType() == commandList.GetType())
    {
      command->SetParentCommandList(commandList);
      command->Execute();
    }
    else
    {
      nsLog::Error("CommandQueue Execution Failure: Command of Type: {0}, Failed to execute/failed, Has a bad function, is locked somehow, or has a bad CommandType.", CommandTypeToString(command->GetCommandType()));
      // Break here, so we can debug.
      NS_DEBUG_BREAK;
    }
  }
}
void aperture::core::IAPCCommandQueue::ExecuteResidentCommandList(nsUInt8 m_iIndex)
{
}
void aperture::core::IAPCCommandQueue::ClearQueue()
{
}
nsResult aperture::core::IAPCCommandQueue::Execute(bool m_bExecWithRespectiveOfPriority)
{
  for (auto& commandList : m_commandLists)
  {
    if (commandList->VerifyAndCommitCommands() && commandList->GetRunType() == GetRunType())
    {
      for (auto& command : commandList->GetCommands())
      {
        if (command && command->GetCommandType() == commandList->GetType())
        {
          command->SetParentCommandList(*commandList);
          command->Execute();
        }
        else
        {
          nsLog::Error("CommandQueue Execution Failure: Command of Type: {0}, Failed to execute/failed, Has a bad function, is locked somehow, or has a bad CommandType.", CommandTypeToString(command->GetCommandType()));
          // Break here, so we can debug.
          NS_DEBUG_BREAK;
        }
      }
    }
    else
    {
      nsLog::Error("CommandList of Type: {0}, Has a bad command, is locked somehow or has a bad Run Type.", CommandTypeToString(m_commandLists[m_commandLists.IndexOf(commandList)]->GetType()));
      // Break here, so we can debug.
      NS_DEBUG_BREAK;
    }
  }
  return NS_SUCCESS;
}

nsResult aperture::core::IAPCCommandQueue::RequestLock()
{
  if (m_mutex.TryLock() == NS_SUCCESS)
  {
    return NS_SUCCESS;
  }
  return NS_FAILURE;
}

void aperture::core::IAPCCommandQueue::ReleaseLock()
{
  m_mutex.Unlock();
}


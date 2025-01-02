#include <APHTML/CommandExecutor/IAPCCommand.h>
#include <APHTML/CommandExecutor/IAPCCommandList.h>
#include <APHTML/CommandExecutor/IAPCCommandQueue.h>

bool aperture::core::IAPCCommandList::VerifyAndCommitCommands()
{
  for (auto& command : m_commands)
  {
    if (command && command->GetCommandType() == GetType())
    {
      command->SetParentCommandList(*this);
    }
    else
    {
      nsLog::Error("Command of Type: {0}, Has a bad function, is locked somehow, or has a bad CommandType.", CommandTypeToString(command->GetCommandType()));
      // Break here, so we can debug.
      NS_DEBUG_BREAK;
      return false;
    }
  }
  return true;
}

#include <APHTML/CommandExecutor/IAPCCommand.h>
#include <APHTML/CommandExecutor/IAPCCommandList.h>
#include <APHTML/CommandExecutor/IAPCCommandQueue.h>


nsResult aperture::core::IAPCCommand::RequestCommandQueueLock()
{
  if (m_parentCommandList->GetQueue()->RequestLock() == NS_SUCCESS)
  {
    return NS_SUCCESS;
  }
  return NS_FAILURE;
}


nsResult aperture::core::IAPCCommand::RequestCommandQueueUnlock()
{
  m_parentCommandList->GetQueue()->ReleaseLock();
  return NS_SUCCESS;
}
void aperture::core::IAPCCommand::SetRunType(Runtype runtype)
{
  if (!IsExecuting())
  {
    if (RequestCommandQueueLock() == NS_SUCCESS)
    {
      m_internalruntype = runtype;
    }
  }
  if (RequestCommandQueueUnlock() == NS_SUCCESS)
  {
  }
}

void aperture::core::IAPCCommand::SetCommandType(CommandType commandtype)
{
  if (!IsExecuting())
  {
    if (RequestCommandQueueLock() == NS_SUCCESS)
    {
      m_internaltype = commandtype;
    }
  }
  if (RequestCommandQueueUnlock() == NS_SUCCESS)
  {
  }
}

#pragma once

#include <APHTML/APEngineDLL.h>
#include <APHTML/CommandExecutor/IAPCCommandCommon.h>
#include <APHTML/APEngineCommonIncludes.h>
#include <functional>
#include <future>

namespace aperture::core
{
  class IAPCCommandList;
  /**
   * @class IAPCCommand
   * @brief Interface for defining and executing commands within the Aperture SDK.
   *
   * The IAPCCommand class provides a framework for creating and managing commands that can be executed
   * within different threads and contexts in the Aperture SDK. It supports various command types
   * and runtimes, and allows for custom functionality to be defined and executed.
   */
  class NS_APERTURE_DLL IAPCCommand
  {
  public:
    explicit IAPCCommand(CommandType type = CommandType::Unknown, Runtype runtype = Runtype::AnyThread)
      : m_internaltype(type)
      , m_internalruntype(runtype)
      , m_barewexecuting(false)
    {
    }
    virtual ~IAPCCommand() = default;

    void SetParentCommandList(IAPCCommandList& queue) { m_parentCommandList = &queue; }
    
    virtual nsResult RequestCommandQueueLock();
    virtual nsResult RequestCommandQueueUnlock();

    void SetRunType(Runtype runtype);
    void SetCommandType(CommandType commandtype);

    CommandType GetCommandType() const { return m_internaltype; }
    Runtype GetRunType() const { return m_internalruntype; }

    nsVariant GetEndResult() const { return endresult; }

    template <typename Func>
    void SetFunction(Func&& func)
    {
      m_function = std::forward<Func>(func);
    }

    void Execute()
    {
      if (m_function)
      {
        m_barewexecuting = true;
        m_function();
        m_barewexecuting = false;
      }
    }

    bool IsExecuting() const { return m_barewexecuting; }

  private:
    bool m_barewexecuting;
    nsVariant endresult;
    CommandType m_internaltype;
    Runtype m_internalruntype;
    std::function<void()> m_function;
    IAPCCommandList* m_parentCommandList = nullptr;
  };
} // namespace aperture::core

/*
 *   Copyright (c) 2023 Watch Dogs LLC
 *   All rights reserved.
 *   You are only allowed access to this code, if given WRITTEN permission by WD Studios L.L.C.
 */

#pragma once

#include <APCore/COREAPEngineDLL.h>
#include <APCore/Interfaces/Internal/APCThreadSafeQueue.h>
#include <chrono>
#include <condition_variable>
#include <cstdarg>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <sstream>
#include <thread>
#include <vector>

namespace aperture::core
{
  class IAPCLogInterface : public nsLogInterface
  {
  public:
    virtual void HandleLogMessage(const nsLoggingEventData& le) override
    {
      switch (le.m_EventType)
      {
        case nsLogMsgType::Flush:
          m_Result.Append("[Flush]\n");
          return;
        case nsLogMsgType::BeginGroup:
          m_Result.Append(">", le.m_sTag, " ", le.m_sText, "\n");
          break;
        case nsLogMsgType::EndGroup:
          m_Result.Append("<", le.m_sTag, " ", le.m_sText, "\n");
          break;
        case nsLogMsgType::ErrorMsg:
          m_Result.Append("Aperture Log: ERROR:", le.m_sTag, " ", le.m_sText, "\n");
          break;
        case nsLogMsgType::SeriousWarningMsg:
          m_Result.Append("Aperture Log:  SeriousWarning:", le.m_sTag, " ", le.m_sText, "\n");
          break;
        case nsLogMsgType::WarningMsg:
          m_Result.Append("Aperture Log: Warning:", le.m_sTag, " ", le.m_sText, "\n");
          break;
        case nsLogMsgType::SuccessMsg:
          m_Result.Append("Aperture Log: Success:", le.m_sTag, " ", le.m_sText, "\n");
          break;
        case nsLogMsgType::InfoMsg:
          m_Result.Append("Aperture Log: Info:", le.m_sTag, " ", le.m_sText, "\n");
          break;
        case nsLogMsgType::DevMsg:
          m_Result.Append("Aperture Log: DevMsg:", le.m_sTag, " ", le.m_sText, "\n");
          break;
        case nsLogMsgType::DebugMsg:
          m_Result.Append("Aperture Log: Debug:", le.m_sTag, " ", le.m_sText, "\n");
          break;

        default:
          NS_REPORT_FAILURE("Invalid msg type");
          break;
      }
    }

    nsStringBuilder m_Result;
  };

  class NS_COREAPENGINE_DLL IAPCLoggingSystem
  {
  public:
    IAPCLoggingSystem()
    {
      running_ = true;
      worker_thread_ = std::thread(&IAPCLoggingSystem::processQueue, this);
    }

    virtual ~IAPCLoggingSystem()
    {
      {
        std::lock_guard<std::mutex> lock(mutex_);
        running_ = false;
        cv_.notify_one();
      }
      if (worker_thread_.joinable())
      {
        worker_thread_.join();
      }
    }

    virtual void LogError(const char* message, ...) = 0;
    virtual void LogWarning(const char* message, ...) = 0;
    virtual void LogInfo(const char* message, ...) = 0;
    virtual void LogFatalError(const char* message, ...) = 0;

    std::vector<std::string> GetMessageLog()
    {
      std::lock_guard<std::mutex> lock(mutex_);
      return message_log_;
    }

  protected:
    std::vector<std::string> message_log_;
    mutable std::mutex mutex_;
    std::condition_variable cv_;
    std::queue<std::string> log_queue_;
    std::atomic<bool> running_;
    std::thread worker_thread_;

    void addLogMessage(const std::string& message)
    {
      {
        std::lock_guard<std::mutex> lock(mutex_);
        log_queue_.push(message);
      }
      cv_.notify_one();
    }

  private:
    void processQueue()
    {
      while (running_)
      {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this]
          { return !log_queue_.empty() || !running_; });

        while (!log_queue_.empty())
        {
          std::string log_entry = log_queue_.front();
          log_queue_.pop();
          std::cout << log_entry << std::endl;
          message_log_.push_back(log_entry);
        }
      }
    }
  };

  class NS_COREAPENGINE_DLL DefaultLoggingSystem : public IAPCLoggingSystem
  {
  public:
    template <typename... Args>
    std::future<void> LogAsync(const std::string& level, const char* format, Args... args)
    {
      return std::async(std::launch::async, [this, level, format, args...]()
        { Log(level, format, args...); });
    }


    void LogError(const char* message, ...) override
    {
      va_list args;
      va_start(args, message);
      LogAsync("ERROR", message, args).get();
      va_end(args);
    }

    void LogWarning(const char* message, ...) override
    {
      va_list args;
      va_start(args, message);
      LogAsync("WARNING", message, args).get();
      va_end(args);
    }

    void LogInfo(const char* message, ...) override
    {
      va_list args;
      va_start(args, message);
      LogAsync("INFO", message, args).get();
      va_end(args);
    }

    void LogFatalError(const char* message, ...) override
    {
      va_list args;
      va_start(args, message);
      LogAsync("FATAL", message, args).get();
      va_end(args);
    }

  private:
    template <typename... Args>
    void Log(const std::string& level, const char* format, Args... args)
    {
      std::string formatted_message = FormatMessage(format, args...);
      std::string log_entry = "[" + level + "] " + formatted_message;
      addLogMessage(log_entry);
    }

    template <typename... Args>
    std::string FormatMessage(const char* format, Args... args)
    {
      size_t size = snprintf(nullptr, 0, format, args...) + 1;
      std::unique_ptr<char[]> buf(new char[size]);
      snprintf(buf.get(), size, format, args...);
      return std::string(buf.get(), buf.get() + size - 1);
    }
  };
} // namespace aperture::core

/*
 *   Copyright (c) 2023 Watch Dogs LLC
 *   All rights reserved.
 *   You are only allowed access to this code, if given WRITTEN permission by WD Studios L.L.C.
 */

#pragma once

#include <APCore/COREAPEngineDLL.h>
#include <APCore/Interfaces/APCPlatform.h>
#include <chrono>
// TODO: sigh....
namespace aperture::core
{
  /**
   * @brief The interface for the APCProfilerSystem class.
   * This class provides an interface for a profiler system of any kind. all you need to do is override it.
   * By default, the profiler system uses the Foundation's Profiler System to profile the application.
   * You can override it there or here, doesnt matter!
   *
   * @note The Foundation Profiler System Has implementations for Tracy, Googles JSON Based Profiling System, and Superluminal(Requires License!)
   */
  static class NS_COREAPENGINE_DLL IAPCProfilerSystem
  {
  public:
    // TODO: Look for a GlobalEvent with "BeginFrame" to trigger the function?

    virtual void BeginFrame() = 0;

    virtual void EndFrame() = 0;

    // TODO: Look for way to profile scopes with a function call so it can remain overrideable.
  };

  /**
   * @class IAPCLocalBenchmark
   * @brief Represents a local benchmark for measuring the duration of a code block.
   *
   * This class is used to measure the duration of a code block and compare it with a desired duration.
   * If the actual duration exceeds the desired duration, a warning message is logged.
   */
  static class NS_COREAPENGINE_DLL IAPCLocalBenchmark
  {
  public:
    explicit IAPCLocalBenchmark(const char* p_name, const char* p_file, int p_line, nsUInt64 p_startTime, nsUInt64 p_desiredDuration)
      : m_name(p_name)
      , m_file(p_file)
      , m_line(p_line)
      , m_startTime(p_startTime)
      , m_desiredDuration(p_desiredDuration)
    {
      m_startTime = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    }
    ~IAPCLocalBenchmark()
    {
      nsUInt64 endTime = std::chrono::high_resolution_clock::now().time_since_epoch().count();
      nsUInt64 duration = endTime - m_startTime;
      if (duration > m_desiredDuration)
      {
        // Report to the user that we are over frame!.
        core::IAPCPlatform::GetLoggingSystem()->LogWarning("APCore: Frame Over Budget! %s:%d %s took %d ms", m_file, m_line, m_name, duration);
      }
    }

  private:
    const char* m_name;
    const char* m_file = "unknown";
    nsUInt32 m_line = 0;
    nsUInt64 m_startTime;
    nsUInt64 m_desiredDuration;
  };
} // namespace aperture::core
#define APUI_BENCHMARK_THREAD_SCOPE(RegionName, DetailName)                         \
  static nsString __String__##RegionName##DetailName = #RegionName "." #DetailName; \
  aperture::core::IAPCLocalBenchmark __Benchmark__##RegionName##DetailName(__String__##RegionName##DetailName, __FILE__, __LINE__)

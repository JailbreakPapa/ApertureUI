/*
This code is part of Aperture UI - A HTML/CSS/JS UI Middleware

Copyright (c) 2020-2024 WD Studios L.L.C. and/or its licensors. All
rights reserved in all media.

The coded instructions, statements, computer programs, and/or related
material (collectively the "Data") in these files contain confidential
and unpublished information proprietary WD Studios and/or its
licensors, which is protected by United States of America federal
copyright law and by international treaties.

This software or source code is supplied under the terms of a license
agreement and nondisclosure agreement with WD Studios L.L.C. and may
not be copied, disclosed, or exploited except in accordance with the
terms of that agreement. The Data may not be disclosed or distributed to
third parties, in whole or in part, without the prior written consent of
WD Studios L.L.C..

WD STUDIOS MAKES NO REPRESENTATION ABOUT THE SUITABILITY OF THIS
SOURCE CODE FOR ANY PURPOSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER, ITS AFFILIATES,
PARENT COMPANIES, LICENSORS, SUPPLIERS, OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OR PERFORMANCE OF THIS SOFTWARE OR SOURCE CODE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once
#include <APHTML/APEngineDLL.h>
#include <APHTML/CommandExecutor/IAPCCommandCommon.h>
#include <APHTML/APEngineCommonIncludes.h>

namespace aperture::core
{
  class IAPCCommandList;
  /**
   * @class IAPCCommandQueue
   * @brief Interface for a command queue in the Aperture Core Engine.
   *
   * This class provides an interface for managing and executing command lists.
   * It allows adding command lists to the queue, executing them, and clearing the queue.
   *
   * @note This class is part of the Aperture Core Engine and is intended for internal use.
   */
  class NS_APERTURE_DLL IAPCCommandQueue
  {
  public:
    IAPCCommandQueue() = default;
    virtual ~IAPCCommandQueue() = default;

    /**
     * @brief Adds a lone command list to the queue.
     *
     * This function adds a command list that is intended to be executed
     * independently of other command lists in the queue.
     *
     * @param commandList The command list to be added.
     */
    virtual void AddLoneCommandList(IAPCCommandList& commandList);
    /**
     * @brief Adds a command list to the queue.
     *
     * This function adds a command list to the queue for execution.
     *
     * @param commandList The command list to be added.
     */
    virtual void AddCommandList(IAPCCommandList& commandList);
    /**
     * @brief Executes a lone command list.
     *
     * This function executes a command list independently of other command lists in the queue.
     *
     * @param commandList The command list to be executed.
     */

    void ExecuteLoneCommandList(IAPCCommandList& commandList);
    /**
     * @brief Executes a resident command list by index.
     *
     * This function executes a command list from the queue based on its index.
     *
     * @param m_iIndex The index of the command list to be executed.
     */
    void ExecuteResidentCommandList(nsUInt8 m_iIndex);

    /**
     * @brief Clears the command queue.
     *
     * This function removes all command lists from the queue.
     */
    void ClearQueue();
    /**
     * @brief Executes the command queue.
     *
     * This function executes all command lists in the queue, optionally respecting their priority.
     *
     * @param m_bExecWithRespectiveOfPriority If true, executes command lists with respect to their priority.
     * @return The result of the execution.
     */
    nsResult Execute(bool m_bExecWithRespectiveOfPriority = true);

    nsResult RequestLock();

    void ReleaseLock();

    core::CommandType GetType() const { return m_type; }
    void SetType(core::CommandType type) { m_type = type; }

    core::Runtype GetRunType() const { return m_runtype; }
    void SetRunType(core::Runtype runtype) { m_runtype = runtype; }

    bool operator==(const IAPCCommandQueue& other) const
    {
      return m_type == other.m_type && m_runtype == other.m_runtype && m_commandLists == other.m_commandLists;
    }

  private:
    core::CommandType m_type;
    core::Runtype m_runtype;
    nsHybridArray<IAPCCommandList*, 1> m_commandLists; ///< The command lists in the queue.
  public:
    nsMutex m_mutex;
  };
} // namespace aperture::core

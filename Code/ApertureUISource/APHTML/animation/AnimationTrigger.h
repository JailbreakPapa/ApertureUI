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

#include <Foundation/Containers/HybridArray.h>
#include <Foundation/Containers/List.h>
#include <Foundation/Containers/Map.h>
#include <Foundation/Reflection/Reflection.h>
#include <Foundation/Types/RefCounted.h>
#include <Foundation/Types/UniquePtr.h>
/// NOTE: The DLL/PCH Header should always be included last.
#include <APHTML/APEngineDLL.h>

namespace aperture::animation
{               
/**
 * @brief Interface for animation triggers.
 *
 * This interface defines the different types of animation triggers that can be used.
 */
class NS_APERTURE_DLL IAnimationTrigger : public nsRefCounted
{
    NS_ALLOW_PRIVATE_PROPERTIES(aperture::animation::IAnimationTrigger);

public:
    enum class AnimationTriggerType : nsUInt8
    {
        None = 0,
        OnClick = 1,
        OnHover = 2,
        OnScroll = 3,
        OnLoad = 4,
        OnKeyPress = 5,
        OnCustom = 6
    };

    /**
     * @brief Default constructor for IAnimationTrigger.
     */
    explicit IAnimationTrigger()
    {
        /* code */
    }

    /**
     * @brief Get the animation trigger type.
     * @return The animation trigger type.
     */
    IAnimationTrigger::AnimationTriggerType GetAnimationTriggerType() const
    {
        return m_triggerType;
    }

    /**
     * @brief Check if the animation trigger type matches the given trigger type.
     * @param in_triggerType The trigger type to compare with.
     * @return True if the trigger types match, false otherwise.
     */
    bool IsTriggerType(IAnimationTrigger::AnimationTriggerType in_triggerType) const
    {
        return m_triggerType == in_triggerType;
    }

    /**
     * @brief Check if the animation trigger is an auto trigger.
     * @return True if the trigger is an auto trigger, false otherwise.
     */
    bool IsAutoTrigger() const
    {
        return m_triggerType == IAnimationTrigger::AnimationTriggerType::OnLoad;
    }

    /**
     * @brief Check if the animation trigger is a click trigger.
     * @return True if the trigger is a click trigger, false otherwise.
     */
    bool IsClickTrigger() const
    {
        return m_triggerType == IAnimationTrigger::AnimationTriggerType::OnClick;
    }

    /**
     * @brief Check if the animation trigger is a hover trigger.
     * @return True if the trigger is a hover trigger, false otherwise.
     */
    bool IsHoverTrigger() const
    {
        return m_triggerType == IAnimationTrigger::AnimationTriggerType::OnHover;
    }

    /**
     * @brief Check if the animation trigger is a scroll trigger.
     * @return True if the trigger is a scroll trigger, false otherwise.
     */
    bool IsScrollTrigger() const
    {
        return m_triggerType == IAnimationTrigger::AnimationTriggerType::OnScroll;
    }

    /**
     * @brief Check if the animation trigger is a key press trigger.
     * @return True if the trigger is a key press trigger, false otherwise.
     */
    bool IsKeyPressTrigger() const
    {
        return m_triggerType == IAnimationTrigger::AnimationTriggerType::OnKeyPress;
    }

    /**
     * @brief Check if the animation trigger is a custom trigger.
     * @return True if the trigger is a custom trigger, false otherwise.
     */
    bool IsCustomTrigger() const
    {
        return m_triggerType == IAnimationTrigger::AnimationTriggerType::OnCustom;
    }

private:
    IAnimationTrigger::AnimationTriggerType m_triggerType = IAnimationTrigger::AnimationTriggerType::None; ///< The type of trigger
};
} // namespace aperture::animation
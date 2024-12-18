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

#include <Foundation/Basics.h>
#include <Foundation/Math/Math.h>
#include <Foundation/Reflection/Reflection.h>
/// NOTE: The DLL/PCH Header should always be included last.
#include <APHTML/APEngineDLL.h>
#include <Foundation/Types/RefCounted.h>

namespace aperture::animation
{
            
        /**
        * @brief The AnimationBaseEvaluator class represents a generic animation evaluator.
        * 
        * This class provides basic functionality for evaluating animations over the course of a specified duration.
        * 
        * @note Aperture Can have as many animation evaluators as needed, but it is recommended to keep the number of evaluators to a minimum, as they can be computationally expensive on limited platforns.
        */
        template <typename AnimationFrame>
        class NS_APERTURE_DLL AnimationBaseEvaluator
        {
        public:
            /**
            * @brief Default constructor for the AnimationBaseEvaluator class.
            */
            AnimationBaseEvaluator()
            {
                /* code */
            }
    
            /**
            * @brief Virtual destructor for the AnimationBaseEvaluator class.
            */
            virtual ~AnimationBaseEvaluator()
            {
                /* code */
            }
    
            /**
            * @brief Evaluates the animation.
            * 
            * @param in_time The time at which to evaluate the animation.
            */
            void Evaluate(float in_time);
    
            /**
            * @brief Sets the duration of the animation.
            * 
            * @param in_duration The new duration of the animation.
            */
            void SetDuration(float in_duration)
            {
                m_duration = in_duration;
            }
    
            /**
            * @brief Gets the duration of the animation.
            * 
            * @return The duration of the animation.
            */
            float GetDuration() const
            {
                return m_duration;
            }
    
        protected:
            /**
            * @brief The duration of the animation.
            */
            float m_duration = 0.0f;
            nsDynamicArray<AnimationFrame> m_frames;
        };
}
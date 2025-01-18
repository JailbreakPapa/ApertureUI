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
#include <APHTML/APEngineCommonIncludes.h>

namespace aperture::css
{
    namespace syntaxfunctions
    {
        NS_FORCE_INLINE const char* var = "var";
        NS_FORCE_INLINE const char* calc = "calc";
        NS_FORCE_INLINE const char* min = "min";
        NS_FORCE_INLINE const char* max = "max";
        NS_FORCE_INLINE const char* clamp = "clamp";
        NS_FORCE_INLINE const char* rgb = "rgb";
        NS_FORCE_INLINE const char* rgba = "rgba";
        NS_FORCE_INLINE const char* hsl = "hsl";
        NS_FORCE_INLINE const char* hsla = "hsla";
        NS_FORCE_INLINE const char* url = "url";
        NS_FORCE_INLINE const char* linear = "linear";
        NS_FORCE_INLINE const char* radial = "radial";
        NS_FORCE_INLINE const char* linear_gradient = "linear-gradient";
        NS_FORCE_INLINE const char* radial_gradient = "radial-gradient";
        NS_FORCE_INLINE const char* conic_gradient = "conic-gradient";
        NS_FORCE_INLINE const char* repeating_linear_gradient = "repeating-linear-gradient";
        NS_FORCE_INLINE const char* repeating_radial_gradient = "repeating-radial-gradient";
        NS_FORCE_INLINE const char* repeating_conic_gradient = "repeating-conic-gradient";
        NS_FORCE_INLINE const char* cubic_bezier = "cubic-bezier";
    }
}
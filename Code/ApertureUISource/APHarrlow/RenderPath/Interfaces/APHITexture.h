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

#include <APHTML/Interfaces/Internal/APCBuffer.h>
#include <APHarrlow/APEngineDLL.h>
#include <APHarrlow/renderpath/Types.h>
#include <Foundation/Math/Vec2.h>
#include <Foundation/Types/ObjectPtr.h>
#include <Foundation/Types/VariantType.h>

namespace aperture::harrlow
{
  enum class EAlphaChannelType : nsUInt8
  {
    k_EAlphaChannelType_None,
    k_EAlphaChannelType_Normal,
    k_EAlphaChannelType_PreMultiplied,
  };
  enum class ETextureFormat : nsUInt8
  {
    TF_FormatRGBA8,
    TF_FormatBGRA8,
    TF_FormatBGR8,
    TF_FormatA8,
    TF_FormatYUV420,
    TF_FormatR16G16B16A16,
    TF_FormatDXT1,
    TF_FormatDXT5,
  };

  class NS_APERTURE_DLL APHITexture
  {
  public:
    virtual ETextureFormat GetTextureFormat() = 0;
    virtual EAlphaChannelType GetTextureAlphaChannelType() = 0;
    virtual nsUInt32 GetTextureID() = 0;
    virtual nsUInt32 GetTextureOriginalWidth() = 0;
    virtual nsUInt32 GetTextureOriginalHeight() = 0;
    virtual nsUInt32 GetTextureWidth() = 0;
    virtual nsUInt32 GetTextureHeight() = 0;
    virtual nsUInt32 GetTextureStride() = 0;

    virtual bool IsTextureCooked() = 0;
  };
} // namespace aperture::harrlow
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

#include <APHarrlow/renderpath/Types.h>
#include <APHTML/Interfaces/Internal/APCBuffer.h>
#include <Foundation/Types/VariantType.h>
#include <Foundation/Math/Vec2.h>
#include <Foundation/Types/ObjectPtr.h>
#include <APHarrlow/APEngineDLL.h>

namespace aperture::harrlow::renderpath
{
    enum ClipMaskOp
    {
        ClipMaskOp_None = 0,
        ClipMaskOp_Set = 1,
        ClipMaskOp_Add = 2,
        ClipMaskOp_Subtract = 3,
        ClipMaskOp_Intersect = 4,
    };

    enum BlendMode
    {
        Blend, // Normal Alpha Blending
        Replace // Replace the destination with the source
    };
    /*
    * Interface for a render path. A render path is a interface for platform specific rendering code, and is needed to render anything to the screen.
    */
    class NS_APERTURE_DLL APHIRenderPath
    {
    public:
        virtual ~APHIRenderPath() = default;

        virtual nsObjectPtr<APHIGeometryBuffer> createGeometryBuffer(nsHybridArray<APHIVertex,128> verts, nsHybridArray<APHIVertex,12> indices) = 0;

        virtual void RenderGeometryBuffer(APHIGeometryBuffer* buffer) = 0;
        
        virtual void DestroyGeometryBuffer(APHIGeometryBuffer* buffer) = 0;

        virtual void SetViewport(const nsVec2& pos, const nsVec2& size) = 0;

        virtual void SetScissor(const nsVec2& pos, const nsVec2& size) = 0;

        virtual nsObjectPtr<APHITexture> CreateTexture(const nsVec2& size, const nsString& data) = 0;

        virtual void DestroyTexture(APHITexture* texture) = 0;

        virtual void HarrlowGenerateTexture(APHITexture* texture, const nsVec2& size, const core::CoreBuffer<nsUInt32>& data) = 0;

        virtual nsObjectPtr<APHIShader> CompileUserShader(const nsString& vertexShader, const nsVariantDictionary& params) = 0;

        virtual nsObjectPtr<APHITexture> SaveViewAsTexture();
    };
}

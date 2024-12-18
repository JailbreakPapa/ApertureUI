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

#include "Common.hpp"

namespace aperture::harrlow::vector
{

	struct TextCache
	{
		TextOptions	  opts;
		Array<Vertex> vtxBuffer;
		Array<Index>  indxBuffer;
	};

	struct RectOverrideData
	{
		bool overrideRectPositions = false;
		Vec2 m_p1				   = Vec2(0, 0);
		Vec2 m_p2				   = Vec2(0, 0);
		Vec2 m_p3				   = Vec2(0, 0);
		Vec2 m_p4				   = Vec2(0, 0);
	};

	struct UVOverrideData
	{
		bool m_override = false;
		Vec2 m_uvTL		= Vec2(0, 0);
		Vec2 m_uvBR		= Vec2(1, 1);
	};

	/// <summary>
	/// Management for draw buffers.
	/// </summary>
	struct BufferStoreData
	{
		Array<DrawBuffer>				m_defaultBuffers;
		Array<int>						m_drawOrders;
		APHARRLOW_MAP<uint32_t, TextCache> m_textCache;
		int								m_gcFrameCounter		= 0;
		int								m_textCacheFrameCounter = 0;
		RectOverrideData				m_rectOverrideData;
		UVOverrideData					m_uvOverride;
		Vec4i							m_clipRect = {0, 0, 0, 0};

		void		SetDrawOrderLimits(int drawOrder);
		int			GetBufferIndexInDefaultArray(DrawBuffer* buf);
		DrawBuffer& GetDefaultBuffer(void* userData, int drawOrder, DrawBufferShapeType shapeType, TextureHandle txtHandle, const Vec4& textureUV);
		void		AddTextCache(uint32_t sid, const TextOptions& opts, DrawBuffer* buf, int vtxStart, int indexStart);
		TextCache*	CheckTextCache(uint32_t sid, const TextOptions& opts, DrawBuffer* buf);
	};

	struct BufferStoreCallbacks
	{
		std::function<void(DrawBuffer* buf)> draw;
	};

	class BufferStore
	{
	public:
		BufferStore();
		~BufferStore();

		/// <summary>
		/// Clears up or shrinks (depending on gc interval) internal vertex/index buffers. Call each frame at the end of your draw commands, after Flushing.
		/// </summary>
		/// <returns></returns>
		APHARRLOW_API void ResetFrame();

		/// <summary>
		/// Call after you have submitted your draw commands, this will flush the buffers in batches to your callback functions.
		/// </summary>
		/// <returns></returns>
		APHARRLOW_API void FlushBuffers();

		/// <summary>
		/// Sets the scissors/clipping data.
		/// </summary>
		/// <returns></returns>
		APHARRLOW_API void SetClipRect(const Vec4i& pos);

		/// <summary>
		/// Erases all vertex & index data on all buffers.
		/// </summary>
		APHARRLOW_API void ClearAllBuffers();

		APHARRLOW_API inline BufferStoreData& GetData()
		{
			return m_data;
		}

		inline BufferStoreCallbacks& GetCallbacks()
		{
			return m_callbacks;
		}

	private:
		BufferStoreData		 m_data;
		BufferStoreCallbacks m_callbacks;
	};

}; // namespace aperture::harrlow::vector

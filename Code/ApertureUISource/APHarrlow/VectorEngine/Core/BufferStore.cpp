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

#include <iostream>

#include "APHarrlow/VectorEngine/Core/BufferStore.hpp"
#include "APHarrlow/VectorEngine/Core/Drawer.hpp"
#include "APHarrlow/VectorEngine/Core/Math.hpp"
#include "APHarrlow/VectorEngine/Core/Text.hpp"
#include "APHarrlow/VectorEngine/Utility/Utility.hpp"
#include <math.h>
#include <cassert>

namespace aperture::harrlow::vector
{
	BufferStore::BufferStore()
	{
		m_data.m_defaultBuffers.reserve(Config.defaultBufferReserve);

		if (Config.textCachingEnabled)
			m_data.m_textCache.reserve(Config.textCacheReserve);
	}

	BufferStore::~BufferStore()
	{
		ClearAllBuffers();
	}

	void BufferStore::ClearAllBuffers()
	{
		m_data.m_gcFrameCounter = 0;

		for (int i = 0; i < m_data.m_defaultBuffers.m_size; i++)
			m_data.m_defaultBuffers[i].Clear();

		m_data.m_defaultBuffers.clear();

		m_data.m_drawOrders.clear();
	}

	void BufferStore::ResetFrame()
	{
		m_data.m_gcFrameCounter++;

		if (m_data.m_gcFrameCounter > Config.gcCollectInterval)
		{
			ClearAllBuffers();
		}
		else
		{
			for (int i = 0; i < m_data.m_defaultBuffers.m_size; i++)
				m_data.m_defaultBuffers[i].ShrinkZero();
		}

		if (Config.textCachingEnabled)
			m_data.m_textCacheFrameCounter++;

		if (m_data.m_textCacheFrameCounter > Config.textCacheExpireInterval)
		{
			m_data.m_textCacheFrameCounter = 0;
			m_data.m_textCache.clear();
		}
	}

	void BufferStore::FlushBuffers()
	{
		int	 thread		 = 0;
		auto renderBuffs = [this, thread](int drawOrder, DrawBufferShapeType shapeType) {
			for (int i = 0; i < m_data.m_defaultBuffers.m_size; i++)
			{
				DrawBuffer& buf = m_data.m_defaultBuffers[i];

				if (buf.drawOrder == drawOrder && buf.shapeType == shapeType && buf.vertexBuffer.m_size != 0 && buf.indexBuffer.m_size != 0)
				{
					if (m_callbacks.draw)
						m_callbacks.draw(&buf);
					else
					{
						if (aperture::harrlow::vector::Config.logCallback)
							aperture::harrlow::vector::Config.logCallback("aperture::harrlow::vector: No callback is setup for Draw.");
					}
				}
			}
		};

		auto& arr = m_data.m_drawOrders;

		for (int i = 0; i < arr.m_size; i++)
		{
			const int drawOrder = arr[i];
			renderBuffs(drawOrder, DrawBufferShapeType::Shape);
			renderBuffs(drawOrder, DrawBufferShapeType::Text);
			renderBuffs(drawOrder, DrawBufferShapeType::SDFText);
			renderBuffs(drawOrder, DrawBufferShapeType::AA);
		}
	}

	APHARRLOW_API void BufferStore::SetClipRect(const Vec4i& rect)
	{
		m_data.m_clipRect = rect;
	}

	DrawBuffer& BufferStoreData::GetDefaultBuffer(void* userData, int drawOrder, DrawBufferShapeType shapeType, TextureHandle txtHandle, const Vec4& textureUV)
	{
		for (int i = 0; i < m_defaultBuffers.m_size; i++)
		{
			auto& buf = m_defaultBuffers[i];

			if (buf.shapeType != shapeType)
				continue;

			if (buf.userData != userData)
				continue;

			if (buf.drawOrder != drawOrder)
				continue;

			if (buf.IsClipDifferent(m_clipRect))
				continue;

			if (buf.textureHandle != txtHandle)
				continue;

			if (!Math::IsEqual(buf.textureUV, textureUV))
				continue;

			return buf;
		}

		SetDrawOrderLimits(drawOrder);
		m_defaultBuffers.push_back(DrawBuffer(userData, drawOrder, shapeType, txtHandle, textureUV, m_clipRect));
		return m_defaultBuffers.last_ref();
	}

	void BufferStoreData::AddTextCache(uint32_t sid, const TextOptions& opts, DrawBuffer* buf, int vtxStart, int indexStart)
	{
		TextCache& newCache = m_textCache[sid];
		newCache.opts		= opts;
		newCache.indxBuffer.clear();
		newCache.vtxBuffer.clear();

		for (int i = vtxStart; i < buf->vertexBuffer.m_size; i++)
			newCache.vtxBuffer.push_back(buf->vertexBuffer[i]);

		for (int i = indexStart; i < buf->indexBuffer.m_size; i++)
			newCache.indxBuffer.push_back(buf->indexBuffer[i] - vtxStart);
	}

	TextCache* BufferStoreData::CheckTextCache(uint32_t sid, const TextOptions& opts, DrawBuffer* buf)
	{
		auto it = m_textCache.find(sid);

		if (it == m_textCache.end())
			return nullptr;

		if (!it->second.opts.IsSame(opts))
			return nullptr;

		const int vtxStart = buf->vertexBuffer.m_size;

		for (auto& b : it->second.vtxBuffer)
			buf->PushVertex(b);

		for (auto& i : it->second.indxBuffer)
			buf->PushIndex(i + vtxStart);

		return &it->second;
	}

	int BufferStoreData::GetBufferIndexInDefaultArray(DrawBuffer* buf)
	{
		for (int i = 0; i < m_defaultBuffers.m_size; i++)
		{
			if (buf == &m_defaultBuffers[i])
				return i;
		}
		return -1;
	}

	void BufferStoreData::SetDrawOrderLimits(int drawOrder)
	{
		bool found = false;
		for (int i = 0; i < m_drawOrders.m_size; i++)
		{
			if (m_drawOrders[i] == drawOrder)
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			m_drawOrders.push_back(drawOrder);
			Utility::QuickSortArray<int>(m_drawOrders, 0, m_drawOrders.m_size - 1);
		}
	}

} // namespace aperture::harrlow::vector

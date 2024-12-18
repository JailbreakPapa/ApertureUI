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

#ifndef APHARRLOW_DISABLE_TEXT_SUPPORT

#include "Common.hpp"
#include <unordered_map>
#include <functional>
#include <mutex>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace aperture::harrlow::vector
{
	typedef FT_ULong GlyphEncoding;

	struct TextCharacter
	{
		Vec4  m_uv12	= Vec4(0.0f, 0.0f, 0.0f, 0.0f);
		Vec4  m_uv34	= Vec4(0.0f, 0.0f, 0.0f, 0.0f);
		Vec2  m_size	= Vec2(0.0f, 0.0f);
		Vec2  m_bearing = Vec2(0.0f, 0.0f);
		Vec2  m_advance = Vec2(0.0f, 0.0f);
		float m_ascent	= 0.0f;
		float m_descent = 0.0f;

		/// Cleaned after load.
		unsigned char* m_buffer = nullptr;
	};

	struct KerningInformation
	{
		APHARRLOW_MAP<unsigned long, unsigned long> xAdvances;
	};

	class Atlas;

	class Font
	{
	public:
		int			 size			   = 0;
		float		 newLineHeight	   = 0.0f;
		float		 spaceAdvance	   = 0.0f;
		bool		 supportsUnicode   = false;
		bool		 isSDF			   = false;
		bool		 supportsKerning   = false;
		unsigned int atlasRectHeight   = 0;
		unsigned int atlasRectPos	   = 0;
		Atlas*		 atlas			   = nullptr;
		size_t		 structSizeInBytes = 0;

		APHARRLOW_MAP<GlyphEncoding, TextCharacter>	  glyphs;
		APHARRLOW_MAP<unsigned long, KerningInformation> kerningTable;

		void DestroyBuffers();

		~Font()
		{
			DestroyBuffers();
		}
	};

	class Atlas
	{
	public:
		struct Slice
		{
			Slice(unsigned int posY, unsigned int h)
				: pos(posY), height(h){};
			unsigned int pos	= 0;
			unsigned int height = 0;
		};

		Atlas(const Vec2ui& size, std::function<void(Atlas* atlas)> updateFunc);
		~Atlas();

		void Destroy();
		bool AddFont(Font* font);
		void RemoveFont(unsigned int pos, unsigned int height);

		inline const Vec2ui& GetSize() const
		{
			return m_size;
		}

		inline uint8_t* GetData() const
		{
			return m_data;
		}

	private:
		std::function<void(Atlas* atlas)> m_updateFunc;
		APHARRLOW_VEC<Slice*>				  m_availableSlices;
		Vec2ui							  m_size = Vec2ui();
		uint8_t*						  m_data = nullptr;
	};

	struct Callbacks
	{
		std::function<void(Atlas* atlas)> atlasNeedsUpdate;
	};

	extern APHARRLOW_API FT_Library g_ftLib;

	/// <summary>
	/// Call once during your app before loading any fonts to setup free type library.
	/// </summary>
	extern APHARRLOW_API bool InitializeText();

	/// <summary>
	/// Call before terminating the app, will clean up FreeType library.
	/// </summary>
	extern APHARRLOW_API void TerminateText();

	class Text
	{
	public:
		~Text();

		/// <summary>
		/// Loads the given font and generates textures based on given size.
		/// You can load the same font with different sizes to achieve varying text scales.
		/// Alternatively, you can use the scale modifier in TextOptions but it's not recommended to upscale.
		/// Best quality would be achieved by loading fonts with bigger sizes and scaling them (slightly) down using TextOptions.
		/// Its your responsibility to delete the returned font ptr.
		/// </summary>
		/// <param name="file">TTF or OTF file.</param>
		/// <param name="loadAsSDF">Creates an SDF font.</param>
		/// <param name="size">Font height, width will automatically adjust.</param>
		/// <param name="customRanges">Send custom ranges in UTF32 encoding, e.g. 0x1F028, to load specific characters or sets.</param>
		/// <param name="customRangesSize">Size of the range array, each 2 pair in the array is treated as a range. Needs to be power of 2! </param>
		/// <param name="useKerningIfAvailable">If the font face contains a kern table this font will be drawn using kerning information. </param>
		/// <returns></returns>
		APHARRLOW_API static Font* LoadFont(const char* file, bool loadAsSDF, int size = 48, GlyphEncoding* customRanges = nullptr, int customRangesSize = 0, bool useKerningIfAvailable = true);

		/// <summary>
		/// Loads the given font and generates textures based on given size.
		/// You can load the same font with different sizes to achieve varying text scales.
		/// Alternatively, you can use the scale modifier in TextOptions but it's not recommended to upscale.
		/// Best quality would be achieved by loading fonts with bigger sizes and scaling them (slightly) down using TextOptions.
		/// Its your responsibility to delete the returned font ptr.
		/// </summary>
		/// <param name="data">Binary font data.</param>
		/// <param name="size">Binary font data size.</param>
		/// <param name="loadAsSDF">Creates an SDF font.</param>
		/// <param name="size">Font height, width will automatically adjust.</param>
		/// <param name="customRanges">Send custom ranges in UTF32 encoding, e.g. 0x1F028, to load specific characters or sets.</param>
		/// <param name="customRangesSize">Size of the range array, each 2 pair in the array is treated as a range. Needs to be power of 2! </param>
		/// <param name="useKerningIfAvailable">If the font face contains a kern table this font will be drawn using kerning information. </param>
		/// <returns></returns>
		APHARRLOW_API static Font* LoadFontFromMemory(void* data, size_t dataSize, bool loadAsSDF, int size = 48, GlyphEncoding* customRanges = nullptr, int customRangesSize = 0, bool useKerningIfAvailable = true);

		/// <summary>
		/// Uses loaded face (from file or mem) to setup rest of the font data.
		/// </summary>
		APHARRLOW_API static Font* SetupFont(FT_Face& face, bool loadAsSDF, int size, GlyphEncoding* customRanges, int customRangesSize, bool useKerningIfAvailable);

		/// <summary>
		/// Call after SetupFont to fit the loaded font into an atlas.
		/// </summary>
		/// <param name="font"></param>
		/// <returns></returns>
		APHARRLOW_API void AddFontToAtlas(Font* font);

		/// <summary>
		/// Call before deallocating your font to remove from its dedicated atlas.
		/// </summary>
		/// <param name="font"></param>
		/// <returns></returns>
		APHARRLOW_API void RemoveFontFromAtlas(Font* font);

		/// <summary>
		/// Returns the kerning vector between two given glphys.
		/// </summary>
		// APHARRLOW_API Vec2 GetKerning(Font* font, int previousGlyph, int currentGlyph);

		inline Callbacks& GetCallbacks()
		{
			return m_callbacks;
		}

	private:
		APHARRLOW_VEC<Atlas*> m_atlases;
		Callbacks		   m_callbacks;

	}; // namespace Text

}; // namespace aperture::harrlow::vector

#endif

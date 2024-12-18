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

namespace aperture::harrlow::vector
{

	/// <summary>
	/// Color vector, range 0.0f - 1.0f
	/// </summary>
	struct Vec4
	{
		Vec4(){};
		Vec4(float x, float y, float z, float w)
			: x(x), y(y), z(z), w(w){};
		Vec4(const Vec4& v)
		{
			this->x = v.x;
			this->y = v.y;
			this->z = v.z;
			this->w = v.w;
		}

		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		float w = 0.0f;
	};

	struct Vec2
	{
		Vec2(){};
		Vec2(float x, float y)
			: x(x), y(y){};
		Vec2(const Vec2& v)
		{
			x = v.x;
			y = v.y;
		}

		float x = 0.0f;
		float y = 0.0f;
	};

	struct Vec2ui
	{
		Vec2ui(){};
		Vec2ui(unsigned int x, unsigned int y)
			: x(x), y(y){};
		Vec2ui(const Vec2ui& v)
		{
			x = v.x;
			y = v.y;
		}
		Vec2ui(const Vec2& v)
		{
			x = static_cast<unsigned int>(v.x);
			y = static_cast<unsigned int>(v.y);
		}

		unsigned int x = 0;
		unsigned int y = 0;
	};

	struct Vec4i
	{
		Vec4i(){};
		Vec4i(int x, int y, int z, int w)
			: x(x), y(y), z(z), w(w){};
		Vec4i(const Vec4i& v)
		{
			x = v.x;
			y = v.y;
			z = v.z;
			w = v.w;
		}

		bool operator==(const Vec4i& other) const
		{
			return x == other.x && y == other.y && z == other.z && w == other.w;
		}
		bool IsOtherInside(const Vec4i& other) const
		{
			return other.x >= x && other.y >= y && other.x + other.z <= x + z && other.y + other.w <= y + w;
		}


		int x = 0;
		int y = 0;
		int z = 0;
		int w = 0;
	};
	struct Vec4ui
	{
		Vec4ui(){};
		Vec4ui(unsigned int x, unsigned int y, unsigned int z, unsigned int w)
			: x(x), y(y), z(z), w(w){};
		Vec4ui(const Vec4ui& v)
		{
			x = v.x;
			y = v.y;
			z = v.z;
			w = v.w;
		}

		Vec4ui(const Vec2ui& p, const Vec2ui& s)
		{
			x = p.x;
			y = p.y;
			z = s.x;
			w = s.y;
		}

		bool operator==(const Vec4ui& other) const
		{
			return x == other.x && y == other.y && z == other.z && w == other.w;
		}
		bool IsOtherInside(const Vec4ui& other) const
		{
			return other.x >= x && other.y >= y && other.x + other.z <= x + z && other.y + other.w <= y + w;
		}

		bool IsPointInside(const Vec2ui& point) const
		{
			return point.x >= x && point.x <= x + z && point.y >= y && point.y <= y + w;
		}

		unsigned int x = 0;
		unsigned int y = 0;
		unsigned int z = 0;
		unsigned int w = 0;
	};

} // namespace aperture::harrlow::vector

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
	class APHARRLOW_API Math
	{
	public:
		static float Mag(const Vec2& v);
		static int	 GetAreaIndex(const Vec2& diff);
		static float GetAngleFromCenter(const Vec2& center, const Vec2& point);
		static float GetAngleBetween(const Vec2& p1, const Vec2& p2);
		static float GetAngleBetweenDirs(const Vec2& dir1, const Vec2& dir2);
		static float GetAngleBetweenShort(const Vec2& p1, const Vec2& p2);
		static Vec2	 Normalized(const Vec2& v);
		static Vec2	 Rotate90(const Vec2& v, bool ccw = true);
		static Vec2	 GetPointOnCircle(const Vec2& center, float radius, float angle); // Angle in degrees.
		static Vec2	 SampleParabola(const Vec2& p1, const Vec2& p2, const Vec2& direction, float height, float t);
		static Vec2	 SampleBezier(const Vec2& p0, const Vec2& p1, const Vec2& p2, const Vec2& p3, float t);
		static Vec2	 LineIntersection(const Vec2& p00, const Vec2& p01, const Vec2& p10, const Vec2& p11);
		static bool	 AreLinesParallel(const Vec2& p00, const Vec2& p01, const Vec2& p10, const Vec2& p11);
		static Vec2	 RotateAround(const Vec2& point, const Vec2& center, float angle);
		static Vec2	 Abs(const Vec2& v);
		static Vec2	 Clamp(const Vec2& v, const Vec2& min, const Vec2& max);
		static Vec2	 ScalePoint(const Vec2& p, const Vec2& center, float scale);
		static Vec2	 GetVertexAverage(const Vec2& point, const Vec2& previousPoint, const Vec2& nextPoint);
		static Vec2	 GetVertexNormal(const Vec2& point, const Vec2& previousPoint, const Vec2& nextPoint, bool ccw = true);
        static Vec2  GetExtrudedFromNormal(const Vec2& point, const Vec2& previousPoint, const Vec2& nextPoint, float thickness = 1.0f, bool ccw = true);
        static Vec2     GetVertexNormalFlatCheck(const Vec2& point, const Vec2& previousPoint, const Vec2& nextPoint, bool ccw = true);
		static Vec2	 GetExtrudedFromNormalFlatCheck(const Vec2& point, const Vec2& previousPoint, const Vec2& nextPoint, float thickness, bool ccw = true);
		static float InverseLerp(float a, float b, float v);
        static int CustomRound(float val);
        
		/// <summary>
		/// Returns the centroid of a given polygon.
		/// https://stackoverflow.com/questions/2792443/finding-the-centroid-of-a-polygon
		/// </summary>
		/// <returns></returns>
		static Vec2 GetPolygonCentroid(Vec2* points, int size);

		/// <summary>
		/// Returns the center of the given points via simple average.
		/// </summary>
		/// <returns></returns>
		static Vec2 GetPolygonCentroidFast(Vec2* points, int size);

		static bool	 IsEqual(const Vec2& v1, const Vec2& v2);
		static bool	 IsEqualMarg(const Vec2& v1, const Vec2& v2, float err = 0.001f);
		static bool	 IsEqualMarg(float f1, float f2, float err = 0.001f);
		static bool	 IsEqual(const Vec4& v1, const Vec4& v2);
		static float Abs(float f);
		static float Clamp(float f, float min, float max);
		static int	 Clamp(int i, int min, int max);

		template <typename T>
		static T Min(T a, T b)
		{
			return a < b ? a : b;
		}

		template <typename T>
		static T Max(T a, T b)
		{
			return a > b ? a : b;
		}

		template <typename T, typename U>
		static T Lerp(const T& val1, const T& val2, const U& amt)
		{
			return (T)(val1 * ((U)(1) - amt) + val2 * amt);
		}

		static Vec4 Lerp(const Vec4& val, const Vec4& val2, float amt);
		static Vec2 Lerp(const Vec2& val, const Vec2& val2, float amt);

		template <typename T>
		static T Remap(const T& val, const T& fromLow, const T& fromHigh, const T& toLow, const T& toHigh)
		{
			return toLow + (val - fromLow) * (toHigh - toLow) / (fromHigh - fromLow);
		}
	};

} // namespace aperture::harrlow::vector

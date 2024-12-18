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

/*
Timestamp: 3/28/2022 2:38:21 PM
*/

#pragma once

#ifndef APHarrlowUtility_HPP
#define APHarrlowUtility_HPP

#include "../Core/Common.hpp"

namespace aperture::harrlow::vector
{
	namespace Utility
	{
		// https://gist.github.com/hwei/1950649d523afd03285c
		class FnvHash
		{
			static const unsigned int FNV_PRIME	   = 16777619u;
			static const unsigned int OFFSET_BASIS = 2166136261u;
			template <unsigned int N>
			static constexpr unsigned int fnvHashConst(const char (&str)[N], unsigned int I = N)
			{
				return I == 1 ? (OFFSET_BASIS ^ str[0]) * FNV_PRIME : (fnvHashConst(str, I - 1) ^ str[I - 1]) * FNV_PRIME;
			}
			static uint32_t fnvHash(const char* str)
			{
				const size_t length = strlen(str) + 1;
				uint32_t	 hash	= OFFSET_BASIS;
				for (size_t i = 0; i < length; ++i)
				{
					hash ^= *str++;
					hash *= FNV_PRIME;
				}
				return hash;
			}
			struct Wrapper
			{
				Wrapper(const char* str)
					: str(str)
				{
				}
				const char* str;
			};
			unsigned int hash_value;

		public:
			// calulate in run-time
			FnvHash(Wrapper wrapper)
				: hash_value(fnvHash(wrapper.str))
			{
			}
			// calulate in compile-time
			template <unsigned int N>
			constexpr FnvHash(const char (&str)[N])
				: hash_value(fnvHashConst(str))
			{
			}
			// output result
			constexpr operator unsigned int() const
			{
				return this->hash_value;
			}
		};

		template <typename T>
		int QuickSortPartition(Array<T>& arr, int start, int end)
		{
			int pivot = arr[start];

			int count = 0;
			for (int i = start + 1; i <= end; i++)
			{
				if (arr[i] <= pivot)
					count++;
			}

			// Giving pivot element its correct position
			int pivotIndex = start + count;
			arr.swap(pivotIndex, start);

			// Sorting left and right parts of the pivot element
			int i = start, j = end;

			while (i < pivotIndex && j > pivotIndex)
			{

				while (arr[i] <= pivot)
				{
					i++;
				}

				while (arr[j] > pivot)
				{
					j--;
				}

				if (i < pivotIndex && j > pivotIndex)
				{
					arr.swap(i++, j--);
				}
			}

			return pivotIndex;
		}

		template <typename T>
		void QuickSortArray(Array<T>& arr, int start, int end)
		{
			if (start >= end)
				return;

			int p = QuickSortPartition<T>(arr, start, end);

			//  Left
			QuickSortArray<T>(arr, start, p - 1);

			// Right
			QuickSortArray<T>(arr, p + 1, end);
		}

		int	 GetTextCharacterSize(const char* text);
		Vec4 HexToVec4(int hex);

	} // namespace Utility
} // namespace aperture::harrlow::vector
#endif

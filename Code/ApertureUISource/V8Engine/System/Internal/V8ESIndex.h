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

#include <Foundation/Strings/String.h>
#include <Foundation/Types/SharedPtr.h>
#include <Foundation/Types/Variant.h>
#include <Foundation/Threading/Mutex.h>
#include <V8Engine/V8EngineDLL.h>

namespace aperture::v8::internal
{
    template<typename UnderlyingType, uint8_t TMaskBit = 6, UnderlyingType TMask = 0x3f>
    struct TIndex
    {
        typedef uint32_t RevisionType;
        static constexpr uint8_t kRevisionBits = TMaskBit;
        static constexpr UnderlyingType kRevisionMask = TMask;

        static const TIndex& none()
        {
            static TIndex index = {};
            return index;
        }

        TIndex(): packed_(0) {}

        TIndex(int32_t index, RevisionType revision):
            packed_(((UnderlyingType)index << kRevisionBits) | ((UnderlyingType)revision & kRevisionMask))
        {
            // index overflow check
            jsb_check(!((UnderlyingType) index >> (sizeof(UnderlyingType) * 8 - kRevisionBits)));
        }

        explicit TIndex(UnderlyingType p_value) : packed_(p_value)
        {
        }

        // shorthands
        operator UnderlyingType() const { return packed_; }
        UnderlyingType value() const { return packed_; }
        UnderlyingType operator *() const { return packed_; }
        nsString to_string() const { return uitos(packed_); }

        TIndex(const TIndex& other) = default;
        TIndex(TIndex&& other) = default;
        TIndex& operator=(const TIndex& other) = default;
        TIndex& operator=(TIndex&& other) = default;
        ~TIndex() = default;

        bool is_valid() const { return packed_ != 0; }
        operator bool() const { return packed_ != 0; }

        int32_t get_index() const { return (int32_t) (packed_ >> kRevisionBits); }
        RevisionType get_revision() const { return (RevisionType) (packed_ & kRevisionMask); }

        friend bool operator==(const TIndex& lhs, const TIndex& rhs)
        {
            return lhs.packed_ == rhs.packed_;
        }

        friend bool operator<(const TIndex& lhs, const TIndex& rhs)
        {
            return lhs.packed_ < rhs.packed_;
        }

        friend bool operator!=(const TIndex& lhs, const TIndex& rhs)
        {
            return lhs.packed_ != rhs.packed_;
        }

        uint32_t hash() const
        {
            return (uint32_t) ((packed_ >> kRevisionBits) ^ (packed_ & kRevisionMask));
        }

        static void increase_revision(RevisionType& p_value)
        {
            p_value = MAX((RevisionType) 1, (p_value + 1) & kRevisionMask);
        }

    private:
        UnderlyingType packed_;
    };

    // do not really support the index bigger than int32_t.MaxValue
    // /*if unsigned*/ index(0, 4_294_967_295) revision(1, 4_294_967_295)
    typedef TIndex<uint64_t, 32, 0xffffffff> Index64;

    // index(0, 67_108_863) revision(1, 63)
    typedef TIndex<uint32_t, 6, 0x3f> Index32;
}
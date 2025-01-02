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

#include <APHTML/Interfaces/APCPlatform.h>
#include <APHTML/V8Engine/V8EngineDLL.h>

namespace aperture::v8::utility
{
  struct IndexedSourcePosition
  {
    int index = 0;
    int line = 0;
    int column = 0;
  };

  struct SourcePosition
  {
    nsString function;
    nsString filename;
    int line = 0;
    int column = 0;
  };

  struct SourceMap
  {
  private:
    struct InternalPosition
    {
      // DO NOT CHANGE THE ORDER
      int result_column = 0;
      int source_index = 0;
      int source_line = 0;
      int source_column = 0;
      int _reserved = 0; // name index
      NS_FORCE_INLINE int& operator[](uint8_t index)
      {
        //NS_ISE_JSI_CHECK(index < 5);
        return *((int*)this + index);
      }
    };
    struct InternalLine
    {
      int result_line = 0;
      nsDynamicArray<InternalPosition> elements;
    };
    nsDynamicArray<InternalLine> source_lines_;
    nsDynamicArray<nsString> sources_;
    nsString source_root_;

  public:
    // input string is `mappings` (not the whole json), example: `;;;AAAA,iCAA6B;AAC7B,MAAa,QAAQ;CAAI`
    bool parse_mappings(const char* p_mappings, size_t p_len);

    // parse sourcemap json string
    bool parse(const nsString& p_source_map);

    // input: js source position [line, column]
    // output: ts source position
    // NOTE line & column are both zero-based
    bool find(int p_line, int p_column, IndexedSourcePosition& r_pos) const;

    const nsString& get_source_root() const;
    const nsString& get_source(int index) const;

  private:
    void decode(int p_line, const char* p_token, const char* p_end, InternalPosition& r_pos, int& r_aline, int& r_acol);
    InternalLine& operator[](int p_line);
  };
} // namespace aperture::v8::utility

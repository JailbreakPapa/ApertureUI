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
#include <APHTML/APEngineCommonIncludes.h>
#include <APHTML/core/NumericValue.h>
#include <APHTML/dom/DOMAttribute.h>


namespace aperture
{
  struct NS_APERTURE_DLL PropertySource
  {
    PropertySource(nsString path, int line_number, nsString rule_name)
      : path(std::move(path))
      , line_number(line_number)
      , rule_name(std::move(rule_name))
    {
    }
    nsString path;
    int line_number;
    nsString rule_name;
  };

  class PropertyDefinition;
  template <typename T>

  class NS_APERTURE_DLL Property : public dom::DOMAttribute
  {
    template <typename PropertyType = nsUInt32>
    Property(PropertyType value, core::Unit unit = core::Unit::PX, int specificity = -1)
      : value(value)
      , unit(unit)
      , specificity(specificity)
    {
      definition = nullptr;
      parser_index = -1;
    }
    template <typename EnumType, typename = typename std::enable_if<std::is_enum<EnumType>::value, EnumType>::type>
    Property(EnumType value)
      : value(static_cast<int>(value))
      , unit(core::Unit::KEYWORD)
      , specificity(-1)
    {
    }

    nsString ToString() const;

    /// Get the value of the property as a numeric value, if applicable.
    core::NumericValue GetNumericValue() const;

    /// Templatized accessor.
    template <typename T>
    T Get() const
    {
      return value.Get<T>();
    }

    int specificity;

    const PropertyDefinition* definition = nullptr;
    int parser_index = -1;
    T value;
    const PropertySource* source;
  };
} // namespace aperture

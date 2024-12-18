/*
 *   Copyright (c) 2024 WD Studios L.L.C.
 *   All rights reserved.
 *   You are only allowed access to this code, if given WRITTEN permission by WD Studios L.L.C.
 */
#pragma once
#include <APHTML/css/syntax/CSSSyntaxCommon.h>
#include <APHTML/css/SpiritIncludes.h>
#include <APHTML/APEngineCommonIncludes.h>

using boost::spirit::x3::lexeme;
using boost::spirit::x3::traits::move_to;

namespace aperture::css
{
  template <typename Context, typename... PointerToMemberTypes>
  auto& MemberReference(Context& context, PointerToMemberTypes... member)
  {
    return (_val(context).*....*member);
  }

  template <typename PropertyType, typename ParserType, typename... PointerToMemberTypes>
  auto make_property(const PropertyType& property,
    const ParserType& parser,
    PointerToMemberTypes... member)
  {
    const auto setter = [member...](auto& context)
    {
      move_to(_attr(context), MemberReference(context, member...));
    };
    return lexeme[property] >> ':' >> parser[setter] >> ';';
  }
}

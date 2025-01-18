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

#include <APHTML/CSS/Parser/CSSRuleParser.h>

namespace aperture::css::parser::basic
{
  auto identifier_r = CSSRuleParser::MakeRule<struct identifier, std::string>("identifier", CSSSyntaxProperties::fundamental__identifier);
  auto string_literal_r = CSSRuleParser::MakeRule<struct string_literal, std::string>("string_literal", CSSSyntaxProperties::fundamental__string_literal);
  auto hex_value_r = CSSRuleParser::MakeRule<struct hex_value, std::string>("hex_value", CSSSyntaxProperties::fundamental__hexvalue);
  auto value_r = CSSRuleParser::MakeRule<struct value, std::string>("value", CSSSyntaxProperties::fundamental__value); 
  auto property_r = CSSRuleParser::MakeRule<struct property, std::string /*TODO: Assign AST Representation*/>("property", CSSSyntaxProperties::fundamental__property);
  auto block_r = CSSRuleParser::MakeRule<struct block, std::string>("block", CSSSyntaxProperties::fundamental__block);
  auto css_rule_r = CSSRuleParser::MakeRule<struct css_rule, std::string /*TODO: Assign AST Representation*/>("css_rule", CSSSyntaxProperties::fundamental__css_rule);
  auto css_file_r = CSSRuleParser::MakeRule<struct css_file, std::string /* std::vector<Rule>*/>("css_file", CSSSyntaxProperties::fundamental__css_rule); 

  CSS_RPL_START(identifier)
  CSS_RPL_NOC(bp::char_("a-zA-Z0-9-_"), +)
  CSS_RPL_NOC_END()

  CSS_RPL_START(hex_value)
  CSS_RPL_NOC(bp::char_('#'))
  CSS_RPL(bp::repeat(6)[bp::char_("0-9a-fA-F")])
  CSS_RPL_NOC_END()

  CSS_RPL_START(value)
  CSS_RPL_NOC(bp::char_("a-zA-Z0-9.%"),+)
  CSS_RPL_NOC_END()

  CSS_RPL_START(property)
  CSS_RPL_NOC(bp::char_("a-zA-Z0-9.%"),+)
  CSS_RPL(':')
  CSS_RPL_REF_P(value)
  CSS_RPL_END(';')


  CSS_RPL_START(string_literal)
  CSS_RPL_NOC('"')
  CSS_RPL(bp::char_('"'), *)
  CSS_RPL_END('"')

  CSS_RPL_START(block)
  CSS_RPL_NOC('{')
  CSS_RPL_REF_P(property,*)
  CSS_RPL_END('}')

  CSS_RPL_START(css_rule)
  CSS_RPL_REF(identifier)
  CSS_RPL_END(CSS_RPL_REF(block))

  CSS_RPL_START(css_file)
  CSS_RPL_REF(css_rule,*)
  CSS_RPL_NOC_END()

  BOOST_PARSER_DEFINE_RULES(identifier_r, string_literal_r, hex_value_r, value_r, property_r)
} // namespace aperture::css::parser::basic

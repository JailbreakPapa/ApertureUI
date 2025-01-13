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

#include <APHTML/CSS/Parser/CSSParser.h>
#include <APHTML/CSS/Parser/CSSParserUtils.hpp>
#include <APHTML/CSS/syntax/CSSSyntaxProperties.h>

/**
 * @file CSSRuleParser.h
 * @brief Defines the CSSRuleBank and CSSRuleParser classes for managing and parsing CSS rules.
 */

namespace aperture::css::parser
{
  /**
   * @class CSSRuleBank
   * @brief Singleton class that manages a collection of CSS rules and their associated properties.
   *
   * The CSSRuleBank class provides methods to add, retrieve, and manage CSS rules
   * along with their corresponding syntax properties. It ensures that rules are
   * uniquely stored and accessible throughout the application.
   *
   * @tparam StructOrClass The structure or class type associated with the CSS rules.
   */
  template <typename StructOrClass = struct unknownplaceholder>
  class NS_APERTURE_DLL CSSRuleBank
  {
  public:
    /**
     * @brief Adds a new CSS rule pair with the specified syntax properties.
     *
     * @param in_rule The first CSS rule to add.
     * @param in_rule2 The second CSS rule to add.
     * @param in_propertytoassign The syntax properties associated with the rule pair.
     */
    void AddRule(const bp::rule<StructOrClass>& in_rule, const bp::rule<StructOrClass>& in_rule2, CSSSyntaxProperties in_propertytoassign);

    /**
     * @brief Retrieves a CSS rule pair based on the provided rules.
     *
     * @param in_rule The first CSS rule to search for.
     * @param in_rule2 The second CSS rule to search for.
     * @return The matched CSS rule pair.
     */
    bp::rule<StructOrClass> GetRule(const bp::rule<StructOrClass>& in_rule, const bp::rule<StructOrClass>& in_rule2);

    /**
     * @brief Retrieves a CSS rule based on a single rule input.
     *
     * @param in_rule The CSS rule to search for.
     * @return The matched CSS rule or nullptr if not found.
     */
    bp::rule<StructOrClass> GetRule(const bp::rule<StructOrClass>& in_rule);

    /**
     * @brief Retrieves a CSS rule based on syntax properties.
     *
     * @param in_rulename The syntax property name to search for.
     * @param m_inFirstOrSecond Determines which part of the rule pair to return.
     * @return The matched CSS rule.
     */
    bp::rule<StructOrClass> GetRule(CSSSyntaxProperties in_rulename, bool m_inFirstOrSecond = true);

    /**
     * @brief Retrieves a pair of CSS rules based on syntax properties.
     *
     * @param in_rulename The syntax property name to search for.
     * @return A pair of matched CSS rules.
     */
    std::pair<bp::rule<StructOrClass>, bp::rule<StructOrClass>> GetRulePair(CSSSyntaxProperties in_rulename);

  private:
    nsMap<std::pair<bp::rule<StructOrClass>, bp::rule<StructOrClass>>, CSSSyntaxProperties> m_ruleBank; /**< Container storing CSS rule pairs and their properties */
  };



  /**
   * @class CSSRuleParser
   * @brief Helper class for parsing CSS rules using Boost Parser.
   *
   * The CSSRuleParser class provides static methods to create CSS rules and
   * parse CSS data according to defined rules. It facilitates error handling
   * and integrates with the CSSRuleBank for rule management.
   */
  class NS_APERTURE_DLL CSSRuleParser
  {
  public:
    /**
     * @brief Creates a new CSS rule with the specified name and properties.
     *
     * @tparam StructOrClass The structure or class type associated with the rule.
     * @tparam TypeToContain The type contained by the rule.
     * @param in_rulename The name of the CSS rule.
     * @param in_propertytoassign The syntax properties to assign to the rule.
     * @return The created Boost Parser rule.
     */
    template <typename StructOrClass, typename TypeToContain>
    static bp::rule<StructOrClass, TypeToContain> MakeRule(const char* in_rulename, CSSSyntaxProperties in_propertytoassign);

    /**
     * @brief Parses the given data and checks it against the specified rule.
     *
     * @warning Your Rule should already be pre-declared to parser with this: BOOST_PARSER_DEFINE_RULES()
     *
     * @tparam StructOrClass The structure or class type associated with the rule.
     * @tparam TypeToContain The type contained by the rule.
     * @tparam ParserObject The parser object type.
     * @param data The data buffer to parse.
     * @param object The parser object to handle parsing actions.
     * @param rule The Boost Parser rule to apply (optional).
     * @return True if parsing is successful, false otherwise.
     */
    template <typename ParserObject>
    static bool ParseAndCheck(const char* in_name, core::CoreBuffer<nsUInt8>& data, ParserObject& object);
  };

} // namespace aperture::css::parser

/**
 * @brief Declares a CSS rule parser with the specified name.
 *
 * This macro defines a parser rule by creating an `auto` variable named
 * `name_p` which is assigned to a literal parser for the given name.
 *
 * @param name The name of the CSS rule to declare.
 */
#define CSS_RULE_PARSER_DECL(name) \
  auto name##_p = bp::lit(#name) >>

/**
 * @brief Helper macro to select the correct version of the macro based on the number of arguments.
 * TODO: Whitespace Support
 */
#define CSS_RP_2(symbol, breaker) symbol >> bp::lit(breaker)
#define CSS_RP_1(symbol) CSS_RP_2(symbol, ',') >> // Default breaker is ','

/**
 * @brief Macro to declare a symbol. Variadic macro to handle 1 or 2 arguments.
 */
#define CSS_RP(...) GET_MACRO(__VA_ARGS__, CSS_RP_2, CSS_RP_1)(__VA_ARGS__)

#define CSS_RP_BREAKER(symbol, breaker) symbol >> bp::lit(breaker) >>
/**
 * @brief Macro to extract the correct version.
 */
#define GET_MACRO(_1, _2, NAME, ...) NAME

/*
 * @brief Ends the CSS rule parser declaration.
 */
#define CSS_RP_END(symbol) symbol;

#define CSS_RPL_START(name, containingsymbol) \
auto name##_p = bp::lexeme[##containingsymbol 

#define CSS_RPL(symbol,operation) >> operation(symbol)
#define CSS_RPL_NOC(symbol, operation) operation(symbol)
#define CSS_RPL_END(symbol) >> ##symbol];
#define CSS_RPL_NOC_END() ];
/*
// @example How to use the CSS Lexer Based Macros:
inline namespace
{
  void testrpl()
  {
    CSS_RPL_START(test, '{')
    CSS_RPL(bp::int_, bp::int_)
    CSS_RPL_END('}')
  }
}
*/
/*
@example How to use the CSSRuleParser class and macros:
inline namespace
{
// Define a CSS rule parser for a test rule
  void fucsss()
  {
    CSS_RULE_PARSER_DECL(test)
    CSS_RP('{')
    CSS_RP(bp::int_)
    CSS_RP_END('}')
  }
// Using a another declared rule to use within another rule
    void fucsss2()
    {
        CSS_RULE_PARSER_DECL(test1)
        CSS_RP('{')
        CSS_RP_BREAKER(bp::char_, "break") // Use a custom breaker
        CSS_RP(bp::char_)
        CSS_RP(bp::char_)
        CSS_RP(bp::char_)
        CSS_RP(bp::char_)
        CSS_RP_END('}')

        CSS_RULE_PARSER_DECL(test2)
        CSS_RP('{')
        CSS_RP(bp::int_)
        CSS_RP(test1_p)
        CSS_RP_END('}')
    }
} // namespace
*/

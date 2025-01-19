#include <APHTML/CSS/Parser/CSSRuleParser.h>
#include <Foundation/Types/UniquePtr.h>

template <typename StructOrClass /*= struct unknownplaceholder*/>
std::pair<bp::rule<StructOrClass>, bp::rule<StructOrClass>> aperture::css::parser::CSSRuleBank<StructOrClass>::GetRulePair(CSSSyntaxProperties in_rulename)
{
  m_ruleBank.Find(in_rulename);
}

template <typename StructOrClass>
inline bp::rule<StructOrClass> aperture::css::parser::CSSRuleBank<StructOrClass>::GetRule(CSSSyntaxProperties in_rulename, bool m_inFirstOrSecond)
{
  if (m_inFirstOrSecond)
  {
    return m_ruleBank.Find(in_rulename).first;
  }
  else
  {
    return m_ruleBank.Find(in_rulename).second;
  }
}

template <typename StructOrClass>
void aperture::css::parser::CSSRuleBank<StructOrClass>::AddRule(const bp::rule<StructOrClass>& in_rule, const bp::rule<StructOrClass>& in_rule2, CSSSyntaxProperties in_propertytoassign)
{
  m_ruleBank.Add(std::make_pair(in_rule, in_rule2), in_propertytoassign);
}

template <typename StructOrClass>
bp::rule<StructOrClass> aperture::css::parser::CSSRuleBank<StructOrClass>::GetRule(const bp::rule<StructOrClass>& in_rule, const bp::rule<StructOrClass>& in_rule2)
{
  return m_ruleBank.Find(std::make_pair(in_rule, in_rule2));
}

template <typename StructOrClass /*= struct unknownplaceholder*/>
bp::rule<StructOrClass> aperture::css::parser::CSSRuleBank<StructOrClass>::GetRule(const bp::rule<StructOrClass>& in_rule)
{
  return m_ruleBank.Find(std::make_pair(in_rule, nullptr));
}

template <typename StructOrClass, typename TypeToContain>
bp::rule<StructOrClass, TypeToContain> aperture::css::parser::CSSRuleParser::MakeRule(const char* in_rulename, CSSSyntaxProperties in_propertytoassign)
{
  bp::rule<StructOrClass, TypeToContain> rule = in_rulename;
  AddRule(rule, rule, in_propertytoassign);
  return rule;
}

template <typename ParserObject>
bool aperture::css::parser::CSSRuleParser::ParseAndCheck(const char* in_name, core::CoreBuffer<nsUInt8>& data, ParserObject& object)
{
  css_logging_error_handler error_handler(in_name);

  if (bp::parse(data, bp::with_error_handler(object, error_handler)))
  {
    return true;
  }
  return false;
}

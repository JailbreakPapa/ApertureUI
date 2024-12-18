#include <APHTML/dom/DOMElement.h>
#include "DOMAttribute.h"

aperture::dom::DOMAttribute::DOMAttribute(const nsString& in_attrname, const nsVariant& in_attrvalue)
{
  m_attrname = in_attrname;
  /// Index Variant Type so we can serialize and Cache, Then figure out what to do with it.
  IndexAttrValueType(in_attrvalue);
}

bool aperture::dom::DOMAttribute::operator==(const aperture::dom::DOMAttribute& r) const
{
  return m_attrname == r.m_attrname && m_attrvalue == r.m_attrvalue && m_attrtype == r.m_attrtype;
}

bool aperture::dom::DOMAttribute::operator!=(const aperture::dom::DOMAttribute& r) const
{
  return m_attrname != r.m_attrname;
}

bool aperture::dom::DOMAttribute::operator<(const aperture::dom::DOMAttribute& r) const
{
  return m_attrname < r.m_attrname && m_attrtype < r.m_attrtype && m_attrvalue.GetType() < r.m_attrvalue.GetType();
}

bool aperture::dom::DOMAttribute::operator>(const aperture::dom::DOMAttribute& r) const
{
  return m_attrname > r.m_attrname && m_attrtype > r.m_attrtype && m_attrvalue.GetType() > r.m_attrvalue.GetType();
}

bool aperture::dom::DOMAttribute::operator<=(const aperture::dom::DOMAttribute& r) const
{
  return !(*this > r);
}

bool aperture::dom::DOMAttribute::operator>=(const aperture::dom::DOMAttribute& r) const
{
  return !(*this < r);
}


void aperture::dom::DOMAttribute::SetAttrName(const nsString& in_newname)
{
  m_attrname = in_newname;
}

void aperture::dom::DOMAttribute::SetAttrValue(const nsVariant& in_newvalue)
{
  IndexAttrValueType(in_newvalue);
  m_attrvalue = in_newvalue;
}

void aperture::dom::DOMAttribute::IndexAttrValueType(const nsVariant& in_newvalue)
{
  switch(in_newvalue.GetType())
  {
      case(nsVariantType::Angle):
      {
        m_attrtype = nsVariantType::Angle;
        break;
      }
      case(nsVariantType::Bool):
      {
        m_attrtype = nsVariantType::Bool;
        break;
      }
      case(nsVariantType::Color):
      {
        m_attrtype = nsVariantType::Color;
        break;
      }
      case(nsVariantType::Float):
      {
        m_attrtype = nsVariantType::Float;
        break;
      }
      case(nsVariantType::String):
      {
        m_attrtype = nsVariantType::String;
        break;
      }
      case(nsVariantType::Vector3I):
      {
        m_attrtype = nsVariantType::Vector3I;
        break;
      }
      case(nsVariantType::Vector3U):
      {
        m_attrtype = nsVariantType::Vector3U;
        break;
      }
      case(nsVariantType::Vector4I):
      {
        m_attrtype = nsVariantType::Vector4I;
        break;
      }
      case(nsVariantType::Vector4):
      {
        m_attrtype = nsVariantType::Vector4;
        break;
      }
      case(nsVariantType::Matrix3):
      {
        m_attrtype = nsVariantType::Matrix3;
        break;
      }
      case(nsVariantType::Matrix4):
      {
        m_attrtype = nsVariantType::Matrix4;
        break;
      }
      case(nsVariantType::Quaternion):
      {
        m_attrtype = nsVariantType::Quaternion;
        break;
      }
      case(nsVariantType::Transform):
      {
        m_attrtype = nsVariantType::Transform;
        break;
      }
  }
}

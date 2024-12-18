#include <V8Engine/Utils/V8EValueMover.h>

namespace aperture::v8
{
  V8EValueMover::V8EValueMover(const nsUniquePtr<class V8EEngineRealm>& in_realm, const ::v8::Local<::v8::Value>& in_value)
  {
  }
  bool V8EValueMover::is_valid() const
  {
    return true;
  }
  nsString V8EValueMover::to_string() const
  {
    return "";
  }
  nsVariant V8EValueMover::to_variant() const
  {
    return "";
  }
} // namespace aperture::v8

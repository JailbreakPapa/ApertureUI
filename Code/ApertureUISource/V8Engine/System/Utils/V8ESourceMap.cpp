#include <V8Engine/System/Utils/V8ESourceMap.h>

using namespace aperture::v8::utility;

namespace
{
  constexpr uint8_t kBase64Unmap[] =
    {
      // 0x2b (+) -->
      0x3e, 0xff, 0xff, 0xff, 0x3f,
      // 0x30 -> 0x3f
      0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
      0xff, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
      0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0xff, 0xff, 0xff, 0xff, 0xff,
      0xff, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
      0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, // <-- 0x7a (z)
  };
}

void SourceMap::decode(int p_line, const char* p_token, const char* p_end, InternalPosition& r_pos, int& r_aline, int& r_acol)
{
  if (p_end - p_token <= 1)
    return;
  uint8_t shift = 0;
  int32_t value = 0;
  uint8_t index = 0;
  while (p_end != p_token)
  {
    const char token = *(p_token++);
    //NS_ASSERT_DEBUG(token >= '+' && token <= 'z');
    int32_t integer = kBase64Unmap[(uint8_t)(token - '+')];
    const bool cont = integer & 0x20;
    integer &= 0x1f;
    value |= integer << shift;
    if (cont)
    {
      shift += 5;
      continue;
    }
    const bool neg = value & 1;
    value = value >> 1;
    r_pos[index++] = neg ? -value : value;
    value = shift = 0;
  }
  r_aline += r_pos.source_line;
  r_acol += r_pos.source_column;
  nsDynamicArray<InternalPosition>& elements = (*this)[p_line].elements;
  elements.PushBack({r_pos.result_column, r_pos.source_index, r_aline, r_acol, r_pos._reserved});
}

SourceMap::InternalLine& aperture::v8::utility::SourceMap::operator[](int p_line)
{
  const int size = source_lines_.GetCount();
  if (size == 0 || source_lines_[size - 1].result_line != p_line)
  {
    source_lines_.PushBack({p_line, {}});
    return source_lines_[size];
  }
  return source_lines_[size - 1];
}


#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_RECT_PACK_IMPLEMENTATION
#define STBI_WRITE_NO_STDIO

#ifdef _MSC_VER
#define STBI_MSC_SECURE_CRT
#endif

//#define STBI_ASSERT(x) NS_ASSERT(x, "stbi_image assertion.")

#define STBI_ONLY_PNG
#define STBI_ONLY_JPEG
#define STBI_ONLY_HDR

#pragma warning(push)
#pragma warning(disable: 4505)  // Unreferenced local function has been removed

#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_rect_pack.h"

#pragma warning(pop)

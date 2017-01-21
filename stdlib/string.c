#include <string.h>

#undef memcpy

void* memcpy(void* dst, const void* src, size_t count)
{
  unsigned char* c_dst = dst;
  const unsigned char* c_src = src;

  for (size_t i = 0; i < count; ++i)
    c_dst[i] = c_src[i];

  return dst;
}

#ifndef STRING_H
#define STRING_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void* memcpy(void* dst, const void* src, size_t count);

#ifdef __cplusplus
} // extern "C"
#endif

#define memcpy(...) __builtin_memcpy(__VA_ARGS__)

#endif

// This is free and unencumbered software released into the public domain.
// 
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.
// 
// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
// 
// For more information, please refer to <https://unlicense.org>

// Released to public domain by Mark Whitney 2020

#ifndef WAU8_H_
#define WAU8_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define WAU8_KEY_SZ             (8U)
#define WAU8_ALL_WHEEL_SZ       (1981U)


typedef const uint8_t (*wau8_key_t)[WAU8_KEY_SZ];
typedef const uint8_t (*wau8_wheels_t)[WAU8_ALL_WHEEL_SZ];

typedef struct
{
    uint8_t pos[WAU8_KEY_SZ];
    wau8_wheels_t wheels;
} wau8_context_t;


extern const uint8_t WAU8_WHEEL_SZ[WAU8_KEY_SZ];
extern const uint16_t WAU8_WHEEL_OFFSET[WAU8_KEY_SZ];


void wau8_set_key(wau8_context_t * pcontext, const wau8_key_t pkey);
void wau8_set_wheels(wau8_context_t* pcontext, const wau8_wheels_t pwheels);

uint8_t wau8_advance(wau8_context_t * pcontext);

uint8_t wau8_get_val(const wau8_context_t * pcontext);

#ifdef __cplusplus
}
#endif

#endif // WAU8_H_

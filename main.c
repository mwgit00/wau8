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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "wau8.h"
#include "mywheels.h"

#define TEST_BUFF_SZ    (32U)

#if 0
#define SANITY_SZ   (64768) // 256 * 253 = 64768
uint8_t arr[65536];
void sanity_check(void)
{
    int ii;
    uint8_t ct[2];
    ct[0] = 0;
    ct[1] = 0;
    memset(arr, 0, sizeof(arr));

    // do a two-wheel test for wheel 256 and wheel 253
    // record how many times each position combo occurs
    for (ii = 0; ii < SANITY_SZ; ii++)
    {
        int idx = (ct[0] * 256) + ct[1];
        arr[idx]++;
        ct[0]++;
        ct[1] = (ct[1] + 1) % 253;
    }
    
    for (ii = 0; ii < 65536; ii++)
    {
        if ((ii & 0xFF) >= 253)
        {
            // there should be no occurrences where low byte >= 253
            if (arr[ii] != 0)
            {
                break;
            }
        }
        else
        {
            // everything else should occur only once
            if (arr[ii] != 1)
            {
                break;
            }
        }
    }
    
    if (ii == 65536)
    {
        printf("NOT INSANE\n");
    }
    else
    {
        printf("INSANE\n");
    }
}
#endif


#if 0
// code that was used for creating tables
void dump_tables(void)
{
    int ii;
    int jj;
    int jjmax;
    int nn;
    int zz;

    zz = 0;
    for (ii = 1; ii < WAU8_KEY_SZ; ii++)
    {
        nn = 0;
        printf("// wheel %u\n", WAU8_WHEEL_SZ[ii]);
        for (jj = 0; jj < WAU8_WHEEL_SZ[ii]; jj++)
        {
            printf("%iU,", (jj + 1) % WAU8_WHEEL_SZ[ii]);
            if (((nn + 1) % 15) == 0)
            {
                printf("\n");
            }
            nn++;
            zz++;
        }
        printf("\n");
    }
    
    printf("%i\n", zz);
    printf("%i\n", zz + 256);
}
#endif


// perform encryption/decryption with daisy-chained wau8 blocks
// arbitrary number of wau8 blocks can be used
void wau8xN_apply(
    wau8_context_t * pcon,
    const size_t ct,
    const char* itxt,
    char * otxt,
    const size_t sz)
{
    size_t ii;
    size_t jj;
    uint8_t val;
    uint8_t zeropos;

    // loop through input bytes
    for (jj = 0; jj < sz; jj++)
    {
        val = 0U;
        zeropos = 0U;

        // loop through wheel sets
        for (ii = 0; ii < ct; ii++)
        {
            val ^= wau8_get_val(&pcon[ii]);
            if (zeropos == 0)
            {
                // first wheel set is always advanced
                // next wheel set is advanced
                // if previous wheel set rolled over to pos 0
                zeropos = wau8_advance(&pcon[ii]);
            }
        }

        // encrypt or decrypt input value
        otxt[jj] = itxt[jj] ^ val;
    }
}


void set_rand_wheels(unsigned int seed)
{
    srand(seed);
    for (int ii = 0; ii < 2; ii++)
    {
        for (int nn = 0; nn < WAU8_WHEEL_SZ[0]; nn++) rand_wheels[ii].a[nn] = rand();
        for (int nn = 0; nn < WAU8_WHEEL_SZ[1]; nn++) rand_wheels[ii].b[nn] = rand();
        for (int nn = 0; nn < WAU8_WHEEL_SZ[2]; nn++) rand_wheels[ii].c[nn] = rand();
        for (int nn = 0; nn < WAU8_WHEEL_SZ[3]; nn++) rand_wheels[ii].d[nn] = rand();
        for (int nn = 0; nn < WAU8_WHEEL_SZ[4]; nn++) rand_wheels[ii].e[nn] = rand();
        for (int nn = 0; nn < WAU8_WHEEL_SZ[5]; nn++) rand_wheels[ii].f[nn] = rand();
        for (int nn = 0; nn < WAU8_WHEEL_SZ[6]; nn++) rand_wheels[ii].g[nn] = rand();
        for (int nn = 0; nn < WAU8_WHEEL_SZ[7]; nn++) rand_wheels[ii].h[nn] = rand();
    }
}


void wau8_test(const wau8_wheels_t * pw)
{
    // test two wau8 blocks daisy-chained together for a "128 bit" cipher

    wau8_context_t con[2];
    const uint8_t key[2][WAU8_KEY_SZ] =
    {
        { 255,252,250,248,246,244,240,238 },    // rolls over at next advance
        { 0,1,2,3,4,5,6,7 },
    };

    char atxt[TEST_BUFF_SZ] = "Hello world!";
    char btxt[TEST_BUFF_SZ];

    wau8_set_wheels(&con[0], &pw[0]);
    wau8_set_wheels(&con[1], &pw[1]);

    printf("\n----\n");
    printf("ENCRYPT\n");
    memset(btxt, 0, sizeof(btxt));
    wau8_set_key(&con[0], &key[0]);
    wau8_set_key(&con[1], &key[1]);
    wau8xN_apply(con, 2, atxt, btxt, TEST_BUFF_SZ);
    printf("PLAIN:  %s\n", atxt);
    printf("CIPHER:");
    for (int ii = 0; ii < 32; ii++)
    {
        printf(" %02X", btxt[ii] & 0xFF);
    }
    printf("\n");

    printf("DECRYPT\n");
    memset(atxt, 0, sizeof(atxt));
    wau8_set_key(&con[0], &key[0]);
    wau8_set_key(&con[1], &key[1]);
    wau8xN_apply(con, 2, btxt, atxt, TEST_BUFF_SZ);
    printf("PLAIN:  %s\n", atxt);
}


int main(int argc, char* argv[])
{
    // "null" wheels are all 0 so ciphertext will equal plaintext and vice versa
    
    wau8_test(null_wheels);
    
    // use random wheels to perform encryption/decryption
    
    set_rand_wheels(12345U);
    wau8_test(rand_wheels);
    
    set_rand_wheels(54321U);
    wau8_test(rand_wheels);

    return 0;
}

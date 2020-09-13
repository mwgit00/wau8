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

// Mark Whitney 2020

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "wau8.h"
#include "mywheels.h"

#define TEST_BUFF_SZ    (16U)


#define XWHEELCT (4U)
uint8_t xbuff[262144];

void sanity_check(void)
{
    size_t ii;
    size_t jj;
    
    // declare some relatively prime wheel sizes
    // and number of bits needed to store each wheel size
    // total bits is 18; 2^18 is 262144
    // expected number of wheel pos combos is product of wheel sizes
    const int xrp[XWHEELCT] = { 14 /* 2x7 */, 15 /* 3x5 */, 23, 29 };
    const int xbits[XWHEELCT] = { 4,4,5,5 };
    const int ncombos = xrp[0] * xrp[1] * xrp[2] * xrp[3];

    // init wheel positions and flag buffer
    int xct[XWHEELCT] = { 0,0,0,0 };
    memset(xbuff, 0, sizeof(xbuff));

    // loop through expected number of combos
    for (jj = 0; jj < ncombos; jj++)
    {
        // create index based on wheel positions
        int ix = 0;
        for (ii = 0; ii < XWHEELCT; ii++)
        {
            ix <<= xbits[ii];
            ix += xct[ii];
        }
        
        // flag each occurrence of index
        xbuff[ix] = 1;

        // advance the wheels
        for (ii = 0; ii < XWHEELCT; ii++)
        {
            xct[ii] = (xct[ii] + 1) % xrp[ii];
        }
    }

    // count number of times each index was flagged
    int nflags = 0;
    for (jj = 0; jj < sizeof(xbuff); jj++)
    {
        if (xbuff[jj] > 0) nflags++;
    }

    // number of flagged indexes
    // should equal number of expected combos
    printf("%i %i\n", ncombos, nflags);
}


// perform encryption/decryption
void process_message(
    wau8_context_t * pcon,
    const char* itxt,
    char * otxt,
    const size_t sz)
{
    size_t jj;
    uint8_t val;

    // loop through input bytes
    for (jj = 0; jj < sz; jj++)
    {
        val = wau8_get_val(pcon);
        wau8_advance(pcon);

        // encrypt or decrypt input value
        otxt[jj] = itxt[jj] ^ val;
    }
}


void set_rand_wheels(unsigned int seed)
{
    srand(seed);
    for (int nn = 0; nn < WAU8_WHEEL_SZ[0]; nn++) rand_wheels.a[nn] = rand();
    for (int nn = 0; nn < WAU8_WHEEL_SZ[1]; nn++) rand_wheels.b[nn] = rand();
    for (int nn = 0; nn < WAU8_WHEEL_SZ[2]; nn++) rand_wheels.c[nn] = rand();
    for (int nn = 0; nn < WAU8_WHEEL_SZ[3]; nn++) rand_wheels.d[nn] = rand();
    for (int nn = 0; nn < WAU8_WHEEL_SZ[4]; nn++) rand_wheels.e[nn] = rand();
    for (int nn = 0; nn < WAU8_WHEEL_SZ[5]; nn++) rand_wheels.f[nn] = rand();
    for (int nn = 0; nn < WAU8_WHEEL_SZ[6]; nn++) rand_wheels.g[nn] = rand();
    for (int nn = 0; nn < WAU8_WHEEL_SZ[7]; nn++) rand_wheels.h[nn] = rand();
}


void wau8_test(const wau8_wheels_t * pw)
{
    wau8_context_t con;

    // key rolls over to all 0s at next advance
    const uint8_t key[WAU8_KEY_SZ] =
    {
        255,252,250,248,246,244,240,238,
    };

    char atxt[TEST_BUFF_SZ] = "Hello world!";
    char btxt[TEST_BUFF_SZ];

    wau8_set_wheels(&con, pw);

    printf("\n----\n");
    printf("ENCRYPT\n");
    memset(btxt, 0, sizeof(btxt));
    wau8_set_key(&con, &key);
    process_message(&con, atxt, btxt, TEST_BUFF_SZ);
    printf("PLAIN:  %s\n", atxt);
    printf("CIPHER:");
    for (int ii = 0; ii < TEST_BUFF_SZ; ii++)
    {
        printf(" %02X", btxt[ii] & 0xFF);
    }
    printf("\n");

    printf("DECRYPT\n");
    memset(atxt, 0, sizeof(atxt));
    wau8_set_key(&con, &key);
    process_message(&con, btxt, atxt, TEST_BUFF_SZ);
    printf("PLAIN:  %s\n", atxt);
}


int main(int argc, char* argv[])
{
    sanity_check();
    
    // "null" wheels are all 0 so ciphertext will equal plaintext and vice versa
    
    wau8_test(&null_wheels);
    
    // use random wheels to perform encryption/decryption
    
    set_rand_wheels(01234U);
    wau8_test(&rand_wheels);

    set_rand_wheels(56789U);
    wau8_test(&rand_wheels);

    set_rand_wheels(98765U);
    wau8_test(&rand_wheels);

    return 0;
}

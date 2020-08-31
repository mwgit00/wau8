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

#include "wau8.h"


typedef struct
{
    uint8_t b[253];
    uint8_t c[251];
    uint8_t d[249];
    uint8_t e[247];
    uint8_t f[245];
    uint8_t g[241];
    uint8_t h[239];
} wau8_cheats_t;



// relatively prime wheel sizes
// should yield cycle size of 2^63.613300375511121451554498835938
const uint16_t WAU8_WHEEL_SZ[WAU8_KEY_SZ] =
{
    256U,   // 256 (2^8)
    253U,   // 253 (11x23)
    251U,   // 251 (prime)
    249U,   // 249 (3x83),
    247U,   // 247 (13x19)
    245U,   // 245 (5x7x7)
    241U,   // 241 (prime)
    239U    // 239 (prime)
};


// cheat table for incrementing wheel position and handling
// wraparound without having to do modulo or use an if-block
// wheel 256 has no table since it rolls over from 255 to 0 automatically
static const wau8_cheats_t WAU8_CHEATS =
{
// wheel 253
{
1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 9U, 10U, 11U, 12U, 13U, 14U, 15U,
16U, 17U, 18U, 19U, 20U, 21U, 22U, 23U, 24U, 25U, 26U, 27U, 28U, 29U, 30U,
31U, 32U, 33U, 34U, 35U, 36U, 37U, 38U, 39U, 40U, 41U, 42U, 43U, 44U, 45U,
46U, 47U, 48U, 49U, 50U, 51U, 52U, 53U, 54U, 55U, 56U, 57U, 58U, 59U, 60U,
61U, 62U, 63U, 64U, 65U, 66U, 67U, 68U, 69U, 70U, 71U, 72U, 73U, 74U, 75U,
76U, 77U, 78U, 79U, 80U, 81U, 82U, 83U, 84U, 85U, 86U, 87U, 88U, 89U, 90U,
91U, 92U, 93U, 94U, 95U, 96U, 97U, 98U, 99U, 100U, 101U, 102U, 103U, 104U, 105U,
106U, 107U, 108U, 109U, 110U, 111U, 112U, 113U, 114U, 115U, 116U, 117U, 118U, 119U, 120U,
121U, 122U, 123U, 124U, 125U, 126U, 127U, 128U, 129U, 130U, 131U, 132U, 133U, 134U, 135U,
136U, 137U, 138U, 139U, 140U, 141U, 142U, 143U, 144U, 145U, 146U, 147U, 148U, 149U, 150U,
151U, 152U, 153U, 154U, 155U, 156U, 157U, 158U, 159U, 160U, 161U, 162U, 163U, 164U, 165U,
166U, 167U, 168U, 169U, 170U, 171U, 172U, 173U, 174U, 175U, 176U, 177U, 178U, 179U, 180U,
181U, 182U, 183U, 184U, 185U, 186U, 187U, 188U, 189U, 190U, 191U, 192U, 193U, 194U, 195U,
196U, 197U, 198U, 199U, 200U, 201U, 202U, 203U, 204U, 205U, 206U, 207U, 208U, 209U, 210U,
211U, 212U, 213U, 214U, 215U, 216U, 217U, 218U, 219U, 220U, 221U, 222U, 223U, 224U, 225U,
226U, 227U, 228U, 229U, 230U, 231U, 232U, 233U, 234U, 235U, 236U, 237U, 238U, 239U, 240U,
241U, 242U, 243U, 244U, 245U, 246U, 247U, 248U, 249U, 250U, 251U, 252U, 0U,
},
// wheel 251
{
1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 9U, 10U, 11U, 12U, 13U, 14U, 15U,
16U, 17U, 18U, 19U, 20U, 21U, 22U, 23U, 24U, 25U, 26U, 27U, 28U, 29U, 30U,
31U, 32U, 33U, 34U, 35U, 36U, 37U, 38U, 39U, 40U, 41U, 42U, 43U, 44U, 45U,
46U, 47U, 48U, 49U, 50U, 51U, 52U, 53U, 54U, 55U, 56U, 57U, 58U, 59U, 60U,
61U, 62U, 63U, 64U, 65U, 66U, 67U, 68U, 69U, 70U, 71U, 72U, 73U, 74U, 75U,
76U, 77U, 78U, 79U, 80U, 81U, 82U, 83U, 84U, 85U, 86U, 87U, 88U, 89U, 90U,
91U, 92U, 93U, 94U, 95U, 96U, 97U, 98U, 99U, 100U, 101U, 102U, 103U, 104U, 105U,
106U, 107U, 108U, 109U, 110U, 111U, 112U, 113U, 114U, 115U, 116U, 117U, 118U, 119U, 120U,
121U, 122U, 123U, 124U, 125U, 126U, 127U, 128U, 129U, 130U, 131U, 132U, 133U, 134U, 135U,
136U, 137U, 138U, 139U, 140U, 141U, 142U, 143U, 144U, 145U, 146U, 147U, 148U, 149U, 150U,
151U, 152U, 153U, 154U, 155U, 156U, 157U, 158U, 159U, 160U, 161U, 162U, 163U, 164U, 165U,
166U, 167U, 168U, 169U, 170U, 171U, 172U, 173U, 174U, 175U, 176U, 177U, 178U, 179U, 180U,
181U, 182U, 183U, 184U, 185U, 186U, 187U, 188U, 189U, 190U, 191U, 192U, 193U, 194U, 195U,
196U, 197U, 198U, 199U, 200U, 201U, 202U, 203U, 204U, 205U, 206U, 207U, 208U, 209U, 210U,
211U, 212U, 213U, 214U, 215U, 216U, 217U, 218U, 219U, 220U, 221U, 222U, 223U, 224U, 225U,
226U, 227U, 228U, 229U, 230U, 231U, 232U, 233U, 234U, 235U, 236U, 237U, 238U, 239U, 240U,
241U, 242U, 243U, 244U, 245U, 246U, 247U, 248U, 249U, 250U, 0U,
},
// wheel 249
{
1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 9U, 10U, 11U, 12U, 13U, 14U, 15U,
16U, 17U, 18U, 19U, 20U, 21U, 22U, 23U, 24U, 25U, 26U, 27U, 28U, 29U, 30U,
31U, 32U, 33U, 34U, 35U, 36U, 37U, 38U, 39U, 40U, 41U, 42U, 43U, 44U, 45U,
46U, 47U, 48U, 49U, 50U, 51U, 52U, 53U, 54U, 55U, 56U, 57U, 58U, 59U, 60U,
61U, 62U, 63U, 64U, 65U, 66U, 67U, 68U, 69U, 70U, 71U, 72U, 73U, 74U, 75U,
76U, 77U, 78U, 79U, 80U, 81U, 82U, 83U, 84U, 85U, 86U, 87U, 88U, 89U, 90U,
91U, 92U, 93U, 94U, 95U, 96U, 97U, 98U, 99U, 100U, 101U, 102U, 103U, 104U, 105U,
106U, 107U, 108U, 109U, 110U, 111U, 112U, 113U, 114U, 115U, 116U, 117U, 118U, 119U, 120U,
121U, 122U, 123U, 124U, 125U, 126U, 127U, 128U, 129U, 130U, 131U, 132U, 133U, 134U, 135U,
136U, 137U, 138U, 139U, 140U, 141U, 142U, 143U, 144U, 145U, 146U, 147U, 148U, 149U, 150U,
151U, 152U, 153U, 154U, 155U, 156U, 157U, 158U, 159U, 160U, 161U, 162U, 163U, 164U, 165U,
166U, 167U, 168U, 169U, 170U, 171U, 172U, 173U, 174U, 175U, 176U, 177U, 178U, 179U, 180U,
181U, 182U, 183U, 184U, 185U, 186U, 187U, 188U, 189U, 190U, 191U, 192U, 193U, 194U, 195U,
196U, 197U, 198U, 199U, 200U, 201U, 202U, 203U, 204U, 205U, 206U, 207U, 208U, 209U, 210U,
211U, 212U, 213U, 214U, 215U, 216U, 217U, 218U, 219U, 220U, 221U, 222U, 223U, 224U, 225U,
226U, 227U, 228U, 229U, 230U, 231U, 232U, 233U, 234U, 235U, 236U, 237U, 238U, 239U, 240U,
241U, 242U, 243U, 244U, 245U, 246U, 247U, 248U, 0U,
},
// wheel 247
{
1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 9U, 10U, 11U, 12U, 13U, 14U, 15U,
16U, 17U, 18U, 19U, 20U, 21U, 22U, 23U, 24U, 25U, 26U, 27U, 28U, 29U, 30U,
31U, 32U, 33U, 34U, 35U, 36U, 37U, 38U, 39U, 40U, 41U, 42U, 43U, 44U, 45U,
46U, 47U, 48U, 49U, 50U, 51U, 52U, 53U, 54U, 55U, 56U, 57U, 58U, 59U, 60U,
61U, 62U, 63U, 64U, 65U, 66U, 67U, 68U, 69U, 70U, 71U, 72U, 73U, 74U, 75U,
76U, 77U, 78U, 79U, 80U, 81U, 82U, 83U, 84U, 85U, 86U, 87U, 88U, 89U, 90U,
91U, 92U, 93U, 94U, 95U, 96U, 97U, 98U, 99U, 100U, 101U, 102U, 103U, 104U, 105U,
106U, 107U, 108U, 109U, 110U, 111U, 112U, 113U, 114U, 115U, 116U, 117U, 118U, 119U, 120U,
121U, 122U, 123U, 124U, 125U, 126U, 127U, 128U, 129U, 130U, 131U, 132U, 133U, 134U, 135U,
136U, 137U, 138U, 139U, 140U, 141U, 142U, 143U, 144U, 145U, 146U, 147U, 148U, 149U, 150U,
151U, 152U, 153U, 154U, 155U, 156U, 157U, 158U, 159U, 160U, 161U, 162U, 163U, 164U, 165U,
166U, 167U, 168U, 169U, 170U, 171U, 172U, 173U, 174U, 175U, 176U, 177U, 178U, 179U, 180U,
181U, 182U, 183U, 184U, 185U, 186U, 187U, 188U, 189U, 190U, 191U, 192U, 193U, 194U, 195U,
196U, 197U, 198U, 199U, 200U, 201U, 202U, 203U, 204U, 205U, 206U, 207U, 208U, 209U, 210U,
211U, 212U, 213U, 214U, 215U, 216U, 217U, 218U, 219U, 220U, 221U, 222U, 223U, 224U, 225U,
226U, 227U, 228U, 229U, 230U, 231U, 232U, 233U, 234U, 235U, 236U, 237U, 238U, 239U, 240U,
241U, 242U, 243U, 244U, 245U, 246U, 0U,
},
// wheel 245
{
1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 9U, 10U, 11U, 12U, 13U, 14U, 15U,
16U, 17U, 18U, 19U, 20U, 21U, 22U, 23U, 24U, 25U, 26U, 27U, 28U, 29U, 30U,
31U, 32U, 33U, 34U, 35U, 36U, 37U, 38U, 39U, 40U, 41U, 42U, 43U, 44U, 45U,
46U, 47U, 48U, 49U, 50U, 51U, 52U, 53U, 54U, 55U, 56U, 57U, 58U, 59U, 60U,
61U, 62U, 63U, 64U, 65U, 66U, 67U, 68U, 69U, 70U, 71U, 72U, 73U, 74U, 75U,
76U, 77U, 78U, 79U, 80U, 81U, 82U, 83U, 84U, 85U, 86U, 87U, 88U, 89U, 90U,
91U, 92U, 93U, 94U, 95U, 96U, 97U, 98U, 99U, 100U, 101U, 102U, 103U, 104U, 105U,
106U, 107U, 108U, 109U, 110U, 111U, 112U, 113U, 114U, 115U, 116U, 117U, 118U, 119U, 120U,
121U, 122U, 123U, 124U, 125U, 126U, 127U, 128U, 129U, 130U, 131U, 132U, 133U, 134U, 135U,
136U, 137U, 138U, 139U, 140U, 141U, 142U, 143U, 144U, 145U, 146U, 147U, 148U, 149U, 150U,
151U, 152U, 153U, 154U, 155U, 156U, 157U, 158U, 159U, 160U, 161U, 162U, 163U, 164U, 165U,
166U, 167U, 168U, 169U, 170U, 171U, 172U, 173U, 174U, 175U, 176U, 177U, 178U, 179U, 180U,
181U, 182U, 183U, 184U, 185U, 186U, 187U, 188U, 189U, 190U, 191U, 192U, 193U, 194U, 195U,
196U, 197U, 198U, 199U, 200U, 201U, 202U, 203U, 204U, 205U, 206U, 207U, 208U, 209U, 210U,
211U, 212U, 213U, 214U, 215U, 216U, 217U, 218U, 219U, 220U, 221U, 222U, 223U, 224U, 225U,
226U, 227U, 228U, 229U, 230U, 231U, 232U, 233U, 234U, 235U, 236U, 237U, 238U, 239U, 240U,
241U, 242U, 243U, 244U, 0U,
},
// wheel 241
{
1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 9U, 10U, 11U, 12U, 13U, 14U, 15U,
16U, 17U, 18U, 19U, 20U, 21U, 22U, 23U, 24U, 25U, 26U, 27U, 28U, 29U, 30U,
31U, 32U, 33U, 34U, 35U, 36U, 37U, 38U, 39U, 40U, 41U, 42U, 43U, 44U, 45U,
46U, 47U, 48U, 49U, 50U, 51U, 52U, 53U, 54U, 55U, 56U, 57U, 58U, 59U, 60U,
61U, 62U, 63U, 64U, 65U, 66U, 67U, 68U, 69U, 70U, 71U, 72U, 73U, 74U, 75U,
76U, 77U, 78U, 79U, 80U, 81U, 82U, 83U, 84U, 85U, 86U, 87U, 88U, 89U, 90U,
91U, 92U, 93U, 94U, 95U, 96U, 97U, 98U, 99U, 100U, 101U, 102U, 103U, 104U, 105U,
106U, 107U, 108U, 109U, 110U, 111U, 112U, 113U, 114U, 115U, 116U, 117U, 118U, 119U, 120U,
121U, 122U, 123U, 124U, 125U, 126U, 127U, 128U, 129U, 130U, 131U, 132U, 133U, 134U, 135U,
136U, 137U, 138U, 139U, 140U, 141U, 142U, 143U, 144U, 145U, 146U, 147U, 148U, 149U, 150U,
151U, 152U, 153U, 154U, 155U, 156U, 157U, 158U, 159U, 160U, 161U, 162U, 163U, 164U, 165U,
166U, 167U, 168U, 169U, 170U, 171U, 172U, 173U, 174U, 175U, 176U, 177U, 178U, 179U, 180U,
181U, 182U, 183U, 184U, 185U, 186U, 187U, 188U, 189U, 190U, 191U, 192U, 193U, 194U, 195U,
196U, 197U, 198U, 199U, 200U, 201U, 202U, 203U, 204U, 205U, 206U, 207U, 208U, 209U, 210U,
211U, 212U, 213U, 214U, 215U, 216U, 217U, 218U, 219U, 220U, 221U, 222U, 223U, 224U, 225U,
226U, 227U, 228U, 229U, 230U, 231U, 232U, 233U, 234U, 235U, 236U, 237U, 238U, 239U, 240U,
0U,
},
// wheel 239
{
1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 9U, 10U, 11U, 12U, 13U, 14U, 15U,
16U, 17U, 18U, 19U, 20U, 21U, 22U, 23U, 24U, 25U, 26U, 27U, 28U, 29U, 30U,
31U, 32U, 33U, 34U, 35U, 36U, 37U, 38U, 39U, 40U, 41U, 42U, 43U, 44U, 45U,
46U, 47U, 48U, 49U, 50U, 51U, 52U, 53U, 54U, 55U, 56U, 57U, 58U, 59U, 60U,
61U, 62U, 63U, 64U, 65U, 66U, 67U, 68U, 69U, 70U, 71U, 72U, 73U, 74U, 75U,
76U, 77U, 78U, 79U, 80U, 81U, 82U, 83U, 84U, 85U, 86U, 87U, 88U, 89U, 90U,
91U, 92U, 93U, 94U, 95U, 96U, 97U, 98U, 99U, 100U, 101U, 102U, 103U, 104U, 105U,
106U, 107U, 108U, 109U, 110U, 111U, 112U, 113U, 114U, 115U, 116U, 117U, 118U, 119U, 120U,
121U, 122U, 123U, 124U, 125U, 126U, 127U, 128U, 129U, 130U, 131U, 132U, 133U, 134U, 135U,
136U, 137U, 138U, 139U, 140U, 141U, 142U, 143U, 144U, 145U, 146U, 147U, 148U, 149U, 150U,
151U, 152U, 153U, 154U, 155U, 156U, 157U, 158U, 159U, 160U, 161U, 162U, 163U, 164U, 165U,
166U, 167U, 168U, 169U, 170U, 171U, 172U, 173U, 174U, 175U, 176U, 177U, 178U, 179U, 180U,
181U, 182U, 183U, 184U, 185U, 186U, 187U, 188U, 189U, 190U, 191U, 192U, 193U, 194U, 195U,
196U, 197U, 198U, 199U, 200U, 201U, 202U, 203U, 204U, 205U, 206U, 207U, 208U, 209U, 210U,
211U, 212U, 213U, 214U, 215U, 216U, 217U, 218U, 219U, 220U, 221U, 222U, 223U, 224U, 225U,
226U, 227U, 228U, 229U, 230U, 231U, 232U, 233U, 234U, 235U, 236U, 237U, 238U, 0U,
},
};



// set initial wheel positions (the key)
// do modulo operations to prevent key value from exceeding wheel size
void wau8_set_key(wau8_context_t* pcontext, const wau8_key_t pkey)
{
    pcontext->posa = (*pkey)[0];
    pcontext->posb = (*pkey)[1] % WAU8_WHEEL_SZ[1];
    pcontext->posc = (*pkey)[2] % WAU8_WHEEL_SZ[2];
    pcontext->posd = (*pkey)[3] % WAU8_WHEEL_SZ[3];
    pcontext->pose = (*pkey)[4] % WAU8_WHEEL_SZ[4];
    pcontext->posf = (*pkey)[5] % WAU8_WHEEL_SZ[5];
    pcontext->posg = (*pkey)[6] % WAU8_WHEEL_SZ[6];
    pcontext->posh = (*pkey)[7] % WAU8_WHEEL_SZ[7];
}


void wau8_set_wheels(wau8_context_t* pcontext, const wau8_wheels_t * pwheels)
{
    pcontext->pwheels = pwheels;
}


// advance the wheels
void wau8_advance(wau8_context_t * pcontext)
{
    // increment positions and perform wraparound
    pcontext->posa++;
    pcontext->posb = WAU8_CHEATS.b[pcontext->posb];
    pcontext->posc = WAU8_CHEATS.c[pcontext->posc];
    pcontext->posd = WAU8_CHEATS.d[pcontext->posd];
    pcontext->pose = WAU8_CHEATS.e[pcontext->pose];
    pcontext->posf = WAU8_CHEATS.f[pcontext->posf];
    pcontext->posg = WAU8_CHEATS.g[pcontext->posg];
    pcontext->posg = WAU8_CHEATS.h[pcontext->posh];
}


// returns the current encrypting/decrypting byte
uint8_t wau8_get_val(const wau8_context_t * pcontext)
{
    uint8_t result = 0U;
    result ^= pcontext->pwheels->a[pcontext->posa];
    result ^= pcontext->pwheels->b[pcontext->posb];
    result ^= pcontext->pwheels->c[pcontext->posc];
    result ^= pcontext->pwheels->d[pcontext->posd];
    result ^= pcontext->pwheels->e[pcontext->pose];
    result ^= pcontext->pwheels->f[pcontext->posf];
    result ^= pcontext->pwheels->g[pcontext->posg];
    result ^= pcontext->pwheels->h[pcontext->posh];
    return result;
}

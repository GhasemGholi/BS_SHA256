#include <iostream>
#include <array>
#include <immintrin.h>
#include <bitset>
#include <cstring>

using namespace std;
 
constexpr uint32_t padding[] = {
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

constexpr uint32_t K[64] = { // round constants
    0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
    0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
    0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
    0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
    0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
    0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
    0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
    0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

//get a specific bit out of a uint32_t
#define GET_BIT(value, index) value & (1 << index)
//set a specific bit in a uint32_t
#define SET_BIT(value, index, valueOfBit)  value &= ~(1 << index);\
                                           value |= valueOfBit; 

#define XOR256(A, B, result) result[0] = _mm256_xor_si256(A[0], B[0]); result[1] = _mm256_xor_si256(A[1], B[1]); result[2] = _mm256_xor_si256(A[2], B[2]); result[3] = _mm256_xor_si256(A[3], B[3]); result[4] = _mm256_xor_si256(A[4], B[4]); result[5] = _mm256_xor_si256(A[5], B[5]); result[6] = _mm256_xor_si256(A[6], B[6]); result[7] = _mm256_xor_si256(A[7], B[7]); result[8] = _mm256_xor_si256(A[8], B[8]); result[9] = _mm256_xor_si256(A[9], B[9]); result[10] = _mm256_xor_si256(A[10], B[10]); result[11] = _mm256_xor_si256(A[11], B[11]); result[12] = _mm256_xor_si256(A[12], B[12]); result[13] = _mm256_xor_si256(A[13], B[13]); result[14] = _mm256_xor_si256(A[14], B[14]); result[15] = _mm256_xor_si256(A[15], B[15]); result[16] = _mm256_xor_si256(A[16], B[16]); result[17] = _mm256_xor_si256(A[17], B[17]); result[18] = _mm256_xor_si256(A[18], B[18]); result[19] = _mm256_xor_si256(A[19], B[19]); result[20] = _mm256_xor_si256(A[20], B[20]); result[21] = _mm256_xor_si256(A[21], B[21]); result[22] = _mm256_xor_si256(A[22], B[22]); result[23] = _mm256_xor_si256(A[23], B[23]); result[24] = _mm256_xor_si256(A[24], B[24]); result[25] = _mm256_xor_si256(A[25], B[25]); result[26] = _mm256_xor_si256(A[26], B[26]); result[27] = _mm256_xor_si256(A[27], B[27]); result[28] = _mm256_xor_si256(A[28], B[28]); result[29] = _mm256_xor_si256(A[29], B[29]); result[30] = _mm256_xor_si256(A[30], B[30]); result[31] = _mm256_xor_si256(A[31], B[31]); 
#define OR256(A, B, result) result[0] = _mm256_or_si256(A[0], B[0]); result[1] = _mm256_or_si256(A[1], B[1]); result[2] = _mm256_or_si256(A[2], B[2]); result[3] = _mm256_or_si256(A[3], B[3]); result[4] = _mm256_or_si256(A[4], B[4]); result[5] = _mm256_or_si256(A[5], B[5]); result[6] = _mm256_or_si256(A[6], B[6]); result[7] = _mm256_or_si256(A[7], B[7]); result[8] = _mm256_or_si256(A[8], B[8]); result[9] = _mm256_or_si256(A[9], B[9]); result[10] = _mm256_or_si256(A[10], B[10]); result[11] = _mm256_or_si256(A[11], B[11]); result[12] = _mm256_or_si256(A[12], B[12]); result[13] = _mm256_or_si256(A[13], B[13]); result[14] = _mm256_or_si256(A[14], B[14]); result[15] = _mm256_or_si256(A[15], B[15]); result[16] = _mm256_or_si256(A[16], B[16]); result[17] = _mm256_or_si256(A[17], B[17]); result[18] = _mm256_or_si256(A[18], B[18]); result[19] = _mm256_or_si256(A[19], B[19]); result[20] = _mm256_or_si256(A[20], B[20]); result[21] = _mm256_or_si256(A[21], B[21]); result[22] = _mm256_or_si256(A[22], B[22]); result[23] = _mm256_or_si256(A[23], B[23]); result[24] = _mm256_or_si256(A[24], B[24]); result[25] = _mm256_or_si256(A[25], B[25]); result[26] = _mm256_or_si256(A[26], B[26]); result[27] = _mm256_or_si256(A[27], B[27]); result[28] = _mm256_or_si256(A[28], B[28]); result[29] = _mm256_or_si256(A[29], B[29]); result[30] = _mm256_or_si256(A[30], B[30]); result[31] = _mm256_or_si256(A[31], B[31]); 
#define AND256(A, B, result) result[0] = _mm256_and_si256(A[0], B[0]); result[1] = _mm256_and_si256(A[1], B[1]); result[2] = _mm256_and_si256(A[2], B[2]); result[3] = _mm256_and_si256(A[3], B[3]); result[4] = _mm256_and_si256(A[4], B[4]); result[5] = _mm256_and_si256(A[5], B[5]); result[6] = _mm256_and_si256(A[6], B[6]); result[7] = _mm256_and_si256(A[7], B[7]); result[8] = _mm256_and_si256(A[8], B[8]); result[9] = _mm256_and_si256(A[9], B[9]); result[10] = _mm256_and_si256(A[10], B[10]); result[11] = _mm256_and_si256(A[11], B[11]); result[12] = _mm256_and_si256(A[12], B[12]); result[13] = _mm256_and_si256(A[13], B[13]); result[14] = _mm256_and_si256(A[14], B[14]); result[15] = _mm256_and_si256(A[15], B[15]); result[16] = _mm256_and_si256(A[16], B[16]); result[17] = _mm256_and_si256(A[17], B[17]); result[18] = _mm256_and_si256(A[18], B[18]); result[19] = _mm256_and_si256(A[19], B[19]); result[20] = _mm256_and_si256(A[20], B[20]); result[21] = _mm256_and_si256(A[21], B[21]); result[22] = _mm256_and_si256(A[22], B[22]); result[23] = _mm256_and_si256(A[23], B[23]); result[24] = _mm256_and_si256(A[24], B[24]); result[25] = _mm256_and_si256(A[25], B[25]); result[26] = _mm256_and_si256(A[26], B[26]); result[27] = _mm256_and_si256(A[27], B[27]); result[28] = _mm256_and_si256(A[28], B[28]); result[29] = _mm256_and_si256(A[29], B[29]); result[30] = _mm256_and_si256(A[30], B[30]); result[31] = _mm256_and_si256(A[31], B[31]);
#define ANDNOT256(A, B, result) result[0] = _mm256_andnot_si256(A[0], B[0]); result[1] = _mm256_andnot_si256(A[1], B[1]); result[2] = _mm256_andnot_si256(A[2], B[2]); result[3] = _mm256_andnot_si256(A[3], B[3]); result[4] = _mm256_andnot_si256(A[4], B[4]); result[5] = _mm256_andnot_si256(A[5], B[5]); result[6] = _mm256_andnot_si256(A[6], B[6]); result[7] = _mm256_andnot_si256(A[7], B[7]); result[8] = _mm256_andnot_si256(A[8], B[8]); result[9] = _mm256_andnot_si256(A[9], B[9]); result[10] = _mm256_andnot_si256(A[10], B[10]); result[11] = _mm256_andnot_si256(A[11], B[11]); result[12] = _mm256_andnot_si256(A[12], B[12]); result[13] = _mm256_andnot_si256(A[13], B[13]); result[14] = _mm256_andnot_si256(A[14], B[14]); result[15] = _mm256_andnot_si256(A[15], B[15]); result[16] = _mm256_andnot_si256(A[16], B[16]); result[17] = _mm256_andnot_si256(A[17], B[17]); result[18] = _mm256_andnot_si256(A[18], B[18]); result[19] = _mm256_andnot_si256(A[19], B[19]); result[20] = _mm256_andnot_si256(A[20], B[20]); result[21] = _mm256_andnot_si256(A[21], B[21]); result[22] = _mm256_andnot_si256(A[22], B[22]); result[23] = _mm256_andnot_si256(A[23], B[23]); result[24] = _mm256_andnot_si256(A[24], B[24]); result[25] = _mm256_andnot_si256(A[25], B[25]); result[26] = _mm256_andnot_si256(A[26], B[26]); result[27] = _mm256_andnot_si256(A[27], B[27]); result[28] = _mm256_andnot_si256(A[28], B[28]); result[29] = _mm256_andnot_si256(A[29], B[29]); result[30] = _mm256_andnot_si256(A[30], B[30]); result[31] = _mm256_andnot_si256(A[31], B[31]);

//memset single __m256i to val
static inline void * memset_256bit_single(void *dest, const __m256i val){
    __m256i *ptr = (__m256i*)dest;
    _mm256_storeu_si256(ptr++, val);

    return dest;
}

//memset array __m256i of size 32 to val
static inline void * memset_256bit(void *dest, const __m256i val){
    __m256i *ptr = (__m256i*)dest;
    uint8_t len = 32;
    while (len--){
        _mm256_storeu_si256(ptr++, val);
    }

    return dest;
}


//memset array __m256i of size 32 to 0
static inline void * memset_256bit_zero(void *dest){
    __m256i *ptr = (__m256i*)dest;
    uint8_t len = 32;
    while (len--){
        *ptr++ = _mm256_setzero_si256();
    }

    return dest;
}

    
//memset array __m256i of size 32 to val
static inline void * memset_256bit_n(void *dest, const __m256i val, uint8_t from){
    __m256i *ptr = (__m256i*)dest;
    while (from++ < 32){
        _mm256_storeu_si256(ptr++, val);
    }

    return dest;
}

//memcpy array of __m256i to other array of __m256i, both of size 32
static inline void * memcpy_256bit(void *dest, const void *src){
    const __m256i* s = (__m256i*)src;
    __m256i* d = (__m256i*)dest;
    uint8_t len = 32;
    while (len--){
        *d++ = _mm256_load_si256(s++);
    }
    return dest;
}

//memcpy array of __m256i to other array of __m256i, both of size 32, starting from a specified index
static inline void * memcpy_256bit_nbits(void *dest, const void *src, size_t from){
    const __m256i* s = (__m256i*)src;
    __m256i* d = (__m256i*)dest;
    d += from;
    while (from++ < 32){
        *d++ = _mm256_load_si256(s++);
    }

    return dest;
}

//macro that transposes a uint32_t to a bitsliced representation in the form of a  __m256i array
#define BSCONST(value, result, i) result[i] = (value & (1 << i)) > 0 ? _mm256_set1_epi32(0xffffffff) : _mm256_setzero_si256();

#define do_bsconst32(value, result) BSCONST(value, result, 0); BSCONST(value, result, 1); BSCONST(value, result, 2); BSCONST(value, result, 3); BSCONST(value, result, 4); BSCONST(value, result, 5); BSCONST(value, result, 6); BSCONST(value, result, 7); BSCONST(value, result, 8); BSCONST(value, result, 9); BSCONST(value, result, 10); BSCONST(value, result, 11); BSCONST(value, result, 12); BSCONST(value, result, 13); BSCONST(value, result, 14); BSCONST(value, result, 15); BSCONST(value, result, 16); BSCONST(value, result, 17); BSCONST(value, result, 18); BSCONST(value, result, 19); BSCONST(value, result, 20); BSCONST(value, result, 21); BSCONST(value, result, 22); BSCONST(value, result, 23); BSCONST(value, result, 24); BSCONST(value, result, 25); BSCONST(value, result, 26); BSCONST(value, result, 27); BSCONST(value, result, 28); BSCONST(value, result, 29); BSCONST(value, result, 30); BSCONST(value, result, 31)

// this shifts the __m256i array to the right by indices and pads the leftover bits to 0
static inline void paddify_right_shift_temp(__m256i Old[],__m256i result[], const uint32_t &shift){
    memset_256bit_zero(result);
    for(uint32_t i = 0, j = shift; i < 32 - shift; i++, j++){
        result[i] = Old[j];
    }
}

// these macros shifts the __m256i array to the right by x(which are defined after pad_rs_*x indices and pads the leftover bits to 0
#define pad_rs_3(old, result) result[0] = old[3]; result[1] = old[4]; result[2] = old[5]; result[3] = old[6]; result[4] = old[7]; result[5] = old[8]; result[6] = old[9]; result[7] = old[10]; result[8] = old[11]; result[9] = old[12]; result[10] = old[13]; result[11] = old[14]; result[12] = old[15]; result[13] = old[16]; result[14] = old[17]; result[15] = old[18]; result[16] = old[19]; result[17] = old[20]; result[18] = old[21]; result[19] = old[22]; result[20] = old[23]; result[21] = old[24]; result[22] = old[25]; result[23] = old[26]; result[24] = old[27]; result[25] = old[28]; result[26] = old[29]; result[27] = old[30]; result[28] = old[31]; result[29] = _mm256_setzero_si256(); result[30] = _mm256_setzero_si256(); result[31] = _mm256_setzero_si256();
#define pad_rs_7(old, result) result[0] = old[7]; result[1] = old[8]; result[2] = old[9]; result[3] = old[10]; result[4] = old[11]; result[5] = old[12]; result[6] = old[13]; result[7] = old[14]; result[8] = old[15]; result[9] = old[16]; result[10] = old[17]; result[11] = old[18]; result[12] = old[19]; result[13] = old[20]; result[14] = old[21]; result[15] = old[22]; result[16] = old[23]; result[17] = old[24]; result[18] = old[25]; result[19] = old[26]; result[20] = old[27]; result[21] = old[28]; result[22] = old[29]; result[23] = old[30]; result[24] = old[31]; result[25] = _mm256_setzero_si256(); result[26] = _mm256_setzero_si256(); result[27] = _mm256_setzero_si256(); result[28] = _mm256_setzero_si256(); result[29] = _mm256_setzero_si256(); result[30] = _mm256_setzero_si256(); result[31] = _mm256_setzero_si256();
#define pad_rs_18(old, result) result[0] = old[18]; result[1] = old[19]; result[2] = old[20]; result[3] = old[21]; result[4] = old[22]; result[5] = old[23]; result[6] = old[24]; result[7] = old[25]; result[8] = old[26]; result[9] = old[27]; result[10] = old[28]; result[11] = old[29]; result[12] = old[30]; result[13] = old[31]; result[14] = _mm256_setzero_si256(); result[15] = _mm256_setzero_si256(); result[16] = _mm256_setzero_si256(); result[17] = _mm256_setzero_si256(); result[18] = _mm256_setzero_si256(); result[19] = _mm256_setzero_si256(); result[20] = _mm256_setzero_si256(); result[21] = _mm256_setzero_si256(); result[22] = _mm256_setzero_si256(); result[23] = _mm256_setzero_si256(); result[24] = _mm256_setzero_si256(); result[25] = _mm256_setzero_si256(); result[26] = _mm256_setzero_si256(); result[27] = _mm256_setzero_si256(); result[28] = _mm256_setzero_si256(); result[29] = _mm256_setzero_si256(); result[30] = _mm256_setzero_si256(); result[31] = _mm256_setzero_si256();
#define pad_rs_17(old, result) result[0] = old[17]; result[1] = old[18]; result[2] = old[19]; result[3] = old[20]; result[4] = old[21]; result[5] = old[22]; result[6] = old[23]; result[7] = old[24]; result[8] = old[25]; result[9] = old[26]; result[10] = old[27]; result[11] = old[28]; result[12] = old[29]; result[13] = old[30]; result[14] = old[31]; result[15] = _mm256_setzero_si256(); result[16] = _mm256_setzero_si256(); result[17] = _mm256_setzero_si256(); result[18] = _mm256_setzero_si256(); result[19] = _mm256_setzero_si256(); result[20] = _mm256_setzero_si256(); result[21] = _mm256_setzero_si256(); result[22] = _mm256_setzero_si256(); result[23] = _mm256_setzero_si256(); result[24] = _mm256_setzero_si256(); result[25] = _mm256_setzero_si256(); result[26] = _mm256_setzero_si256(); result[27] = _mm256_setzero_si256(); result[28] = _mm256_setzero_si256(); result[29] = _mm256_setzero_si256(); result[30] = _mm256_setzero_si256(); result[31] = _mm256_setzero_si256(); 
#define pad_rs_19(old, result) result[0] = old[19]; result[1] = old[20]; result[2] = old[21]; result[3] = old[22]; result[4] = old[23]; result[5] = old[24]; result[6] = old[25]; result[7] = old[26]; result[8] = old[27]; result[9] = old[28]; result[10] = old[29]; result[11] = old[30]; result[12] = old[31]; result[13] = _mm256_setzero_si256(); result[14] = _mm256_setzero_si256(); result[15] = _mm256_setzero_si256(); result[16] = _mm256_setzero_si256(); result[17] = _mm256_setzero_si256(); result[18] = _mm256_setzero_si256(); result[19] = _mm256_setzero_si256(); result[20] = _mm256_setzero_si256(); result[21] = _mm256_setzero_si256(); result[22] = _mm256_setzero_si256(); result[23] = _mm256_setzero_si256(); result[24] = _mm256_setzero_si256(); result[25] = _mm256_setzero_si256(); result[26] = _mm256_setzero_si256(); result[27] = _mm256_setzero_si256(); result[28] = _mm256_setzero_si256(); result[29] = _mm256_setzero_si256(); result[30] = _mm256_setzero_si256(); result[31] = _mm256_setzero_si256(); 
#define pad_rs_2(old, result) result[0] = old[2]; result[1] = old[3]; result[2] = old[4]; result[3] = old[5]; result[4] = old[6]; result[5] = old[7]; result[6] = old[8]; result[7] = old[9]; result[8] = old[10]; result[9] = old[11]; result[10] = old[12]; result[11] = old[13]; result[12] = old[14]; result[13] = old[15]; result[14] = old[16]; result[15] = old[17]; result[16] = old[18]; result[17] = old[19]; result[18] = old[20]; result[19] = old[21]; result[20] = old[22]; result[21] = old[23]; result[22] = old[24]; result[23] = old[25]; result[24] = old[26]; result[25] = old[27]; result[26] = old[28]; result[27] = old[29]; result[28] = old[30]; result[29] = old[31]; result[30] = _mm256_setzero_si256(); result[31] = _mm256_setzero_si256();
#define pad_rs_13(old, result) result[0] = old[13]; result[1] = old[14]; result[2] = old[15]; result[3] = old[16]; result[4] = old[17]; result[5] = old[18]; result[6] = old[19]; result[7] = old[20]; result[8] = old[21]; result[9] = old[22]; result[10] = old[23]; result[11] = old[24]; result[12] = old[25]; result[13] = old[26]; result[14] = old[27]; result[15] = old[28]; result[16] = old[29]; result[17] = old[30]; result[18] = old[31]; result[19] = _mm256_setzero_si256(); result[20] = _mm256_setzero_si256(); result[21] = _mm256_setzero_si256(); result[22] = _mm256_setzero_si256(); result[23] = _mm256_setzero_si256(); result[24] = _mm256_setzero_si256(); result[25] = _mm256_setzero_si256(); result[26] = _mm256_setzero_si256(); result[27] = _mm256_setzero_si256(); result[28] = _mm256_setzero_si256(); result[29] = _mm256_setzero_si256(); result[30] = _mm256_setzero_si256(); result[31] = _mm256_setzero_si256(); 
#define pad_rs_6(old, result) result[0] = old[6]; result[1] = old[7]; result[2] = old[8]; result[3] = old[9]; result[4] = old[10]; result[5] = old[11]; result[6] = old[12]; result[7] = old[13]; result[8] = old[14]; result[9] = old[15]; result[10] = old[16]; result[11] = old[17]; result[12] = old[18]; result[13] = old[19]; result[14] = old[20]; result[15] = old[21]; result[16] = old[22]; result[17] = old[23]; result[18] = old[24]; result[19] = old[25]; result[20] = old[26]; result[21] = old[27]; result[22] = old[28]; result[23] = old[29]; result[24] = old[30]; result[25] = old[31]; result[26] = _mm256_setzero_si256(); result[27] = _mm256_setzero_si256(); result[28] = _mm256_setzero_si256(); result[29] = _mm256_setzero_si256(); result[30] = _mm256_setzero_si256(); result[31] = _mm256_setzero_si256();
#define pad_rs_11(old, result) result[0] = old[11]; result[1] = old[12]; result[2] = old[13]; result[3] = old[14]; result[4] = old[15]; result[5] = old[16]; result[6] = old[17]; result[7] = old[18]; result[8] = old[19]; result[9] = old[20]; result[10] = old[21]; result[11] = old[22]; result[12] = old[23]; result[13] = old[24]; result[14] = old[25]; result[15] = old[26]; result[16] = old[27]; result[17] = old[28]; result[18] = old[29]; result[19] = old[30]; result[20] = old[31]; result[21] = _mm256_setzero_si256(); result[22] = _mm256_setzero_si256(); result[23] = _mm256_setzero_si256(); result[24] = _mm256_setzero_si256(); result[25] = _mm256_setzero_si256(); result[26] = _mm256_setzero_si256(); result[27] = _mm256_setzero_si256(); result[28] = _mm256_setzero_si256(); result[29] = _mm256_setzero_si256(); result[30] = _mm256_setzero_si256(); result[31] = _mm256_setzero_si256(); 
#define pad_rs_10(old, result) result[0] = old[10]; result[1] = old[11]; result[2] = old[12]; result[3] = old[13]; result[4] = old[14]; result[5] = old[15]; result[6] = old[16]; result[7] = old[17]; result[8] = old[18]; result[9] = old[19]; result[10] = old[20]; result[11] = old[21]; result[12] = old[22]; result[13] = old[23]; result[14] = old[24]; result[15] = old[25]; result[16] = old[26]; result[17] = old[27]; result[18] = old[28]; result[19] = old[29]; result[20] = old[30]; result[21] = old[31]; result[22] = _mm256_setzero_si256(); result[23] = _mm256_setzero_si256(); result[24] = _mm256_setzero_si256(); result[25] = _mm256_setzero_si256(); result[26] = _mm256_setzero_si256(); result[27] = _mm256_setzero_si256(); result[28] = _mm256_setzero_si256(); result[29] = _mm256_setzero_si256(); result[30] = _mm256_setzero_si256(); result[31] = _mm256_setzero_si256(); 
#define pad_rs_22(old, result) result[0] = old[22]; result[1] = old[23]; result[2] = old[24]; result[3] = old[25]; result[4] = old[26]; result[5] = old[27]; result[6] = old[28]; result[7] = old[29]; result[8] = old[30]; result[9] = old[31]; result[10] = _mm256_setzero_si256(); result[11] = _mm256_setzero_si256(); result[12] = _mm256_setzero_si256(); result[13] = _mm256_setzero_si256(); result[14] = _mm256_setzero_si256(); result[15] = _mm256_setzero_si256(); result[16] = _mm256_setzero_si256(); result[17] = _mm256_setzero_si256(); result[18] = _mm256_setzero_si256(); result[19] = _mm256_setzero_si256(); result[20] = _mm256_setzero_si256(); result[21] = _mm256_setzero_si256(); result[22] = _mm256_setzero_si256(); result[23] = _mm256_setzero_si256(); result[24] = _mm256_setzero_si256(); result[25] = _mm256_setzero_si256(); result[26] = _mm256_setzero_si256(); result[27] = _mm256_setzero_si256(); result[28] = _mm256_setzero_si256(); result[29] = _mm256_setzero_si256(); result[30] = _mm256_setzero_si256(); result[31] = _mm256_setzero_si256();
#define pad_rs_25(old, result) result[0] = old[25]; result[1] = old[26]; result[2] = old[27]; result[3] = old[28]; result[4] = old[29]; result[5] = old[30]; result[6] = old[31]; result[7] = _mm256_setzero_si256(); result[8] = _mm256_setzero_si256(); result[9] = _mm256_setzero_si256(); result[10] = _mm256_setzero_si256(); result[11] = _mm256_setzero_si256(); result[12] = _mm256_setzero_si256(); result[13] = _mm256_setzero_si256(); result[14] = _mm256_setzero_si256(); result[15] = _mm256_setzero_si256(); result[16] = _mm256_setzero_si256(); result[17] = _mm256_setzero_si256(); result[18] = _mm256_setzero_si256(); result[19] = _mm256_setzero_si256(); result[20] = _mm256_setzero_si256(); result[21] = _mm256_setzero_si256(); result[22] = _mm256_setzero_si256(); result[23] = _mm256_setzero_si256(); result[24] = _mm256_setzero_si256(); result[25] = _mm256_setzero_si256(); result[26] = _mm256_setzero_si256(); result[27] = _mm256_setzero_si256(); result[28] = _mm256_setzero_si256(); result[29] = _mm256_setzero_si256(); result[30] = _mm256_setzero_si256(); result[31] = _mm256_setzero_si256(); 

// these macros shifts the __m256i array to the left by x(which are defined after pad_rs_*x indices and pads the leftover bits to 0
#define pad_ls_7(old, result) result[0] = _mm256_setzero_si256(); result[1] = _mm256_setzero_si256(); result[2] = _mm256_setzero_si256(); result[3] = _mm256_setzero_si256(); result[4] = _mm256_setzero_si256(); result[5] = _mm256_setzero_si256(); result[6] = _mm256_setzero_si256(); result[7] = _mm256_setzero_si256(); result[8] = _mm256_setzero_si256(); result[9] = _mm256_setzero_si256(); result[10] = _mm256_setzero_si256(); result[11] = _mm256_setzero_si256(); result[12] = _mm256_setzero_si256(); result[13] = _mm256_setzero_si256(); result[14] = _mm256_setzero_si256(); result[15] = _mm256_setzero_si256(); result[16] = _mm256_setzero_si256(); result[17] = _mm256_setzero_si256(); result[18] = _mm256_setzero_si256(); result[19] = _mm256_setzero_si256(); result[20] = _mm256_setzero_si256(); result[21] = _mm256_setzero_si256(); result[22] = _mm256_setzero_si256(); result[23] = _mm256_setzero_si256(); result[24] = _mm256_setzero_si256(); result[25] = old[0]; result[26] = old[1]; result[27] = old[2]; result[28] = old[3]; result[29] = old[4]; result[30] = old[5]; result[31] = old[6];
#define pad_ls_18(old, result) result[0]= _mm256_setzero_si256(); result[1] = _mm256_setzero_si256(); result[2] = _mm256_setzero_si256(); result[3] = _mm256_setzero_si256(); result[4] = _mm256_setzero_si256(); result[5] = _mm256_setzero_si256(); result[6] = _mm256_setzero_si256(); result[7] = _mm256_setzero_si256(); result[8] = _mm256_setzero_si256(); result[9] = _mm256_setzero_si256(); result[10] = _mm256_setzero_si256(); result[11] = _mm256_setzero_si256(); result[12] = _mm256_setzero_si256(); result[13] = _mm256_setzero_si256(); result[14] = old[0]; result[15] = old[1]; result[16] = old[2]; result[17] = old[3]; result[18] = old[4]; result[19] = old[5]; result[20] = old[6]; result[21] = old[7]; result[22] = old[8]; result[23] = old[9]; result[24] = old[10]; result[25] = old[11]; result[26] = old[12]; result[27] = old[13]; result[28] = old[14]; result[29] = old[15]; result[30] = old[16]; result[31] = old[17]; 
#define pad_ls_17(old, result) result[0] = _mm256_setzero_si256(); result[1] = _mm256_setzero_si256(); result[2] = _mm256_setzero_si256(); result[3] = _mm256_setzero_si256(); result[4] = _mm256_setzero_si256(); result[5] = _mm256_setzero_si256(); result[6] = _mm256_setzero_si256(); result[7] = _mm256_setzero_si256(); result[8] = _mm256_setzero_si256(); result[9] = _mm256_setzero_si256(); result[10] = _mm256_setzero_si256(); result[11] = _mm256_setzero_si256(); result[12] = _mm256_setzero_si256(); result[13] = _mm256_setzero_si256(); result[14] = _mm256_setzero_si256(); result[15] = old[0]; result[16] = old[1]; result[17] = old[2]; result[18] = old[3]; result[19] = old[4]; result[20] = old[5]; result[21] = old[6]; result[22] = old[7]; result[23] = old[8]; result[24] = old[9]; result[25] = old[10]; result[26] = old[11]; result[27] = old[12]; result[28] = old[13]; result[29] = old[14]; result[30] = old[15]; result[31] = old[16];
#define pad_ls_19(old, result) result[0] = _mm256_setzero_si256(); result[1] = _mm256_setzero_si256(); result[2] = _mm256_setzero_si256(); result[3] = _mm256_setzero_si256(); result[4] = _mm256_setzero_si256(); result[5] = _mm256_setzero_si256(); result[6] = _mm256_setzero_si256(); result[7] = _mm256_setzero_si256(); result[8] = _mm256_setzero_si256(); result[9] = _mm256_setzero_si256(); result[10] = _mm256_setzero_si256(); result[11] = _mm256_setzero_si256(); result[12] = _mm256_setzero_si256(); result[13] = old[0]; result[14] = old[1]; result[15] = old[2]; result[16] = old[3]; result[17] = old[4]; result[18] = old[5]; result[19] = old[6]; result[20] = old[7]; result[21] = old[8]; result[22] = old[9]; result[23] = old[10]; result[24] = old[11]; result[25] = old[12]; result[26] = old[13]; result[27] = old[14]; result[28] = old[15]; result[29] = old[16]; result[30] = old[17]; result[31] = old[18];
#define pad_ls_2(old, result) result[0] = _mm256_setzero_si256(); result[1] = _mm256_setzero_si256(); result[2] = _mm256_setzero_si256(); result[3] = _mm256_setzero_si256(); result[4] = _mm256_setzero_si256(); result[5] = _mm256_setzero_si256(); result[6] = _mm256_setzero_si256(); result[7] = _mm256_setzero_si256(); result[8] = _mm256_setzero_si256(); result[9] = _mm256_setzero_si256(); result[10] = _mm256_setzero_si256(); result[11] = _mm256_setzero_si256(); result[12] = _mm256_setzero_si256(); result[13] = _mm256_setzero_si256(); result[14] = _mm256_setzero_si256(); result[15] = _mm256_setzero_si256(); result[16] = _mm256_setzero_si256(); result[17] = _mm256_setzero_si256(); result[18] = _mm256_setzero_si256(); result[19] = _mm256_setzero_si256(); result[20] = _mm256_setzero_si256(); result[21] = _mm256_setzero_si256(); result[22] = _mm256_setzero_si256(); result[23] = _mm256_setzero_si256(); result[24] = _mm256_setzero_si256(); result[25] = _mm256_setzero_si256(); result[26] = _mm256_setzero_si256(); result[27] = _mm256_setzero_si256(); result[28] = _mm256_setzero_si256(); result[29] = _mm256_setzero_si256(); result[30] = old[0]; result[31] = old[1];
#define pad_ls_13(old, result) result[0] = _mm256_setzero_si256(); result[1] = _mm256_setzero_si256(); result[2] = _mm256_setzero_si256(); result[3] = _mm256_setzero_si256(); result[4] = _mm256_setzero_si256(); result[5] = _mm256_setzero_si256(); result[6] = _mm256_setzero_si256(); result[7] = _mm256_setzero_si256(); result[8] = _mm256_setzero_si256(); result[9] = _mm256_setzero_si256(); result[10] = _mm256_setzero_si256(); result[11] = _mm256_setzero_si256(); result[12] = _mm256_setzero_si256(); result[13] = _mm256_setzero_si256(); result[14] = _mm256_setzero_si256(); result[15] = _mm256_setzero_si256(); result[16] = _mm256_setzero_si256(); result[17] = _mm256_setzero_si256(); result[18] = _mm256_setzero_si256(); result[19] = old[0]; result[20] = old[1]; result[21] = old[2]; result[22] = old[3]; result[23] = old[4]; result[24] = old[5]; result[25] = old[6]; result[26] = old[7]; result[27] = old[8]; result[28] = old[9]; result[29] = old[10]; result[30] = old[11]; result[31] = old[12];
#define pad_ls_6(old, result) result[0] = _mm256_setzero_si256(); result[1] = _mm256_setzero_si256(); result[2] = _mm256_setzero_si256(); result[3] = _mm256_setzero_si256(); result[4] = _mm256_setzero_si256(); result[5] = _mm256_setzero_si256(); result[6] = _mm256_setzero_si256(); result[7] = _mm256_setzero_si256(); result[8] = _mm256_setzero_si256(); result[9] = _mm256_setzero_si256(); result[10] = _mm256_setzero_si256(); result[11] = _mm256_setzero_si256(); result[12] = _mm256_setzero_si256(); result[13] = _mm256_setzero_si256(); result[14] = _mm256_setzero_si256(); result[15] = _mm256_setzero_si256(); result[16] = _mm256_setzero_si256(); result[17] = _mm256_setzero_si256(); result[18] = _mm256_setzero_si256(); result[19] = _mm256_setzero_si256(); result[20] = _mm256_setzero_si256(); result[21] = _mm256_setzero_si256(); result[22] = _mm256_setzero_si256(); result[23] = _mm256_setzero_si256(); result[24] = _mm256_setzero_si256(); result[25] = _mm256_setzero_si256(); result[26] = old[0]; result[27] = old[1]; result[28] = old[2]; result[29] = old[3]; result[30] = old[4]; result[31] = old[5];
#define pad_ls_24(old, result) result[0] = _mm256_setzero_si256(); result[1] = _mm256_setzero_si256(); result[2] = _mm256_setzero_si256(); result[3] = _mm256_setzero_si256(); result[4] = _mm256_setzero_si256(); result[5] = _mm256_setzero_si256(); result[6] = _mm256_setzero_si256(); result[7] = _mm256_setzero_si256(); result[8] = _mm256_setzero_si256(); result[9] = _mm256_setzero_si256(); result[10] = _mm256_setzero_si256(); result[11] = _mm256_setzero_si256(); result[12] = _mm256_setzero_si256(); result[13] = _mm256_setzero_si256(); result[14] = _mm256_setzero_si256(); result[15] = _mm256_setzero_si256(); result[16] = _mm256_setzero_si256(); result[17] = _mm256_setzero_si256(); result[18] = _mm256_setzero_si256(); result[19] = _mm256_setzero_si256(); result[20] = _mm256_setzero_si256(); result[21] = _mm256_setzero_si256(); result[22] = _mm256_setzero_si256(); result[23] = _mm256_setzero_si256(); result[24] = old[0]; result[25] = old[1]; result[26] = old[2]; result[27] = old[3]; result[28] = old[4]; result[29] = old[5]; result[30] = old[6]; result[31] = old[7];
#define pad_ls_8(old, result) result[0] = _mm256_setzero_si256(); result[1] = _mm256_setzero_si256(); result[2] = _mm256_setzero_si256(); result[3] = _mm256_setzero_si256(); result[4] = _mm256_setzero_si256(); result[5] = _mm256_setzero_si256(); result[6] = _mm256_setzero_si256(); result[7] = _mm256_setzero_si256(); result[8] = old[0]; result[9] = old[1]; result[10] = old[2]; result[11] = old[3]; result[12] = old[4]; result[13] = old[5]; result[14] = old[6]; result[15] = old[7]; result[16] = old[8]; result[17] = old[9]; result[18] = old[10]; result[19] = old[11]; result[20] = old[12]; result[21] = old[13]; result[22] = old[14]; result[23] = old[15]; result[24] = old[16]; result[25] = old[17]; result[26] = old[18]; result[27] = old[19]; result[28] = old[20]; result[29] = old[21]; result[30] = old[22]; result[31] = old[23];
#define pad_ls_16(old, result) result[0] = _mm256_setzero_si256(); result[1] = _mm256_setzero_si256(); result[2] = _mm256_setzero_si256(); result[3] = _mm256_setzero_si256(); result[4] = _mm256_setzero_si256(); result[5] = _mm256_setzero_si256(); result[6] = _mm256_setzero_si256(); result[7] = _mm256_setzero_si256(); result[8] = _mm256_setzero_si256(); result[9] = _mm256_setzero_si256(); result[10] = _mm256_setzero_si256(); result[11] = _mm256_setzero_si256(); result[12] = _mm256_setzero_si256(); result[13] = _mm256_setzero_si256(); result[14] = _mm256_setzero_si256(); result[15] = _mm256_setzero_si256(); result[16] = old[0]; result[17] = old[1]; result[18] = old[2]; result[19] = old[3]; result[20] = old[4]; result[21] = old[5]; result[22] = old[6]; result[23] = old[7]; result[24] = old[8]; result[25] = old[9]; result[26] = old[10]; result[27] = old[11]; result[28] = old[12]; result[29] = old[13]; result[30] = old[14]; result[31] = old[15];
#define pad_ls_11(old, result) result[0] = _mm256_setzero_si256(); result[1] = _mm256_setzero_si256(); result[2] = _mm256_setzero_si256(); result[3] = _mm256_setzero_si256(); result[4] = _mm256_setzero_si256(); result[5] = _mm256_setzero_si256(); result[6] = _mm256_setzero_si256(); result[7] = _mm256_setzero_si256(); result[8] = _mm256_setzero_si256(); result[9] = _mm256_setzero_si256(); result[10] = _mm256_setzero_si256(); result[11] = _mm256_setzero_si256(); result[12] = _mm256_setzero_si256(); result[13] = _mm256_setzero_si256(); result[14] = _mm256_setzero_si256(); result[15] = _mm256_setzero_si256(); result[16] = _mm256_setzero_si256(); result[17] = _mm256_setzero_si256(); result[18] = _mm256_setzero_si256(); result[19] = _mm256_setzero_si256(); result[20] = _mm256_setzero_si256(); result[21] = old[0]; result[22] = old[1]; result[23] = old[2]; result[24] = old[3]; result[25] = old[4]; result[26] = old[5]; result[27] = old[6]; result[28] = old[7]; result[29] = old[8]; result[30] = old[9]; result[31] = old[10];
#define pad_ls_10(old, result) result[0] = _mm256_setzero_si256(); result[1] = _mm256_setzero_si256(); result[2] = _mm256_setzero_si256(); result[3] = _mm256_setzero_si256(); result[4] = _mm256_setzero_si256(); result[5] = _mm256_setzero_si256(); result[6] = _mm256_setzero_si256(); result[7] = _mm256_setzero_si256(); result[8] = _mm256_setzero_si256(); result[9] = _mm256_setzero_si256(); result[10] = _mm256_setzero_si256(); result[11] = _mm256_setzero_si256(); result[12] = _mm256_setzero_si256(); result[13] = _mm256_setzero_si256(); result[14] = _mm256_setzero_si256(); result[15] = _mm256_setzero_si256(); result[16] = _mm256_setzero_si256(); result[17] = _mm256_setzero_si256(); result[18] = _mm256_setzero_si256(); result[19] = _mm256_setzero_si256(); result[20] = _mm256_setzero_si256(); result[21] = _mm256_setzero_si256(); result[22] = old[0]; result[23] = old[1]; result[24] = old[2]; result[25] = old[3]; result[26] = old[4]; result[27] = old[5]; result[28] = old[6]; result[29] = old[7]; result[30] = old[8]; result[31] = old[9];
#define pad_ls_22(old, result) result[0] = _mm256_setzero_si256(); result[1] = _mm256_setzero_si256(); result[2] = _mm256_setzero_si256(); result[3] = _mm256_setzero_si256(); result[4] = _mm256_setzero_si256(); result[5] = _mm256_setzero_si256(); result[6] = _mm256_setzero_si256(); result[7] = _mm256_setzero_si256(); result[8] = _mm256_setzero_si256(); result[9] = _mm256_setzero_si256(); result[10] = old[0]; result[11] = old[1]; result[12] = old[2]; result[13] = old[3]; result[14] = old[4]; result[15] = old[5]; result[16] = old[6]; result[17] = old[7]; result[18] = old[8]; result[19] = old[9]; result[20] = old[10]; result[21] = old[11]; result[22] = old[12]; result[23] = old[13]; result[24] = old[14]; result[25] = old[15]; result[26] = old[16]; result[27] = old[17]; result[28] = old[18]; result[29] = old[19]; result[30] = old[20]; result[31] = old[21];
#define pad_ls_25(old, result) result[0] = _mm256_setzero_si256(); result[1] = _mm256_setzero_si256(); result[2] = _mm256_setzero_si256(); result[3] = _mm256_setzero_si256(); result[4] = _mm256_setzero_si256(); result[5] = _mm256_setzero_si256(); result[6] = _mm256_setzero_si256(); result[7] = old[0]; result[8] = old[1]; result[9] = old[2]; result[10] = old[3]; result[11] = old[4]; result[12] = old[5]; result[13] = old[6]; result[14] = old[7]; result[15] = old[8]; result[16] = old[9]; result[17] = old[10]; result[18] = old[11]; result[19] = old[12]; result[20] = old[13]; result[21] = old[14]; result[22] = old[15]; result[23] = old[16]; result[24] = old[17]; result[25] = old[18]; result[26] = old[19]; result[27] = old[20]; result[28] = old[21]; result[29] = old[22]; result[30] = old[23]; result[31] = old[24];


// this represents -> maj(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
// where x = a, y = b, z = c
#define MAJ256()\
    AND256(a, b, temp1);\
    AND256(a, c, temp2);\
    AND256(b, c, temp3);\
    XOR256(temp1, temp2, second);\
    XOR256(temp3, second, second);

// this represents -> (rotate_right(x, 7) ^ rotate_right(x, 18) ^ rightshift(x,3))
// where x = W[i - 15]
#define SIGMA0()\
    pad_rs_7(W[i - 15], temp1);\
    pad_ls_7(W[i - 15], temp3);\
    OR256(temp1, temp3, temp3);\
    pad_rs_18(W[i - 15], temp1);\
    pad_ls_18(W[i - 15], temp2);\
    OR256(temp1, temp2, temp2);\
    XOR256(temp3, temp2, second);\
    pad_rs_3(W[i - 15], temp1);\
    XOR256(temp1, second, second);

// this represents -> (rotate_right(x, 17) ^ rotate_right(x, 19) ^ rightshift(x,10))
// where x = hashed_w[1 - 2]
#define SIGMA1()\
    pad_rs_17(W[i - 2], temp1);\
    pad_ls_17(W[i - 2], temp3);\
    OR256(temp1, temp3, temp3);\
    pad_rs_19(W[i - 2], temp1);\
    pad_ls_19(W[i - 2], temp2);\
    OR256(temp1, temp2, temp2);\
    XOR256(temp3, temp2, first);\
    pad_rs_10(W[i - 2], temp1);\
    XOR256(temp1, first, first);

//this represents -> (rotate_right(x, 2) ^ rotate_right(x, 13) ^ rotate_right(x, 22))
// where x = a
#define SIGMA2()\
    pad_rs_2(a, temp1);\
    pad_ls_2(a, temp3);\
    OR256(temp1, temp3, temp3);\
    pad_rs_13(a, temp1);\
    pad_ls_13(a, temp2);\
    OR256(temp1, temp2, temp2);\
    XOR256(temp3, temp2, first);\
    pad_rs_22(a, temp1);\
    pad_ls_22(a, temp2);\
    OR256(temp1, temp2, temp1);\
    XOR256(temp1, first, first);

// this represents -> (rotate_right(x, 6) ^ rotate_right(x, 11) ^ rotate_right(x, 25))
// where x = e
#define SIGMA3()\
    pad_rs_6(e, temp1);\
    pad_ls_6(e, temp3);\
    OR256(temp1, temp3, temp3);\
    pad_rs_11(e, temp1);\
    pad_ls_11(e, temp2);\
    OR256(temp1, temp2, temp2);\
    XOR256(temp3, temp2, first);\
    pad_rs_25(e, temp1);\
    pad_ls_25(e, temp2);\
    OR256(temp1, temp2, temp1);\
    XOR256(temp1, first, first);

// this represents -> ((x & y) ^ (~x & z)) 
// where e = x, f = y, g = z
#define CH256()\
    AND256(e, f, temp1);\
    ANDNOT256(e, g, third);\
    XOR256(temp1, third, third);

// /**
//  * For each character in the array of strings order them and prepare them to bitslice
//  */
void get_value_at_index(string words[], uint32_t result[], uint32_t index, uint32_t offset){
    for(uint32_t i = 0; i < 32; i++){
        result[i] = words[i + offset][index];
    }
}

// /**
//  * Transpose 8 arrays of normal representation to one single const of 32 * 32 bits 
//  *
//  * @param values 8 unsigned 32 bit integer arrays of 32 indices and a __m256i 256 bit integer array of 32 indices
//  * @return returns the bitsliced result representation
//  */
void bsconst8x(const uint32_t a[], const uint32_t b[], const uint32_t c[], const uint32_t d[], const uint32_t e[], const uint32_t f[], const uint32_t g[], const uint32_t h[], __m256i result[]){
    for(uint32_t i = 0; i < 32; i++){
        result[i] = _mm256_setr_epi32(a[i], b[i], c[i], d[i], e[i], f[i], g[i], h[i]);
    }
}

// /**
//  * Transpose normal representation to one single const of 32 * 32 bits
//  *
//  * @param values 1 unsigned 32 bit integer and a __m256i 256 bit integer array of 32 indices
//  * @return returns the bitsliced result representation
//  */
void bsconst(const uint32_t value, __m256i result[]){
    for(uint32_t i = 0; i < 32; i++){
        result[i] = (value & (1 << i)) > 0 ? _mm256_set1_epi32(0xffffffff) : _mm256_setzero_si256();
    }
}

// /**
//  * Transpose normal representation to bitslice
//  *
//  * @param values 2 unsigned 32 bit integer arrays of 32 indices
//  * @return returns the bitsliced result representation
//  */
void transposeblock2bitslice(uint32_t A[32], uint32_t result[32]) {
    uint32_t temp = 0, temp2 = 0;
    for (uint32_t i = 0; i < 32; i++) {
        for (uint32_t j = 0; j < 32; j++) {
            temp = GET_BIT(A[i], j);
            temp2 = GET_BIT(A[j], i);
            SET_BIT(result[j], i, (temp >> j) << i);
            SET_BIT(result[i], j, (temp2 >> i) << j);
        }
    }
}

void print_m256i(__m256i input, bool hex){
    uint32_t *casted_input = (uint32_t*)&input;
    for(uint32_t i = 0; i < 8; i++){
        if(hex)
            cout << hex << casted_input[i] << " ";
        else
            cout << bitset<32>(casted_input[i]) << endl;
    }
    cout << endl;
};

// bitsliced add of 2 __m256i
static inline void bitsliced_add(__m256i A[], __m256i B[], __m256i *result){
    __m256i carry = {0}, x = {0}, y = {0};
    for(uint32_t i = 0; i < 32; ++i){
        x = A[i];
        y = B[i];
        result[i] = _mm256_xor_si256(_mm256_xor_si256(x, y), carry);
        carry = _mm256_or_si256(_mm256_and_si256(_mm256_xor_si256(x, y), carry), _mm256_and_si256(x, y));
    }
}

//bitsliced add of 2 __m256i
// inline void bitsliced_add(__m256i A[], __m256i B[], __m256i result[]){
//     __m256i carry = {0}, x = {0}, y = {0};
//     for(uint32_t i = 0; i < 32; i++){
//         x = A[i];
//         y = B[i];
//         result[i] = (x ^ y) ^ carry;
//         carry = ((x ^ y) & carry) | (x & y);
//     }
// }

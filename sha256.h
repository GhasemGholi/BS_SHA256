// #include <bits/stdc++.h>
#include <iostream>
#include <array>

// SHA256 implementation based on the wiki page https://en.wikipedia.org/wiki/SHA-2. 
// Functions and operations from https://csrc.nist.gov/csrc/media/publications/fips/180/4/final/documents/fips180-4-draft-aug2014.pdf

using namespace std;


#define leftshift(x,y) x << y
#define rightshift(x,y) x >> y
#define rotate_right(x,y) (rightshift(x,y) | leftshift(x,32 - y))
#define sigma0(x) (rotate_right(x, 7) ^ rotate_right(x, 18) ^ rightshift(x,3))
#define sigma1(x) (rotate_right(x, 17) ^ rotate_right(x, 19) ^ rightshift(x,10))
#define sigma2(x) (rotate_right(x, 2) ^ rotate_right(x, 13) ^ rotate_right(x, 22))
#define sigma3(x) (rotate_right(x, 6) ^ rotate_right(x, 11) ^ rotate_right(x, 25))
#define ch(x, y, z) ((x & y) ^ (~x & z))
#define maj(x, y, z) ((x & y) ^ (x & z) ^ (y & z))

inline int bstrlen(const uint8_t *str){
    int counter = 0;
    while(*str){
        *str++;
        counter++;
    }
    return counter;
}

class sha256{
    public:
        sha256();
        void parse_text(string text, int len);
        void pre_processing(uint32_t* text); //padding
        void processing(); // process into successive 512-bit chunks
        inline const uint8_t *bitify(const string &text);
        void update_buffer(uint8_t *buf, uint32_t len);
    private:
        uint32_t L;
        uint64_t blocks;
        uint32_t W[64]; //message schedule
        uint32_t H[8]; // working variables
};
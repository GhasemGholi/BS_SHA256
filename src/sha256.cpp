#include "sha256.h"
#include <chrono>
#include <fstream>
#include <cstring>
#include <bitset>
#include <algorithm>
#include <vector>
#include <iterator>
#include <numeric>
#include <openssl/ssl.h>
#include <openssl/sha.h>
#include <fstream>
#include <filesystem>
#include <iterator>

long long firstinfo = 0;
long long secondinfo = 0;

void sha256::pre_processing(uint32_t* text){
    uint32_t end = this->L > 56 ? 56 : 64, len = this->L;

    this->W[len++] = 0x80;
    while(len < end){
        this->W[len++] = 0x00;
    }
    
    if(this->L >= 56) {
        processing();
        memset(this->W, 0, sizeof(this->W));
    }
	this->blocks += this->L * 8;

    for(int i = 63, j = 0; i > 55; j += 8, i--){
        this->W[i] = this->blocks >> j;
    }
    

	processing();

    for (uint8_t i = 0 ; i < 4 ; i++) 
		for(uint8_t j = 0 ; j < 8 ; j++){
			text[i + (j * 4)] = this->H[j] >> (24 - i * 8) & 0xff;
        }
		
}


sha256::sha256(){
    this->blocks = 0;
    this->L = 0;
    this->H[0] = 0x6a09e667; // a
    this->H[1] = 0xbb67ae85; // b
    this->H[2] = 0x3c6ef372; // c
    this->H[3] = 0xa54ff53a; // d
    this->H[4] = 0x510e527f; // e
    this->H[5] = 0x9b05688c; // f
    this->H[6] = 0x1f83d9ab; // g
    this->H[7] = 0x5be0cd19; // h
}   

using namespace std::chrono;
double cummulative_time = 0;

void sha256::processing(){
    auto start = high_resolution_clock::now();
    firstinfo += cpucycles_x86cpuinfo();
    uint32_t a = this->H[0], 
          b = this->H[1], 
          c = this->H[2], 
          d = this->H[3], 
          e = this->H[4], 
          f = this->H[5], 
          g = this->H[6], 
          h = this->H[7];

    uint32_t temp1, temp2;
    array<uint32_t, 64> hashed_W = {0};
    

    for(uint32_t i = 0, j = 0; i < 16; i++, j += 4){
	    hashed_W[i] = leftshift(this->W[j], 24) | leftshift(this->W[j + 1], 16) | leftshift(this->W[j + 2], 8) | (this->W[j + 3]);
    }

    for(uint32_t i = 16; i < 64; i++){ 
		hashed_W[i] = sigma1(hashed_W[i - 2]) + hashed_W[i - 7] + sigma0(hashed_W[i - 15]) + hashed_W[i - 16];
    }

    for (uint32_t i = 0; i < 64; i++) {
        temp1 = h + sigma3(e) + ch(e, f, g) + K[i] + hashed_W[i];
        temp2 = sigma2(a) + maj(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + temp1;
        d = c;
        c = b;
        b = a;
        a = temp1 + temp2;
    }

    this->H[0] += a;
    this->H[1] += b; 
    this->H[2] += c;
    this->H[3] += d;
    this->H[4] += e;
    this->H[5] += f;
    this->H[6] += g;
    this->H[7] += h;

    secondinfo += cpucycles_x86cpuinfo(); 
    
	auto end = high_resolution_clock::now();
    auto dur = duration_cast<nanoseconds>(end - start);
    cummulative_time += dur.count();
}

void sha256::parse_text(string text, int len){
    // string text = "Hello";
    for(int i = 0; i < len; i++){
        this->W[this->L++] = text[i];
        if(this->L == 64){
            processing();
            this->blocks += 512;
            this->L = 0;
        } 
    }
}
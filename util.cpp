#include <fstream>
#include <filesystem>
#include <iterator>
#include <vector>
#include <numeric>
#include <math.h>
#include <cmath>
#include <chrono>
#include <string>
#include <bitset>
#include <cstring>
#include <iomanip>
#include <fmtmsg.h>
#include "optimized_bssha256.hpp"
#include "cpucycles.h"
#include "x86cpuinfo.c"

using namespace std;

[[maybe_unused]]
void appendLineToFile(string filepath, string line){
    std::ofstream file;
    file.open(filepath, std::ios::out | std::ios::app);
    if (file.fail())
        throw std::ios_base::failure(std::strerror(errno));

    file.exceptions(file.exceptions() | std::ios::failbit | std::ifstream::badbit);

    file << line << std::endl;
}

string read_file(const char *fileinput){
    ifstream ifs(fileinput, ios::in | ios::binary | ios::ate);

    size_t fileSize = ifs.tellg();
    ifs.seekg(0, ios::beg);
    vector<char> bytes(fileSize);
    ifs.read(bytes.data(), fileSize);

    return string(bytes.data(), fileSize);
}

size_t len;
static void split_into_256_chunks(string splitted[], string file){
    len = file.size();
    uint32_t restriction = len / 256;

    int counter = 0;
    for(uint32_t i = 0; i < len; i += restriction){
        splitted[counter].append(file.substr(i, restriction));
        counter++;
    }
}

template <typename T> int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

string BytesToString(long long size){
    string suffix[7] = {"B", "KB", "MB", "GB", "TB", "PB", "EB" }; 
    if (size == 0)
        return "0" + suffix[0];
    size_t bytes = abs(size);
    double exponent = log(bytes) / log(1024);
    uint32_t place = floor(exponent);
    double num = nearbyint(bytes / pow(1024, place));
    string s(to_string(static_cast<int>(sign(size) * num)));
    s.append(suffix[place]);
    return s;
}

void print_hashes(__m256i hashed[32][32]){
    uint32_t result[32][8][32] = {0};
    uint32_t tempval[32][8][32] = {0};
    for(uint32_t i = 0; i < 32; i++){
        for(uint32_t j = 0; j < 32; j++){
            uint32_t *temp = (uint32_t*)&hashed[i][j];
            result[i][0][j] = temp[0];
            result[i][1][j] = temp[1];
            result[i][2][j] = temp[2];
            result[i][3][j] = temp[3];
            result[i][4][j] = temp[4];
            result[i][5][j] = temp[5];
            result[i][6][j] = temp[6];
            result[i][7][j] = temp[7];
        }
    }

    for(uint32_t i = 0; i < 32; i++){
        for(uint32_t j = 0; j < 8; j++){
            transposeblock2bitslice(result[i][j], tempval[i][j]);
        }
    }
    int counter = 0;
    for(uint32_t i = 0; i < 8; i++){
        for(uint32_t k = 0; k < 32; k++){
            cout << dec << "Pos " << counter << " ";
            for(uint32_t j = 0; j < 32; j++){
                cout << setfill('0') << setw(2) << hex << tempval[j][i][k];
            }
            cout << endl;
            counter++;
        }
        cout << endl;
    }
}

void n_amount_charstofile(long long size){
    std::ofstream file;
    file.open("output.txt", std::ios::out | std::ios::trunc);
    string s = "";
    for(long long i = 0; i < size; i++){
        s.push_back('a' + rand()%26);
    }
    file << s << std::endl;
}

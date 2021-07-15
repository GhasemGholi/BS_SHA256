// this file is called in a tree level, so no need to include other files

void init_sha256();
void parse_text_bssha(const string* listofinput, uint32_t len);
void pre_processing_bs(__m256i text[32][32]); //padding
void processing(); // process into successive 512-bit chunks
void appendLineToFile(string filepath, string line);

uint32_t L;
uint64_t blocks;
__m256i W[64][32]; //message schedule bitsliced
uint32_t H[8]; // working variables
__m256i bs_H[8][32]; // working variables bitsliced
__m256i bs_K[64][32]; // working variables bitsliced

// all the variables globally processed.
__m256i a[32], b[32], c[32], d[32], e[32], f[32], g[32], h[32];
__m256i first[32] = {0}, second[32] = {0}, third[32] = {0}, last[32] = {0}, sum1 = {0}, sum2 = {0}, carry = {0};
__m256i t1[32] = {0}, t2[32] = {0}, temp1[32] = {0}, temp2[32] = {0}, temp3[32] = {0},temp4[32] = {0}, x[32] = {0}, y[32] = {0}, z[32] = {0};
__m256i const1[32] = {0}, const2[32], X, Y;

const __m256i always_zero[32] = {0};

long long firstinfo_bs = 0;
long long secondinfo_bs = 0;

// /**
//  * Memset all variables to 0 each iteration of processing, otherwise alot of garbage values will be put into the hash
//  */

void memset_all(){
    memset_256bit(a, _mm256_setzero_si256());
    memset_256bit(b, _mm256_setzero_si256());
    memset_256bit(c, _mm256_setzero_si256());
    memset_256bit(d, _mm256_setzero_si256());
    memset_256bit(e, _mm256_setzero_si256());
    memset_256bit(f, _mm256_setzero_si256());
    memset_256bit(g, _mm256_setzero_si256());
    memset_256bit(h, _mm256_setzero_si256());
    memset_256bit(first, _mm256_setzero_si256());
    memset_256bit(second, _mm256_setzero_si256());
    memset_256bit(third, _mm256_setzero_si256());
    memset_256bit(last, _mm256_setzero_si256());
    memset_256bit_single(&sum1, _mm256_setzero_si256());
    memset_256bit_single(&sum2, _mm256_setzero_si256());
    memset_256bit_single(&carry, _mm256_setzero_si256());
    memset_256bit(t1, _mm256_setzero_si256());
    memset_256bit(t2, _mm256_setzero_si256());
    memset_256bit(temp1, _mm256_setzero_si256());
    memset_256bit(temp2, _mm256_setzero_si256());
    memset_256bit(temp3, _mm256_setzero_si256());
    memset_256bit(x, _mm256_setzero_si256());
    memset_256bit(y,_mm256_setzero_si256());
    memset_256bit(z, _mm256_setzero_si256());
}


// /**
//  * Pre processing and padding the message schedule
//  */

void pre_processing_bs(__m256i text[32][32]){
    uint32_t len = L < 56 ? 56 : 64;
    if(len < 64)
        for(uint32_t i = L, j = 0; i < len; i++, j++){
            bsconst(padding[j], W[i]);
        }
    
    if(L >= 56) {
        processing();
        memset(W, 0, 64 * sizeof(__m256i));
    }
    
	blocks += L * 8;

    do_bsconst32(blocks, W[63]);
    do_bsconst32(blocks >> 8, W[62]);
    do_bsconst32(blocks >> 16, W[61]);
    do_bsconst32(blocks >> 24, W[60]);
    do_bsconst32(blocks >> 32, W[59]);
    do_bsconst32(blocks >> 40, W[58]);
    do_bsconst32(blocks >> 48, W[57]);
    do_bsconst32(blocks >> 56, W[56]);
    
	processing();

    do_bsconst32(0x000000ff, const2);

    for (uint32_t i = 0 ; i < 4 ; i++){
		for(uint32_t j = 0 ; j < 8 ; j++){
            paddify_right_shift_temp(bs_H[j], const1,(24 - i * 8));
            AND256(const1, const2, const1);
            memcpy_256bit(text[i + (j * 4)], const1);
        }
    }   
}

// /**
//  * Initializes the SHA256 encryption
//  */ 

void init_sha256(){
    blocks = 0;
    L = 0;

    H[0] = 0x6a09e667; // a
    H[1] = 0xbb67ae85; // b
    H[2] = 0x3c6ef372; // c
    H[3] = 0xa54ff53a; // d
    H[4] = 0x510e527f; // e
    H[5] = 0x9b05688c; // f
    H[6] = 0x1f83d9ab; // g
    H[7] = 0x5be0cd19; // h

    for(uint32_t i = 0; i < 8; i++){
        bsconst(H[i], bs_H[i]);
    }   

    for(uint32_t i = 0; i < 64; i++){
        bsconst(K[i], bs_K[i]);
    }
}

using namespace std::chrono;

double cummulative_time_bs = 0.0;


// /**
//  * Main compression function that processes the SHA256 encryption
//  *
//  * @return Processes the Message schedule and returns the hash into the 8 working variables
//  */ 
void processing(){
    memset_all();

    auto start_bs = high_resolution_clock::now();
    firstinfo_bs += cpucycles_x86cpuinfo();

    // copy the bitsliced H into the 8 working variables
    std::copy(bs_H[0], bs_H[0] + 32, a);
    std::copy(bs_H[1], bs_H[1] + 32, b);
    std::copy(bs_H[2], bs_H[2] + 32, c);
    std::copy(bs_H[3], bs_H[3] + 32, d);
    std::copy(bs_H[4], bs_H[4] + 32, e);
    std::copy(bs_H[5], bs_H[5] + 32, f);
    std::copy(bs_H[6], bs_H[6] + 32, g);
    std::copy(bs_H[7], bs_H[7] + 32, h);

    // prepare the message schedule for the first 16 bits
    for(uint32_t i = 0, j = 0; i < 16; ++i, j += 4){   
        memcpy_256bit_nbits(first, W[j], 24);
        memcpy_256bit_nbits(second, W[j + 1], 16);
        memcpy_256bit_nbits(third, W[j + 2], 8);

        OR256(first, second, W[i]);
        OR256(W[i], third, W[i]);
        OR256(W[i], W[j + 3], W[i]);
    }

    // extend the first 16 words into the rest of the message schedule
    for(uint32_t i = 16; i < 64; ++i) {
        SIGMA1();
        SIGMA0();
        bitsliced_add(first, W[i - 7], third);
        bitsliced_add(second, third, third);
        bitsliced_add(third, W[i - 16], W[i]);
    }

    // main compression function
    for (uint32_t i = 0; i < 64; ++i) {
        SIGMA3();
        bitsliced_add(first, h, second);
        CH256();
        bitsliced_add(second, third, third);
        bitsliced_add(third, bs_K[i], third);
        bitsliced_add(third, W[i], t1);
        SIGMA2();
        MAJ256();
        bitsliced_add(first, second, t2);
        memcpy_256bit(h, g);
        memcpy_256bit(g, f);
        memcpy_256bit(f, e);

        bitsliced_add(d, t1, e);    
        
        memcpy_256bit(d, c);
        memcpy_256bit(c, b);
        memcpy_256bit(b, a);
        bitsliced_add(t1, t2, a);
    }
    
    // add the 8 working variables to the bitsliced H and store them in the bitsliced H array
    bitsliced_add(bs_H[0], a, bs_H[0]);
    bitsliced_add(bs_H[1], b, bs_H[1]); 
    bitsliced_add(bs_H[2], c, bs_H[2]);
    bitsliced_add(bs_H[3], d, bs_H[3]);
    bitsliced_add(bs_H[4], e, bs_H[4]);
    bitsliced_add(bs_H[5], f, bs_H[5]);
    bitsliced_add(bs_H[6], g, bs_H[6]);
    bitsliced_add(bs_H[7], h, bs_H[7]); 
    
    secondinfo_bs += cpucycles_x86cpuinfo();

    auto end_bs = high_resolution_clock::now();
    auto dur_bs = duration_cast<nanoseconds>(end_bs - start_bs);
    cummulative_time_bs += dur_bs.count();
}

// /**
//  * Transposes the input into a bitsliced representation 256 times in parallel
//  *
//  * @param Array of strings that represent the input splitted into 256 parts before passing in this function, and the length of each chunk
//  * @handles the parsing and processes it if len == 64
//  */
void parse_text_bssha(string *listofinput, uint32_t len){
    // 5 char input
    // string listofinput[256] = {"stare","avoid","exist","enter","lower","smile","share","creep","break","plead","apply","leave","guess","trace","spell","incur","stand","upset","price","mount","start","strip","shine","worry","quote","drift","value","speak","teach","smash","flash","crack","begin","drive","dream","sweep","phone","treat","train","claim","crash","award","widen","prove","arise","state","delay","admit","twist","weigh", "bring","float","shout","sleep","hurry","sweep","fight","share","adapt","break","shift","prove","clean","score","spoil","going","plead","focus","cover","house","doubt","state","bound","smile","sense","learn","laugh","upset","worry","throw","limit","seize","lower","crush","block","drown","avoid","stick","enter","alert","check","write","argue","boast","drive","claim","admit","grant","blame","found", "equip","crush","swing","solve","frown","close","marry","judge","seize","react","dance","refer","adapt","shrug","crawl","value","apply","admit","flood","laugh","flash","waste","amend","house","stage","imply","smash","climb","place","burst","blame","arise","chase","phone","store","cease","plant","light","reply","price","steal","march","spell","stare","label","bring","found","carve","teach","voice", "award","creep","agree","drain","trust","spill","prove","shoot","crush","split","waste","greet","train","offer","twist","merge","spare","shake","shift","elect","price","unite","start","fancy","light","renew","equip","share","fight","lower","refer","round","think","press","dream","brush","knock","arise","imply","sweep","crack","grasp","admit","strip","quote","phone","raise","block","shrug","swear", "award","creep","agree","drain","trust","spill","prove","shoot","crush","split","waste","greet","train","offer","twist","merge","spare","shake","shift","elect","price","unite","start","fancy","light","renew","equip","share","fight","lower","refer","round","think","press","dream","brush","knock","arise","imply","sweep","crack","grasp","admit","strip","quote","phone","raise","block","shrug","swear", "Hello","Hello","Hello","Hello","Hello", "Hello"};   
    // 8 char input
    // string listofinput[256] = {"6cfHsIC9","h7aSKPfd","ZqaOzwFF","H1EESpMz","kU2afSlk","Zqtgbsbp","lBFxjCXS","FOETJZIa","fUjEBpNV","FLlbRYky","7ohc7oOp","759sAI0n","tph3OB09","nULOpE5H","R2CO6lnH","9AWPX9Cz","efNEQkgw","QLfwIg1t","6rQG1VPv","ROFb2Iiv","DVVMxE0r","m97dqaa7","5rDvjKKM","X273GqL4","sUd72jJ8","HTNX21ST","hiFetsvy","aPPIAwpW","EDvWdxhs","DIqnvQ2J","vYJtbGU7","mabqHEeL","pvT7vkLL","KKSeEpVq","R7bPJb1b","7R8XFnlJ","5GeLcfgI","i5NWIjF6","6RNq1Mhf","YONDaSy2","6Y4Rk76C","juGaOLeN","54PNmhtL","ouTz3hlb","TwBm4Adv","JbYicc5b","JeDQKloS","vOmcJjjz","MO8VJuRt","HnPfbZne","a9BEBt9d","o4oYzbfT","Sdc4hk79","lbSv971L","vLQZnCgl","vWjvKhBk","zGXbjAd9","AtfrE2db","0oIR3BjS","OlhgiFro","mBxsqsnu","h6Askf1v","VGBvtcU3","WyICDZ6C","yhSL7REN","UVS6yQe0","e4pdi6Tw","k1icyDnw","TY1UEfEl","a4mmRkAA","9p7mplcg","HRvIcGA5","y9zTeL6K","HKfWOuBb","4AMjYBpT","hNsFYpCQ","B8FIMcAy","IMIOuzwP","78QCpIlW","HCD43xFN","SzX5mNqJ","7Abz5mE0","LGlLYBuF","GLvnCqdL","ixtkRmoN","Dw0hXYN8","fvynEmzX","hLrrvhhw","X6I2kUM2","2hLKB7nj","vKvOHkTj","k82QLlFn","vPUalfIf","QKnQCUGC","tZ7NjYGv","ChyZ5Yzl","5shWWKk4","W1zMqVF5","C7Xdxsm3","Qb9YA9In","MNAF6iha","OoWcXWTz","7L1PRUmO","zTKHMGTm","cJtWab4d","rnTPbFeZ","1fjeZAbj","V5tyvDX1","Y4oRBrP2","BDWsUrRI","8YdbE9h3","ITLFpj7q","jE8lujeX","ocTE0Yrm","kyIkbkad","A9QULGEu","XLxRPj7M","DO8vnU88","vL6WQp9o","XsE3UYw6","c6mglck6","q5IBLZK8","JNhqsXO4","tlB1t2OW","oDxhq5BU","ADRvXMPb","f4S4XeL1","NQUq5DAZ","Mku7KCYJ","Ch0DgT0o","YMweQs8d","oVcQtpC7","J4CdcGmD","CNcAoymb","G7Co1qTN","DBzAO8Gr","0cndiakQ","5A0RPeoe","ECLhWktA","unaAH0qa","jjKOMsqh","OY71u9ru","dmpcBmZv","oDJa3hds","UXXjGndA","3BfMwyJS","ZWTJFFXP","BHmSFoht","pKSd012W","QnJopUFM","TjZD1mUD","8kZA6OAd","sTNnUSQF","MyDGBiR2","V0OyTpi5","18nvHLsM","D3tePXCP","q2ixPhX4","JlDt6EpQ","nKl9SpAZ","cRfVEr9v","d5uJfz93","awo5YGlb","krApICGA","F1MH2bNV","9syIWvGf","hNDTUme2","JiUataKM","xq0C3R8s","j41zwcKe","kcd1fid4","zAbhk0I0","te7YdbzF","SpBZiplM","WjLhQABe","UsREoHOP","p8EqDkEu","7xfHJvCv","z9FEpYyR","qAh9oC1C","I9lwStrV","swLzG2jN","y2IF9pP6","aS9vROYO","bV2kOjnE","R4F1PGrO","t3xMgUpD","3dMcX80y","03IQcLad","MpdWfjnZ","6Jycae4p","xdf9PfIF","LA86Yygy","eHjoQ9iq","dta15nJB","9S1Enzhj","M9pWypBo","uNptv7EY","DlYGSH9c","IFgKLBID","hOHDjCU5","UPXd5HRg","sIzjGVqR","SZd66dpk","WfVJmgj9","NAQzqihe","NXEsu5Iy","k0HkqnWn","xzqwD704","RQ5QWCB6","DwoRpITL","NUrAtJne","GmhBu9a7","RZXXSmgk","UQkjxIQl","jhpRHzWI","1XDvEVBa","C3WnfKeG","FN1jauzb","KWylat4K","wQrgesUn","WtyqeWQH","YCS2W39P","NnEM17pg","EodQUqtM","EBduDqkc","X5OPJ38e","1aTu2lWq","UG5kNiFu","fDQZz6VH","qrv1qRkO","z9V9bWOt","6OexX4M5","spm30WaM","SYsyGABE","tyDmtCh8","Bn14thX0","CJm9lMba","xoq7n3o6","2wOThVje","GWD1uBuu","1tNtZyUT","VWIGwWoe","UyE0i45F","bBZCMIBJ","kcceDoAz","c1Xviuo8","YMfjIpDi","rqUOI8cY","gjupsGZC","nMDs039S","ssOyqbHN","ajGGJfVw","sRfsGags","LpCYLoof","xLd3ygZ6"};
    // 16 char input
    // string listofinput[256] = {"3w9RiamICVFsLU5u","h7hyktB0b2hG8GJr","5tR5PCR1SCQaxhBn","IEPtMCmyUCcSv7V8","Ce0MtL8lNonTZq9w","ORX3piaWKx2IDnLJ","AAPlz7vP6Q9EcNF6","y7EitvZYJhhKIvFH","VWSHWlxcKmGeJrUJ","GR7vXkmY7FPzhj8u","ahwKDEt6hnoegYuz","N4SrVt3YSej5AAE6","KcdcfUX6FUq81rv7","A6ic6OVoubwwLtWa","OtDSyZplO8amyuV0","K4CCOkFvo7vWCYkY","N1k8Nf6YVz1vS73B","uiFfOlgFdsS6zE6Q","WXYKREMzHBZtjLyX","cjI7cIEcyh5XSJgW","hziT1bPABYjoeUzB","PMSLDOfZocPs8t5f","dV3qkb0ax66kFlkz","wT3yZrJpwht7gm0Y","7hx8eNEd0agZyFjd","AlnN00mV0pHRlgAG","dyCdw5KhDH8xPN15","wIs6nZ9A3zXAyhgZ","5D3Il5R2kk13EJyZ","6yt71OGsL0SB9Mh2","h8rbm8fUl4PQBC3v","yTp5434rWzm7QXKS","llHx9QTlRzHEokOM","XR7vaOTfhs02FR9F","LX2ktgPZGO88S0HH","koHeZ1iIFXvcUIqz","0JXgtSSeQniGP4aH","1oGbDrdMzBHsURv0","z7azY6i0NzFeTQNI","mm0GA3zaFznVAMSB","slaFZEfYMs5FxGfV","lw6Rna7qjgYIut8b","pw9bfvOjubwFKSOO","8lIeon6QkO6ARLNk","fmOqOoM7RHCqcWTj","8dCHJqLwBW8n98O7","Z6X3e756P0K6hoDQ","QW40whvZp8JrROxj","oY8kMKY5CpvUsDl4","p820RMqFxHmStkBu","6GrjhisCRsIuyijF","QBXq0pUctQcHSaOz","s9FUUKP7vot1dtlq","W3EgOklI1wGF4kOg","ASj9nPBbp6F0PmII","Ar7gOMUOR2fr5VB2","vgqm9PoJti6qFdxX","IItNLyPkC07gBeZm","DSR817Sm2fl9VsjK","zynTxZjpiINHJYlZ","nJyu4mhHQVqcTGjS","0OEulfh4TWbekv2n","pgz1ayZMf80xwLrn","WItnkrs1sZDYp8CI","xdn3oLYOQxZxxpxX","BjMIwuI03MwPMRdk","sReytsYKruISKZHC","va9OKUFSXq6nkcep","c33eBQA5BHZkiot8","Ea4GdPfhcFAtyfjx","mOy6JWjJ1lneNnaG","DAir06RqYRjYoIGI","vN1DFUrzlNE7ir13","HiSNLyEQmaqY4ReZ","l9khPMZMnh4ogJLu","xEeqjcLlUhdHXWG1","ZykvrmIekMGajcPZ","OEAj2bjQmQ6tUFoQ","53RFCk9lIzQvKNxW","YMoDfZbd5P8EuHeL","d4dVCZTLJ5JT4P5R","iwT6HlKuGurB7ZsL","o0ZruvyxOvBeGvCv","Thj6a24XsNFlkPBU","2aZOo6PbSSrcrekr","b2H2hudQKx84N6jH","Iiky025NSTFixSlJ","fFoNqS5SnkqmVNRe","qggI0DJo8Mbf0xjz","MWgSIdcKcluFnNiV","KYKRTcokrL3zEfr6","ZHfdKR1GrgcKxe5P","XMUCFlJcTeiy52is","VUpaSdKC6kYHrHb9","SQwNh2EyyN5xa63Q","VdfncC4LFPd7L1zg","N1Td5bFbmunBnoC3","JIDYm7BfAnIAg3Ov","cbzj7D9G1CqEMBFv","uGZR5uX382hMzBKh","ZUX0maTOsFq0uGB4","ZrVY0pOrGmTs1aoi","Rvm54p2zHJ7qviDO","d2KzCSgNaKrx764D","VwbJBIQc6n5R4oJz","7VaA8mmNU5gQednE","c0szycpaS7S0JBT4","UhWDaY159tb1cKJu","wnqolAWHiDDu375V","0jp1dypicz73vFF5","31dBmLOySr2OXfJN","yRLFkI6cZfWNLuus","EYBtLg98PVdnRy6f","3vIDAqdlBw2v8q8G","jtHoQlkAEcvpiVKz","dSjI9SfF4XVUuUTN","e7qK69mjJAur6uZa","dF2QrdG0EUrtVOus","hgE1752YRdFhbigN","y8MsiHK2odRq1TPl","rl6cZRJNrNwcsZSk","TRgmyQMkAIFqgwgz","97SXyGiVZwhSposS","t06gm3zLCmYbtbr3","FGwmyPcmy8RBos10","wDOitFTNI6Q0IBoB","rnNZUEyOXnnMJ3m6","Qi0Cu62vbmxvjwHD","4Arjj4uTCOKfRnjS","3tNFvWqbdaY5tfGi","ExJppQdEiC0C5Xba","hcMCvBUF7AlZ2JuH","2PE7R23wuv0qV0UY","uPGu9h2ONFO382h8","BwWht3eVhdEQsTmm","CiSsjqjOtCzh6ifG","S3INUm784celfYAv","iQWTeFVUawIkq34G","sCexcZppFYu6d1lD","DScgEeR8RNhrKshW","YHpsvgUk8epJFwtt","IHCzA4qVV3cweswv","8x1a4Qjxz9hWN3fA","A3M3NoxyhzG89tsT","nSj1jLLAwXPoUB0S","VVnC2jPzl8QsTiTg","uuu17MOksp9oGl0e","91EdJeQsZTVOsa0L","KGtcU6iZtiu1AJaU","bTxesIBCRWZ7xGyR","03sgJuoLAIE6J1LH","7YXcGEKUyz6Nxvrc","aMekkEneV8Zg9ROX","XzsqZgz7QIy02X7P","kWn2Jrx3S2dPEzhi","wMJaugnOUKTYXvZu","Is0ZHNJRH033FviL","raSY8VDWHR6JlKg8","rBfkQJWCnPT0sPkA","2xknxEH3swM5hgiN","nSSWRipttOfHMgtz","memZPYNW4rLYqXi1","Tk3Y3XAB6WvF8dG4","GHDxs4w0qRe75Zed","GX5oxkBkX3Ve3pPv","cnzDcKMFixoN55px","t6OPSBVDWMOOGcGX","QrNBFt1bg4qOLV6W","SWB65k3YUCZaQZFf","zksV9Ok1czQJMdWQ","RaeFluFJohpwWa4K","wAM6kjed6XAkAhlW","KaLimPbbOzzvUihN","uefRf1m01jwo82T8","hbNPPuF3l7jXaJwk","ffhYjtNh3cM3YxcD","ZyxdR6mrkWTeS63Z","F6oc7OxFzNQTCP9Q","17nMQmXUOGvkfCr0","mQlI2CpMwGzVNpzF","7xk5Y9Mg9u42aU2l","Q36ez0HF2ny7hptf","vxOYBd5sa1MkEEBh","gKVuiJikKZS7o7B5","DokOo7OFOrsC4NE4","YQ78HBCWFjg4Leya","jqVI9gSylTOCdkOr","jT2aQDPaeDcrMP6q","iYbt9eX21332pz4X","sikTbk26seXEZzyH","52ehoXVNHUNCmi1M","gMUppEA0tkroYeYa","gHbn8BFDdpvYVi8o","gA0FVBYhbJBX4ANf","0IyHF9cuIIKGp7PJ","DOEaRW1Fg0oRKEV7","ZLTYIkaZVxgeJuDj","WXLiLUsH7cpw8c2Y","LHLmapUYBvlGSorS","s81LP5H8oStO2kY3","mc8IxLOLtgeK8xWP","nYUbDyqf2OgIdCkp","oHoLUn02gTyDcNvk","owEsRout6hyBjGpq","w7OeLLHhB0aV7MHG","UiIr1Ylp3xul3FVs","RGuoAzCIeuPdrwq2","QSfZZ9NFPnpcfnpK","GZdYRf4Dt5wHyPnL","4b5KMAGbJjdgaFf7","xZojlqF3jCZObW19","lhgEpWpRXhz9yLGf","u9xtU2V0ijJxXcgA","MlfI6hn69Unh0Amw","3tqmCCpgE3gjLVcF","MaOCIGiClIv3X4tY","4jdqWakhF60E29Ij","orNPKj5octNqBepE","iN3mkaHGMIQJYh5z","dBP8c9DC0MY09YxA","qRUllKiwG89AhyJI","ok8AmfHvIgdUjzeT","atTRk5Fwqb8r6sfQ","B2hZKYfdmAWd2D8P","WmLXe9ESeu2aZskz","rwZIxeaQxuqQgkBc","RzPFh8M78QH3syji","Th4WHwNMH1xQPc1w","qiGBNLWWjh0fKZpa","x3GWyAhvCx40ArMk","D9FlRnR0RwY7SosT","MMohMMSXbGmeFf3E","pzN4W17TSxNVRF69","ZFLnHKC3z7R3mXFW","XOpi65zTyvwPRB7M","BQkdRbEEhuEZEMuv","a46weqT2sAYqPlPW","uBo2uhrCA6b641oE","HYS9CAC9EizOF6kI","XIIBArcVO7KFYtfV","52MoNf4nGfkdUfEq","jVISHwz1xDquwRcq","FxTL7B2A3CksaRQK","fv9XE0QlwYUAMjU9","mgKZKGiDz7Yesxdp","kE50f3BxvoRMbI4o","d83UrVjchuxi9Uz9","zY9h4715IkrTI3FG","aByD6kRHqfNtDHej","Xav2OeVKoS4bUKEU","qpzNUYLbWYqO64I7","uO7HpQEjVALc9yZj","EuozuPC16FpvJ4bB","GkZsqNHvJABAqEM1","DkiJp15RTfiZjFYB","9eViw40CyxdBkGfX"};
        
    /** this below is a 256 parallel string test 
     set to 1 if want to test
     **/
    #if 1
    uint32_t res[8][32] = {0}, op[8][32] = {0};
    for(uint32_t j = 0; j < len; ++j){
        get_value_at_index(listofinput, res[0], j, 0);
        get_value_at_index(listofinput, res[1], j, 32);
        get_value_at_index(listofinput, res[2], j, 32 * 2);
        get_value_at_index(listofinput, res[3], j, 32 * 3);
        get_value_at_index(listofinput, res[4], j, 32 * 4);
        get_value_at_index(listofinput, res[5], j, 32 * 5);
        get_value_at_index(listofinput, res[6], j, 32 * 6);
        get_value_at_index(listofinput, res[7], j, 32 * 7);
        transposeblock2bitslice(res[0], op[0]);
        transposeblock2bitslice(res[1], op[1]);
        transposeblock2bitslice(res[2], op[2]);
        transposeblock2bitslice(res[3], op[3]);
        transposeblock2bitslice(res[4], op[4]);
        transposeblock2bitslice(res[5], op[5]);
        transposeblock2bitslice(res[6], op[6]);
        transposeblock2bitslice(res[7], op[7]);
        
        bsconst8x(op[0], op[1], op[2], op[3], op[4], op[5], op[6], op[7], W[L++]);
        if(L == 64){
            processing();
            L = 0;
            blocks += 512;
        } 
    }
    #endif

    /** this below is a single string test 
     set to 1 if want to test
     **/
    #if 0
    string text = "Hello";
    for(int i = 0; i < text.length(); i++){
        bsconst(text[i], W[L++]);
        if(L == 64){
            processing();
            L = 0;
            blocks += 512;
        } 
    }
    #endif
}

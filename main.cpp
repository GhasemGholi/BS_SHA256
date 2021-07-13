#include "optimized_bssha256.cpp"
#include "sha256.cpp"
/** 
 VariadicTable is a pretty print header that I got from stackoverflow and github.
 source: https://github.com/friedmud/variadic_table
 **/
#include "VariadicTable.h"

void pretty_print(double totaltimebs, double totaltimenm, long long totalcpucyclesbs, long long totalcpucyclesnm, size_t len){
    VariadicTable<string, string, long long, string, string, long long> vt({"SHA256 TYPE", "FILE SIZE" ,"CPU CYCLES", "TOTAL TIME", "SPEED", "HASHES PER SECOND"},
                                                            10);

    string bstotaltime = to_string(totaltimebs / 1000000000);
    bstotaltime.push_back('s');

    double bs_mbps = totaltimebs / 1000000000;

    string transfer_rate_bs;
    string bs_speed;
    if((1 / bs_mbps) > 1000){
        transfer_rate_bs= " GB/s";
        bs_speed = to_string((1 / bs_mbps) / 1000);
    }
    else{
        transfer_rate_bs = " MB/s";
        bs_speed = to_string(1 / bs_mbps);
    }

    string nmtotaltime = to_string(totaltimenm / 1000000000);
    nmtotaltime.push_back('s');

    string nm_speed;
    double nm_mbps = totaltimenm / 1000000000;
    string transfer_rate_nm;
    if((1 / nm_mbps) > 1000){
        transfer_rate_nm = " GB/s";
        nm_speed = to_string((1 / nm_mbps) / 1000);
    }
    else{
        transfer_rate_nm = " MB/s";
        nm_speed = to_string(1 / nm_mbps);
    }    

    bs_speed.append(transfer_rate_bs);
    nm_speed.append(transfer_rate_nm);

    vt.addRow("BITSLICED SHA256", BytesToString(len), totalcpucyclesbs, bstotaltime, bs_speed, (((1/bs_mbps) * pow(2, 20)) / len) * 256);
    vt.addRow("NORMAL SHA256", BytesToString(len), totalcpucyclesnm, nmtotaltime, nm_speed, (((1/nm_mbps) * pow(2, 20)) / len));
    
    
    vt.print(std::cout);
}

int main(int argc, char *argv[]){
    bool isFile = true;
    string splitted_input[256];
    if(strcmp(argv[1], "test5") == 0){
        // 5 char input
        isFile = false;
        string input[256] = {"stare","avoid","exist","enter","lower","smile","share","creep","break","plead","apply","leave","guess","trace","spell","incur","stand","upset","price","mount","start","strip","shine","worry","quote","drift","value","speak","teach","smash","flash","crack","begin","drive","dream","sweep","phone","treat","train","claim","crash","award","widen","prove","arise","state","delay","admit","twist","weigh", "bring","float","shout","sleep","hurry","sweep","fight","share","adapt","break","shift","prove","clean","score","spoil","going","plead","focus","cover","house","doubt","state","bound","smile","sense","learn","laugh","upset","worry","throw","limit","seize","lower","crush","block","drown","avoid","stick","enter","alert","check","write","argue","boast","drive","claim","admit","grant","blame","found", "equip","crush","swing","solve","frown","close","marry","judge","seize","react","dance","refer","adapt","shrug","crawl","value","apply","admit","flood","laugh","flash","waste","amend","house","stage","imply","smash","climb","place","burst","blame","arise","chase","phone","store","cease","plant","light","reply","price","steal","march","spell","stare","label","bring","found","carve","teach","voice", "award","creep","agree","drain","trust","spill","prove","shoot","crush","split","waste","greet","train","offer","twist","merge","spare","shake","shift","elect","price","unite","start","fancy","light","renew","equip","share","fight","lower","refer","round","think","press","dream","brush","knock","arise","imply","sweep","crack","grasp","admit","strip","quote","phone","raise","block","shrug","swear", "award","creep","agree","drain","trust","spill","prove","shoot","crush","split","waste","greet","train","offer","twist","merge","spare","shake","shift","elect","price","unite","start","fancy","light","renew","equip","share","fight","lower","refer","round","think","press","dream","brush","knock","arise","imply","sweep","crack","grasp","admit","strip","quote","phone","raise","block","shrug","swear", "Hello","Hello","Hello","Hello","Hello", "Hello"};   
        copy(input, input + 256, splitted_input);
        len = 5;
    }

    if(strcmp(argv[1], "test8") == 0){
        // 8 char input
        isFile = false;
        string input[256] = {"6cfHsIC9","h7aSKPfd","ZqaOzwFF","H1EESpMz","kU2afSlk","Zqtgbsbp","lBFxjCXS","FOETJZIa","fUjEBpNV","FLlbRYky","7ohc7oOp","759sAI0n","tph3OB09","nULOpE5H","R2CO6lnH","9AWPX9Cz","efNEQkgw","QLfwIg1t","6rQG1VPv","ROFb2Iiv","DVVMxE0r","m97dqaa7","5rDvjKKM","X273GqL4","sUd72jJ8","HTNX21ST","hiFetsvy","aPPIAwpW","EDvWdxhs","DIqnvQ2J","vYJtbGU7","mabqHEeL","pvT7vkLL","KKSeEpVq","R7bPJb1b","7R8XFnlJ","5GeLcfgI","i5NWIjF6","6RNq1Mhf","YONDaSy2","6Y4Rk76C","juGaOLeN","54PNmhtL","ouTz3hlb","TwBm4Adv","JbYicc5b","JeDQKloS","vOmcJjjz","MO8VJuRt","HnPfbZne","a9BEBt9d","o4oYzbfT","Sdc4hk79","lbSv971L","vLQZnCgl","vWjvKhBk","zGXbjAd9","AtfrE2db","0oIR3BjS","OlhgiFro","mBxsqsnu","h6Askf1v","VGBvtcU3","WyICDZ6C","yhSL7REN","UVS6yQe0","e4pdi6Tw","k1icyDnw","TY1UEfEl","a4mmRkAA","9p7mplcg","HRvIcGA5","y9zTeL6K","HKfWOuBb","4AMjYBpT","hNsFYpCQ","B8FIMcAy","IMIOuzwP","78QCpIlW","HCD43xFN","SzX5mNqJ","7Abz5mE0","LGlLYBuF","GLvnCqdL","ixtkRmoN","Dw0hXYN8","fvynEmzX","hLrrvhhw","X6I2kUM2","2hLKB7nj","vKvOHkTj","k82QLlFn","vPUalfIf","QKnQCUGC","tZ7NjYGv","ChyZ5Yzl","5shWWKk4","W1zMqVF5","C7Xdxsm3","Qb9YA9In","MNAF6iha","OoWcXWTz","7L1PRUmO","zTKHMGTm","cJtWab4d","rnTPbFeZ","1fjeZAbj","V5tyvDX1","Y4oRBrP2","BDWsUrRI","8YdbE9h3","ITLFpj7q","jE8lujeX","ocTE0Yrm","kyIkbkad","A9QULGEu","XLxRPj7M","DO8vnU88","vL6WQp9o","XsE3UYw6","c6mglck6","q5IBLZK8","JNhqsXO4","tlB1t2OW","oDxhq5BU","ADRvXMPb","f4S4XeL1","NQUq5DAZ","Mku7KCYJ","Ch0DgT0o","YMweQs8d","oVcQtpC7","J4CdcGmD","CNcAoymb","G7Co1qTN","DBzAO8Gr","0cndiakQ","5A0RPeoe","ECLhWktA","unaAH0qa","jjKOMsqh","OY71u9ru","dmpcBmZv","oDJa3hds","UXXjGndA","3BfMwyJS","ZWTJFFXP","BHmSFoht","pKSd012W","QnJopUFM","TjZD1mUD","8kZA6OAd","sTNnUSQF","MyDGBiR2","V0OyTpi5","18nvHLsM","D3tePXCP","q2ixPhX4","JlDt6EpQ","nKl9SpAZ","cRfVEr9v","d5uJfz93","awo5YGlb","krApICGA","F1MH2bNV","9syIWvGf","hNDTUme2","JiUataKM","xq0C3R8s","j41zwcKe","kcd1fid4","zAbhk0I0","te7YdbzF","SpBZiplM","WjLhQABe","UsREoHOP","p8EqDkEu","7xfHJvCv","z9FEpYyR","qAh9oC1C","I9lwStrV","swLzG2jN","y2IF9pP6","aS9vROYO","bV2kOjnE","R4F1PGrO","t3xMgUpD","3dMcX80y","03IQcLad","MpdWfjnZ","6Jycae4p","xdf9PfIF","LA86Yygy","eHjoQ9iq","dta15nJB","9S1Enzhj","M9pWypBo","uNptv7EY","DlYGSH9c","IFgKLBID","hOHDjCU5","UPXd5HRg","sIzjGVqR","SZd66dpk","WfVJmgj9","NAQzqihe","NXEsu5Iy","k0HkqnWn","xzqwD704","RQ5QWCB6","DwoRpITL","NUrAtJne","GmhBu9a7","RZXXSmgk","UQkjxIQl","jhpRHzWI","1XDvEVBa","C3WnfKeG","FN1jauzb","KWylat4K","wQrgesUn","WtyqeWQH","YCS2W39P","NnEM17pg","EodQUqtM","EBduDqkc","X5OPJ38e","1aTu2lWq","UG5kNiFu","fDQZz6VH","qrv1qRkO","z9V9bWOt","6OexX4M5","spm30WaM","SYsyGABE","tyDmtCh8","Bn14thX0","CJm9lMba","xoq7n3o6","2wOThVje","GWD1uBuu","1tNtZyUT","VWIGwWoe","UyE0i45F","bBZCMIBJ","kcceDoAz","c1Xviuo8","YMfjIpDi","rqUOI8cY","gjupsGZC","nMDs039S","ssOyqbHN","ajGGJfVw","sRfsGags","LpCYLoof","xLd3ygZ6"};
        copy(input, input + 256, splitted_input);
        len = 8;
    }
    if(strcmp(argv[1], "test16") == 0){
        // 16 char input
        isFile = false;
        string input[256] = {"3w9RiamICVFsLU5u","h7hyktB0b2hG8GJr","5tR5PCR1SCQaxhBn","IEPtMCmyUCcSv7V8","Ce0MtL8lNonTZq9w","ORX3piaWKx2IDnLJ","AAPlz7vP6Q9EcNF6","y7EitvZYJhhKIvFH","VWSHWlxcKmGeJrUJ","GR7vXkmY7FPzhj8u","ahwKDEt6hnoegYuz","N4SrVt3YSej5AAE6","KcdcfUX6FUq81rv7","A6ic6OVoubwwLtWa","OtDSyZplO8amyuV0","K4CCOkFvo7vWCYkY","N1k8Nf6YVz1vS73B","uiFfOlgFdsS6zE6Q","WXYKREMzHBZtjLyX","cjI7cIEcyh5XSJgW","hziT1bPABYjoeUzB","PMSLDOfZocPs8t5f","dV3qkb0ax66kFlkz","wT3yZrJpwht7gm0Y","7hx8eNEd0agZyFjd","AlnN00mV0pHRlgAG","dyCdw5KhDH8xPN15","wIs6nZ9A3zXAyhgZ","5D3Il5R2kk13EJyZ","6yt71OGsL0SB9Mh2","h8rbm8fUl4PQBC3v","yTp5434rWzm7QXKS","llHx9QTlRzHEokOM","XR7vaOTfhs02FR9F","LX2ktgPZGO88S0HH","koHeZ1iIFXvcUIqz","0JXgtSSeQniGP4aH","1oGbDrdMzBHsURv0","z7azY6i0NzFeTQNI","mm0GA3zaFznVAMSB","slaFZEfYMs5FxGfV","lw6Rna7qjgYIut8b","pw9bfvOjubwFKSOO","8lIeon6QkO6ARLNk","fmOqOoM7RHCqcWTj","8dCHJqLwBW8n98O7","Z6X3e756P0K6hoDQ","QW40whvZp8JrROxj","oY8kMKY5CpvUsDl4","p820RMqFxHmStkBu","6GrjhisCRsIuyijF","QBXq0pUctQcHSaOz","s9FUUKP7vot1dtlq","W3EgOklI1wGF4kOg","ASj9nPBbp6F0PmII","Ar7gOMUOR2fr5VB2","vgqm9PoJti6qFdxX","IItNLyPkC07gBeZm","DSR817Sm2fl9VsjK","zynTxZjpiINHJYlZ","nJyu4mhHQVqcTGjS","0OEulfh4TWbekv2n","pgz1ayZMf80xwLrn","WItnkrs1sZDYp8CI","xdn3oLYOQxZxxpxX","BjMIwuI03MwPMRdk","sReytsYKruISKZHC","va9OKUFSXq6nkcep","c33eBQA5BHZkiot8","Ea4GdPfhcFAtyfjx","mOy6JWjJ1lneNnaG","DAir06RqYRjYoIGI","vN1DFUrzlNE7ir13","HiSNLyEQmaqY4ReZ","l9khPMZMnh4ogJLu","xEeqjcLlUhdHXWG1","ZykvrmIekMGajcPZ","OEAj2bjQmQ6tUFoQ","53RFCk9lIzQvKNxW","YMoDfZbd5P8EuHeL","d4dVCZTLJ5JT4P5R","iwT6HlKuGurB7ZsL","o0ZruvyxOvBeGvCv","Thj6a24XsNFlkPBU","2aZOo6PbSSrcrekr","b2H2hudQKx84N6jH","Iiky025NSTFixSlJ","fFoNqS5SnkqmVNRe","qggI0DJo8Mbf0xjz","MWgSIdcKcluFnNiV","KYKRTcokrL3zEfr6","ZHfdKR1GrgcKxe5P","XMUCFlJcTeiy52is","VUpaSdKC6kYHrHb9","SQwNh2EyyN5xa63Q","VdfncC4LFPd7L1zg","N1Td5bFbmunBnoC3","JIDYm7BfAnIAg3Ov","cbzj7D9G1CqEMBFv","uGZR5uX382hMzBKh","ZUX0maTOsFq0uGB4","ZrVY0pOrGmTs1aoi","Rvm54p2zHJ7qviDO","d2KzCSgNaKrx764D","VwbJBIQc6n5R4oJz","7VaA8mmNU5gQednE","c0szycpaS7S0JBT4","UhWDaY159tb1cKJu","wnqolAWHiDDu375V","0jp1dypicz73vFF5","31dBmLOySr2OXfJN","yRLFkI6cZfWNLuus","EYBtLg98PVdnRy6f","3vIDAqdlBw2v8q8G","jtHoQlkAEcvpiVKz","dSjI9SfF4XVUuUTN","e7qK69mjJAur6uZa","dF2QrdG0EUrtVOus","hgE1752YRdFhbigN","y8MsiHK2odRq1TPl","rl6cZRJNrNwcsZSk","TRgmyQMkAIFqgwgz","97SXyGiVZwhSposS","t06gm3zLCmYbtbr3","FGwmyPcmy8RBos10","wDOitFTNI6Q0IBoB","rnNZUEyOXnnMJ3m6","Qi0Cu62vbmxvjwHD","4Arjj4uTCOKfRnjS","3tNFvWqbdaY5tfGi","ExJppQdEiC0C5Xba","hcMCvBUF7AlZ2JuH","2PE7R23wuv0qV0UY","uPGu9h2ONFO382h8","BwWht3eVhdEQsTmm","CiSsjqjOtCzh6ifG","S3INUm784celfYAv","iQWTeFVUawIkq34G","sCexcZppFYu6d1lD","DScgEeR8RNhrKshW","YHpsvgUk8epJFwtt","IHCzA4qVV3cweswv","8x1a4Qjxz9hWN3fA","A3M3NoxyhzG89tsT","nSj1jLLAwXPoUB0S","VVnC2jPzl8QsTiTg","uuu17MOksp9oGl0e","91EdJeQsZTVOsa0L","KGtcU6iZtiu1AJaU","bTxesIBCRWZ7xGyR","03sgJuoLAIE6J1LH","7YXcGEKUyz6Nxvrc","aMekkEneV8Zg9ROX","XzsqZgz7QIy02X7P","kWn2Jrx3S2dPEzhi","wMJaugnOUKTYXvZu","Is0ZHNJRH033FviL","raSY8VDWHR6JlKg8","rBfkQJWCnPT0sPkA","2xknxEH3swM5hgiN","nSSWRipttOfHMgtz","memZPYNW4rLYqXi1","Tk3Y3XAB6WvF8dG4","GHDxs4w0qRe75Zed","GX5oxkBkX3Ve3pPv","cnzDcKMFixoN55px","t6OPSBVDWMOOGcGX","QrNBFt1bg4qOLV6W","SWB65k3YUCZaQZFf","zksV9Ok1czQJMdWQ","RaeFluFJohpwWa4K","wAM6kjed6XAkAhlW","KaLimPbbOzzvUihN","uefRf1m01jwo82T8","hbNPPuF3l7jXaJwk","ffhYjtNh3cM3YxcD","ZyxdR6mrkWTeS63Z","F6oc7OxFzNQTCP9Q","17nMQmXUOGvkfCr0","mQlI2CpMwGzVNpzF","7xk5Y9Mg9u42aU2l","Q36ez0HF2ny7hptf","vxOYBd5sa1MkEEBh","gKVuiJikKZS7o7B5","DokOo7OFOrsC4NE4","YQ78HBCWFjg4Leya","jqVI9gSylTOCdkOr","jT2aQDPaeDcrMP6q","iYbt9eX21332pz4X","sikTbk26seXEZzyH","52ehoXVNHUNCmi1M","gMUppEA0tkroYeYa","gHbn8BFDdpvYVi8o","gA0FVBYhbJBX4ANf","0IyHF9cuIIKGp7PJ","DOEaRW1Fg0oRKEV7","ZLTYIkaZVxgeJuDj","WXLiLUsH7cpw8c2Y","LHLmapUYBvlGSorS","s81LP5H8oStO2kY3","mc8IxLOLtgeK8xWP","nYUbDyqf2OgIdCkp","oHoLUn02gTyDcNvk","owEsRout6hyBjGpq","w7OeLLHhB0aV7MHG","UiIr1Ylp3xul3FVs","RGuoAzCIeuPdrwq2","QSfZZ9NFPnpcfnpK","GZdYRf4Dt5wHyPnL","4b5KMAGbJjdgaFf7","xZojlqF3jCZObW19","lhgEpWpRXhz9yLGf","u9xtU2V0ijJxXcgA","MlfI6hn69Unh0Amw","3tqmCCpgE3gjLVcF","MaOCIGiClIv3X4tY","4jdqWakhF60E29Ij","orNPKj5octNqBepE","iN3mkaHGMIQJYh5z","dBP8c9DC0MY09YxA","qRUllKiwG89AhyJI","ok8AmfHvIgdUjzeT","atTRk5Fwqb8r6sfQ","B2hZKYfdmAWd2D8P","WmLXe9ESeu2aZskz","rwZIxeaQxuqQgkBc","RzPFh8M78QH3syji","Th4WHwNMH1xQPc1w","qiGBNLWWjh0fKZpa","x3GWyAhvCx40ArMk","D9FlRnR0RwY7SosT","MMohMMSXbGmeFf3E","pzN4W17TSxNVRF69","ZFLnHKC3z7R3mXFW","XOpi65zTyvwPRB7M","BQkdRbEEhuEZEMuv","a46weqT2sAYqPlPW","uBo2uhrCA6b641oE","HYS9CAC9EizOF6kI","XIIBArcVO7KFYtfV","52MoNf4nGfkdUfEq","jVISHwz1xDquwRcq","FxTL7B2A3CksaRQK","fv9XE0QlwYUAMjU9","mgKZKGiDz7Yesxdp","kE50f3BxvoRMbI4o","d83UrVjchuxi9Uz9","zY9h4715IkrTI3FG","aByD6kRHqfNtDHej","Xav2OeVKoS4bUKEU","qpzNUYLbWYqO64I7","uO7HpQEjVALc9yZj","EuozuPC16FpvJ4bB","GkZsqNHvJABAqEM1","DkiJp15RTfiZjFYB","9eViw40CyxdBkGfX"};
        copy(input, input + 256, splitted_input);
        len = 16;
    }
    __m256i hashed_bs[32][32];
    uint32_t hashed[32];

    init_sha256();

    if(isFile){
        sha256 sha;
        string r_file = read_file(argv[1]);
        string splitted_input[256];

        split_into_256_chunks(splitted_input, r_file);
        sha.parse_text(r_file, len);
        sha.pre_processing(hashed);
        parse_text_bssha(splitted_input, len / 256);
    }

    if(!isFile){
        for(uint32_t i = 0; i < 256; i++){
            sha256 sha;
            sha.parse_text(splitted_input[i], len);
            sha.pre_processing(hashed);
            #if 0 // print normal hash
            cout << "pos " << i << " ";
            for(uint32_t i = 0; i < 32; i++){
                printf("%02x", hashed[i]);
            }
            cout << endl;
            #endif
        }
        parse_text_bssha(splitted_input, len);
    }

    pre_processing_bs(hashed_bs);

    if(isFile){
        if(argc > 2 && strlen(argv[2]) < 4){
            appendLineToFile("resultbssha.txt", to_string(secondinfo_bs - firstinfo_bs));
            appendLineToFile("resultnormalsha.txt", to_string(secondinfo - firstinfo));
            appendLineToFile("resultbsshaseconds.txt", to_string(cummulative_time_bs));
            appendLineToFile("resultshaseconds.txt", to_string(cummulative_time));
        }
        else if(argc > 2 && strlen(argv[2]) == 4){
            std::ifstream      file("resultbssha.txt");
            long long bssum = 0;
            long long temp = 0;
            for(int i = 0; i < 1000; i++){
                file >> temp;
                bssum += temp;
            }
            std::ifstream      fl("resultbsshaseconds.txt");
            long double bssec = 0;
            long double temp1 = 0;
            for(int i = 0; i < 1000; i++){
                fl >> temp1;
                bssec += temp1;
            }
            std::ifstream      f("resultnormalsha.txt");
            long long nmsum = 0;
            for(int i = 0; i < 1000; i++){
                f >> temp;
                nmsum += temp;
            }
            std::ifstream      fsec("resultshaseconds.txt");
            long double nmsec = 0;
            for(int i = 0; i < 1000; i++){
                fsec >> temp1;
                nmsec += temp1;
            }
            
            pretty_print(bssec / 1000, nmsec / 1000, bssum / 1000, nmsum / 1000, len);
                
            ofstream delbs, delbssec, delnm, delnmsec;
            delbs.open("resultbssha.txt", std::ios::out | std::ios::trunc);
            delbssec.open("resultbsshaseconds.txt", std::ios::out | std::ios::trunc);
            delnm.open("resultnormalsha.txt", std::ios::out | std::ios::trunc);
            delnmsec.open("resultshaseconds.txt", std::ios::out | std::ios::trunc);
        }
    }
    if(argc < 3){
        pretty_print(cummulative_time_bs, cummulative_time, secondinfo_bs - firstinfo_bs, secondinfo - firstinfo, len);
    }
    #if 0
        //must be a power of 2, pow(2, 14) = 16kb
        // n_amount_charstofile(pow(2, 29));
        print_hashes(hashed_bs);
    #endif

    return 0;
}

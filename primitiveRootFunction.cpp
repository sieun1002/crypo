#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <vector>


// //내가 짠 코드 function(is_primitive)
// int powm(int base, int exp, int mod){

//     int r = 1;
//     int i;

//     for(i=0; i<exp; i++){
//         r *= base;
//         r %= mod;
//     }

//     return r;   
// }

// bool is_primitive(int base, int mod){
//     int result;
//     result = powm(base, mod, mod);

//     if(result == base){
//         return true;
//     }
//     else{
//         return false;
//     }

// }



//교수님 코드
using namespace std;

bool is_primitive(int base, int mod);
vector<int> primitive_root(int mod);

int main(void){
    
    // for(int i: primitive_root(29))
    //     cout<< i << ',';
    // cout << endl;
    bool result = is_primitive(4, 5);
    printf("result: %d", result);

    return 0;
}

bool is_primitive(int base, int mod){
    //mod should be prime number
    int exp = 2;
    int r = base * base;
    for(; r!=base; exp++){
        r *= base;
        r %= mod;
    }
    return exp == mod;
}

vector<int> primitive_root(int mod){
    vector<int> v;
    for(int base=2; base<mod; base++){
        if(is_primitive(base, mod))
            v.push_back(base);
    }
    return v;
}
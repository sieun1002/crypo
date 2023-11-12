#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>

using namespace std;

bool is_primitive(int base, int mod);
vector<int> primitive_root(int mod);

int main(void){
  for(int i: primitive_root(5))
    cout<< i << ',';
  cout << endl;
    return 0;
}

bool is_primitive(int base, int mod){
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



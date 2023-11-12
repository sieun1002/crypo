#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>

using namespace std;

int powm(int base, int exp, int mod){
  int r = 1;
  int i;

  for(i=0; i<exp; i++){
    r *= base;
    r %= mod;
  }

  return r;
}

int main(void){
  // powm 함수 잘 되는 지 확인
  // int base;
  // int exp;
  // int mod;
  // int result;

  // printf("base: ");
  // scanf("%d", &base);

  // printf("exp: ");
  // scanf("%d", &exp);

  // printf("mod: ");
  // scanf("%d", &mod);

  // result = powm(base, exp, mod);

  // printf("result: %d \n", result);

  // // cout << result << endl

  
  //A, B 간단한 DH 키교환 했을 때 결과 값이 같은 지 확인 (동일한 키를 가지고 있나?)
  int base = 19;
  int mod = 29;

  int A_exp = 11;
  int B_exp = 23;

  int A_result1, B_result1, A_result2, B_result2; 
  A_result1 = powm(base, A_exp, mod);
  B_result1 = powm(base, B_exp, mod);

  A_result2 = powm(B_result1, A_exp, mod);
  B_result2 = powm(A_result1, B_exp, mod);

  cout<<"A의 키 값:"<< A_result2 << "\n" << "B의 키 값:"<< B_result2 << endl;

  return 0;

}
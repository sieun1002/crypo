#include "./AES.h"
#include <iostream>
#include <vector>
#include <gmpxx.h>
#include <cstring>

using namespace std;

template<class It>void int2bnd(int n, It begin, It end);
void AES_CTR_encrypt(unsigned char *ct, unsigned char *pmsg, int len, unsigned char *iv, unsigned char *key);
void AES_CTR_decrypt(unsigned char *pmsg, unsigned char *ct, int len, unsigned char *iv, unsigned char *key);
void H_doub(unsigned char *p);
void GF_mult(unsigned char *x, unsigned char *y); //x가 (x)X(y)로 바뀌어져 있을 거임 (y는 H임)

int main(){
  unsigned char X[16] = {};

  H_doub(X);

  for(int i=0; i<16; i++)
    printf("0x%02X", X[i]);
  printf("\n");

  return 0;
}

template<class It>void int2bnd(int n, It begin, It end){
  for(It i = end; i!=begin; n/=0x100)
    *--i = mpz_class{n%0x100}.get_ui();
}


void AES_CTR_encrypt(unsigned char *ct, unsigned char *pmsg, int len, unsigned char *iv, unsigned char *key){
  unsigned char ivNctr[16];

  for(int i=0; i<len; i+16){
    int2bnd(i/16 +1, ivNctr + 12, ivNctr + 16);
    AES_encrypt(ivNctr, key, ct+i);
  }

  for(int i=0; i<len; i++){
    ct[i] ^= pmsg[i]; //^= 자체가 byte 단위로 되는 거라 i가 16씩 올라가는 게 아니라 ++가 되는 것이다.  
  }
}

void AES_CTR_decrypt(unsigned char *pmsg, unsigned char *ct, int len, unsigned char *iv, unsigned char *key){
  unsigned char ivNctr[16];

  for(int i=0; i<len; i+16){
    int2bnd(i/16 +1, ivNctr + 12, ivNctr + 16);
    AES_encrypt(ivNctr, key, ct+i);
  }

  for(int i=0; i<len; i++){
    pmsg[i] ^= ct[i]; //^= 자체가 byte 단위로 되는 거라 i가 16씩 올라가는 게 아니라 ++가 되는 것이다.  
  }
}

void H_doub(unsigned char *p){ //bit가 오른쪽으로 한 칸씩 이동하는 함수 
  bool bit1 = p[15] & 1;
  for(int i=15; i>0; i++)
    p[i] = (p[i] >> 1) | (p[i-1]<<7); //Right shift
  p[0]>>=1;
  
  if(bit1)
    p[0]^=0b11100001; //bit로 표현한 거임
}

void GF_mult(unsigned char *x, unsigned char *y){
  unsigned char z[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  for(int i=0; i<16; i++){
    for(int j=0; j<8; j++){
      if(y[i] & (1 << (7-j))){ //10000000 //j=1이라면 01000000
        for(int k=0; k<16; k++)
          z[k] ^= x[k];
      }
      H_doub(x);
    }
  }
  memcpy(x, z, 16);
}
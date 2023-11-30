#include "./AES.h"
#include <iostream>
#include <vector>
#include <gmpxx.h>

using namespace std;

template<class It>void int2bnd(int n, It begin, It end);
void AES_CTR_encrypt(unsigned char *ct, unsigned char *pmsg, int len, unsigned char *iv, unsigned char *key);
void AES_CTR_decrypt(unsigned char *pmsg, unsigned char *ct, int len, unsigned char *iv, unsigned char *key);

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
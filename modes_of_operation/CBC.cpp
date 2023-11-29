#include "./AES.h"
#include <iostream>
#include <vector>
#include <gmpxx.h>

using namespace std;

void AES_CBC_encrypt(unsigned char *ct, unsigned char *pmsg, int len, unsigned char *iv, unsigned char *key);
void AES_CBC_decrypt(unsigned char *dmsg, unsigned char *ct, int len, unsigned char *iv, unsigned char *key);

int main(void){
  unsigned char msg[] = "Single block msg";
  unsigned char aes_128_key[] = {0x06, 0xa9, 0x40, 0x36, 0xb8, 0xa1, 0x56,
                                0x51, 0x2e, 0x03, 0xd5, 0x34, 0x12, 0x00, 0x06};
  unsigned char iv[] = {0x3d, 0xaf, 0xba, 0x42, 0x9d, 0x9e, 0xb4, 0x30, 
                        0xb4, 0x22, 0xda, 0x80, 0x2c, 0x9f, 0xac, 0x41};
  int len = ((sizeof(msg)/BLOCK_SIZE)+1)*BLOCK_SIZE;
  unsigned char ct[len];
  unsigned char dmsg[len];

  
// pmsg는 padding 된 거임... 이것도 코드에 구현해야함. 
  // AES_CBC_encrypt(ct, pmsg, len, iv, aes_128_key);
  cout<<ct<<endl;
  AES_CBC_decrypt(dmsg, ct, len, iv, aes_128_key);
  cout<<dmsg<<endl;

  return 0;
}

void AES_CBC_encrypt(unsigned char *ct, unsigned char *pmsg, int len, unsigned char *iv, unsigned char *key){
  memcpy(ct, pmsg, len);

  for(int i=0; i<BLOCK_SIZE; i++)
    *(ct + i) ^= iv[i];

  for(int i=1; i<(len/BLOCK_SIZE); i++){
    AES_encrypt(ct, key, ct); //16바이트만 암호화를 한다 (마지막 ct의 의미)
    for(int j=0; j<BLOCK_SIZE; j++, ct++)
      *(ct+BLOCK_SIZE) ^= *ct;
  }
  AES_encrypt(ct, key, ct);
}

void AES_CBC_decrypt(unsigned char *dmsg, unsigned char *ct, int len, unsigned char *iv, unsigned char *key){
  memcpy(dmsg, ct, len);

  for(int i=0; i<len; i+=BLOCK_SIZE)
    AES_decrypt(dmsg+i, key, dmsg+i);
  
  for(int i=0; i<BLOCK_SIZE; i++, dmsg++)
    *dmsg ^= iv[i];
  
  for(int i=0; i<(len-BLOCK_SIZE); i++, dmsg++)
    *dmsg ^= ct[i];
}
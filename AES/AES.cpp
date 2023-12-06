#include <iostream>
#include <vector>
#include <gmpxx.h>
#include <cstring>

using namespace std;

#define ROUND 10 //AES-128
#define N 4 //key size in words;

void print_state(unsigned char *state);

void sub_bytes(unsigned char *msg);
void inv_sub_bytes(unsigned char *msg);
void shift_rows(unsigned char *msg);
void inv_shift_rows(unsigned char *msg);
unsigned char doub(unsigned char c);
void mix_columns(unsigned char *msg);
void inv_mix_columns(unsigned char *msg);
void add_round_key(unsigned char *msg, unsigned char round_key[ROUND+1][16] ,int round);
void key_schedule(unsigned char *key, unsigned char round_key[ROUND+1][16]);

void AES_encrypt(unsigned char *msg, unsigned char *key, unsigned char *ct);
void AES_decrypt(unsigned char *Ct, unsigned char *key, unsigned char *msg);

int main(){
  unsigned char msg[16] = {0x04, 0x00, 0x12, 0x18, 
                          0x02, 0x11, 0x18, 0x0F, 
                          0x13, 0x0E, 0x06, 0x11,
                          0x00, 0x0F, 0x07, 0x18};

  unsigned char key[16] = {0x54, 0x68, 0x61, 0x74,
							            0x73, 0x20, 0x6D, 0x79,
							            0x20, 0x4B, 0x75, 0x6E,
							            0x67, 0x20, 0x46, 0x75};

  unsigned char ct[16];

  print_state(msg);
  printf("\n");

  AES_encrypt(msg, key, ct);
  print_state(ct);
  printf("\n");

  AES_decrypt(ct, key, msg);
  print_state(msg);
  printf("\n");

	return 0;
}

void print_state(unsigned char *state){
  int i;
  for(i = 0; i<N; i++){
    printf("0x%02X ", state[i]);
    printf("0x%02X ", state[i+4]);
    printf("0x%02X ", state[i+8]);
    printf("0X%02X ", state[i+12]);
    printf("\n");
  }
}

void sub_bytes(unsigned char *msg){
  unsigned char sbox[256] = {
		0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 
		0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
		0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 
		0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
		0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc,
		0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
		0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a,
		0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
		0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 
		0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
		0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b,
		0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
		0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85,
		0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
		0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
		0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
		0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17,
		0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
		0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88,
		0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
		0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
		0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
		0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9,
		0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
		0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6,
		0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
		0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
		0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
		0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94,
		0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
		0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68,
		0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
	};

  for(int i=0; i<16; i++){
    msg[i] = sbox[msg[i]];
  }
}

void inv_sub_bytes(unsigned char *msg){
  unsigned char inv_sbox[256] = {
		0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38,
		0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
		0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 
		0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
		0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d,
		0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
		0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2,
		0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
		0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16,
		0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
		0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda,
		0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
		0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a,
		0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
		0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02,
		0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
		0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea,
		0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
		0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85,
		0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
		0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89,
		0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
		0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20,
		0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
		0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31,
		0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
		0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d,
		0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
		0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0,
		0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
		0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26,
		0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d 
	};

  for(int i=0; i<16; i++){
    msg[i] = inv_sbox[msg[i]];
  }
}

void shift_rows(unsigned char *msg)
{
    unsigned char tmp, tmp2;
    tmp = msg[1];
    msg[1] = msg[5]; msg[5] = msg[9]; msg[9] = msg[13]; msg[13] = tmp;

    tmp = msg[2]; tmp2 = msg[6];
    msg[2] = msg[10]; msg[6] = msg[14]; msg[10] = tmp; msg[14] = tmp2;

    tmp = msg[15];
    msg[15] = msg[11]; msg[11] = msg[7]; msg[7] = msg[3]; msg[3] = tmp;
}

void inv_shift_rows(unsigned char *msg)
{
    unsigned char tmp, tmp2;
    tmp = msg[13];
    msg[13] = msg[9]; msg[9] = msg[5]; msg[5] = msg[1]; msg[1] = tmp;

    tmp = msg[10]; tmp2 = msg[14];
    msg[14] = msg[6]; msg[10] = msg[2]; msg[6] = tmp2; msg[2] = tmp;

    tmp = msg[3];
    msg[3] = msg[7]; msg[7] = msg[11]; msg[11] = msg[15]; msg[15] = tmp;
}

unsigned char doub(unsigned char c){
  bool left_most_bit = c & 1<<7;
  c = c<<1;
  if(left_most_bit)
    c ^= 0x1b;
  return c;
}

void mix_columns(unsigned char *msg){
    unsigned char mix[4][4] = 
        {{2, 3, 1, 1}, {1, 2, 3, 1}, {1, 1, 2, 3}, {3, 1, 1, 2}};
    unsigned char c[4], d, result[16];

    for(int y=0; y<4; y++){
      for(int x=0; x<4; x++){
        for(int i=0; i<4; i++){
          d = msg[4*x+i];
        //   switch(mix[y][i]){
        //     case 1: c[i] = d; break;
        //     case 2: c[i] = d<<1; break;
        //     case 3: c[i] = d<<1 ^ d; break;
        //   }if((mix[y][i] != 1) && (d & 1<<7))
        //     c[i] = c[i] ^ 0x1b;
        // }

        switch(mix[y][i]){
            case 1: c[i] = d; break;
            case 2: c[i] = doub(d); break;
            case 3: c[i] = doub(d) ^ d; break;
          }
        }
        result[4*x+y] = c[0]^c[1]^c[2]^c[3];
      }
    }
    memcpy(msg, result, 16);
}

void inv_mix_columns(unsigned char *msg){
  unsigned char inv_mix[4][4] = 
      {{14, 11, 13, 9}, {9, 14, 11, 13}, {13, 9, 14, 11}, {11, 13, 9, 14}};
  unsigned char d, c[4], result[16];

  for(int y=0; y<4; y++){
    for(int x=0; x<4; x++){
      for(int i=0; i<4; i++){
        d = msg[4*x+i];
        switch(inv_mix[y][i]){
          case 9: c[i] = doub(doub(doub(d)))^d; break;
          case 11: c[i] = doub(doub(doub(d))^d)^d; break;
          case 13: c[i] = doub(doub(doub(d)^d))^d; break;
          case 14: c[i] = doub(doub(doub(d)^d)^d); break;
        }
      }
      result[4*x+y] = c[0]^c[1]^c[2]^c[3];
    }
  }
  memcpy(msg, result, 16);
}

void key_schedule(unsigned char *key, unsigned char round_key[ROUND+1][16]){
  unsigned char rcon[10] = {0x01, 0x02, 0x04, 0x08, 0x10,
                            0x20, 0x40, 0x80, 0x1B, 0x36};

  unsigned char sbox[256] = {
		0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 
		0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
		0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 
		0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
		0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc,
		0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
		0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a,
		0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
		0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 
		0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
		0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b,
		0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
		0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85,
		0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
		0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
		0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
		0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17,
		0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
		0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88,
		0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
		0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
		0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
		0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9,
		0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
		0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6,
		0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
		0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
		0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
		0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94,
		0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
		0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68,
		0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
	};

  memcpy(round_key[0], key, 16);
  unsigned char *p = &round_key[1][0];

  for(int i=0; i<ROUND; i++){
    //rotword
    *p = *(p-3);
    *(p+1) = *(p-2);
    *(p+2) = *(p-1);
    *(p+3) = *(p-4);

    //subword
    for(int j=0; j<4; j++)
      *(p+j) = sbox[*(p+j)]; 

    //rcon
    for(int j=0; j<4; j++, p++){
      if(j==0)
        *p ^= rcon[4*i/N];
      *p ^= *(p-4*N);
    }

    //12 left
    for(int j=0; j<3*N; j++, p++)
      *p = *(p-4*N) ^ *(p-4);
  }
}

void add_round_key(unsigned char *msg, unsigned char round_key[ROUND+1][16], int round){
  for(int i=0; i<16; i++)
    msg[i] ^= round_key[round][i];
}

void AES_encrypt(unsigned char *msg, unsigned char *key, unsigned char *ct){
  unsigned char round_key[ROUND+1][16];
  memcpy(ct, msg, 16);

  key_schedule(key, round_key);
  // for(int i=0; i<ROUND+1; i++){
  //   for(int j=0; j<16; j++){
  //       printf("0x%02X ", round_key[i][j]); 
  //   }
  //   printf("\n");
  // }
  // printf("\n");

  add_round_key(ct, round_key, 0);

  for(int i=1; i<ROUND; i++){
    sub_bytes(ct);
    shift_rows(ct);
    mix_columns(ct);
    add_round_key(ct, round_key, i);
  }

  sub_bytes(ct);
  shift_rows(ct);
  add_round_key(ct, round_key, ROUND);
}

void AES_decrypt(unsigned char *ct, unsigned char *key, unsigned char *msg){
  unsigned char round_key[ROUND+1][16];
  memcpy(msg, ct, 16);

  key_schedule(key, round_key);
  add_round_key(msg, round_key, ROUND);

  for(int i=(ROUND-1); i>0; i--){
    inv_shift_rows(msg);
    inv_sub_bytes(msg);
    add_round_key(msg, round_key, i);
    inv_mix_columns(msg);
  }

  inv_shift_rows(msg);
  inv_sub_bytes(msg);
  add_round_key(msg, round_key, 0);
}

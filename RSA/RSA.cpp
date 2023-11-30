#include <iostream>
#include <vector>
#include <gmpxx.h>

using namespace std;
struct RSA_PK{
  mpz_class e;
  mpz_class N;
};

struct RSA_SK{
  mpz_class e;
  mpz_class N;
  mpz_class d;
  mpz_class p;
  mpz_class q;
};

//wrapper 함수
mpz_class powm(mpz_class base, mpz_class exp, mpz_class mod);
mpz_class random_prime(unsigned int bit);
mpz_class mod_inv(mpz_class x, mpz_class mod);
mpz_class urandom(gmp_randstate_t state, int bit);
mpz_class nextprime(mpz_class x);
mpz_class lcm(mpz_class a, mpz_class b);
mpz_class gcd(mpz_class a, mpz_class b);
int cmp(mpz_class a, mpz_class b);
mpz_class mod(mpz_class x, mpz_class mod);

//본 함수
void RSA_keygen(RSA_PK *pk, RSA_SK *sk, int key_size);
mpz_class encrypt(RSA_PK pk, mpz_class msg);
mpz_class decrypt(RSA_SK sk, mpz_class ct);
mpz_class RSA_sign(RSA_SK sk, mpz_class msg);
bool RSA_verify(RSA_PK pk, mpz_class sig, mpz_class msg);
mpz_class RSA_decrypt_CRT(RSA_SK sk, mpz_class ct);



int main(){
  //encrypt(), decrypt() 함수 테스트
  // RSA_PK pk;
  // RSA_SK sk;

  // RSA_keygen(&pk, &sk, 2048);

  // mpz_class msg = 4;
  // mpz_class ct = encrypt(pk, msg);
  // mpz_class newmsg = decrypt(sk, ct);

  // cout<<"msg = "<<msg<<endl;
  // cout<<"ct = "<<ct<<endl;
  // cout<<"newmsg = "<<newmsg<<endl;

  // if(cmp(msg, newmsg) == 0)
  //   cout<<"success"<<endl;
  // else
  //   cout<<"fail"<<endl;

  //RSA_sign(), RSA_verify() 함수 확인
  // bool rop;
  // mpz_class msg;
  // mpz_class sig; 
  // struct RSA_PK pk;
  // struct RSA_SK sk; 

  // msg = mpz_class{"0x143214324234"};

  // RSA_keygen(&pk, &sk, 2048);
  // sig = RSA_sign(sk, msg);
  // rop = RSA_verify(pk, sig, msg);

  // if(rop==true)
  //   cout<<"true"<<endl;
  // else if(rop==false)
  //   cout<<"false"<<endl;
  // else 
  //   cout<<"error"<<endl;

  //RSA_decrypt_CRT() 함수 확인
  bool rop;
  mpz_class msg;
  mpz_class ct; 
  mpz_class dec;
  struct RSA_PK pk;
  struct RSA_SK sk; 
  msg = mpz_class{"0x143214324234"};

  RSA_keygen(&pk, &sk, 2048);
  ct = encrypt(pk, msg);
  dec = RSA_decrypt_CRT(sk, ct); 

  if(cmp(msg, dec) == 0)
    cout<<"success"<<endl;
  else
    cout<<"faild"<<endl;



 
}

//wrapper 함수
mpz_class powm(mpz_class base, mpz_class exp, mpz_class mod){
  mpz_class r;
  mpz_powm(r.get_mpz_t(), base.get_mpz_t(), exp.get_mpz_t(), mod.get_mpz_t());
  return r;
}

mpz_class mod_inv(mpz_class x, mpz_class mod){
  mpz_class r;
  mpz_invert(r.get_mpz_t(), x.get_mpz_t(), mod.get_mpz_t());
  return r;
}

mpz_class urandom(gmp_randstate_t state, int bit){
  mpz_class r;
  mpz_urandomb(r.get_mpz_t(), state, bit);
  return r;
}

mpz_class nextprime(mpz_class x){
  mpz_class r;
  mpz_nextprime(r.get_mpz_t(), x.get_mpz_t());
  return r;
}

mpz_class lcm(mpz_class a, mpz_class b){
  mpz_class r;
  mpz_lcm(r.get_mpz_t(), a.get_mpz_t(), b.get_mpz_t());
  return r;
}

mpz_class gcd(mpz_class a, mpz_class b){
  mpz_class r;
  mpz_gcd(r.get_mpz_t(), a.get_mpz_t(), b.get_mpz_t());
  return r;
}

mpz_class mod(mpz_class x, mpz_class mod){
  mpz_class r;
  mpz_mod(r.get_mpz_t(),x.get_mpz_t(), mod.get_mpz_t());
  return r;
}

//본 함수
mpz_class random_prime(unsigned int bit){
  mpz_class r;
  mpz_class p;

  unsigned long seed;
  gmp_randstate_t state;

  seed = time(NULL);
  gmp_randinit_mt(state);
  gmp_randseed_ui(state, seed);

  r = urandom(state, bit);
  
  p =nextprime(r);

  if(mpz_sizeinbase(p.get_mpz_t(), 2) != bit)
    return random_prime(bit);
  else
    return p;
}

void RSA_keygen(RSA_PK *pk, RSA_SK *sk, int key_size){
  mpz_class ell;

  sk->p = random_prime(key_size/2);
  sk->q = random_prime(key_size/2);

  while(cmp(sk->p, sk->q) == 0)
    sk->p = random_prime(key_size/2);

  sk->N = sk->p * sk->q;
  pk->N = sk->N;

  ell = lcm(sk->p-1, sk->q-1);
  pk->e = 0x100001;

  if(gcd(ell, pk->e) != 1)
    // pk->e = nextprime(pk->e);
    return RSA_keygen(pk, sk, 2048);
  
  sk->e = pk->e;
  sk->d = mod_inv(pk->e, ell);
}

mpz_class encrypt(RSA_PK pk, mpz_class msg){
  mpz_class ct;
  ct = powm(msg, pk.e, pk.N);
  return ct;
}

mpz_class decrypt(RSA_SK sk, mpz_class ct){
  mpz_class msg;
  msg = powm(ct, sk.d, sk.N);
  return msg;
}

int cmp(mpz_class a, mpz_class b){
  int r;
  r = mpz_cmp(a.get_mpz_t(), b.get_mpz_t());
  return r;
}

mpz_class RSA_sign(RSA_SK sk, mpz_class msg){
  mpz_class sig;
  sig = powm(msg, sk.d, sk.N);
  return sig;
}

bool RSA_verify(RSA_PK pk, mpz_class sig, mpz_class msg){
  mpz_class r;
  r = powm(sig, pk.e, pk.N);
  if(cmp(r, msg) == 0)
    return true;
  else 
    return false;
}

mpz_class RSA_decrypt_CRT(RSA_SK sk, mpz_class ct){
  mpz_class m1, m2;
  mpz_class M1_inv, M2_inv;
  mpz_class msg; 
  m1 = powm(ct, sk.d, sk.p);
  m2 = powm(ct, sk.d, sk.q);
  M1_inv = mod_inv(sk.q, sk.p);
  M2_inv = mod_inv(sk.p, sk.q); 
  msg = (m1*sk.q*M1_inv);
  msg = mod(msg, sk.N);
  msg += (m2*sk.p*M2_inv);
  msg = mod(msg, sk.N);
  return msg;
}
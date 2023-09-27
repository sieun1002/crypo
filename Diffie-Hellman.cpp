#include <gmpxx.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

mpz_class powm(mpz_class base, mpz_class exp, mpz_class mod);
mpz_class urandomm(gmp_randstate_t state, const mpz_class n);
int cmp(mpz_class A, mpz_class B);

int main(){
  mpz_class xA;
  mpz_class xB;
  mpz_class pubA;
  mpz_class pubB;
  mpz_class KA;
  mpz_class KB;

  mpz_class p {"0xFFFFFFFFFFFFFFFFADF85458A2BB4A9AAFDC5620273D3CF1D8B9C583CE2D3695A9E13641146433FBCC939DCE249B3ER97D2FE363630C75D8F681B202"};
  mpz_class g = 2;

  ////////////
  /*Generating public keys*/
  /*Alice*/
  xA = urandomm(state, p);
  pubA = powm(g, xA, p);
  /*Bob*/
  xB = urandomm(state, p);
  pubB = powm(g, xB, p);

}

mpz_class powm(mpz_class base, mpz_class exp, mpz_class mod){
  mpz_class r;
  mpz_powm(r.get_mpz_t(), base.get_mpz_t(), exp.get_mpz_t(), mod.get_mpz_t());
  return r;
}

mpz_class urandomm(gmp_randstate_t state, const mpz_class n){
  mpz_class r;
  mpz_urandomm(r.get_mpz_t(), state, n.get_mpz_t());
  return r;
}

int cmp(mpz_class A, mpz_class B){
  int r;
  r = mpz_cmp(A.get_mpz_t(), B.get_mpz_t());
  return r;
}
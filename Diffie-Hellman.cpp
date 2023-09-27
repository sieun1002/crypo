#include <gmpxx.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

mpz_class powm(mpz_class base, mpz_class exp, mpz_class mod);
mpz_class urandomm(gmp_randstate_t state, const mpz_class n);
int cmp(mpz_class A, mpz_class B);

int main(){

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
#include <iostream>
#include <vector>
#include <gmpxx.h>


using namespace std;

struct Point{
  mpz_class x;
  mpz_class y;
};

struct EC{
  mpz_class a;
  mpz_class b;
  mpz_class p;
};

int main(){
  struct EC secp256k1;
  struct Point G;

  secp256k1 = {0, 7, mpz_class{"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC2F"}};
  G = {mpz_class{"0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798"},
      mpz_class{"0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8"}};
};

mpz_class mod_inv(mpz_class x, mpz_class mod){
  mpz_class r;
  if(mpz_invert(r.get_mpz_t(), x.get_mpz_t(), mod.get_mpz_t()) == 0)
    cout << "error: an inverse doesn't exist" << endl;
  return r;
}

int cmp(mpz_class x, mpz_class y){
  int r;
  r = mpz_cmp(x.get_mpz_t(), y.get_mpz_t());
  return r;
}

mpz_class mod(mpz_class n, mpz_class p){
  mpz_class r;
  mpz_mod(r.get_mpz_t(), n.get_mpz_t(), p.get_mpz_t()); // r = n mod p
  return r;
}

void EC_add(EC E, Point *R, Point P, Point Q){
  mpz_class r;
  mpz_class xtemp, ytemp;
  mpz_class Rx, Ry;

  if(P.x == 0 && P.y == 0){
    //R->x = Q.x;
    //R->y = Q.y;
    *R = Q;
  }

  else if(Q.x == 0 && Q.y == 0){
    //R->x = P.x;
    //R->y = P.y;
    *R = P;
  }

  else{
    xtemp = Q.x - P.x;
    xtemp = mod(xtemp, E.p);
    xtemp = mod_inv(xtemp, E.p);

    ytemp = Q.y - P.y;
    ytemp = mod(ytemp, E.p);
    
    r = xtemp * ytemp;
    r = mod(r, E.p);

    Rx = (r*r - P.x - Q.x);
    Rx = mod(Rx, E.p);
    R -> x = Rx;

    Ry = r * (P.x - Rx) - P.y;
    Ry = mod(Ry, E.p);
    R -> y = Ry;
  }
}

void EC_double(EC E, Point *R, Point P){
  mpz_class r;
  mpz_class xtemp, ytemp;
  mpz_class Rx, Ry;

  ytemp = 2 * P.y;
  ytemp = mod(ytemp, E.p);
  ytemp = mod_inv(ytemp, E.p);

  xtemp = (3 * P.x * P.x) + E.a;
  xtemp = mod(xtemp, E.p);

  r = xtemp * ytemp;
  r = mod(r, E.p);

  Rx = r*r - (2 * P.x);
  Rx = mod(Rx, E.p);
  R -> x = Rx;

  Ry = r*(P.x - Rx) - P.y;
  Ry = mod(Ry, E.p);
  R -> y = Ry;
}

void bits_inv(vector<int> *bits, int r){
  for(; r>0; r/=2){
    (*bits).push_back(r%2);
  }
}

void EC_mult(EC E, Point *R, Point P, int r){
  vector<int> bits;
  int n;

  bits_inv(&bits, r);
  n = bits.size();

  *R = P; //R -> x = P.x; R->y = P.y;

  for(; n>1; n--){
    EC_double(E, R, *R);
    if(bits.at(n-2) == 1)
      EC_add(E, R, *R, P);
  }
}
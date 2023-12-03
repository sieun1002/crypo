#include <iostream>
#include <vector>
#include <gmpxx.h>
#include <random>

using namespace std;

struct EC{
  mpz_class a;
  mpz_class b;
  mpz_class p;

};
struct Point{
  mpz_class x;
  mpz_class y;
};

struct ECDSA_PK{
  EC E;
  Point G;
  mpz_class n;
  Point Q;
};

struct ECDSA_SK{
  EC E;
  Point G;
  mpz_class n;
  Point Q;
  mpz_class d;
};

struct ECDSA_SIG{
  mpz_class S1;
  mpz_class S2;
};

void EC_add(EC E, Point *R, Point P, Point Q);
void EC_double(EC E, Point *R, Point P);
void EC_mult(EC E, Point *R, Point P, mpz_class r);
void bits_inv(vector<mpz_class> *bits, mpz_class r);

void ECDSA_keygen(ECDSA_PK *pk, ECDSA_SK *sk);
ECDSA_SIG ECDSA_sign(ECDSA_SK sk, mpz_class msg);
bool ECDSA_verify(ECDSA_PK pk, ECDSA_SIG sig, mpz_class msg);

mpz_class mod(mpz_class x, mpz_class mod);
mpz_class mod_inv(mpz_class x, mpz_class mod);
mpz_class urandomm(gmp_randstate_t state, mpz_class n);

int main(){
  struct ECDSA_PK pk;
  struct ECDSA_SK sk;
  struct ECDSA_SIG sig;
  mpz_class msg{"0x10"};
  bool rop;

  ECDSA_keygen(&pk, &sk);
  sig = ECDSA_sign(sk, msg);
  rop = ECDSA_verify(pk, sig, msg);

  if(rop==true)
    cout<<"true"<<endl;
  else if(rop==false)
    cout<<"false"<<endl;
  else 
    cout<<"error"<<endl;
  return 0;
}

mpz_class mod_inv(mpz_class x, mpz_class mod){
  mpz_class r;
  mpz_invert(r.get_mpz_t(), x.get_mpz_t(), mod.get_mpz_t());
  return r;
}

mpz_class urandomm(gmp_randstate_t state, mpz_class n){
  mpz_class r;
  mpz_urandomm(r.get_mpz_t(), state, n.get_mpz_t());
  return r;
}

mpz_class mod(mpz_class x, mpz_class n){
  mpz_class r;
  mpz_mod(r.get_mpz_t(), x.get_mpz_t(), n.get_mpz_t());
  return r;
}

void EC_add(EC E, Point *R, Point P, Point Q){
  if(P.x == 0 && P.y==0)
    *R=Q;
  else if(Q.x==0 && Q.y==0)
    *R=P;
  else if(P.x == Q.x && P.y == Q.y)
    EC_double(E, R, P);
  else{
    mpz_class r;
    r = mod_inv((Q.x - P.x), E.p);

    if(r==0){
      R->x = 0;
      R->y = 0;
    }else if(r<0){
      cout<<"eroor"<<endl;
    }else{
      r *= (Q.y - P.y);
      r = mod(r, E.p);

      R->x = r*r-P.x-Q.x;
      R->x = mod(R->x, E.p);

      R->y = r*(P.x-R->x)-P.y;
      R->y = mod(R->y, E.p);
    }
  }
}

void EC_double(EC E, Point *R, Point P){
  mpz_class r; 
  r = (2*P.y);
  r = mod_inv(r, E.p);

  if(r==0){
    R->x = 0;
    R->y = 0;
    cout<<"infinity"<<endl;
  }else if(r<0)
    cout<<"error"<<endl;
  else{
    r *=(3*P.x * P.x)+E.a;
    r = mod(r, E.p);

    R->x = r*r - 2*P.x;
    R->x = mod(R->x, E.p);

    R->y = r*(P.x-R->x)- P.y;
    R->y = mod(R->y, E.p);
  }
}

void bits_inv(vector<mpz_class> *bits, mpz_class r){
  for(; r>0; r/=2)
    (*bits).push_back(r%2);
}

void EC_mult(EC E, Point *R, Point P, mpz_class r){
  vector<mpz_class> bits;
  int n;

  if(P.x == 0 && P.y == 0){
    R->x = 0;
    R->y = 0;
    cout<<"infinity"<<endl;
  }else if(r<=0){
    cout<<"error"<<endl;
  }else{
    bits_inv(&bits, r);
    n = bits.size();

    *R = P;

    for(; n>1; n--){
      EC_double(E, R, *R);
      if(bits.at(n-2)==1){
        EC_add(E, R, *R, P);
      }
    }
  }
}

void ECDSA_keygen(ECDSA_PK *pk, ECDSA_SK *sk){
  unsigned long seed;
  random_device rd;
  gmp_randstate_t state;

  pk->E = {2, 2, 17};
  sk->E = pk->E;
  pk->G = {5, 1};
  sk->G = pk->G;
  pk->n = 19;
  sk->n = pk->n;

  // seed = time(NULL)
  seed = rd();
  gmp_randinit_mt(state);
  gmp_randseed_ui(state, seed);

  sk ->d = urandomm(state, sk->n);
  EC_mult(sk->E, &sk->Q, sk->G, sk->d);

  pk->Q = sk->Q;
}

ECDSA_SIG ECDSA_sign(ECDSA_SK sk, mpz_class msg){
  unsigned seed;
  gmp_randstate_t state;
  random_device rd;
  mpz_class k, inv_k;
  mpz_class h_m;
  Point R;
  int nBit, mBit;
  ECDSA_SIG sig;

  nBit = mpz_sizeinbase(sk.n.get_mpz_t(), 2);
  mBit = mpz_sizeinbase(msg.get_mpz_t(), 2);
  h_m  = msg>>max(mBit-nBit, 0);

  gmp_randinit_mt(state);

  do{
    // seed = time(NULL);
    seed = rd();
    gmp_randseed_ui(state, seed);
    k = urandomm(state, sk.n);
    EC_mult(sk.E, &R, sk.G, k);
  }while(mod(R.x, sk.n)==0);

  sig.S1 = mod(R.x, sk.n);

  inv_k = mod_inv(k, sk.n);

  if(inv_k == 0){
    cout<<"inv_k doesn't exist";
    ECDSA_sign(sk, msg);
  }

  sig.S2 = h_m + (sig.S1*sk.d);
  sig.S2 = mod(sig.S2, sk.n);
  sig.S2 *= inv_k;
  sig.S2 = mod(sig.S2, sk.n);

  if(mod(sig.S2, sk.n)==0 || mod_inv(sig.S2, sk.n)==0){
    cout<<"repeat SIGN"<<endl;
    return ECDSA_sign(sk, msg);
  }else 
    return sig;
}

bool ECDSA_verify(ECDSA_PK pk, ECDSA_SIG sig, mpz_class msg){
  Point R;
  mpz_class c;
  mpz_class u1, u2;
  mpz_class h_m;
  int nBit, mBit;

  nBit = mpz_sizeinbase(pk.n.get_mpz_t(), 2);
  mBit = mpz_sizeinbase(msg.get_mpz_t(), 2);
  h_m = msg>>max(mBit-nBit, 0);

  c = mod_inv(sig.S2, pk.n);
  u1 = h_m*c;
  u1 = mod(u1, pk.n);

  u2 = sig.S1 * c;
  u2 = mod(u2, pk.n);

  Point R1, R2;
  EC_mult(pk.E, &R1, pk.G, u1);
  EC_mult(pk.E, &R2, pk.Q, u2);
  EC_add(pk.E, &R, R1, R2);

  if(R.x==0 && R.y==0)
    return false;
  else{
    if(cmp(sig.S1, mod(R.x, pk.n)) == 0)
      return true;
    else 
      return false;
  }
  
}
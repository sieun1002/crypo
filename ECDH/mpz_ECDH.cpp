#include <vector>
#include <iostream>
#include <gmpxx.h>

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

void EC_add(EC E, Point *R, Point P, Point Q);
void EC_double(EC E, Point *R, Point P);
void bits_inv(vector<mpz_class> *bits, mpz_class r);
void EC_mult(EC E, Point *R, Point P, mpz_class r);
//wrapper 함수
mpz_class mod_inv(mpz_class n, mpz_class mod);
mpz_class mod(mpz_class x, mpz_class mod);
int cmp(mpz_class A, mpz_class B);
mpz_class urandomm(gmp_randstate_t state, const mpz_class n);

int main(){
  EC secp256k1;
  Point G;
  Point R;
  secp256k1 = {0, 7, mpz_class{"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F"}};
  G = {mpz_class{"0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798"},
      mpz_class{"0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8"}};

  mpz_class ck_A, ck_B;
  Point pk_A, pk_B;
  Point s_A, s_B;

  gmp_randstate_t state;
  gmp_randinit_mt(state);

  ck_A = urandomm(state, secp256k1.p);
  ck_B = urandomm(state, secp256k1.p);

  while(cmp(ck_A, ck_B) == 0)
    ck_B = urandomm(state, secp256k1.p);
  
  EC_mult(secp256k1, &pk_A, G, ck_A);
  EC_mult(secp256k1, &pk_B, G, ck_B);

  EC_mult(secp256k1, &s_A, pk_B, ck_A);
  EC_mult(secp256k1, &s_B, pk_A, ck_B);

  cout<<"ka = "<<ck_A<<endl;
  cout<<"kb = "<<ck_B<<endl;
  cout<<"cka = "<<pk_A.x<< ", "<<pk_A.y<<endl;
  cout<<"ckb = "<<pk_B.x<< ", "<<pk_B.y<<endl;
  cout<<"ska = "<<s_A.x<< ", "<<s_A.y<<endl;
  cout<<"skb = "<<s_B.x<< ", "<<s_B.y<<endl;

  if((cmp(s_A.x, s_B.x) == 0) && (cmp(s_A.y, s_B.y) == 0))
    cout<<"success"<<endl;
  else
    cout<<"faild"<<endl;


  //함수 test
  //Point A, B, C, D;
  // //A = 1G
  // A = {mpz_class{"0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798"},
  //     mpz_class{"0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8"}};

  // //B = 2G
  // B = {mpz_class{"0xC6047F9441ED7D6D3045406E95C07CD85C778E4B8CEF3CA7ABAC09B95C709EE5"},
  //     mpz_class{"0x1AE168FEA63DC339A3C58419466CEAEEF7F632653266D0E1236431A950CFE52A"}};

  // //C = 3G
  // C = {mpz_class{"0xF9308A019258C31049344F85F89D5229B531C845836F99B08601F113BCE036F9"},
  //     mpz_class{"0x388F7B0F632DE8140FE337E62A37F3566500A99934C2231B6CB9FD7584B8E672"}};

  // //D = 4G
  // D = {mpz_class{"0xE493DBF1C10D80F3581E4904930B1404CC6C13900EE0758474FA94ABE8C4CD13"},
  //     mpz_class{"0x51ED993EA0D455B75642E2098EA51448D967AE33BFBDFE40CFE97BDC47739922"}};
  
  // EC_add(secp256k1, &R, A, B);
  // EC_double(secp256k1, &R, B);
  // EC_mult(secp256k1, &R, A, 3);

  // if((cmp(R.x, D.x) == 0) && cmp(R.y, D.y) == 0)
  //   cout<<"success!"<<endl;
  // else 
  //   cout<<"failed!"<<endl;
  return 0;
}

//wrapper 함수
mpz_class mod_inv(mpz_class n, mpz_class mod){
  mpz_class r;
  mpz_invert(r.get_mpz_t(), n.get_mpz_t(), mod.get_mpz_t());
  return r;
}

mpz_class mod(mpz_class x, mpz_class mod){
  mpz_class r;
  mpz_mod(r.get_mpz_t(), x.get_mpz_t(), mod.get_mpz_t());
  return r;
}

int cmp(mpz_class A, mpz_class B){
  int r;
  r = mpz_cmp(A.get_mpz_t(), B.get_mpz_t());
  return r;
}

mpz_class urandomm(gmp_randstate_t state, const mpz_class n){
  mpz_class r;
  mpz_urandomm(r.get_mpz_t(), state, n.get_mpz_t());
  return r;
}

//본 함수 
void EC_add(EC E, Point *R, Point P, Point Q){

  if(P.x == 0 && P.y == 0){
    *R = Q;
  }else if(Q.x == 0 && Q.y == 0){
    *R = P;
  }else{
    mpz_class r;
    r = (Q.x - P.x);
    r = mod_inv(r, E.p);
    if(r == 0){
      R->x = 0;
      R->y = 0;
      cout<<"infinity"<<endl;
    }else if(r < 0){
      cout<<"error"<<endl;
    }else{
      r *= (Q.y - P.y);
      r = mod(r, E.p);

      R->x = r * r - P.x - Q.x;
      R->x = mod(R->x, E.p);

      R->y = r * (P.x - R->x) - P.y;
      R->y = mod(R->y, E.p);
    }
  }
}

void EC_double(EC E, Point *R, Point P){
  mpz_class r;
  r = (2 * P.y);
  r = mod_inv(r, E.p);

  if(r == 0){
    R->x = 0;
    R->y = 0;
    cout << "infinity"<<endl;
  }else if(r < 0){
    cout<<"error"<<endl;
  }else{
    r *= (3*P.x*P.x)+E.a;
    r = mod(r, E.p);

    R->x = r*r - 2*P.x;
    R->x = mod(R->x, E.p);

    R->y = r*(P.x-R->x) - P.y;
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
  }else if(r <= 0)
    cout<<"error"<<endl;
  else{
    bits_inv(&bits, r);
    n = bits.size();

    *R = P;

    for(; n>1; n--){
      EC_double(E, R, *R);
      if(bits.at(n-2) == 1){
        EC_add(E, R, *R, P);
      }
    }
  }
}


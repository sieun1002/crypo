#include <iostream>
#include <vector>

using namespace std;

struct EC{
  int a;
  int b;
  int p;
};

struct Point{
  int x;
  int y;
};

int mod_inv(int x, int mod);
void EC_add(EC E, Point *R, Point P, Point Q);
void EC_double(EC E, Point *R, Point P);
void bits_inv(vector<int> *bits, int r);
void EC_mult(EC E, Point *R, Point P, int r);

int main(){
  Point A, B;
  Point R;
  EC E;

  A = {1, 1};
  B = {3, 1};
  E.a = 2; E.b = 3; E.p = 5;

  cout << "A = (" << A.x << "," << A.y << ")" << endl;
  EC_mult(E, &R, A, 9);
  cout << "9A = " << "(" << R.x << "," << R.y << ")" << endl;


  // cout << "B = (" << B.x << "," << B.y << ")" << endl;

  // EC_add(E, &R, A, B);
  // cout << "A + B = " << "(" << R.x << "," << R.y << ")" << endl;

  // EC_double(E, &R, A);
  // cout << "2A = " << "(" << R.x << "," << R.y << ")" << endl;

  return 0;
};

int mod_inv(int x, int mod){
  int r;

  if(x == 0){
    return 0;
  }else if(mod < 0){
    return -1;
  }else{
    while(x<0){
      x += mod;
    }

    for(r=1; r<mod; r++){
      if((x*r) % mod == 1){
        return r;
      }
    }
    return -1;
  }
}

void EC_add(EC E, Point *R, Point P, Point Q){
  if(P.x==0 && P.y == 0){
    *R = Q;
  }else if(Q.x == 0 && Q.y == 0){
    *R = P;
  }else{
    int r;
    r = mod_inv((Q.x - P.x), E.p);

    if(r==0){
      R->x = 0;
      R->y = 0;
    }else if(r<0){
      cout << "error" << endl;
    }else{
      r *= (Q.y - P.y);
      r %= E.p;

      R->x = r*r - P.x - Q.x;
      while(R->x < 0){
        R->x += E.p;
      }
      R->x %= E.p;

      R->y = r*(P.x - R->x) - P.y;
      while(R->y < 0){
        R->y += E.p;
      }
      R->y %= E.p;
    }
  }
  
};

void EC_double(EC E, Point * R, Point P){
  int r;
  r = mod_inv((2*P.y), E.p);
  // r %= E.p; 어차피... mod_inv 값이 mod보다 작을거임. 

  if(r==0){
    R->x = 0;
    R->y = 0;
  }else if(r<0){
    cout<<"error"<<endl;
  }else{
    r *= 3*P.x*P.x + E.a;
    r %= E.p;

    R->x = r * r - 2*P.x;
    while(R->x <0){
      R->x += E.p;
    }
    R->x %= E.p;

    R->y = r*(P.x - R->x) - P.y;
    while(R->y <0){
      R->y += E.p;
    }
    R->y %= E.p;
  }
};

void bits_inv(vector<int> *bits, int r){
  for(; r>0; r/=2){
    (*bits).push_back(r % 2);
  }
}
void EC_mult(EC E, Point *R, Point P, int r){
  int n;
  vector<int> bits;

  if(P.x == 0 && P.y == 0){
    R->x = 0;
    R->y = 0;
    cout<<"Point of infinity"<<endl;
  }else if(r<=0){
    cout<<"error"<<endl;
  }else{
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
#include <iostream>

using namespace std;

struct Point{
  int x;
  int y;
};

struct EC{
  int a;
  int b;
  int p;
};

int mod_inv(int x, int mod);
void EC_add(EC E, Point *R, Point P, Point Q);
void EC_double(EC E, Point *R, Point P);

int main(){
  Point A, B;
  Point R;
  EC E;

  A = {1, 4};
  B = {3, 1};
  E.a = 2; E.b = 3; E.p = 5;

  cout << "A = (" << A.x << "," << A.y << ")" << endl;
  cout << "B = (" << B.x << "," << B.y << ")" << endl;
  
  EC_add(E, &R, A, B);
  cout <<"A+B = " << "(" << R.x << "," << R.y << ")" << endl;

  EC_double(E, &R, A);
  cout <<"2A = " << "(" << R.x << "," << R.y << ")" << endl;

  return 0;
}

int mod_inv(int x, int mod){
  int r;

  if (x == 0)
    return 0; //infinity
  else if(mod <= 0)
    return -1; //error
  
  else{
    while(x<0){
      x += mod;
    }

    for(r=0; r<mod; r++){
      if((r*x)%mod == 1)
        return r;
    }

    return -1; //no inverse exists
  }
}

void EC_add(EC E, Point *R, Point P, Point Q){
  int r;

  r=mod_inv((Q.x-P.x), E.p);

  if(r==0) /**infinity*/{
    R->x=0;
    R->y=0;
  }

  else if(r<0)
    cout<<"error"<<endl;

  else{
    r*=(Q.y-P.y);
    r%=E.p;

    R->x=(r*r-P.x-Q.x);
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

void EC_double(EC E, Point *R, Point P){
  int r;

  r = mod_inv(2*P.y, E.p);

  if(r==0) /*infinity*/{
    R->x = 0;
    R->y = 0;
  }
  else if(r<0)
    cout << "error" << endl;
  else{
    r *= (3*P.x*P.x + E.a);
    r %= E.p;

    R->x = r*r - 2*P.x;
    while(R->x < 0){
      R->x += E.p;
    }
    R->x %= E.p;

    R->y = r*(P.x - R->x) - P.y;
    while(R->y <0){
      R->y += E.p;
    }
    R->y %= E.p;
  }
}
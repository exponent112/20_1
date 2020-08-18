#include <stdio.h>

long long recursion(int n, int k);
long long DP(int n, int k,long long temp[n+1][k+1]){

  for(int i = 0;i<n+1;i++){
    for(int j = 0;j<k+1;j++){
      temp[i][j] = -1;
    }
  }
  for(int i = 0; i<n+1;i++){
    temp[i][0] = 1; //C(n,0)
  }
  for(int i = 0; i<k+1; i++){
    temp[0][i] = 0; //C(0,k)
    temp[i][i] = 1; //C(n,n)
  }
  for(int i = 0; i<n+1; i++){
    for(int j = 0; j<k+1; j++){
      if(temp[i][j]<0) temp[i][j] = temp[i-1][j-1] + temp[i-1][j];
    }
  }
  return temp[n][k];
}


int main(void) {
  long long result = 0;
  int n;
  int k;
  printf("n k?(ex>20 10) ");
  scanf("%d %d",&n,&k);
  long long temp[n+1][k+1];
  getchar();
  result = DP(n, k,temp);
  printf("DP result = %llu\n",result);
  result = recursion(n,k);
  printf("recursion result = %llu\n",result);
  return 0;
}


long long recursion(int n, int k){
  if(n>=0 && k == 0){
    return 1;
  }
  else if(n == 0 && k > 0){
    return 0;
  }
  else if(n == k) return 1;

  return recursion(n-1,k-1) + recursion(n-1,k);
}

#include "util.hpp"
#include <cstring>
#include <cstdio>
#include <iostream>

unsigned long long factorial(int n)
{
  unsigned long long fac[n+1];
  fac[0]=1;
  for (int i = 1; i <= n; ++i)
    fac[i]=i*fac[i-1];
  return fac[n];
}

void wc(char* buf){
  int n=strlen(buf);
  int words=0;
  int lines=0;
  int chars=0;
  char prev=' ';
  int empty=1;
 for(int i=0;i<n;i++){

  if(buf[i]!='\n' && buf[i]!=' ') {
     if(prev==' ' || prev=='\n')
            words++;
     if(empty){     
        lines++;
        empty=0;
     }  
     chars++;  
  }

  if(buf[i]=='\n') empty=1;

  prev=buf[i];
 } 
printf("lines:%d\nwords:%d\nchars:%d",lines,words,chars);
}

int atoi(char *buf, int radix){
 int ans=0;
 int n=strlen(buf);
 int prev=0;
   
 
for(int i=0;i<n;i++){
  ans*=radix; 
  ans+=buf[i]-48;
  if(ans<prev){
  // overflow

}

  prev=ans;
  
 }
return ans;
}

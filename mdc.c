#include<stdio.h>
#include<stdlib.h>

int main(int argc,char **argv)
{ 
  int a,b,r;
   char s[50];
   
   string s;
   

  if(argc!=3)
  {
    printf("erro...Parâmetros infusficientes...\n");
    exit(1);
  }

  a=atoi(argv[1]);
  b=atoi(argv[2]);
  
  
  //scanf("%d%d",&a,&b);
  while(b)
  {
    r=a%b;
    a=b;
    b=r;
    
  }
  
  printf("mdc %d",a);

    return(0);
}

#include <stdio.h>

int main()
{
  printf("hi\n");
  FILE *hndl;
  hndl = fopen("testfile.txt","r");
  char buf[32];
  fread(buf,31,1,hndl);
  printf("buf:%s\n",buf);
  fseek(hndl,0,SEEK_SET);
  return 0;
}

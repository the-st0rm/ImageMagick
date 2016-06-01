#include <stdio.h>

int main()
{
  printf("hi\n");
  FILE *hndl;
  hndl = fopen("/etc/issue","r");
  char buf[32];
  fread(buf,31,1,hndl);
  printf("buf:%s\n",buf);
  return 0;
}

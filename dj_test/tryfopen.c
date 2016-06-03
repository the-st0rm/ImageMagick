#include <stdio.h>

int main(int argc, char* argv[])
{

  printf("hi\n");
  FILE *hndl;
  hndl = fopen(argv[1],"r");
  printf("File is opened now\n");
  long int pos; 
  pos = ftell(hndl);
  printf("ftell result[%ld]\n", pos);
  char buf[32];
  fread(buf,31,1,hndl);

  printf("Data read from the file\tbuf:%s\n",buf);
  fseek(hndl,0,SEEK_SET);
  char buf2[10];
  fseek(hndl,0,SEEK_END);
  pos = ftell(hndl);
  printf("Final position: [%ld]\n", pos);
  //fread(buf2,9,0,hndl);
  //prtinf(
  return 0;
}

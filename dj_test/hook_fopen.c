#include <stdio.h>
#include <dlfcn.h>

#define _GNU_SOURCE

typedef FILE * (*orig_fopen_f_type)(const char *pathname, const char * mode);

FILE * fopen(const char * filename, const char * mode)
{
  printf("dj_fopen_hook: %s\n",filename);
  void *handle;
  FILE *fp;
  handle = dlopen("/lib/x86_64-linux-gnu/libc.so.6",RTLD_LOCAL|RTLD_LAZY);
  orig_fopen_f_type orig_fopen;
  orig_fopen = (orig_fopen_f_type)dlsym(handle,"fopen");
  fp = orig_fopen(filename,mode);
  char buf[8];
  fread(buf,7,1,fp);
  buf[7]=0;
  printf("dj_fopen_hook: snipped 8 bytes off: %s\n",buf);
  return fp;
}

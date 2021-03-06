#include <stdio.h>
#include <dlfcn.h>

#define _GNU_SOURCE
#define HEADER_SIZE 0x40

typedef FILE * (*orig_fopen_f_type)(const char *pathname, const char * mode);
typedef int (*orig_fseek_f_type)(FILE * stream, long int offset, int origin);
typedef long int (*orig_ftell_f_type)(FILE * stream);

FILE * fopen(const char * filename, const char * mode)
{
  printf("dj_fopen_hook: %s\n",filename);
  void *handle;
  FILE *fp;
  handle = dlopen("/lib/x86_64-linux-gnu/libc.so.6",RTLD_LOCAL|RTLD_LAZY);
  orig_fopen_f_type orig_fopen;
  orig_fopen = (orig_fopen_f_type)dlsym(handle,"fopen");
  fp = orig_fopen(filename,mode);
  char buf[HEADER_SIZE];
  fread(buf,HEADER_SIZE,1,fp);
  buf[HEADER_SIZE-1]=0;
  printf("dj_fopen_hook: snipped bytes off: [%s]\n",buf);
  return fp;
}

int fseek(FILE * stream, long int offset, int origin)
{
  printf("dj_fseek_hook: %d\n",offset);
  void *handle;
  FILE *fp;
  handle = dlopen("/lib/x86_64-linux-gnu/libc.so.6",RTLD_LOCAL|RTLD_LAZY);
  orig_fseek_f_type orig_fseek;
  orig_fseek = (orig_fseek_f_type)dlsym(handle,"fseek");
  if (origin == SEEK_SET)
  {
		fp = orig_fseek(stream, offset, origin+HEADER_SIZE);
  }
	else
		fp = orig_fseek(stream, offset, origin);

  //fp = orig_fseek(stream,offset,origin+HEADER_SIZE);
  return fp;
}

long int ftell(FILE * stream)
{
	long int ret_val;
	printf("inside ftell\n");
	void* handle;
	handle = dlopen("/lib/x86_64-linux-gnu/libc.so.6",RTLD_LOCAL|RTLD_LAZY);
	orig_ftell_f_type orig_ftell;
	orig_ftell = (orig_ftell_f_type)dlsym(handle,"ftell");
	ret_val = orig_ftell(stream);
	ret_val -= HEADER_SIZE;
	return ret_val;
}

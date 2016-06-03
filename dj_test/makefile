all:
	gcc -fPIC -shared -o hook_fopen.so hook_fopen.c -ldl
	gcc -o tryfopen tryfopen.c

clean:
	rm tryfopen hook_fopen.so

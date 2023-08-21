#Create lib_simpleloader.so from loader.c
loader  : loader.c 
	gcc -o fib ../test/fib.c
	gcc loader.c -o  loader
	./loader  fib
	rm loader
#Provide the command for cleanup

#Create lib_simpleloader.so from loader.c
loader: loader.c
	gcc -m32 -shared -o ../bin/lib_simpleloader.so -L../bin -fPIC loader.c -l_simpleloader
#Provide the command for cleanup
clean:
	-@rm -f ../bin/lib_simpleloader.so
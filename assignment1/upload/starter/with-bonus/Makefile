#invoke make inside following directories and in this order: loader, launch, fib
#move the lib_simpleloader.so and launch binaries inside bin directory
#Provide the command for cleanup
all: ./launcher/launch.c ./loader/loader.c ./test/*.c
	cd ./loader; make loader; cd ..; cd ./launcher; make launch; cd ..

execute: ./test/fib.c
	cd ./test; make fib; cd ..; ./bin/launch ../test/fib

clean: ./test/fib
	rm ./test/fib

clean_all:
	rm -r ./bin
	mkdir ./bin
	rm ./test/fib

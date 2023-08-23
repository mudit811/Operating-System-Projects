#Compile the launch.c by linking it with the lib_simpleloader.so
launch: launcher.c
	gcc -m32 -o ../bin/launch launch.c -L../bin -l_simpleloader
#Provide the command for cleanup
clean:
	-@rm -f ../bin/launch
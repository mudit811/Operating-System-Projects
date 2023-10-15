#Compile the launch.c by linking it with the lib_simpleloader.so
launch: launch.c
	gcc -m32 -o ../bin/launch launch.c -L/home/the_nice_guy/Codes/OS/assignments/assignment1/upload/starter/with-bonus/bin -l_simpleloader -Wl,-rpath,'../bin'
#Provide the command for cleanup
clean:
	-@rm -f ../bin/launch
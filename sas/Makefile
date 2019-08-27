
all:
	gcc -c main.c hello.c test_sas.c 
	#ar cr libsas.a test_sas.o
	#gcc main.o hello.o libsas.a -o main 
	gcc main.o hello.o test_sas.o -o main 

clean:
	rm *.o *.a

clean&make:
	make clean
	make all

all : 	ClientSocket.o ServerSocket.o 
	g++-7 -o Client.out ClientSocket.o  -Iinc/
	g++-7 -o Server.out ServerSocket.o  -Iinc/
	rm -f *.o

Server : ServerSocket.o 
	g++-7 -o Server.out ServerSocket.o  -Iinc/
	rm -f *.o

Client : ClientSocket.o 
	g++-7 -o Client.out ClientSocket.o  -Iinc/
	rm -f *.o

Client.o : ClientSocket.cpp 
	g++-7 -Wall -fpermissive -c ClientSocket.cpp 

Server.o : ServerSocket.cpp 
	g++-7 -Wall -fpermissive -c ServerSocket.cpp 

clean : 
	rm -f *.out *.o

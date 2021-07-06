CC=g++

.PHONY: clean, tcp_client, tcp_server, all

client:
<<<<<<< HEAD
	$(CC) tcp_client.cc ../wrap/wrap.c -I ../wrap/ -o client  
=======
	$(CC) tcp_client.cpp ../wrap/wrap.c -I ../wrap/ -o client  
>>>>>>> 5b000f584bd78ddd0ead5875138c8dcdf7343b14
server:
	$(CC) tcp_server.cc ../wrap/wrap.c -I ../wrap/  -o server 

all: client server	
	@echo "make client && make server"

clean:
	@echo "clean"
	rm ./server && rm ./client

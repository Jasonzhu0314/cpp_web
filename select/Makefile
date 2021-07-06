CC=g++

.PHONY: clean, tcp_client, tcp_server, all

client:
	$(CC) tcp_client.cc ../wrap/wrap.c -I ../wrap/ -o client  
server:
	$(CC) tcp_server.cc ../wrap/wrap.c -I ../wrap/  -o server 

all: client server	
	@echo "make client && make server"

clean:
	@echo "clean"
	rm ./server && rm ./client

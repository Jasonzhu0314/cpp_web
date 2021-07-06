CC=g++

.PHONY: clean, all

client:
	$(CC) ./tcp_client.cc ../wrap/wrap.c -o ./client -I ../wrap/ 
server:
	$(CC) ./tcp_server.cc ../wrap/wrap.c -o ./server -I ../wrap/ 

all: client server	
	@echo "make client && make server"

clean:
	@echo "clean"
	rm ./server
	rm ./client

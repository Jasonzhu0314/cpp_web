CC=g++

.PHONY: clean, all


client:
	$(CC) ./tcp_client.cc ../warp.c -o ./client -I ./ -lpthread --std=c++11
server:
	$(CC) ./tcp_server.cc ../warp.c -o ./server -I ./ -lpthread --std=c++11 

all: client server	
	@echo "make client && make server"

clean:
	@echo "clean"
	rm ./server
	rm ./client

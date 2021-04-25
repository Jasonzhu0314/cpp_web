CC=g++

.PHONY: clean, tcp_client, tcp_server, all

threadpools:
	$(CC) -c ./thread_pools.cc -o ./thread_pools.o -I ./ -lpthread --std=c++11
client:
	$(CC) ./tcp_client.cc -I ./ -o ./client --std=c++11
server: threadpools
	$(CC) ./tcp_server.cc ./thread_pools.o  -I ./ -o ./server -lpthread --std=c++11

all: client server	
	@echo "make client && make server"

clean:
	@echo "clean"
	rm ./server && rm ./client && rm ./thread_pools.o

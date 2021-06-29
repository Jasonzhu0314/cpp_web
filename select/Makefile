CC=g++

.PHONY: clean, tcp_client, tcp_server, all

client:
	$(CC) ./tcp_client.cc -I ./ -o ./client --std=c++11
server:
	$(CC) ./tcp_server.cc -I ./ -o ./server -lpthread --std=c++11

all: client server	
	@echo "make client && make server"

clean:
	@echo "clean"
	rm ./server && rm ./client

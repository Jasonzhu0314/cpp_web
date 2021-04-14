CC := g++
CLIENT := ./src/main_client.cc
SERVER := ./src/main_server.cc

.PHONY: clean, tcp_client, tcp_server, all

tcp_client:
	$(CC) -c ./src/tcp_client.cc -o ./bin/tcp_client.o -I ./include  --std=c++11
client: tcp_client
	$(CC) $(CLIENT) ./bin/tcp_client.o -I ./include -o ./bin/client --std=c++11

server: 
	$(CC) $(SERVER) ./src/tcp_server.cc -I ./include -o ./bin/server -lpthread --std=c++11

all: client server	
	@echo "make client && make server"

clean:
	@echo "clean"
	rm ./bin/*

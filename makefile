CC=g++
SRC=$(type)
THREAD=$(thread)
CLIENT=./main_client.cc
SERVER=./main_server.cc

.PHONY: clean, tcp_client, tcp_server, all

tcp_client:
	$(CC) -c $(SRC)/tcp_client.cc -o ./bin/tcp_client.o -I ./include $(THREAD) --std=c++11
client: tcp_client
	$(CC) $(CLIENT) ./bin/tcp_client.o -I ./include -o ./bin/client $(THREAD) --std=c++11
tcp_server:
	$(CC) -c $(SRC)/tcp_server.cc -o ./bin/tcp_server.o -I ./include $(THREAD) --std=c++11
server: tcp_server
	$(CC) $(SERVER) ./bin/tcp_server.o -I ./include -o ./bin/server $(THREAD) --std=c++11

all: client server	
	@echo "make client && make server"

clean:
	@echo "clean"
	rm ./bin/*

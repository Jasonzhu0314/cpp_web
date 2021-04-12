.PHONY: clean
tcp_client:
	g++ -c ./src/tcp_client.cc -o ./bin/tcp_client.o -I ./include  --std=c++11
client: tcp_client
	g++ ./src/main_client.cc ./bin/tcp_client.o -I ./include -o ./bin/client --std=c++11
server: 
	g++ ./src/main_server.cc ./src/tcp_server.cc -I ./include -o ./bin/server --std=c++11
all: client server	
	@echo "make client && make server"
clean:
	@echo "clean"
	rm ./bin/*

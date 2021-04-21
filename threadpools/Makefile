CC=g++
SRC=$(type)
THREAD=$(thread)


.PHONY: clean, tcp_client, tcp_server, all

threadpools:
	$(CC) -c ./threadpools.cc -o ./threadpools.o -I ./ $(THREAD) --std=c++11
client:
	$(CC) ./tcp_client.cc -I ./ -o ./client --std=c++11
server: threadpools
	$(CC) ./tcp_server.cc ./threadpools.o  -I ./ -o ./server $(THREAD) --std=c++11

all: client server	
	@echo "make client && make server"

clean:
	@echo "clean"
	rm ./server && rm ./client

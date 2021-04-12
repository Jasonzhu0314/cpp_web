
client:
	g++ client.cc -o client.o

all: client
	g++ server.cc -o server.o

clean:
	rm -f server.o
	rm -f client.o
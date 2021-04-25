#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "thread_pools.h"
 
#define MAXSIZE 1024

void processClient(int);

class tcp_server
{
private:
    int socket_fd,accept_fd;
    sockaddr_in myserver;
    sockaddr_in remote_addr;
    // 创建线程池和线程池中的线程数
	ThreadPool* thread_pools;

public:
    tcp_server(int listen_port, int num);
    ~tcp_server();
    int recvMessage();
};
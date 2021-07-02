#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
 
#define MAXSIZE 1024
 
class tcp_server
{
private:
    int socket_fd,accept_fd;
    sockaddr_in myserver;
    sockaddr_in remote_addr;
 
public:
    tcp_server(int listen_port);
    void process_client(int accept_fd);
    int recv_msg();
};
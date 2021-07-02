#include <unistd.h>  
#include <iostream>  
#include <sys/socket.h>  
#include <arpa/inet.h>  
#include <errno.h>
#define MAX_NUM 1024  
  
class tcp_client  
{  
private:  
	int socket_fd;  
	char message[MAX_NUM] = {0};  
	struct sockaddr_in server_addr;  
  
public:  
    tcp_client(char* server_ip,char* server_port);  
};

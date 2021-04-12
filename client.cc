#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

#include <sys/types.h>
#include <errno.h>
#include <netdb.h>
using namespace std;

#define IP "127.0.0.1"//这个一定要用自己服务器的IP
#define port 8001 //监听端口，可以在范围内自由设定
#define message "HTTP server is ready!"//返回给客户机的信息
const int MAX_LINE = 1024;

int main() {
    int local_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (local_fd == -1) {
        cout << "socket error" << endl;
        exit(-1);
    }
    cout << "socket begin" << endl;

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, IP, &server_addr.sin_addr) == -1) {
        cout << "ip addr error" << endl;
        exit(-1);
    }

    if (connect(local_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        cout << "connect failed" << endl;
        exit(-1);
    }
    cout << "connect sucess" << endl;


    char sendline[MAX_LINE], recvline[MAX_LINE];
    while(fgets(sendline , MAX_LINE , stdin) != NULL)	
	{
		write(local_fd , sendline , strlen(sendline));

		if(read(local_fd , recvline , MAX_LINE) == 0)
		{
			perror("server terminated prematurely");
			exit(1);
		}//if

		if(fputs(recvline , stdout) == EOF)
		{
			perror("fputs error");
			exit(1);
		}//if
	}//while
    close(local_fd);

    return 0;
}


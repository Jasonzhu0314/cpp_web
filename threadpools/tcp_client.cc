#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tcp_client.h"  

tcp_client::tcp_client(char* server_ip, char* server_port)  
{  
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        //指定期望的通信协议类型，返回的文件描述符和套接字描述符类似，我们成为套接字描述符，简称sockfd
        printf("create socket error: %s(errno:%d)\n)",strerror(errno),errno);  
        exit(0);  
    }  
    printf("client socket fd: %d\n", socket_fd);

    memset(&server_addr, 0, sizeof(server_addr));  
    server_addr.sin_family = AF_INET;  
    server_addr.sin_port = htons(atoi(server_port));  

    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <=0 ) {  
        printf("inet_pton error for %s\n",server_ip);  
        exit(0);  
    }  

    if (connect(socket_fd, (struct sockaddr* ) &server_addr, sizeof(server_addr) ) < 0) {  
        // 创建ip和socket套接字的连接
        printf("connect error: %s(errno: %d)\n", strerror(errno), errno);  
        exit(0);  
    }  

    printf("send message to server: \n");  
    fgets(message, 4096, stdin);  
    // 发送数据
    if (send(socket_fd, message, strlen(message), 0) < 0) {  
        printf("send message error\n");  
        exit(0);  
    }  

    char buffer[1024];
	memset(buffer, 0, 1024);
    read(socket_fd, buffer, 1024);
    printf("%s\n", buffer);

    close(socket_fd);  
    exit(0);
}  

int main(int argc,char* argv[])  
{  
    tcp_client tc(argv[1],argv[2]);  
    return 0;  
}
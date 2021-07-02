#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tcp_client.h"  
#include "../wrap.h"

tcp_client::tcp_client(char* server_ip, char* server_port)  
{  
    socket_fd = Socket(AF_INET, SOCK_STREAM, 0);
    //指定通信协议，返回的文件描述符和套接字描述符类似，简称sockfd
    bzero(&server_addr, 0);  
    server_addr.sin_family = AF_INET;  
    server_addr.sin_port = htons(atoi(server_port));  

    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <=0 ) {  
        perr_exit("inet_pton");
    }  

    Connect(socket_fd, (struct sockaddr* ) &server_addr, sizeof(server_addr));
    // 创建ip和socket套接字的连接
    int n;
    while (1) {
        printf("send message to server: \n"); 
        n = Read(STDIN_FILENO, message, MAX_NUM);
        if (!strncmp(message, "quit", 4)) break;        
        // 发送数据
        Write(socket_fd, message, n);
        n = Read(socket_fd, message, MAX_NUM);
        Write(1, message, n);
    }
    close(socket_fd);  
}  

int main(int argc,char* argv[])  
{  
    tcp_client tc(argv[1],argv[2]);  
    return 0;  
}



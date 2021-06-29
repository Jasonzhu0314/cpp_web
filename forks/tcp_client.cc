#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tcp_client.h"  

#define PRR_EXIT(msg) { \
	perror(msg); \
	exit(1); \
}

tcp_client::tcp_client(char* server_ip, char* server_port)  
{  
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        //指定期望的通信协议类型，返回的文件描述符和套接字描述符类似，我们成为套接字描述符，简称sockfd
        printf("create socket error: %s(errno:%d)\n)",strerror(errno),errno);  
        exit(0);  
    }  
    printf("client socket fd: %d\n", socket_fd);

    bzero(&server_addr, 0);  
    server_addr.sin_family = AF_INET;  
    server_addr.sin_port = htons(atoi(server_port));  

    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <=0 ) {  
        PRR_EXIT("inet_pton")
    }  

    if (connect(socket_fd, (struct sockaddr* ) &server_addr, sizeof(server_addr) ) < 0) {  
        // 创建ip和socket套接字的连接
        PRR_EXIT("connect")
    }  
    int n;
    //while(1) {
        printf("send message to server: \n"); 
        bzero(message, MAX_NUM); 
        if ( (n = read(STDIN_FILENO, message, MAX_NUM)) < 0) {
            PRR_EXIT("read");
        }
        //if ( !strncmp(message, "quit", 4)) break;        
        // // 发送数据
        if (write(socket_fd, message, n) < 0) {
            PRR_EXIT("send message error\n");
        } 

        //bzero(message, sizeof(message));
        //if ( (n = read(socket_fd, message, MAX_NUM) ) < 0)
        //    PRR_EXIT("read from server")
        //write(1, message, n);
    //}

    close(socket_fd);  
}  

int main(int argc,char* argv[])  
{  
    tcp_client tc(argv[1],argv[2]);  
    return 0;  
}



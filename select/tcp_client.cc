#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tcp_client.h"  
#include "wrap.h"

tcp_client::tcp_client(char* server_ip, char* server_port)  
{  
    socket_fd = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // 指定期望的通信协议类型，返回的文件描述符和套接字描述符类似，我们成为套接字描述符，简称sockfd
    memset(&server_addr, 0, sizeof(server_addr));  
    server_addr.sin_family = AF_INET;  
    server_addr.sin_port = htons(atoi(server_port));  

    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <=0 ) {  
        perr_exit("inet_pton");
    }  

    Connect(socket_fd, (struct sockaddr* ) &server_addr, sizeof(server_addr) );
    // 创建ip和socket套接字的连接
    int n;
    while (1) {
        printf("send message to server: \n");
        n = Read(0, message, MAXSIZE);
        // 发送数据
        Write(socket_fd, message, n);
        if (!strncmp(message, "quit", 4)) {
           break; 
        }
        n = Read(socket_fd, message, MAXSIZE);
        Write(STDOUT_FILENO, message, n);
    }
    close(socket_fd);  
    // TODO: shutdown函数能只关闭读缓存或写缓存中的一个
    //    if ((n = read(socket_fd, message, n))) {
    //        PRR_EXIT("fin read from server");
    //    }
    //    if (n == 0) {
    //        write(1, "server fin", 10);
    //        //break;
    //    }
    //    if ((n = write(STDOUT_FILENO, message, n)) < 0) {
    //        PRR_EXIT("write to stdout");
    //    } 
}  

int main(int argc,char* argv[])  
{  
    tcp_client tc(argv[1],argv[2]);  
    return 0;  
}
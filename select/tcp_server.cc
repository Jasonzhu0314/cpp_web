#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "tcp_server.h"
#include "wrap.h"

tcp_server::tcp_server(int listen_port) {
    socket_fd = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	memset(&myserver, 0, sizeof(myserver));
	myserver.sin_family = AF_INET;
	//myserver.sin_addr.s_addr = htonl(INADDR_ANY);
	inet_aton("127.0.0.1", &myserver.sin_addr);
	myserver.sin_port = htons(listen_port);

	// 本地ip绑定套接口
	Bind(socket_fd, (sockaddr*) &myserver, sizeof(myserver));
	//开始监听
	Listen(socket_fd, 128); 
	
	socklen_t sin_size = sizeof(struct sockaddr_in);
	int maxfd = socket_fd;
	int maxi = -1;
	int current_fd_num = 0;
	int nready, n, i;
	char buf[MAXSIZE];

	int client_fd_set[FD_SETSIZE];
	for (i = 0; i < FD_SETSIZE; i++) {
		client_fd_set[i] = -1;
	}

	fd_set read_set, all_set;
	FD_ZERO(&all_set);
	FD_SET(socket_fd, &all_set);

	while (1) {
		read_set = all_set;
		nready = select(maxfd + 1, &read_set, NULL, NULL, NULL);
		// 初始为0,1,2,3 共maxfd + 1个，新创建的文件描述符一定是编号中最小的，会阻塞等待网卡产生信号
		if (nready < 0)  {
			break;
		}
		if (FD_ISSET(socket_fd, &read_set)) { // 判断监听socket口是否有变化
			accept_fd = Accept(socket_fd, (struct sockaddr*) &remote_addr, &sin_size);
			printf("Received a connection from %s:%d\n",
					(char*) inet_ntoa(remote_addr.sin_addr),
					ntohs(remote_addr.sin_port));
			
			for (i = 0; i < FD_SETSIZE; i++) { //可优化
				if (client_fd_set[i] < 0) {  // 将创建的socket放到clien_fd_set队列中
					printf("new connect[%d]:%d\n", i, accept_fd);
					client_fd_set[i] = accept_fd;
					current_fd_num = i;
					break;
				}
			}

			if (current_fd_num == FD_SETSIZE) {
				fputs("too many clients\n",stderr);
			}

			FD_SET(accept_fd, &all_set);

			if (accept_fd > maxfd) maxfd = accept_fd;
			if (current_fd_num > maxi) maxi = current_fd_num;
			if (--nready == 0) continue;
		}
		else {
			for (i = 0; i < FD_SETSIZE; i++) {
				accept_fd = client_fd_set[i];
				if (accept_fd < 0) continue;

				if (FD_ISSET(accept_fd, &read_set)) {
					n = Read(accept_fd, buf, MAXSIZE);
					if (n == 0 || !strncmp(buf, "quit", 4)) {
						close(accept_fd);
						FD_CLR(accept_fd, &all_set);
						client_fd_set[i] = -1;
						printf("closed client[%d]:%d \n", i, accept_fd);
					    if (--nready==0) break;
						continue;
					} else if (n > 0) {
						Write(1, buf, n);
						for(i = 0; i < n; i++) {
							buf[i] = toupper(buf[i]);
						}
						Write(accept_fd, buf, n);
					}
					if (--nready==0) break;
				}
			}
		}
	}

	close(socket_fd);
}

int main(int argc,char* argv[])
{
	tcp_server ts(atoi(argv[1]));
	return 0;
}
#include <unistd.h>
#include <string.h>
#include <cstdio>
#include <stdlib.h>
#include <thread>
#include <sys/wait.h>
#include <sys/types.h>
#include "tcp_server.h"
#include "wrap.h"

#define MAX_CONNECT_NUMS 4

tcp_server::tcp_server(int listen_port) {
	socket_fd = Socket(AF_INET, SOCK_STREAM, 0);

	memset(&myserver, 0, sizeof(myserver));
	myserver.sin_family = AF_INET;
	myserver.sin_addr.s_addr = htonl(INADDR_ANY);
	myserver.sin_port = htons(listen_port);

	Bind(socket_fd, (struct sockaddr* ) &myserver, sizeof(myserver));
	Listen(socket_fd, MAX_CONNECT_NUMS);
}
 
int tcp_server::recv_msg() {
	while (1) {
		socklen_t sin_size = sizeof(struct sockaddr_in);
		accept_fd = Accept(socket_fd, (struct sockaddr*) &remote_addr, &sin_size);
		printf("Received a connection from %s:port %d\n", \
				inet_ntoa(remote_addr.sin_addr), htons(remote_addr.sin_port));

		pid_t pid = fork();
		if (pid < 0)  perr_exit("fork");
		if (pid > 0) {
			close(accept_fd); // 父进程不需要客户端文件描述符
			while(waitpid(-1, NULL, WNOHANG) > 0); // 非阻塞收尸，防止僵尸进程
			continue;
		}
		// 创建子进程处理客户端连接，否则，多个客户端连接时，同一时间，只能响应一个客户端
		close(socket_fd);
		tcp_server::process_client(accept_fd);
		break;
	}
	return 0;
}

void tcp_server::process_client(int accept_fd) {
	int n, i;
	char buf[MAXSIZE] = {0};
	while(1) {
		n = Read(accept_fd, buf, MAXSIZE);
		if (n == 0 || !strncmp(buf, "quit", 4)) {
			close(accept_fd);
			break;
		}
		Write(STDOUT_FILENO, buf, n);
		for(i = 0; i < n; i++) {
			buf[i] = toupper(buf[i]);
		}
		Write(accept_fd, buf, n);
	}
	// TODO:不超过长度的时候，有多少读多少，当超过最大的MAXSIZE的时候处理机制
	printf("server end connect, fd:%u\n", accept_fd);
	close(accept_fd);
}


int main(int argc,char* argv[])
{
	tcp_server ts(atoi(argv[1]));
	ts.recv_msg();
	return 0;
}


#include <string.h>
#include <cstdio>
#include <stdlib.h>
#include <thread>
#include "tcp_server.h"

tcp_server::tcp_server(int listen_port) {
    if (( socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0 ) {
		throw "socket() failed";
    }
	printf("server socket fd: %d\n", socket_fd);

	memset(&myserver, 0, sizeof(myserver));
	myserver.sin_family = AF_INET;
	myserver.sin_addr.s_addr = htonl(INADDR_ANY);
	myserver.sin_port = htons(listen_port);

	if (bind(socket_fd, (sockaddr*) &myserver, sizeof(myserver)) < 0 ) {
		// 本地ip绑定套接口
		throw "bind() failed";
	}

	if( listen(socket_fd, 10) < 0 ) {
		//开始监听
		throw "listen() failed";
	}
}
 
int tcp_server::recv_msg() {
 
	while (1) {
		socklen_t sin_size = sizeof(struct sockaddr_in);
		if(( accept_fd = accept(socket_fd, (struct sockaddr*) &remote_addr, &sin_size)) == -1 )
		{
			throw "Accept error!";
			continue;
		}
		printf("Received a connection from %s\n",(char*) inet_ntoa(remote_addr.sin_addr));
		printf("create a thead for process a client\n");

		// 创建子线程用户处理客户端发来的连接，否则，多个客户端连接时，同一时间，只能响应一个客户端
		std::thread t(&tcp_server::process_client, this, std::ref(accept_fd));
		t.detach(); // 与主线程分离
	}
	return 0;
}


void tcp_server::process_client(int accept_fd) {
	char buffer[MAXSIZE];
	memset(buffer, 0, MAXSIZE);
	// 如果没有数据，则会一直等待下去，不超过长度的时候，有多少读多少，所以需要配合while使用，当超过最大的MAXSIZE的时候就会有问题
	if( (read(accept_fd, buffer, MAXSIZE)) < 0 ) {
		throw("Read() error!");
	} else {
		printf("Received message: %s\n", buffer);
		// html 界面
		std::string first_line = "HTTP/1.0 200 OK\n";
		std::string body = "<html><div><h1>hello world</h1></div></html\n";
		std::string header = "Content-Type: text/html\ncharset: gbk\nContent-Length:" 
							+ std::to_string(body.size())+"\n\n";
		std::string resp = first_line + header + body;
		
		for (int i = 0; i < resp.size(); i++) {
			buffer[i] = resp[i];
		}

		write(accept_fd, buffer, sizeof((buffer)));
	}
}
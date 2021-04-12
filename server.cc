#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

#define IP "127.0.0.1"//这个一定要用自己服务器的IP
#define port 8001 //监听端口，可以在范围内自由设定
#define message "HTTP server is ready!"//返回给客户机的信息

using namespace std;

int main()
{  
    int local_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (local_socket == -1) {
        cout << "socket error" << endl;
        exit(-1);
    }
    cout << "socket ready" << endl;
    struct sockaddr_in local_addr;
    //bzero(&local_addr, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = inet_addr(IP);
    local_addr.sin_port = htons(port);

    int res = bind(local_socket, (struct sockaddr *)&local_addr, sizeof(local_addr));
    cout << res << endl;
    if (res == -1) {
        cout << "bind error" << endl;
        exit(-1);
    }
    cout << "bind success" << endl;

    listen(local_socket, 10);
    cout << "waiting for client connect ......" << endl;

    while (true) {
        struct sockaddr_in client_addr;
        unsigned int len = sizeof(client_addr);
        int client_sock = accept(local_socket, (struct sockaddr *) &client_addr, &len);
        if (client_sock == -1) {
            cout << "accept 错误" << endl;
            exit(-1);
        }

        char *ip = inet_ntoa(client_addr.sin_addr);
        cout << "client: " << ip << "\nconnet sercer success" << endl;


        char buff[1024] = {0};
        int size = read(client_sock, buff, sizeof(buff));
        cout << "Request information:\n"
			 << buff << endl;
		cout << size << " bytes" << endl;

        //write(client_sock, message, sizeof(message));

        //close(client_sock);

    }
    close(local_socket);
    return 0;
}
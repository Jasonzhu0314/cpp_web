#include <algorithm>
#include <iostream>
#include <tcp_server.h>
 
int main(int argc,char* argv[])
{
	tcp_server ts(atoi(argv[1]));
	ts.recv_msg();
	return 0;
}

//./main 9527
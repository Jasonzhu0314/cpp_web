#include <iostream>  
#include <tcp_client.h>  

using namespace std;  
  
int main(int argc,char* argv[])  
{  
    tcp_client tc(argv[1],argv[2]);  
    return 0;  
}

//./main 192.168.99.131 9527 
# cpp_web
c++服务器开发

实现一个简单的socket接口编程，服务端向客户端返回网页，使用套接口中常用的函数进行编程开发，提高自己对多线程编程和服务器开发的认识，以及如何使用多路复用技术提高程序的性能

## 项目目录
```
.
├── Makefile       编译文件
├── README.md      Readme
├── threadpools
│   ├── Makefile   线程池Makefile
│   ├── tcp_client.cc   客户端
│   ├── tcp_server.cc   服务端
│   ├── threadpools.cc  线程池类
│   └── threadpools.h
└── threads
    ├── Makefile   多线程Makefile
    ├── tcp_client.cc   客户端
    └── tcp_server.cc   服务端

```
## socket基础知识

### 字节序

网络字节序：在网络中传输时使用的字节序，大端存储

主机字节序：在主机存储时使用的字节序，小端存储

借助字节序转换函数

```c
unit32_t htonl (unit32_t hostlong);		32位无符号整型，主机字节序到网络字节序
unit16_t htons (unit16_t hostshort);		16位无符号整型，端口，主机字节序 ---> 网路字节序	 
unit32_t ntohl (unit32_t netlong);		32， 网络字节序 ---> 主机字节序
unit16_t ntohs (unit16_t netshort);		16，端口， 网络字节序 ---> 主机字节序
说明：h -----host；n----network ；s------short；l----long。
```

### IP地址结构

使用`man 7 ip`查看地址结构相关的参数，由于结构在很早之前就定义了，所以在使用时需要强转

```c
struct sockaddr_in {
    sa_family_t    sin_family; /* address family: AF_INET */
    // 下面的14个字节，对应sockaddr中的char
    in_port_t      sin_port;   /* port in network byte order */
    struct in_addr sin_addr;   /* internet address */
    // 还有剩余八个字节一般不用，会自动填充
};

/* Internet address. */
struct in_addr {
    uint32_t       s_addr;     /* address in network byte order */
};


int bind(int sockfd, const struct sockaddr *addr,
                socklen_t addrlen);
struct sockaddr {
    sa_family_t sa_family;
    char        sa_data[14];
}

```

### 字符串转网络字节序

通常情况下，普遍使用如`192.168.1.0`点分形式的网路地址，所以需要使用函数将字符串转换为网络字节序的格式，有传统的函数，也有根据IPv6一起出现的新型网路地址转换函数。

传统函数

- inet_aton和inet_addr  把ip地址转化为用于网络传输的二进制数值

```c
1.把ip地址转化为用于网络传输的二进制数值
int inet_aton(const char *cp, struct in_addr *inp);
```

inet_aton() 转换网络主机地址ip(如192.168.1.10)为二进制数值，并存储在struct in_addr结构中，即第二个参数*inp，函数返回非0表示cp主机有地有效，返回0表示主机地址无效。（这个转换完后不能用于网络传输，还需要调用htons或htonl函数才能将主机字节顺序转化为网络字节顺序）

```c
in_addr_t inet_addr(const char *cp);
```

inet_addr函数转换网络主机地址（如192.168.1.10)为网络字节序二进制值，如果参数char *cp无效，函数返回-1(INADDR_NONE),这个函数在处理地址为255.255.255.255时也返回－1,255.255.255.255是一个有效的地址，不过inet_addr无法处理;

- inet_ntoa 将网络传输的二进制数值转化为成点分十进制的ip地址

```c
char *inet_ntoa(struct in_addr in);
inet_ntoa 函数转换网络字节排序的地址为标准的ASCII以点分开的地址,该函数返回指向点分开的字符串地址（如192.168.1.10)的指针，该字符串的空间为静态分配的，这意味着在第二次调用该函数时，上一次调用将会被重写（复盖），所以如果需要保存该串最后复制出来自己管理！
```

新型网路地址转换函数

- inet_pton和inet_ntop

```c
//这两个函数是随IPv6出现的函数，对于IPv4地址和IPv6地址都适用，函数中p和n分别代表表达（presentation)和数值（numeric)。地址的表达格式通常是ASCII字符串，数值格式则是存放到套接字地址结构的二进制值。
#include <arpe/inet.h>
int inet_pton(int family, const char *strptr, void *addrptr);     //将点分十进制的ip地址转化为用于网络传输的数值格式
返回值：若成功则为1，若输入不是有效的表达式则为0，若出错则为-1
const char * inet_ntop(int family, const void *addrptr, char *strptr, size_t len);     //将数值格式转化为点分十进制的ip地址格式
返回值：若成功则为指向结构的指针，若出错则为NULL
```

（1）这两个函数的family参数既可以是AF_INET（ipv4）也可以是AF_INET6（ipv6）。如果，以不被支持的地址族作为family参数，这两个函数都返回一个错误，并将errno置为EAFNOSUPPORT.
（2）第一个函数尝试转换由strptr指针所指向的字符串，并通过addrptr指针存放二进制结果，若成功则返回值为1，否则如果所指定的family而言输入字符串不是有效的表达式格式，那么返回值为0.
（3）inet_ntop进行相反的转换，从数值格式（addrptr）转换到表达式（strptr)。inet_ntop函数的strptr参数不可以是一个空指针。调用者必须为目标存储单元分配内存并指定其大小，调用成功时，这个指针就是该函数的返回值。len参数是目标存储单元的大小，以免该函数溢出其调用者的缓冲区。如果len太小，不足以容纳表达式结果，那么返回一个空指针，并置为errno为ENOSPC。

**将sin_addr设置为INADDR_ANY"的含义是什么?**

转换过来就是0.0.0.0，泛指本机的意思，也就是表示本机的所有IP，因为有些机子不止一块网卡，多网卡的情况下，这个就表示所有网卡ip地址的意思。 比如一台电脑有3块网卡，分别连接三个网络，那么这台电脑就有3个ip地址了，如果某个应用程序需要监听某个端口，那他要监听哪个网卡地址的端口呢？ 如果绑定某个具体的ip地址，你只能监听你所设置的ip地址所在的网卡的端口，其它两块网卡无法监听端口，如果我需要三个网卡都监听，那就需要绑定3个ip，也就等于需要管理3个套接字进行数据交换，这样岂不是很繁琐？ 所以出现INADDR_ANY，你只需绑定INADDR_ANY，管理一个套接字就行，不管数据是从哪个网卡过来的，只要是绑定的端口号过来的数据，都可以接收到





## socket 缓冲区工作机制

详细解释：https://blog.csdn.net/u012997470/article/details/109929715 内核缓冲区和用户内存交互

每个socket被创建后，无论使用的是TCP协议还是UDP协议，都会在内核空间创建自己的接收缓冲区和发送缓冲区。这些socket缓冲区特性可整理如下：

1. socket缓冲区在每个套接字中单独存在；
2. socket缓冲区在创建套接字时自动生成；
3. 即使关闭套接字也会继续传送发送缓冲区中遗留的数据；
4. 关闭套接字将丢失接收缓冲区中的数据。








## 多客户端处理
为了处理多个客户端的连接服务端，使用以下方式进行处理
### I/O阻塞
BIO （block）
每线程，每连接，每个连接中都阻塞，服务器阻塞在accept，线程阻塞在read
#### 多个子线程
每当有一个新的客户端连接到服务端是，创建一个新的子线程处理客户端的连接，服务端则可以继续等待其他新的客户端连接
```c++
(1) 服务端等待accept，阻塞线程
(2) 客户端连接，服务端创建子线程处理
(3) 服务端回到步骤(1)，继续执行
```
- 优点：解决只能连接一个客户端的情况，可以同时连接多个客户端
- 缺点：
（1）每加入一台客户端需要一个IO线程阻塞等待对方数据传送，会导致服务器不断开启线程，但这些线程大部分时间都是阻塞在那里，浪费资源，并且支持不了大并发
（2）每当一个客户端请求连接时，就创建一个线程，关闭时，销毁线程，当处于高并发时，频繁的创建和销毁线程，影响效率
```shell
编译：
make all SUBDIR=threads
# 运行服务端：
./threads/server Port
# 运行客户端：
./threads/client IP Port
```
#### 线程池
为了解决频繁的创建和销毁线程带来的时间消耗
使用线程池创建多个线程，处于wait状态，当有新的任务被放到任务队列时，线程池中的线程被唤醒，处理客户端请求，当处理完成后，线程回到线程池中，处于等待状态
```
(1) 创建一定数量的线程池，并将他们处于等待状态
(2) 客户端发起连接，主线程将任务添加到任务队列，并调用notify，唤醒线程池中的空闲的子线程，处理任务
(3) 主线程等待新的连接，一旦有连接跳转到(2)处理
```
缺点：线程的上下文切换仍然会导致程序的时间消耗
```shell
# 编译：
make all SUBDIR=threadpools
# 运行服务端：
./threadpools/server Port
# 运行客户端：
./threadpools/client IP Port
```


### NIO（nonblocking）

多路复用器(select, poll, epoll) 同步

#### select

#### epoll

## 中断
软中断 cpu
硬中断
硬件发生的中断，任意时间，异步中断


## 用户态到内核态的切换
当用户需要调用内核的代码是，比如说，向屏幕打印输出printf，从缓冲区中读取数据read等函数，实际下面使用的内核封装好的函数，
内核函数
strace命令可以追踪程序的执行状态，包括线程的创建等
```
strace -ff -o log ./bin/server 6001
# -ff 命令有进程的跟踪结果输出到相应的filename.pid中,pid是各进程的进程号
```

## 参考文献
Unix网络编程1 API：可以查看socket的具体的函数的部分


## 需要补充的知识
- [ ] 中断
- [ ] I/O多路复用
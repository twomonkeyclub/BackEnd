# 网络编程

> * [网络编程的步骤](#网络编程的步骤)
> * [常用API](#常用API)
> * [TCP中的accept和connect和listen的关系](#TCP中的accept和connect和listen的关系)
> * [UDP中的connect](#UDP中的connect)
> * [广播和组播过程](#广播和组播过程)
> * [服务端大量TIMEWAIT或CLOSEWAIT状态](#服务端大量TIMEWAIT或CLOSEWAIT状态)
> * [复位报文段RST](#复位报文段RST)
> * [优雅关闭和半关闭](#优雅关闭和半关闭)
> * [解决TCP粘包](#解决TCP粘包)
> * [select可以判断网络断开吗](#select可以判断网络断开吗)
> * [send和read的阻塞和非阻塞情况](#send和read的阻塞和非阻塞情况)
> * [网络字节序和主机序](#网络字节序和主机序)
> * [IP地址分类及转换](#IP地址分类及转换)
> * [select实现异步connect](#select实现异步connect)
> * [为什么忽略SIGPIPE信号](#为什么忽略SIGPIPE信号)
> * [如何设置非阻塞](#如何设置非阻塞)



## 网络编程步骤

### TCP

* 服务端：socket -> bind -> listen -> accept -> recv/send -> close
    * 创建一个socket，用函数socket()，设置SOCK_STREAM
    * 设置服务器地址和侦听端口，初始化要绑定的网络地址结构
    * 绑定服务器端IP地址、端口等信息到socket上，用函数bind()
    * 设置允许的最大连接数，用函数listen()
    * 接收客户端上来的连接，用函数accept()
    * 收发数据，用函数send()和recv()，或者read()和write()
    * 关闭网络连接close()，需要关闭服务端sock和accept产生的客户端sock文件描述符

* 客户端：socket -> connect -> send/recv -> close
    * 创建一个socket，用函数socket()
    * 设置要连接的对方的IP地址和端口等属性
    * 连接服务器，用函数connect()
    * 收发数据，用函数send()和recv()，或read()和write()
    * 关闭网络连接close()
* 注意
    * INADDR_ANY表示本机任意地址，一般服务器端都可以这样写
    * accept中接收的是客户端的地址，返回对应当前客户端的一个clisock文件描述符，表示当前客户端的tcp连接
    * send和recv中接收的是新建立的客户端的sock地址

### UDP
* 服务端：socket -> bind -> recvfrom/sendto -> close
    * 建立套接字文件描述符，使用函数socket()，设置SOCK_DGRAM
    * 设置服务器地址和侦听端口，初始化要绑定的网络地址结构
    * 绑定侦听端口，使用bind()函数，将套接字文件描述符和一个地址类型变量进行绑定
    * 接收客户端的数据，使用recvfrom()函数接收客户端的网络数据
    * 向客户端发送数据，使用sendto()函数向服务器主机发送数据
    * 关闭套接字，使用close()函数释放资源
* 客户端：socket -> sendto/recvfrom -> close
    * 建立套接字文件描述符，socket()
    * 设置服务器地址和端口，struct sockaddr
    * 向服务器发送数据，sendto()
    * 接收服务器的数据，recvfrom()
    * 关闭套接字，close()
* 注意
    * sendto和recvfrom的第56个参数是sock地址
        * 服务器端的recvfrom和sendto都是cli地址
        * 客户端sendto是服务器端的地址，最后一个参数是指针，recvfrom是新建的from地址，最后一个参数是整型
    * UDP不用listen，accept，因为UDP无连接
    * UDP通过sendto函数完成套接字的地址分配工作
        * 第一阶段：向UDP套接字注册IP和端口号
        * 第二阶段：传输数据
        * 第三阶段：删除UDP套接字中注册的目标地址信息
    * 每次调用sendto函数都重复上述过程，每次都变更地址，因此可以重复利用同一UDP套接字向不同的目标传输数据
    
## 常用API
sendto、recvfrom保存对端的地址

* sendto
* recvfrom

## [TCP中的accept和connect和listen的关系](https://blog.csdn.net/tennysonsky/article/details/45621341)

### listen
* listen功能
    * listen函数把一个未连接的套接字转换成一个被动套接字，指示内核应接受指向该套接字的连接请求
    * 参数 backlog 的作用是设置内核中连接队列的长度
    * 根据TCP状态转换图，调用listen导致套接字从CLOSED状态转换成LISTEN状态。 

* 是否阻塞
    * listen()函数不会阻塞，它将该套接字和套接字对应的连接队列长度告诉 Linux 内核，然后，listen()函数就结束。

* backlog的作用
    * backlog是队列的长度，内核为任何一个给定的监听套接口维护两个队列：
        * 未完成连接队列（incomplete connection queue），每个这样的 SYN 分节对应其中一项：已由某个客户发出并到达服务器，而服务器正在等待完成相应的 TCP 三次握手过程。这些套接口处于 SYN_RCVD 状态。
        * 已完成连接队列（completed connection queue），每个已完成 TCP 三次握手过程的客户对应其中一项。这些套接口处于 ESTABLISHED 状态
    * 当有一个客户端主动连接（connect()），Linux 内核就自动完成TCP 三次握手，该项就从未完成连接队列移到已完成连接队列的队尾，将建立好的链接自动存储到队列中，如此重复
    * backlog 参数历史上被定义为上面两个队列的大小之和，大多数实现默认值为 5
    
### connect
* connect功能
    * 对于客户端的 connect() 函数，该函数的功能为客户端主动连接服务器，建立连接是通过三次握手，而这个连接的过程是由内核完成，不是这个函数完成的，这个函数的作用仅仅是通知 Linux 内核，让 Linux 内核自动完成 TCP 三次握手连接最后把连接的结果返回给这个函数的返回值（成功连接为0， 失败为-1）。
    * connect之后是三次握手

* 是否阻塞
    * 通常的情况，客户端的connect() 函数默认会一直阻塞，直到三次握手成功或超时失败才返回（正常的情况，这个过程很快完成）。


### accept
* accept功能
    * accept()函数功能是，从处于 established 状态的连接队列头部取出一个已经完成的连接(**三次握手之后**)

* 是否阻塞
    * 如果这个队列没有已经完成的连接，accept()函数就会阻塞，直到取出队列中已完成的用户连接为止。 
    * 如果，服务器不能及时调用 accept() 取走队列中已完成的连接，队列满掉后会怎样呢？UNP（《unix网络编程》）告诉我们，服务器的连接队列满掉后，服务器不会对再对建立新连接的syn进行应答，所以客户端的 connect 就会返回 ETIMEDOUT

## UDP中的connect
UDP的connect和TCP的connect完全不同，UDP不会引起三次握手

* 未连接的UDP传输数据
    * 第一阶段：向UDP套接字注册IP和端口号
    * 第二阶段：传输数据
    * 第三阶段：删除UDP套接字中注册的目标地址信息
* 已连接的UDP传输数据
    * 第一阶段：向UDP套接字注册IP和端口号
    * 第二阶段：传输数据
    * 第三阶段：传输数据

* 可以提高传输效率
* 采用connect的UDP发送接受报文可以调用send,write和recv,read操作，也可以调用sendto,recvfrom，此时需要将第五和第六个参数置为NULL或0
* 由已连接的UDP套接口引发的异步错误，返回给他们所在的进程。相反我们说过，未连接UDP套接口不接收任何异步错误给一个UDP套接口，connect后的udp套接口write可以检测发送数据成功与否，直接sendto无法检测

* 多次调用connect拥有一个已连接UDP套接口的进程的作用
    * 指定新的IP地址和端口号
    * 断开套接口
    

## 广播和组播过程

* 广播
	* 只适用于局域网
	* 只能向同一网络中的主机传输数据，
* 组播
	* 适用于局域网和广域网（internet） 

## 服务端大量TIMEWAIT或CLOSEWAIT状态
首先通过TCP的四次挥手过程分析确定两个状态的出现背景。TIMEWAIT是大量tcp短连接导致的，确保对方收到最后发出的ACK，一般为2MSL；CLOSEWAIT是tcp连接不关闭导致的，出现在close()函数之前。
### TIMEWAIT
* 可以通过设置SOCKET选项SO_REUSEADDR来重用处于TIMEWAIT的sock地址，对应于内核中的tcp_tw_reuse，这个参数不是“消除” TIME_WAIT的，而是说当资源不够时，可以重用TIME_WAIT的连接
* 修改ipv4.ip_local_port_range，增大可用端口范围，来承受更多TIME
* 设置SOCK选项SO_LINGER选项，这样会直接消除TIMEWAIT

### CLOSEWAIT
客户端主动关闭，而服务端没有close关闭连接，则服务端产生大量CLOSEWAIT，一般都是业务代码有问题

## 复位报文段RST
* 访问不存在的端口，或服务器端没有启动
* 异常终止连接
    * TCP提供了异常终止连接的方法，给对方发送一个复位报文段
    * 此时对端read会返回-1，显示错误errno:Connection reset by peer
    * 这种错误可以通过shutdown来解决
* 处理半打开连接
    *  当某端崩溃退出，此时对端并不知道，若往对端发送数据，会响应一个RST复位报文段

## [优雅关闭和半关闭](https://www.cnblogs.com/liyulong1982/p/3990740.htm)
### 概念
* 一个文件描述符关联一个文件，这里是网络套接字。
* close会关闭用户应用程序中的socket句柄，释放相关资源，从而触发关闭TCP连接
* 关闭TCP连接，是关闭网络套接字，断开连接
* close只是减少引用计数，只有当引用计数为0的时候，才发送fin，真正关闭连接
* shutdown不同，只要以SHUT_WR/SHUT_RDWR方式调用即发送FIN包
* shutdown后要调用close
* 保持连接的某一端想关闭连接了，但它**需要确保要发送的数据全部发送完毕以后才断开连接**，此种情况下需要使用优雅关闭，一种是shutdown，一种是设置SO_LINGER的close
* 半关闭，是关闭写端，但可以读对方的数据，这种只能通过shutdown实现

### close
close函数会关闭文件描述符，不会立马关闭网络套接字，除非引用计数为0，则会触发调用关闭TCP连接。

* 检查接收缓冲区是否有数据未读(不包括FIN包)，如果有数据未读，协议栈会发送RST包，而不是FIN包。如果套接字设置了SO_LINGER选项，并且lingertime设置为0，这种情况下也会发送RST包来终止连接。其他情况下，会检查套接字的状态，只有在套接字的状态是TCP_ESTABLISHED、TCP_SYN_RECV和TCP_CLOSE_WAIT的状态下，才会发送FIN包
* 若有多个进程调用同一个网络套接字，会将网络套接字的文件描述符+1，close调用只是将当前套接字的文件描述符-1，只会对当前的进程有效，只会关闭当前进程的文件描述符，其他进程同样可以访问该套接字
* close函数的默认行为是，关闭一个socket，close将立即返回，TCP模块尝试把该socket对应的TCP缓冲区中的残留数据发送给对方，并不保证能到达对方
* close行为可以通过SO_LINGER修改

```C++
struct linger{
    int l_onoff;    //开启或关闭该选项
    int l_linger;   //滞留时间
}
```
* l_onoff为0，该选项不起作用，采用默认close行为
* l_onoff不为0
    * l_linger为0，close立即返回，TCP模块丢弃被关闭的socket对应的TCP缓冲区中的数据，给对方发送RST复位信号，这样可以异常终止连接，且完全消除了TIME_WAIT状态
    * l_linger不为0
        * 阻塞socket，被关闭的socket对应TCP缓冲区，若还有数据，close会阻塞，进程睡眠，直到收到对方的确认或等待l_linger时间，若超时仍未收到确认，则close返回-1设置errno为EWOULDBLOCK
        * 非阻塞socket，close立即返回，需要根据返回值和errno判断残留数据是够发送完毕

### shutdown
shutdown没有采用引用计数的机制，会影响所有进程的网络套接字，可以只关闭套接字的读端或写端，也可全部关闭，用于实现半关闭，会直接发送FIN包

* SHUT_RD，关闭sockfd上的读端，不能再对sockfd文件描述符进行读操作，且接收缓冲区中的所有数据都会丢弃
* SHUT_WR，关闭写端，确保发送缓冲区中的数据会在真正关闭连接之前会发送出去，不能对其进行写操作，连接处于半关闭状态
* SHUT_RDWR，同时关闭sockfd的读写

## 解决TCP粘包
### 什么是TCP粘包
由于TCP是流协议，因此TCP接收不能确保每次一个包，有可能接收一个包和下一个包的一部分。
### 如何解决
包头和包体，包头中有包体长度

## select可以直接判断网络断开吗
不可以。若网络断开，select检测描述符会发生读事件，这时调用read函数发现读到的数据长度为0.

## send和recv的阻塞和非阻塞情况
send函数返回100，并不是将100个字节的数据发送到网络上或对端，而是发送到了协议栈的写缓冲区，至于什么时候发送，由协议栈决定。

### send
* 阻塞
    * 一直等待，直到写缓冲区有空闲
        * 成功写返回发送数据长度
        * 失败返回-1
* 非阻塞
    * 不等待，立即返回，成功返回数据长度
    *  返回-1，判断错误码
        *  若错误码为EAGAIN或EWOULDBLOCK则表示写缓冲区不空闲
        *  若错误码为ERROR，则表示失败

### recv
* 阻塞
    * 一直等待，直到读缓冲区有数据
        * 成功写返回数据长度
        * 失败返回-1
* 非阻塞
    * 不等待，立即返回，成功返回数据长度
    *  返回-1，判断错误码
        *  若错误码为EAGAIN或EWOULDBLOCK则表示读缓冲区没数据
        *  若错误码为ERROR，则表示失败
    * 返回0
        * 对端关闭连接 

## 网络字节序和主机序
字节序分为大端字节序和小端字节序，大端字节序也称网络字节序，小端字节序也称为主机字节序。

* 大端字节序
    * 一个整数的高位字节存储在低位地址，低位字节存储在高位地址 
* 小端字节序
    *  高位字节存储在高位地址，低位字节存储在低位地址
* 转换API
    * htonl 主机序转网络序，长整型，用于转换IP地址
    * htons 主机序转网络序，短整型，用于转换端口号
    * ntohl 网络序转主机序
    * ntohs 网络序转主机序

## [IP地址分类及转换](https://blog.csdn.net/kzadmxz/article/details/73658168)
### IP地址分类

### IP转换
字符串表示的点分十进制转换成网络字节序的IP地址

* pton，点分十进制转换成地址
* ntop，地址转换成点分十进制
 
## [select实现异步connect](https://blog.csdn.net/nphyez/article/details/10268723)
通常阻塞的connect 函数会等待三次握手成功或失败后返回，0成功，-1失败。如果对方未响应，要隔6s，重发尝试，可能要等待75s的尝试并最终返回超时，才得知连接失败。即使是一次尝试成功，也会等待几毫秒到几秒的时间，如果此期间有其他事务要处理，则会白白浪费时间，而用非阻塞的connect 则可以做到并行，提高效率。

### 实现步骤
* 创建socket，返回套接字描述符；
* 调用fcntl 把套接字描述符设置成**非阻塞**；
* 调用connect 开始建立连接；
* 判断连接是否成功建立。

### 判断连接是否成功建立：
* 如果为**非阻塞**模式，则调用connect()后函数立即返回，如果连接不能马上建立成功（返回-1），则errno设置为EINPROGRESS，此时TCP三次握手仍在继续。 
    * 如果connect 返回0，表示连接成功（服务器和客户端在同一台机器上时就有可能发生这种情况）
    * 失败可以调用select()检测非阻塞connect是否完成。select指定的超时时间可以比connect的超时时间短，因此可以防止连接线程长时间阻塞在connect处。
* 调用select 来等待连接建立成功完成；
    * 如果select 返回0，则表示建立连接超时。我们返回超时错误给用户，同时关闭连接，以防止三路握手操作继续进行下去。
    * 如果select 返回大于0的值，并不是成功建立连接，而是表示套接字描述符可读或可写
        *  当连接建立成功时，套接字描述符变成可写（连接建立时，写缓冲区空闲，所以可写）
        *  当连接建立出错时，套接字描述符变成既可读又可写（由于有未决的错误，从而可读又可写）
    * 如果套接口描述符可写，则我们可以通过调用getsockopt来得到套接口上待处理的错误（SO_ERROR）
        * 如果连接建立成功，这个错误值将是0
        * 如果建立连接时遇到错误，则这个值是连接错误所对应的errno值（比如：ECONNREFUSED,ETIMEDOUT等）。

## 为什么忽略SIGPIPE信号
* 假设server和client 已经建立了连接，server调用了close, 发送FIN 段给client（其实不一定会发送FIN段，后面再说），此时server不能再通过socket发送和接收数据，此时client调用read，如果接收到FIN 段会返回0
* 但client此时还是可以write 给server的，write调用只负责把数据交给TCP发送缓冲区就可以成功返回了，所以不会出错，而server收到数据后应答一个RST段，表示服务器已经不能接收数据，连接重置，client收到RST段后无法立刻通知应用层，只把这个状态保存在TCP协议层。
* 如果client再次调用write发数据给server，由于TCP协议层已经处于RST状态了，因此不会将数据发出，而是发一个SIGPIPE信号给应用层，SIGPIPE信号的缺省处理动作是终止程序。

* 有时候代码中需要连续多次调用write，可能还来不及调用read得知对方已关闭了连接就被SIGPIPE信号终止掉了，这就需要在初始化时调用sigaction处理SIGPIPE信号，对于这个信号的处理我们通常忽略即可

* 往一个读端关闭的管道或者读端关闭的socket连接中写入数据，会引发SIGPIPE信号。当系统受到该信号会结束进程是，但我们不希望因为错误的写操作导致程序退出。

* 通过sigaction函数设置信号，将handler设置为SIG_IGN将其忽略
* 通过send函数的MSG_NOSIGNAL来禁止写操作触发SIGPIPE信号

## 如何设置文件描述符非阻塞
* 通过fcntl设置
```c++
int flag = fcntl(fd, F_GETFL);
flag |= O_NONBLOCK;
fctncl(fd, F_SETFL, flag);
```
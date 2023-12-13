#include "ServerSocket.h"
#include <netinet/in.h>
#include <arpa/inet.h>		
#include <unistd.h>		
#include <iostream>

const int NB = 10;

pr::ServerSocket::ServerSocket(int port) {
	struct sockaddr_in adr;//创建一个IPv4地址结构体struct sockaddr_in
	adr.sin_family = AF_INET;//并设置其地址族
	adr.sin_port = htons(port);//设置端口号（通过htons函数将传入的端口号进行字节顺序转换）
	adr.sin_addr.s_addr = INADDR_ANY;//设置IP地址(INADDR_ANY表示绑定到任何可用的网络接口)

	if((socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) <0) {//创建一个Socket，并将其文件描述符存储在socketfd中
		perror("socket");
		exit(1);
	}

	if(bind(socketfd, (struct sockaddr*)&adr, sizeof(adr)) <0) {//使用bind函数将Socket与指定的地址和端口进行绑定
		perror("bind");
		exit(2);
	}

	int i = 1;
	//设置Socket选项SO_REUSEADDR，以允许地址重用，以防止在Socket关闭后一段时间内再次使用相同地址
	setsockopt(socketfd,SOL_SOCKET,SO_REUSEADDR,&i,sizeof(i));

	if(listen(socketfd, NB) <0) {//开始监听连接请求，允许同时连接的最大连接数为NB
		perror("listen");
		exit(3);
	}

}

pr::Socket pr::ServerSocket::accept() {//接受客户端的连接请求，并返回一个新的Socket对象，该对象可以用于与客户端进行通信
    struct sockaddr_in exp;//存储客户端的地址和端口信息

    socklen_t sz = sizeof(exp);
    int fd;

    if ((fd = ::accept(socketfd, (struct sockaddr*)&exp, &sz)) == -1) {//接受客户端的连接请求，将连接的文件描述符存储在fd中
        perror("accept");
        exit(4);
    }

	//将客户端地址和端口信息转换为可读的形式，并打印到控制台，以提示连接的建立
    std::cout << "Connexion from " << inet_ntoa(exp.sin_addr) << ":" << ntohs(exp.sin_port) << std::endl;
    return Socket(fd);//将接受的连接的文件描述符传递给该对象，从而可以在新的Socket上进行通信
}

void pr::ServerSocket::close() {//用于关闭服务器Socket连接
	if(isOpen()) {
		shutdown(socketfd, SHUT_RDWR);//使用shutdown函数关闭Socket的读写
		::close(socketfd);//使用::close关闭Socket
	}
}
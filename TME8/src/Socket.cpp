#include "Socket.h"
#include <netdb.h>        // getaddrinfo, 将主机名和服务名转换为地址信息
                        //freeaddrinfo, 释放 getaddrinfo 函数返回的 addrinfo 结构链表所占用的内存
                        //getnameinfo将地址信息（如IP地址和端口）转换为对应的主机名和服务名
#include <cstring>        // memset设置内存区域
#include <unistd.h>        // close关闭一个文件描述符，释放与之关联的资源
#include <arpa/inet.h>    // inet_ntoa将网络地址转换为点分十进制的字符串格式
#include <iostream>


void pr::Socket::connect(const std::string & host, int port)
{
    struct addrinfo* ai;
    struct in_addr ipv4;
    if(getaddrinfo(host.c_str(),NULL,NULL,&ai)!= 0)//解析提供的主机名，获取地址信息
	{
        perror("DNS");
        exit(1);
    }

    for(struct addrinfo* rp = ai; rp !=NULL; rp = rp->ai_next)
	{
        if(rp->ai_family == AF_INET  /*IPv4 */ && ai->ai_socktype == SOCK_STREAM /*TCP*/){
            //查找IPv4地址（AF_INET）和TCP套接字类型（SOCK_STREAM）的条目
            ipv4 = (((struct sockaddr_in*) rp->ai_addr)->sin_addr);
            break;
        } 
    }

    freeaddrinfo(ai);
    connect(ipv4,port);
}

void pr::Socket::connect(in_addr ipv4, int port)
{
    
    this->fd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if(fd < 0)
	{
        perror("socket TCP");
        exit(2);
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr = ipv4;
    if(::connect(fd,(struct sockaddr *) & server,sizeof(server))<0)
	{
        close();
        fd = -1;
        perror("connect");
    }
}

//判断如果套接字打开，则关闭连接和描述符
void pr::Socket::close()
{
    if(fd!=-1) {
        shutdown(fd, SHUT_RDWR);
        ::close(fd);
    }
}

//用于输出 sockaddr_in 结构的内容到输出流。
//使用 getnameinfo 函数尝试获取主机名。
//输出IP地址和端口号（端口号使用 ntohs 函数转换为主机字节顺序）。
std::ostream & operator<< (std::ostream & os, struct sockaddr_in & addr)
{
    
    int size_host = 1024;
    char host[size_host];

    if(getnameinfo((struct sockaddr*)&addr, sizeof(addr), host, size_host, nullptr, 0, 0)) 
	{
        os << "host:" << host << std::endl;
    }
    std::cout << inet_ntoa(addr.sin_addr) << ":" <<  ntohs(addr.sin_port) << std::endl; 

    return os;
}
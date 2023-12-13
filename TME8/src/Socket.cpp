#include "Socket.h"
#include <netdb.h>		// getaddrinfo, freeaddrinfo, getnameinfo
#include <cstring>		// memset
#include <unistd.h>		// close
#include <arpa/inet.h>	// inet_ntoa
#include <iostream>


void pr::Socket::connect(const std::string & host, int port){//尝试连接到指定主机和端口
    struct addrinfo* ai;
	struct in_addr ipv4;
    if(getaddrinfo(host.c_str(),NULL,NULL,&ai)!= 0){//解析主机名，获取关于主机的地址信息，然后选择IPv4地址并保存在ipv4中
													//进行实际的连接
        perror("DNS");
        exit(1);
    }

	for(struct addrinfo* rp = ai; rp !=NULL; rp = rp->ai_next){
		if(rp->ai_family == AF_INET  /*IPv4 */ && ai->ai_socktype == SOCK_STREAM /*TCP*/){
			ipv4 = (((struct sockaddr_in*) rp->ai_addr)->sin_addr);//sockaddr_in存储服务器的IPv4地址和端口信息
			break;
		} 
	}

	freeaddrinfo(ai);
    connect(ipv4,port);//尝试连接到服务器。如果连接失败，将关闭Socket并输出错误信息
}

void pr::Socket::connect(in_addr ipv4, int port){//进行Socket连接
    
	this->fd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);//创建一个新的Socket

    if(fd < 0){//检查是否创建成功
        perror("socket TCP");
        exit(2);
    }

    struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);//设置了服务器的IPv4地址和端口信息
	server.sin_addr = ipv4;
	if(::connect(fd,(struct sockaddr *) & server,sizeof(server))<0){//尝试连接到服务器
		close();//连接失败，关闭Socket，输出错误信息
		fd = -1;
		perror("connect");
	}
}

void pr::Socket::close(){//关闭Socket连接
    if(fd!=-1) {
		shutdown(fd, SHUT_RDWR);//关闭Socket的读写
		::close(fd);
	}
}//确保Socket连接被正确地关闭


std::ostream & operator<< (std::ostream & os, struct sockaddr_in & addr){//将sockaddr_in结构体的信息输出到流
    
	int size_host = 1024;
	char host[size_host];

	// if(getnameinfo((struct sockaddr*)&addr, sizeof(addr), host, size_host, nullptr, 0, 0)) {
	// 	os << "host:" << host << std::endl;
	// }
	// std::cout << inet_ntoa(addr. sin_addr) << ":" <<  ntohs(addr.sin_port) << std::endl; 
	
	if(getnameinfo((struct sockaddr*)&addr, sizeof(addr), host, size_host, nullptr, 0, 0) != 0) {//取服务器的主机名和端口号
		perror ("getnameinfo");//输出错误信息并退出程序
		exit(4);
	}

	//将这些信息格式化并输出
	os << "Adresse IPv4: " << inet_ntoa(addr.sin_addr) << std::endl;
	os << "Nom de l'hôte: " << host << std::endl;
	os << "Numéro de port: " << ntohs(addr.sin_port) << std::endl;

	return os;
}



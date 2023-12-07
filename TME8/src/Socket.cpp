#include "Socket.h"
#include <netdb.h>		// getaddrinfo, freeaddrinfo, getnameinfo
#include <cstring>		// memset
#include <unistd.h>		// close
#include <arpa/inet.h>	// inet_ntoa
#include <iostream>


void pr::Socket::connect(const std::string & host, int port){
    struct addrinfo* ai;
    if(getaddrinfo(host.c_str(),NULL,NULL,&ai)!= 0){
        perror("getaddrinfo");
        exit(1);
    }

    struct in_addr addr = ((sockaddr_in*)ai->ai_addr) -> sin_addr;
    freeaddrinfo(ai);
    connect(addr,port);
}

void pr::Socket::connect(in_addr ipv4, int port){
    
    if((fd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) == -1){
        perror("socket");
        exit(2);
    }

    struct sockaddr_in addr;
    memset((void*)&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr = ipv4;

	if(::connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
		perror("connect");
		exit(3);
	}
}



void pr::Socket::close(){
    if(isOpen()) {
		shutdown(fd, SHUT_RDWR);
		::close(fd);
	}
}


std::ostream & operator<< (std::ostream & os, struct sockaddr_in & addr){
    char host[64];

	if(getnameinfo((struct sockaddr*)&addr, sizeof(addr), host, sizeof(host), NULL, 0, 0) != 0) {
		perror("getnameinfo");
		exit(1);
	}

	os << "Adresse IPv4: " << inet_ntoa(addr.sin_addr) << std::endl;
	os << "Nom de l'hôte: " << host << std::endl;
	os << "Numéro de port: " << ntohs(addr.sin_port) << std::endl;

	return os;
}



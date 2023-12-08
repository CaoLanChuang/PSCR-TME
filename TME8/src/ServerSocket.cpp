#include "ServerSocket.h"
#include <netinet/in.h>
#include <arpa/inet.h>		
#include <unistd.h>		
#include <iostream>

const int NB = 10;

pr::ServerSocket::ServerSocket(int port) 
{
	struct sockaddr_in addrclt;
	addrclt.sin_family = AF_INET;
	addrclt.sin_port = htons(port);
	addrclt.sin_addr.s_addr = INADDR_ANY;

	if((socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) 
	{
		perror("socket");
		exit(1);
	}

	if(bind(socketfd, (struct sockaddr*)&addrclt, sizeof(addrclt)) == -1) 
	{
		perror("bind");
		exit(2);
	}

	if(listen(socketfd, NB) == -1) 
	{
		perror("listen");
		exit(3);
	}
}


pr::Socket pr::ServerSocket::accept() 
{
    struct sockaddr_in addrclt;
    socklen_t sz = sizeof(addrclt);
    int fd;

    if ((fd = ::accept(socketfd, (struct sockaddr*)&addrclt, &sz)) == -1) 
	{
        perror("accept");
        exit(4);
    }

    std::cout << "Connexion from " << inet_ntoa(addrclt.sin_addr) << ":" << ntohs(addrclt.sin_port) << std::endl;

    return Socket(fd);
}

void pr::ServerSocket::close() 
{
	if(isOpen()) {
		shutdown(socketfd, SHUT_RDWR);
		::close(socketfd);
	}
}
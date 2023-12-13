#ifndef SRC_SOCKET_H_
#define SRC_SOCKET_H_

#include <netinet/ip.h>
#include <string>
#include <iosfwd>

namespace pr 
{

	class Socket 
	{
		int fd;

		public :
		Socket():fd(-1){}//-1，表示无效的文件描述符
		Socket(int fd):fd(fd){}//接受一个整数参数用于初始化 fd

		// tente de se connecter à l'hôte fourni建立与指定主机的连接
		void connect(const std::string & host, int port);//接受一个字符串（代表主机名）和端口号
		void connect(in_addr ipv4, int port);//接受一个 in_addr 结构（代表IPv4地址）和端口号

		bool isOpen() const 
		{
			return fd != -1;
		}
		
		int getFD() 
		{ 
			return fd ;
		}

		void close();
	};

	std::ostream & operator<< (std::ostream & os, struct sockaddr_in * addr);

}

#endif /* SRC_SOCKET_H_ */

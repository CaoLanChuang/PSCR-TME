#ifndef SRC_SOCKET_H_
#define SRC_SOCKET_H_

#include <netinet/ip.h>
#include <string>
#include <iosfwd>

namespace pr {

class Socket {
	int fd;//存储文件描述符

public :
	Socket():fd(-1){}//默认构造函数，将fd初始化为-1，表示Socket对象未打开
	Socket(int fd):fd(fd){}//构造函数，接受一个文件描述符作为参数，用于创建Socket对象

	void connect(const std::string & host, int port);//尝试连接到指定主机和端口的Socket
	void connect(in_addr ipv4, int port);//尝试连接到指定IPv4地址和端口的Socket

	bool isOpen() const {return fd != -1;}//用于检查Socket是否处于打开状态，如果fd不等于-1，则表示Socket已经打开

	int getFD() { //返回Socket对象的文件描述符
		return fd ;
	}

	void close();//关闭Socket连接
};

std::ostream & operator<< (std::ostream & os, struct sockaddr_in * addr);

}

#endif /* SRC_SOCKET_H_ */

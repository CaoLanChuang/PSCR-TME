#ifndef SRC_SERVERSOCKET_H_
#define SRC_SERVERSOCKET_H_

#include "Socket.h"

namespace pr {

class ServerSocket {//定义服务器端Socket的主要类
	int socketfd;//存储服务器Socket的文件描述符

public :
	// Demarre l'ecoute sur le port donne
	ServerSocket(int port);//创建服务器Socket并开始监听指定的端口

	int getFD() { return socketfd;}//获取服务器Socket对象的文件描述符
	bool isOpen() const {return socketfd != -1;}//检查服务器Socket是否处于打开状态

	Socket accept();//接受客户端的连接请求，返回一个新的Socket对象，该对象用于与客户端进行通信
	void close();//关闭服务器Socket连接
};

} // ns pr
#endif /* SRC_SERVERSOCKET_H_ */

#ifndef SRC_TCPSERVER_H_
#define SRC_TCPSERVER_H_

#include <vector>
#include <thread>
#include "ServerSocket.h"
#include "ConnectionHandler.h"

namespace pr {

// un serveur TCP, la gestion des connections est déléguée
class TCPServer {
	//存储服务器Socket和连接处理器的指针
	ServerSocket * ss; // la socket d'attente si elle est instanciee
	ConnectionHandler * handler; // le gestionnaire de session passe a la constru
	std::vector<std::thread> threads;// Conteneur pour les threads de gestion des clients
public :
	TCPServer(ConnectionHandler * handler): ss(nullptr),handler(handler) {}//构造函数
	~TCPServer(); // Destructeur释放服务器资源
	bool startServer (int port);//启动服务器
	void stopServer () ;//停止服务器
};

} // ns pr

#endif /* SRC_TCPSERVER_H_ */

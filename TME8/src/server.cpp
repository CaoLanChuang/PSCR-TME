#include "ServerSocket.h"
#include <iostream>
#include <unistd.h>

int main00() {
	pr::ServerSocket ss(1664);//在端口1664上开始监听连接请求

	while (1) {//进入一个无限循环，不断接受客户端的连接请求，每当有客户端连接进来时，创建一个新的 pr::Socket 对象 sc 用于代表该连接
		pr::Socket sc = ss.accept();

		int fd = sc.getFD();

		int lu;
		read(fd, &lu, sizeof(int));//从客户端Socket中读取一个整数 lu，然后将其打印到控制台
		std::cout << "lu =" << lu << std::endl;
		lu++;//将 lu 的值加1，然后将结果写回客户端Socket
		write(fd, &lu, sizeof(int));
		sc.close();//关闭客户端Socket（sc.close()）
	}
	ss.close();//最终关闭服务器Socket（ss.close()）
	return 0;
}

int main() {
	pr::ServerSocket ss(1664);//在端口1664上开始监听连接请求

	while (1) {//不断接受客户端的连接请求，每当有客户端连接进来时，创建一个新的 pr::Socket 对象 sc 用于代表该连接
		pr::Socket sc = ss.accept();

		int fd = sc.getFD();

		ssize_t msz = sizeof(int);
		while (1) {
			int lu;
			auto nblu = read(fd, &lu, msz);//不断从客户端Socket中读取整数 lu
			if (nblu == 0) {//如果读取到的数据长度为0，表示客户端已经关闭连接，打印一条消息并退出内层循环
				std::cout << "Fin connexion par client" << std::endl;
				break;
			} else if (nblu < msz) {//如果读取到的数据长度小于整数的大小，表示发生了读取错误，输出错误消息并退出内层循环
				perror("read");
				break;
			}
			std::cout << "lu =" << lu << std::endl;//打印接收到的整数 lu

			if (lu == 0) {//表示客户端请求关闭连接，退出内层循环
				break;
			}
			lu++;//将 lu 的值加1，然后将结果写回客户端Socket，同时打印发送的整数
			if (write(fd, &lu, msz) < msz) {
				perror("write");
				break;
			}
			std::cout << "envoyé =" << lu << std::endl;
		}
		sc.close();//关闭客户端Socket
	}

	ss.close();//最终关闭服务器Socket（ss.close()
	return 0;
}


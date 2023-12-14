#include "ServerSocket.h"
#include <iostream>
#include <unistd.h>
#include <string>


int main00() {
//在这个函数中，没有进行错误处理
	pr::Socket sock;
	sock.connect("localhost", 1664);//连接到名为"localhost"的服务器的1664端口
	int N=42;
	write(sock.getFD(),&N,sizeof(int));//向服务器发送一个整数42
	read(sock.getFD(),&N,sizeof(int));//从服务器接收一个整数并打印出来
	std::cout << N << std::endl;
	return 0;
}


// avec controle
int main0() {

	pr::Socket sock;

	sock.connect("localhost", 1664);

	if (sock.isOpen()) {
		int fd = sock.getFD();
		int i = 10;
		ssize_t msz = sizeof(int);
		if (write(fd, &i, msz) < msz) {//在写入数据和读取数据时，通过检查返回值来处理错误
			perror("write");
		}
		std::cout << "envoyé =" << i << std::endl;
		int lu;
		auto nblu = read(fd, &lu, msz);
		if (nblu == 0) {
			std::cout << "Fin connexion par serveur" << std::endl;
		} else if (nblu < msz) {
			perror("read");//使用perror函数打印出错误消息。
		}
		std::cout << "lu =" << lu << std::endl;
	}

	return 0;
}


// avec une boucle, on attend un 0
int main() {

	pr::Socket sock;

	sock.connect("localhost", 1664);

	if (sock.isOpen()) {
		int fd = sock.getFD();

		ssize_t msz = sizeof(int);
		for (int i = 10; i >= 0; i--) {//循环中多次发送整数（从10到0）
			if (write(fd, &i, msz) < msz) {//接收来自服务器的响应整数，并在每次发送和接收后进行错误检查和处理
				perror("write");
				break;
			}
			std::cout << "envoyé =" << i << std::endl;

			int lu;
			auto nblu = read(fd, &lu, msz);
			if (nblu == 0) {
				std::cout << "Fin connexion par serveur" << std::endl;
				break;
			} else if (nblu < msz) {
				perror("read");
				break;
			}
			std::cout << "lu =" << lu << std::endl;
		}
	}

	return 0;
}


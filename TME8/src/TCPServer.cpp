#include "TCPServer.h"
#include <unistd.h>
#include <thread>

pr::TCPServer::~TCPServer() {
    for(auto& t : threads) {//检查每个线程是否可以被加入（joinable）,等待线程完成（join）,确保所有连接处理线程都已经退出
        if (t.joinable()) {
            t.join();
        }
    }
    delete ss;
    ss = nullptr; // Éviter les dangling pointers
}

void handleClient(pr::Socket dc) {//独立的线程函数，用于处理客户端连接
    int N;
    if (read(dc.getFD(), &N, sizeof(N)) != sizeof(N)) {//从客户端读取一个整数 N
        // Gérer l'erreur de lecture
        perror("lecture");
        exit(1);
    }
    N = 20;
    if (write(dc.getFD(), &N, sizeof(N)) != sizeof(N)) {//将修改后的 N 写回到客户端
        // Gérer l'erreur d'écriture
        perror("écriture");
        exit(1);
    }
    dc.close();
}

bool pr::TCPServer::startServer(int port) {//启动服务器，开始监听指定端口上的连接请求
    ss = new ServerSocket(port);//创建一个新的 ServerSocket 对象，将其指针存储在 ss 成员变量中，用于监听连接请求。
    while (true) { //不断接受客户端的连接请求。每当有客户端连接进来时，会创建一个新的 Socket 对象 dc 来代表该连接
        Socket dc = ss->accept();
        threads.emplace_back(handleClient, std::move(dc));//将 dc 移动（std::move(dc)）到新线程中,在新线程中调用 handleClient 函数来处理客户端连接
    }
    return true;
}


void pr::TCPServer::stopServer (){//用于停止服务器
    if (ss != nullptr) {
        ss->close(); //闭服务器Socket连接（ss->close()）以停止接受新的连接请求
        delete ss;
        ss = nullptr;
    }

    for (auto& t : threads) {//等待所有连接处理线程的完成，确保所有客户端连接都已经处理完毕
        if (t.joinable()) {
            t.join();
        }
    }

    threads.clear();
}

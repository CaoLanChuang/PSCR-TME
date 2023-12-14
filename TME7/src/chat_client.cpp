#include <iostream>
#include <vector>
#include <fcntl.h>
#include <semaphore.h>
#include <algorithm>
#include <thread>
#include "chat_common.h"

#define BUFFER_SIZE 1024
#define SERVER_NAME_DEFAULT "server_sv"
#define CLIENT_NAME_DEFAULT "client_cl"


using namespace std;

bool isRunning = true;          //客户是否在运行
struct message *sharedMsg;      //指向共享内存里的消息结构体的指针
char *sharedContent;            //消息

sem_t *serverSem;               //服务端信号
sem_t *clientSem;               //客户端信号

const char *clientName;             
const char *serverName;

int sharedMemoryFD;             //共享内存的描述符
int sharedContentFD;            
void *sharedMemoryPtr;          //共享内存的指针

vector<thread> threadPool;      //线程池
struct message *connectMsg;     //连接
struct message *disconnectMsg;  //断开

void sendMessage(struct message *m) //向服务端发送消息
{
    sharedMsg->type = m->type;      //改变共享内存里的消息
    strncpy(sharedMsg->content, m->content, BUFFER_SIZE);
    sem_post(serverSem);        //发送信号来通知
}

void readMessage()          //接收服务端消息
{
    sem_unlink(clientName); //删除此前可能存在的同名信号量
    clientSem = sem_open(clientName, O_CREAT | O_EXCL | O_RDWR, 0600, 0);   //重新创建

    shm_unlink(clientName); //同理
    sharedContentFD = shm_open(clientName, O_CREAT | O_EXCL | O_RDWR, 0600);
    ftruncate(sharedContentFD, BUFFER_SIZE);    
    sharedMemoryPtr = mmap(0, BUFFER_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, sharedContentFD, 0); //映射内存空间
    sharedContent = new(sharedMemoryPtr) char();    //在共享内存中初始化字符数组，用于存储接收到的消息内容。

    while (isRunning)       //如果没有离线
    {
        sem_wait(clientSem);    //如果服务器信号量更新，读取并且打印
        if (!isRunning) 
        {
            break;
        }
        cout << clientName << " message: " << sharedContent << endl;
    }
}

void writeMessage()     //从用户获取输入的消息并将其发送到服务器
{
    struct message *outgoingMsg = new struct message;
    outgoingMsg->type = 2; // 广播类型

    while (isRunning) 
    {
        cin.getline(outgoingMsg->content, BUFFER_SIZE); //获取内容
        if (!isRunning) 
        {
            break;
        }
        sendMessage(outgoingMsg);   //发送给服务端
    }   
    delete outgoingMsg;     //内存释放
}

bool sigIntHandled = false;


void terminateClient(int signal) 
{
    if (!sigIntHandled)     //确保关闭信号只被处理一次
    {
        isRunning = false;
        // 唤醒可能在 sem_wait 的线程
        sem_post(clientSem);
        sem_post(serverSem);

        // 等待线程结束
        for (auto &t : threadPool) 
        {
            if (t.joinable()) 
            {
                t.join();
            }
        }

        // 清理资源
        sendMessage(disconnectMsg);
        munmap(sharedMemoryPtr, sizeof(struct message));
        close(sharedMemoryFD);
        close(sharedContentFD);
        shm_unlink(clientName);
        sem_unlink(clientName);
        delete connectMsg;
        delete disconnectMsg;

        sigIntHandled = true;
    }
}

int main(int argc, char **argv) 
{
    signal(SIGINT, terminateClient);        //Ctrl+C关闭客户端

    clientName = (argc > 1) ? argv[1] : CLIENT_NAME_DEFAULT;
    serverName = (argc > 2) ? argv[2] : SERVER_NAME_DEFAULT;
    cout << "Client name: " << clientName << endl;
    cout << "Server name: " << serverName << endl;

    connectMsg = new struct message;    //连接
    connectMsg->type = 1; // connection message
    strncpy(connectMsg->content, clientName,BUFFER_SIZE);

    disconnectMsg = new struct message; //断开
    disconnectMsg->type = 3; // disconnection message
    strncpy(disconnectMsg->content, clientName,BUFFER_SIZE);

    sharedMemoryFD = shm_open(serverName, O_RDWR, 0600);    //打开服务器共享内存
    ftruncate(sharedMemoryFD, sizeof(struct message));      
    sharedMemoryPtr = mmap(0, sizeof(struct message), PROT_READ | PROT_WRITE, MAP_SHARED, sharedMemoryFD, 0);
    //映射空间，方便直接修改

    sharedMsg = new(sharedMemoryPtr) struct message;

    serverSem = sem_open(serverName, O_RDWR, 0600, 0);  //打开服务器信号，开始同步

    sendMessage(connectMsg);        //开始u连接

    threadPool.emplace_back(readMessage);   //开始运行读取和写入
    threadPool.emplace_back(writeMessage);
    for (auto &t : threadPool) 
    {
        t.join();
    }

    delete connectMsg;
    delete disconnectMsg;
}

// test de commande: ./chat_client cl sv

/*
连接：客户端启动时，通过共享内存向服务器发送连接消息，并创建自己的共享内存和信号量用于接收消息。
消息广播：服务器接收到来自某个客户端的消息后，将其写入服务器的共享内存，并通过信号量通知所有客户端有新消息。
读取消息：客户端通过信号量得知有新消息，读取共享内存中的消息内容。
断开连接：客户端在关闭时发送断开连接的消息给服务器，并清理自己的资源。
*/
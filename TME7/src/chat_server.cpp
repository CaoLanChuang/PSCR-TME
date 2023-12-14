#include <iostream>
#include <vector>
#include <fcntl.h>
#include <semaphore.h>
#include "chat_common.h"
#include <cstring>
#include <unistd.h> // 对于 close 和 ftruncate
#define SERVER_NAME_DEFAULT "server_sv"     //默认的服务器名字
#define BUFFER_SIZE 1024                    
#define CLIENT_LIMIT 10                     //client限制

using namespace std;
myshm serverSharedMem;      //共享内存结构体
sem_t syncMutex;            //同步信号量
char *clientList[CLIENT_LIMIT] = {nullptr}; 
int clientCount = 0;
struct message *sharedMsg;  

void cleanup() {
    // 清理共享内存
    munmap(sharedMsg, sizeof(struct message));
    shm_unlink(SHM_NAME);

    // 清理信号量
    sem_destroy(&syncMutex);
    sem_unlink(SHM_NAME);

    // 清理客户端列表
    for (int i = 0; i < CLIENT_LIMIT; ++i) {
        if (clientList[i] != nullptr) {
            delete[] clientList[i];
        }
    }
}

void broadcastMessage() {
    cout << "Broadcasting message: " << sharedMsg->content << endl;
    for (int i = 0; i < CLIENT_LIMIT; i++)  //遍历客户
    {
        if (clientList[i] != nullptr) 
        {
            int fileDescriptor = shm_open(clientList[i], O_RDWR, 0600); //打开客户名对应的共享内存对象
            if (fileDescriptor == -1) 
            {
                cerr << "Failed to open shared memory for client " << clientList[i] << endl;
                continue;
            }

            ftruncate(fileDescriptor, BUFFER_SIZE);     //设置共享内存大小
            void *sharedMem = mmap(0, BUFFER_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, fileDescriptor, 0);
            //将共享内存映射到进程的地址空间，允许进程直接写入
           
            if (sharedMem == MAP_FAILED) 
            {
                cerr << "Failed to map shared memory for client " << clientList[i] << endl;
                close(fileDescriptor);
                continue;
            }
            //在共享内存中创建一个字符messageBuffer，并将要广播的消息复制到这个数组中。
            char* messageBuffer = new(sharedMem) char[BUFFER_SIZE];
            strncpy(messageBuffer, sharedMsg->content, BUFFER_SIZE - 1);
            messageBuffer[BUFFER_SIZE - 1] = '\0'; // 确保字符串以 null 结尾

            munmap(sharedMem, BUFFER_SIZE);     //取消映射mappé
            close(fileDescriptor);

            sem_t *clientSemaphore = sem_open(clientList[i], O_RDWR, 0600, 0);  //打开客户的信号量
            if (clientSemaphore == SEM_FAILED) 
            {
                cerr << "Failed to open semaphore for client " << clientList[i] << endl;
                continue;
            }

            sem_post(clientSemaphore);  //并且发射信号transmettre signal
            sem_close(clientSemaphore); //关闭信号
        }
    }
}

int main(int argc, char** argv) 
{
    atexit(cleanup); // 注册了cleanup函数，以便在程序正常退出时自动调用

    sem_init(&syncMutex, 0, 1); // 第二个参数应为 1，表示可以使用
    const char* serverID = (argc > 1) ? argv[1] : SERVER_NAME_DEFAULT;  //是否设置服务器ID
    int messageSize = sizeof(struct message);
    cout << "Server: " << serverID << endl;

    // Shared memory setup
    int fd = shm_open(serverID, O_CREAT | O_RDWR, 0600);    //创建共享内存对象，用于存储消息
    if (fd == -1) 
    {
        cerr << "Failed to open shared memory" << endl;
        exit(EXIT_FAILURE);
    }

    ftruncate(fd, messageSize);     //调整内存大小
    void *sharedMemory = mmap(0, messageSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);   //映射fd的内存到进程
    if (sharedMemory == MAP_FAILED) 
    {
        cerr << "Failed to map shared memory" << endl;
        exit(EXIT_FAILURE);
    }

    sharedMsg = new(sharedMemory) struct message;       //信息结构体初始化

    // Semaphore initialization
    serverSharedMem.sem = sem_open(serverID, O_CREAT, 0600, 0); //信号初始化
    if (serverSharedMem.sem == SEM_FAILED) 
    {
        cerr << "Failed to open semaphore" << endl;
        exit(EXIT_FAILURE);
    }

    // Main loop
    while(true)
    {
        sem_wait(serverSharedMem.sem);      //等待信号
        cout << "Processing type: " << sharedMsg->type << endl;
        if(sharedMsg->type == 1){ // 如果是连接
            sem_wait(&syncMutex);
            for(int i = 0; i < CLIENT_LIMIT; i++)
            {
                if(clientList[i] == nullptr)        //其实是emplace_back
                {
                    clientList[i] = new char[strlen(sharedMsg->content)];
                    strcpy(clientList[i], sharedMsg->content);
                    clientCount++;
                    break;
                }
            }
            sem_post(&syncMutex);   //解锁
        } 
        else if(sharedMsg->type == 2)
        { // Message broadcast
            broadcastMessage();     //广播消息
        } 
        else if(sharedMsg->type == 3)
        { // Disconnection
            sem_wait(&syncMutex);   //退出
            for(int i = 0; i < CLIENT_LIMIT; i++)
            {
                if(clientList[i] != nullptr && (strcmp(clientList[i], sharedMsg->content) == 0))
                {
                    delete[] clientList[i];
                    clientList[i] = nullptr;
                    clientCount--;
                }
            }
            sem_post(&syncMutex);
        } 
        else 
        {
            cout << "Other type" << endl;
        }
        
        for(int i = 0; i < clientCount; i++)
        {
            if(clientList[i] != nullptr)
            {
                cout << "Client: " << clientList[i] << endl;
            }
        }
    }

    return 0;
}


// test de commande:  ./chat_server sv
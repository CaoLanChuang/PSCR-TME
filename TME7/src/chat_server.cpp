#include <iostream>
#include <vector>
#include <fcntl.h>
#include <semaphore.h>
#include "chat_common.h"
#include <cstring>
#include <unistd.h> // 对于 close 和 ftruncate
#define SERVER_NAME_DEFAULT "server_sv"
#define BUFFER_SIZE 1024
#define CLIENT_LIMIT 10

using namespace std;
myshm serverSharedMem;
sem_t syncMutex;
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
    for (int i = 0; i < CLIENT_LIMIT; i++) {
        if (clientList[i] != nullptr) {
            int fileDescriptor = shm_open(clientList[i], O_RDWR, 0600);
            if (fileDescriptor == -1) {
                cerr << "Failed to open shared memory for client " << clientList[i] << endl;
                continue;
            }

            ftruncate(fileDescriptor, BUFFER_SIZE);
            void *sharedMem = mmap(0, BUFFER_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, fileDescriptor, 0);
            if (sharedMem == MAP_FAILED) {
                cerr << "Failed to map shared memory for client " << clientList[i] << endl;
                close(fileDescriptor);
                continue;
            }

            char* messageBuffer = new(sharedMem) char[BUFFER_SIZE];
            strncpy(messageBuffer, sharedMsg->content, BUFFER_SIZE - 1);
            messageBuffer[BUFFER_SIZE - 1] = '\0'; // 确保字符串以 null 结尾

            munmap(sharedMem, BUFFER_SIZE);
            close(fileDescriptor);

            sem_t *clientSemaphore = sem_open(clientList[i], O_RDWR, 0600, 0);
            if (clientSemaphore == SEM_FAILED) {
                cerr << "Failed to open semaphore for client " << clientList[i] << endl;
                continue;
            }

            sem_post(clientSemaphore);
            sem_close(clientSemaphore);
        }
    }
}

int main(int argc, char** argv) {
    atexit(cleanup); // 注册退出时的清理函数

    sem_init(&syncMutex, 0, 1); // 第二个参数应为 0，表示进程内同步
    const char* serverID = (argc > 1) ? argv[1] : SERVER_NAME_DEFAULT;
    int messageSize = sizeof(struct message);
    cout << "Server: " << serverID << endl;

    // Shared memory setup
    int fd = shm_open(serverID, O_CREAT | O_RDWR, 0600);
    if (fd == -1) {
        cerr << "Failed to open shared memory" << endl;
        exit(EXIT_FAILURE);
    }

    ftruncate(fd, messageSize);
    void *sharedMemory = mmap(0, messageSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (sharedMemory == MAP_FAILED) {
        cerr << "Failed to map shared memory" << endl;
        exit(EXIT_FAILURE);
    }

    sharedMsg = new(sharedMemory) struct message;

    // Semaphore initialization
    serverSharedMem.sem = sem_open(serverID, O_CREAT, 0600, 0);
    if (serverSharedMem.sem == SEM_FAILED) {
        cerr << "Failed to open semaphore" << endl;
        exit(EXIT_FAILURE);
    }

    // Main loop
    while(true)
    {
        sem_wait(serverSharedMem.sem);
        cout << "Processing type: " << sharedMsg->type << endl;
        if(sharedMsg->type == 1){ // Connection
            sem_wait(&syncMutex);
            for(int i = 0; i < CLIENT_LIMIT; i++)
            {
                if(clientList[i] == nullptr)
                {
                    clientList[i] = new char[strlen(sharedMsg->content)];
                    strcpy(clientList[i], sharedMsg->content);
                    clientCount++;
                    break;
                }
            }
            sem_post(&syncMutex);
        } 
        else if(sharedMsg->type == 2)
        { // Message broadcast
            broadcastMessage();
        } 
        else if(sharedMsg->type == 3)
        { // Disconnection
            sem_wait(&syncMutex);
            for(int i = 0; i < CLIENT_LIMIT; i++)
            {
                if(clientList[i] != nullptr && (strcmp(clientList[i], sharedMsg->content) == 0)){
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
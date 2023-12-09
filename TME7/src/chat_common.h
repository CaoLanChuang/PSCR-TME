#ifndef H_CHAT_COMMON
#define H_CHAT_COMMON

#define _XOPEN_SOURCE 700
#define _REENTRANT
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>

#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/stat.h>        // For mode constants
#include <fcntl.h>           // For O_* constants
#include <semaphore.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <errno.h>

#include <string.h>
#include <string>
#include <stdexcept>

#include <string.h>

#define MAX_MESS 50
#define MAX_USERS 10
#define TAILLE_MESS 1024

#define SHM_NAME "/chat_shm"
#define SEM_NAME "/chat_sem"

// Message types
#define MSG_CONNECT 1
#define MSG_DISCONNECT 2
#define MSG_BROADCAST 3

struct message 
{
  long type;
  char content[TAILLE_MESS];
};

struct myshm 
{
  int read;   /* 服务器重传的消息数 */
  int write;  /* 服务器尚未重传的消息数 */
  int nb;     /* 已发送消息总数 */
  sem_t *sem;
  struct message messages[MAX_MESS];
};

char *getName(char *name);




#endif

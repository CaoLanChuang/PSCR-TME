#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <cstdio>
#include <cstdlib>
#include <csignal>

volatile sig_atomic_t child_terminated = 0;
volatile sig_atomic_t timeout = 0;

// SIGALRM的处理器
void alarm_handler(int sig) 
{
    timeout = 1;
}

// SIGCHLD的处理器
void child_handler(int sig) 
{
    child_terminated = 1;
}

int wait_till_pid(pid_t pid, int sec) 
{
    struct sigaction sa;

    // 设置SIGALRM的处理器
    sa.sa_handler = alarm_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGALRM, &sa, nullptr);

    // 设置SIGCHLD的处理器
    sa.sa_handler = child_handler;
    sigaction(SIGCHLD, &sa, nullptr);

    alarm(sec); // sec倒计时结束后触发ALRM信号

    while (true) 
    {
        if (timeout) 
        {  // 如果闹钟信号被触发了
            return 0;  // 超时返回0
        }

        if (child_terminated) 
        {               // 如果子进程已经结束
            alarm(0);   // 取消闹钟
            
            pid_t p = wait(nullptr); // 清理子进程确保不会变成僵尸进程
            if (p == -1 || p == pid) 
            {
                return p;
            }
        }
    }
}

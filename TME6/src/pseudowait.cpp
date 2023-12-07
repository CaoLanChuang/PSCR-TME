#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <cstdio>
#include <cstdlib>
#include <csignal>


volatile sig_atomic_t timeout = 0;

void alarm_handler(int sig) {
    timeout = 1;
}

int wait_till_pid(pid_t pid, int sec) {
    signal(SIGALRM, alarm_handler);     //设置闹钟信号的处理
    alarm(sec); //到sec这个时间之后除法闹钟信号

    while (true) {
        if (timeout) {  //如果闹钟信号被触发了
            return 0;  // 超时返回0
        }

        pid_t p = wait(nullptr);
        if (p == -1 || p == pid) {
            return p;
        }
    }
}




#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <cstdio>
#include <cstdlib>
#include <csignal>
#include "rsleep.h"

static int vie = 10;

void handler (int sig)          //用来处理SIGUSR1信号。每次收到信号，vie减1并打印当前生命值。当vie为0时，程序打印消息并退出。
{
    if(sig == SIGUSR1)
    {
        vie--;
        printf("Attaque reçue par %d; PV restants %d\n",getpid(), vie);
        if(vie == 0)
        {
            printf("Plus de vie pour %d; mort du processus.\n", getpid());
            exit(1);
        }
    }
}

void attaque(pid_t adversaure)  //发送SIGUSR1信号给对手(即攻击对方)，并随机休眠一段时间，adversaure是对手的pid
{
    signal(SIGUSR1,handler);
    if (kill(adversaure,SIGUSR1) < 0)
    {
        printf("Detection de Mort de l’adversaire de pid=%d \n", adversaure);
        exit(0);
    } 
    randsleep();
}

void defense()      //忽略对方发来的SIGUSR1(即忽视一次攻击)，然后随机睡眠
{
    signal(SIGUSR1,handler);
    randsleep();
    // Re-ignore the signal after the sleep
    signal(SIGUSR1, SIG_IGN);
}

void combat(pid_t adversaire)   //在攻击和防御之间循环
{
    while(1)
    {
        defense();
        attaque(adversaire);
        
    }
}

int main() {
    pid_t pere = getpid();
    pid_t pid = fork();
    srand(pid);         //使用pid作为种子来进行数字随机化，保证每次父子进程的随机数都不一样，从而影响sleep的时间
    if (pid == 0) 
    {
        combat(pere);   //子进程攻击父进程并且随机防御
    }
    else 
    {
        combat(pid);    //父进程攻击子进程并且随机防御
    }
    return 0;
}


// Question 6. Le combat est-il encore équitable ? Expliquez pourquoi.
/*  A6:
    Je pense que c'est juste, même si gagner ou perdre dépend en grande partie de la composante chance, 
    mais la soi-disant « chance » est randomisée par la valeur de leur pid, qui à son tour est attribuée aléatoirement. 
    Donc dans l'ensemble c'est juste.
*/

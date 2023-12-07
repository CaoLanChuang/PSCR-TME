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
    signal(SIGUSR1,SIG_IGN);
    randsleep();
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
    Variable Globale Partagée : Chaque processus a sa propre copie de la variable vie post-fork,
     ce qui signifie qu'il n'y a pas de véritable interaction entre les adversaires, mais deux combats séparés avec un compteur.

    Gestion des Signaux : La réinitialisation répétée des gestionnaires de signaux entre l'attaque et la défense peut 
     conduire à une complexité accrue et à des comportements inattendus si les signaux sont envoyés pendant une période d'ignorance temporaire.

    Synchronisation : L'absence de synchronisation entre les processus peut entraîner des attaques et des défenses désordonnées,
     sans garantie d'alternance stricte, affectant l'équité du combat.

    Initialisation du générateur de nombres aléatoires : L'initialisation de srand avec pid peut conduire à des séquences de nombres
     aléatoires très similaires entre les processus, affectant la randomisation des délais entre l'attaque et la défense.

    Terminaison : Un processus qui perd ne notifie pas l'autre processus de sa défaite, laissant l'autre processus dans une boucle indéfinie,
     tentant d'attaquer un processus qui n'existe plus.
*/

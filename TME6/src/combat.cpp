//#include <iostream>
//#include <unistd.h>
//#include <signal.h>
//#include <cstdlib>
//#include <ctime>
//#include <sys/wait.h>
//#include "rsleep.h"
//
//int vies = 3;
//pid_t parent_pid, child_pid;
//
//void handle_sigint(int sig) {
//    vies--;
//    std::cout << "nb. vies " << vies << std::endl;
//    if (vies == 0) {
//        if (child_pid > 0) {
//            kill(child_pid, SIGTERM);
//        }
//        if (parent_pid > 0 && getpid() != parent_pid) {
//            kill(parent_pid, SIGTERM);
//        }
//        std::cout << "~~~fini~~~" << std::endl;
//        exit(1);
//    }
//}
//
//void attaque(pid_t ennemis) {
//    signal(SIGINT, handle_sigint);
//
//    if (kill(ennemis, SIGINT) == -1) {
//        exit(0);
//    }
//    randsleep();
//}
//
//void defense() {
//    signal(SIGINT, SIG_IGN);  // ignorer SIGINT
//
//    sigset_t set;
//    sigemptyset(&set);
//    sigaddset(&set, SIGINT);
//    sigprocmask(SIG_BLOCK, &set, NULL);
//
//    randsleep();
//
//    sigpending(&set);
//    if (sigismember(&set, SIGINT)) {
//        std::cout << "Coup paré" << std::endl;
//    }
//
//    sigprocmask(SIG_UNBLOCK, &set, NULL);
//}
//
//void combat(pid_t ennemis) {
//    while (true) {
//        defense();
//        attaque(ennemis);
//    }
//}
//
//int main() {
//    srand(time(0));
//    parent_pid = getpid();
//    pid_t pid = fork();
//    child_pid = pid;
//
//    if (pid == 0) {      //fils
//        srand(time(0) + getpid());
//        combat(getppid());
//    } else {               //parent
//        srand(time(0) + getpid());
//        combat(pid);
//        wait(NULL);
//    }
//
//    return 0;
//}
//
//// //Q3
////    if (pid == 0) {    //fils
////
////        combat(getppid());
////
////    } else {              //parent
////
////        combat(pid);
////        wait(NULL);
////
////    }
////        return 0;
////    }
//
//
////Q6
////Les batailles sont équitables en raison de la présence de nombres aléatoires,
//// et le comportement des deux parties est imprévisible,
//// ce qui permet des batailles équitables.
//#include <iostream>
//#include <signal.h>
//#include <unistd.h>
//#include <sys/wait.h>
//
//volatile pid_t terminated_pid = -1;
//
//void child_terminated(int signo) {
//    int status;
//    terminated_pid = wait(&status);
//}
//
//void alarm_triggered(int signo) {
//    // ne faire rien
//}
//
//int wait_till_pid(pid_t pid) {
//    pid_t terminated_pid;
//    int status;
//    while ((terminated_pid = wait(&status)) != -1) {
//        if (terminated_pid == pid) {
//            return pid;
//        }
//    }
//    return -1;
//}
//
//int wait_till_pid(pid_t pid, int sec) {
//    struct sigaction sa;
//    sa.sa_handler = child_terminated;
//    sigemptyset(&sa.sa_mask);
//    sa.sa_flags = 0;
//    sigaction(SIGCHLD, &sa, NULL);
//
//    sa.sa_handler = alarm_triggered;
//    sigaction(SIGALRM, &sa, NULL);
//
//    alarm(sec);
//
//    while (true) {
//        pause();  // attendre signal
//        if (terminated_pid == pid) {
//            alarm(0);  // annuler alarm
//            return pid;
//        }
//        if (terminated_pid != -1) {
//            return -1;
//        }
//    }
//}
//
//int main() {
//    pid_t pid = fork();
//    if (pid == 0) { //fils
//
//        sleep(5);  // 5s
//        exit(0);
//    } else {   //parent
//
//        std::cout << "Attente de la fin de processus fils."<<std::endl;
//        if (wait_till_pid(pid, 10) == pid) {
//            std::cout << "Le fils processus est terminé."<<std::endl;
//        }
//    }
//    return 0;
//}
//
#include <ctime>
#include <cstdlib>
#include "rsleep.h"

void randsleep() 
{
  int r = rand();     //生成随机数
  double ratio = (double)r / (double) RAND_MAX;   //r的值在0到RAND_MAX之间
  struct timespec tosleep;
  tosleep.tv_sec =0;    //默认设置为睡眠0秒
  
  // 300 millions de ns = 0.3 secondes
  tosleep.tv_nsec = 300000000 + ratio*700000000;  //总睡眠时间在0.3秒到1秒之间变化

  struct timespec remain;

  while ( nanosleep(&tosleep, &remain) != 0) //如果nanosleep被信号中断，那么会将剩余时间写入remain，然后使用remain继续休眠
  {
    tosleep = remain;
  }
}

// Q4 : Pour attendre une durée aléatoire dans ce scenario on propose la fonction suivante
// randsleep suivante. En lisant le manuel de nanosleep https://man.cx/nanosleep, en particulier les
// erreurs possibles, expliquez la boucle qui est proposée dans ce code.

// A4:  La boucle while dans cette fonction gère une situation où l'appel à nanosleep est interrompu par un signal 
// avant que la période de sommeil spécifiée ne soit écoulée.


// Q5: Comment assurer que les deux processus qui s'affrontent utilisent une graine aléatoire
// différente ?

// A5: On pose une graine avec srand, basée sur time(0) ou sur getpid.
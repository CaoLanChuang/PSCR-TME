#ifndef __RSLEEP_H__
#define __RSLEEP_H__

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

#endif

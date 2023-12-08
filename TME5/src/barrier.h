#pragma once

#include <mutex>
#include <condition_variable>

namespace pr {

    class Barrier {
        mutable std::mutex m;//互斥锁
        std::condition_variable cond;//条件变量
        int cpt = 0;//计数器，用于跟踪已经到达屏障的线程数量。
        int goal;//目标值，表示需要达到屏障的线程总数。

    public:
        Barrier(int n) : goal(n) {}//构造函数，接受一个参数n，表示屏障的目标线程数。

        void done() {//线程调用此方法以表明它已到达屏障。当所有线程都调用了
            std::unique_lock<std::mutex> lg(m);
            cpt++;
            if (cpt == goal) {
                cond.notify_all();//屏障打开，所有等待的线程可以继续执行。
            }
        }

        void waitFor() {//线程调用此方法在屏障处等待。如果cpt还没有达到goal，线程将在这里被阻塞。
            std::unique_lock<std::mutex> lg(m);
            cond.wait(lg, [this] { return cpt == goal; });
        }
    };

}

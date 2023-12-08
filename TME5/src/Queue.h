#ifndef SRC_QUEUE_H_
#define SRC_QUEUE_H_

#include <cstdlib>
#include <mutex>
#include <condition_variable>

namespace pr {

    template <typename T>
    class Queue {
        T ** tab;//指向指针数组的指针，用于存储队列元素。
        const size_t allocsize;//队列的分配大小。
        size_t begin;//队列的起始位置
        size_t sz;//当前大小
        mutable std::mutex m;//互斥锁
        std::condition_variable cond_vide;//条件变量，用于同步线程。
        std::condition_variable cond_plein;
        bool isBlocking;//标记队列是否为阻塞队列。
        bool empty() const {
            return sz == 0;
        }
        bool full() const {
            return sz == allocsize;
        }

    public:

        Queue(size_t size) :allocsize(size), begin(0), sz(0),isBlocking(true) {//构造函数，创建指定大小的队列。
            tab = new T*[size];
            for(size_t i = 0; i < size; ++i) {
                tab[i] = nullptr;
            }
        }

        size_t size() const {//返回队列中的元素数量。
            std::unique_lock<std::mutex> lg(m);
            return sz;
        }

        T* pop() {//从队列中弹出一个元素。如果队列为空，这个方法可能会阻塞（取决于 isBlocking）。
            std::unique_lock<std::mutex> lg(m);
            while (isBlocking && empty()) {
                cond_vide.wait(lg);
            }
            
            if (empty()) {
                return nullptr;
            }
            auto ret = tab[begin];
            tab[begin] = nullptr;
            sz--;
            begin = (begin + 1) % allocsize;
            cond_plein.notify_all();
            return ret;
        }

        bool push(T* elt) {//向队列中推送一个元素。如果队列已满，这个方法可能会阻塞（取决于 isBlocking）。
            std::unique_lock<std::mutex> lg(m);
            while (isBlocking && full()) {
                cond_plein.wait(lg);
            }
            if (full()) {
                return false;
            }
            tab[(begin + sz) % allocsize] = elt;
            sz++;
            cond_vide.notify_all();
            return true;
        }

        void setBlocking(bool block) {//设置队列的阻塞模式。
            std::unique_lock<std::mutex> lg(m);
            isBlocking = block;
            if (!isBlocking) {
                cond_vide.notify_all();
                cond_plein.notify_all();
            }
        }

        ~Queue() {//析构函数
            for (size_t i = 0; i < sz; i++) {
                auto ind = (begin + i) % allocsize;
                delete tab[ind];
            }
            delete[] tab;
        }
    };

}

#endif /* SRC_QUEUE_H_ */

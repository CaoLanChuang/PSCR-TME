#pragma once

#include "Queue.h"
#include "Job.h"
#include <vector>
#include <thread>
#include <cstring>

namespace pr {

    class Pool {
    private:
        Queue<Job> queue;//任务队列，用于存储待处理的任务（Job 对象）
        std::vector<std::thread> threads;//包含所有工作线程的向量

        //这是一个工作线程的函数。它不断地从队列中取出任务（Job 对象），执行它们，然后删除这些对象。
        static void poolWorker(Queue<Job>& queue) {
            while (true) {
                Job* j = queue.pop();
                if (!j) {
                    break;
                }
                j->run();
                delete j;
            }
        }

    public:
        Pool(int qsize) : queue(qsize) {}//构造函数，初始化具有指定大小的任务队列。

        void start(int nbthread) {//启动线程池，创建指定数量的工作线程。
            for (int i = 0; i < nbthread; ++i) {
                threads.emplace_back(poolWorker, std::ref(queue));
            }
        }

        void submit(Job* job) {//向任务队列提交一个新任务。如果队列满了，它将等待直到有空间可用。
            while (!queue.push(job)) {
                std::this_thread::yield();
            }
        }

        void stop() {//停止所有工作线程并清理。它通过向队列中添加空指针作为任务来通知线程结束执行。
            for (int i = 0; i < threads.size(); ++i) {
                queue.push(nullptr);
            }
            for (auto& th : threads) {
                if (th.joinable()) {
                    th.join();
                }
            }
        }

        ~Pool() {//析构函数，调用 stop 方法来确保所有线程都被适当地清理。
            stop();
        }
    };

} // namespace pr

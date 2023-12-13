#include "Stack.h"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <vector>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <signal.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

using namespace std;
using namespace pr;

//生产者从tache里一个字符一个字符的取出来，然后依次加入栈s
//消费者从栈s里取出任务

mutex mtx;          // 互斥锁
condition_variable cv_producer, cv_consumer; // 条件变量，用于notify等操作，配合互斥锁使用
Stack<char> *s;     // 共享栈
string tache;       // 共享任务
int shm_id;         // 共享内存的标识符
int task_index = 0; // 任务索引

const char* shm_name;   //共享内存的名称

atomic<bool> exit_flag(false); // 全局退出标志

void producteur(int id, int M)  //生产者
{
    while (true) 
    {
        unique_lock<mutex> lock(mtx);
        cv_producer.wait(lock, [id, M]{ return (task_index % M == id) || exit_flag; }); 
        //等待lock被解锁(notify_all)，捕获id来让每个生产者轮流进行任务，捕获exit_flag来确定什么时候停止
        //这句话的实际意义是，当lock被解锁后，寻找对应的生产者和停止信号
        //找到其中任意一个就可以解锁整个进程

        if (exit_flag || task_index >= tache.length())  //如果捕获到了退出信号或者没有任务可以执行
        {
            break;      //退出
        }

        char task_char = tache[task_index++];   //从任务里取一个出来
        s->push(task_char);                     //推到栈里
        cout << "Producer " << id+1 << " pushed: " << task_char << endl;

        cv_consumer.notify_all();       //解锁消费者
    }
}

void consomateur(int id) 
{
    while (true) 
    {
        unique_lock<mutex> lock(mtx);       //互斥锁保证数据安全
        cv_consumer.wait(lock, []{ return s->getSz() > 0 || exit_flag; });  //等待栈非空或者退出标志

        if (exit_flag || (task_index >= tache.length() && s->getSz() == 0)) //如果设置了退出标志并且栈空或任务已完成，则退出循环
        {
            break;
        }

        char task_char = s->pop();      //从栈里取出任务
        cout << "Consumer " << id+1 << " popped: " << task_char << endl;

        lock.unlock();  //解锁，因为需要休眠1s，为了提高效率，在休眠之前手动解放lock
        cv_producer.notify_all();   //解锁生产者
        sleep(1); // sleep for a second
    }
}

void signal_handler(int signal) 
{
    if (signal == SIGINT) 
    {
        cout << "\n********terminer proprement***********\n" << endl;
        exit_flag = true; // Set the exit flag for all threads
        cv_producer.notify_all(); 
        cv_consumer.notify_all(); 
    }
}

int main() 
{

	signal(SIGINT, signal_handler);

	int M, N;
	cout << "Saisir le nombre de producteur\n";
	cin >> M;
	cout << "Saisir le nombre de consommateur\n";
	cin >> N;
	

	cout << "Enter the task string: ";
    cin.ignore(); 
    getline(cin, tache);


	// 创建共享内存空间，PRIVATE是唯一，CREAT|0666是权限
    shm_id = shmget(IPC_PRIVATE, sizeof(Stack<char>), IPC_CREAT | 0666);
    s = (Stack<char>*) shmat(shm_id, NULL, 0);  //将栈s附加到shm_id这个共享内存当中

	
	// threads
    vector<thread> producers;
    vector<thread> consumers;

    for (int i = 0; i < M; ++i) 
    {
        producers.push_back(thread(producteur, i, M));
    }

    for (int i = 0; i < N; ++i) 
    {
        consumers.push_back(thread(consomateur, i));
    }

    for (auto& th : producers) 
    {
        th.join();
    }

    for (auto& th : consumers) 
    {
        th.join();
    }

    // cleanup code 

    shmdt(s);   //将栈s从当前进程的内存中分离
    shmctl(shm_id, IPC_RMID, NULL); //销毁共享内存

    return 0;
}






#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <vector>
#include <iostream>


// Question 1 Fork,exec,pipe
int main(int argc, char* argv[])
{
     // 定义 argv
    const char* new_argv[] = {"cat", "pipe.cpp", "|", "wc", "-l", nullptr};
    int new_argc = 6;       // 更新 argc 的值

    if(new_argc < 3)        //如果参数小于3，直接报错
    {
        std::cerr << "Usage: " << argv[0] << " command1 | command2" << std::endl;
        return 1;
    }
    

    // Trouver l'index du pipe '|' dans argv
    int pipeIndex = -1;     //寻找“|”这个符号的位置
    for(int i = 1; i < new_argc; ++i) 
    {
        if(std::string(new_argv[i]) == "|") 
        {
            pipeIndex = i;
            break;          //找到之后直接跳出循环进行下一步
        }
    }

    if(pipeIndex == -1)     //如果没找到，说明参数格式不对，之际报错退出
    {
        std::cerr << "Error: Pipe symbol '|' not found." << std::endl;
        return 1;
    }

    // Construire deux tableaux d'arguments
    // 创建vector来存储两个参数
    std::vector<const char*> cmd1(new_argv + 1, new_argv + pipeIndex);              //“|”前的第一个命令
    std::vector<const char*> cmd2(new_argv + pipeIndex + 1, new_argv + new_argc);   //“|”后的第二个命令  
    cmd1.push_back(nullptr);
    cmd2.push_back(nullptr);

    // Création du pipe
    int tube[2];    //创造pipe，1写，0读
    pipe(tube);

    pid_t pid = fork();
    if(pid == 0) 
    { // Fils   //子进程
        close(tube[1]);
        dup2(tube[0], STDIN_FILENO);    //  从tube[0]里读取数据
        close(tube[0]);                 //  这里的dup2用于两个管道之间的数据传输

        execv(cmd2[0], const_cast<char* const*>(cmd2.data()));  //运行
        perror("execv child");
        exit(1);
    } 
    else 
    { // Père
        close(tube[0]);
        dup2(tube[1], STDOUT_FILENO);   //往tube[1]里写入数据
        close(tube[1]);

        execv(cmd1[0], const_cast<char* const*>(cmd1.data()));  //运行
        perror("execv parent");
        exit(1);
    }

    waitpid(pid, NULL, 0);      //等待子进程结束
    return 0;
}



/*
父进程执行 cmd1，其输出不再是到屏幕，而是通过管道发送到 tube[1]。
子进程执行 cmd2，其输入不再是来自键盘，而是通过管道从 tube[0] 读取，这正是父进程发送到管道的数据。
父进程将pipe.cpp的内容写入tube，然后子进程通过tube读取并且计算有多少行
通过这种方式，两个进程的输入输出被连接起来，实现了类似于在shell中使用管道（|）的效果。
*/
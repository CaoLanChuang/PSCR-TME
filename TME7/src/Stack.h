#include <sys/sem.h>
#include <unistd.h>
 #include <string.h>

namespace pr 
{
    #define STACKSIZE 100
    template<typename T>
    class Stack 
    {
        private:
            T tab [STACKSIZE];
            size_t sz;
            int semid;      //信号标识

            sembuf p_lock[2] = {{0, -1, 0}, {1, -1, 0}}; // 初始化 p_lock， 实现锁定
            sembuf v_unlock[2] = {{0, 1, 0}, {1, 1, 0}}; // 初始化 v_unlock 实现解锁

        public :
            Stack() : sz(0)         //长度0，创建两个信号，一根用于互斥锁，一个用于跟踪元素数量
            {
                memset(tab, 0, sizeof tab);

                // Création des sémaphores
                semid = semget(IPC_PRIVATE, 2, IPC_CREAT | 0666);
                semctl(semid, 0, SETVAL, 1); // Mutex
                semctl(semid, 1, SETVAL, STACKSIZE); // Items

            }

            T pop()             //取出元素，如果为空则卡死，直到非空
            {
                // Attendre si vide
                semop(semid, &p_lock[0], 1);
                T toret = tab[--sz];
                semop(semid, v_unlock, 2); // Débloquer le mutex et incrémenter les items
                return toret;
            }

            void push(T elt)    //放入，如果满则卡死，直到不满
            {
                // Attendre si plein
                semop(semid, p_lock, 2); // Verrouiller le mutex et décrémenter les items
                tab[sz++] = elt;
                semop(semid, &v_unlock[0], 1);
            }

            ~Stack() 
            {
                // Nettoyage du semaphore
                semctl(semid, 0, IPC_RMID);
            }

            size_t getSz()
            {
        	    return sz;
            }

            size_t getSzMax()
            {
        	    return STACKSIZE;
            }
    };

}

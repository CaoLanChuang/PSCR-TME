#pragma once
#include <thread>
#include <mutex>
using namespace std;

namespace pr 
{
	class Compte 
	{
		private:
			//Ex5
			//mutable std::mutex m;
			mutable std::recursive_mutex m;
			int solde;
		public :
			Compte(int solde=0):solde(solde) {}	//初始化银行账户，存款为0

			Compte(const Compte & other)		//复制某个账户，为了确保数据安全
			{									//在复制过程中全程使用互斥锁
				other.m.lock();
				solde = other.solde;
				other.m.unlock();
			}

			void crediter (unsigned int val)	//存款函数，
			{
				//Ex4
				//unique_lock<mutex> g(m);		//unique-lock函数， 多个存款进程工共用这一个锁
				solde+=val ;					//用于保证多线程下，存款数额不会异常
			}

			bool debiter (unsigned int val)		//取款函数
			{
				//Ex4
				//unique_lock<mutex> g(m);		//多个取款函数共用一个互斥锁，保证余额正确
				bool doit = solde >= val;		//存款余额 > 取款数额才能取钱
				if (doit) 
				{
					solde-=val ;
				}
				return doit;
			}

			int getSolde() const 				//互斥锁，保证询问存款余额的时候不会进行存钱取钱操作
			{
				unique_lock<recursive_mutex> g(m);
				return solde;
			}

			//Ex3
			std::recursive_mutex& getMutex() 
			{
        		return m;
    		}
	};
}

#pragma once

#include "Compte.h"
#include <vector>
#include <iostream>
#include <set>

namespace pr {

	class Banque 
	{
		private:
			typedef std::vector<Compte> comptes_t;
			comptes_t comptes;
			//Ex4
			//mutex transfertMutex;	//全局锁
			//Ex5
			recursive_mutex transfertMutex;

			std::set<Compte*> comptesVerifies; 	// 已核实的账户的集合
    		std::mutex verifieMutex; 			// 用于已核实的集合的全局锁

		public :
			
			//银行构造函数，ncomptes为账户数量，solde是每个账户的初始余额
			Banque (size_t ncomptes, size_t solde) : comptes (ncomptes, Compte(solde)){}

			//转账函数，deb是转出账户，cred是转入账户，val是转账数额
			void transfert(size_t deb, size_t cred, unsigned int val)
			{

				Compte &debiteur = comptes[deb];
    			Compte &crediteur = comptes[cred];
				//Ex4。Ex6。Ex9
				//这里已经解决了Ex6的问题，因为我们已经上了全局锁来保证数据稳定

				unique_lock<recursive_mutex> transfertLock(transfertMutex);				//给全局锁上锁
    			unique_lock<recursive_mutex> debiteurLock(debiteur.getMutex());			//锁住转出账户

				std::unique_lock<std::mutex> verifieGuard(verifieMutex);					//检查借出方账户是否已经核实

				if (comptesVerifies.find(&debiteur) == comptesVerifies.end()) 
				{
					// 借方账户尚未被会计师核实，等待直到核实
            		// 这个检查避免了死锁
					verifieGuard.unlock();
            		std::this_thread::sleep_for(std::chrono::milliseconds(10)); // 等待一段时间
            		return transfert(deb, cred, val); 							// 重新尝试转账
				}
				
    			unique_lock<recursive_mutex> crediturLock(crediteur.getMutex());		//锁住收款人
				//判断转出账户内是否有足够的存款，有则取出
				if (debiteur.debiter(val)) 
				{
					crediteur.crediter(val);		//将取出的钱转入账户
				}
			}

			size_t size() const			//返回银行管理的账户数量
			{
				return comptes.size();
			}
			
			bool comptabiliser (int attendu) const	//检查银行账户余额，用于和attendu作比较，也用于检查银行是否有存款为负数的账户
			{
				int bilan = 0;
				int id = 0;
				for (const auto & compte : comptes) 		
				{
					if (compte.getSolde() < 0) 	//检查存款是否为非负数
					{
						cout << "Compte " << id << " en négatif : " << compte.getSolde() << endl;
					}
					bilan += compte.getSolde();	//计算所有账户的总存款
					id++;
				}
		
				if (bilan != attendu) 		//如果不等，则出错
				{
					cout << "Bilan comptable faux : attendu " << attendu << " obtenu : " << bilan << endl;
				}
				return bilan == attendu;
			}

			Compte getCompte(size_t index)
			{
				return this->comptes[index];
			}

			//Ex10
			void effectuerBilan() 
			{
        		std::lock_guard<std::mutex> verrou(verifieMutex);
        		for (auto& compte : comptes) 
				{
            		comptesVerifies.insert(&compte);
        		}
    		}
	};
}

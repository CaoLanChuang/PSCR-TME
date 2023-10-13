#include "Banque.h"
#include <chrono>
#include <random>
#include <cstdlib>

using namespace std;



//Ex1
//转账函数
void transaction(pr::Banque& banque, int id) 
{

	srand((unsigned)time(NULL));
    for (int i = 0; i < 1000; ++i) 
	{
		int cred, deb, val;
		do
		{
			cred = (rand() % (banque.size()-1));
			deb = (rand() % (banque.size()-1));
		} while (cred == deb);
		

		val = (rand()%1000);
		

		banque.transfert(deb, cred, val);
        int sleepTime = (rand()%20);
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
    }
}

const int NB_THREAD = 10;

/*
Ex2
En utilisant le mécanisme de protection mutex actuel sur la classe Compte, 
le comportement du programme est correct dans les situations multithread et il n'y a pas de problème de course aux données. 
En effet, vous utilisez mutex dans la classe Compte pour protéger la modification du solde du compte, 
en garantissant que lorsqu'un thread modifie le solde du compte, les autres threads ne peuvent pas le modifier en même temps.
Cela signifie qu'à un instant donné, un seul thread peut opérer sur un compte spécifique, 
ce qui garantit la cohérence des données.

Cependant, vous avez mentionné que vous souhaitiez effectuer des mises à jour atomiques sur deux comptes associés, 
ce qui signifie que soit les deux comptes sont mis à jour en même temps, soit aucun n'est mis à jour du tout. 
La conception actuelle permet à deux threads différents de modifier deux comptes liés à des moments différents, 
de sorte que dans certains cas, un observateur peut voir un compte débité alors que l'autre compte n'a pas encore été déposé,
et ne répond donc pas à l'exigence d'atomicité. Pour y parvenir, 
vous avez besoin d’un mécanisme de synchronisation supplémentaire.
*/

int main () {
	vector<thread> threads;

	pr::Banque banque(10, 1000);		//10个账户，每个账户1000块



	for (int i = 0; i < NB_THREAD; ++i) 
	{
		//多线程的 std::thread 也是必须显式通过 std::ref 来绑定引用进行传参，否则，形参的引用声明是无效的
        threads.emplace_back(transaction, std::ref(banque), i);		
    }


	for (auto & t : threads) 
	{
		t.join();
	}

	

	// TODO : tester solde = NB_THREAD * JP

	if (banque.comptabiliser(10000))
	{
		for (int i = 0; i < banque.size(); i++)
		{
			cout << "Compte " << i << "  Solde : " << banque.getCompte(i).getSolde() << endl;
		}
	}

	return 0;
}
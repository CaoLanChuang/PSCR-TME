#include "Vector.h"
#include "VectorEx3.h"
#include "HashMap.h"
#include <unordered_map>

int main () {

	ifstream input = ifstream("WarAndPeace.txt");		//读入文件

	auto start = steady_clock::now();					//记录开始时间
	std::cout << "Parsing War and Peace" << endl << endl;			
	string word;										//下一个将被处理的词
	
	regex re( R"([^a-zA-Z])");							//识别异常字符的正则表达式
	
	int choice;
	std::cout << "1. Ex2 : Combien de mots different. " << endl;
	std::cout << "2. Ex3 : Combien de mots different avec le nombre de occurrence" << endl;
	std::cout << "3. Ex5 : Definir le Class HashMap et le utilise" << endl;
	std::cout << "4. TME3 Ex7 : La classe std::unordered_map<K,V>" << endl;
	std::cout << "0. Exit" << endl << endl;
	std::cout << "Give me your choice : " ;

	cin >> choice;
	
	switch (choice)
	{
		case 1 :
		{
			Vector CountDifferentWorld(1);							//创建大小为0的计数Vector

			while (input >> word) 							//如果还能读入词
			{										
				word = regex_replace ( word, re, "");			//消除标点符号和异常字符
		
				transform(word.begin(),word.end(),word.begin(),::tolower);	//把所有单词全部转换成小写
		
				if (!CountDifferentWorld.isInclude(word))
				{
					CountDifferentWorld.pushBack(word);
				}	
			}

			input.close();

			std::cout << "Finished Parsing War and Peace" << endl;

			auto end = steady_clock::now();
    		std::cout << "Parsing took "
              	<< duration_cast<milliseconds>(end - start).count()
              	<< "ms.\n";

			cout << "Found a total of " << CountDifferentWorld.size() << " words different" << endl;
			input.close();
			break;
		}
	
		case 2 : 
		{
			VectorEx3 CountDifferentWorld(1);

			while (input >> word) 					//如果还能读入词
			{								
		
				word = regex_replace ( word, re, "");			//消除标点符号和异常字符
		
				transform(word.begin(),word.end(),word.begin(),::tolower);	//把所有单词全部转换成小写
		
				//TME2 Ex3
				CountDifferentWorld.pushBack(word);
			}
			input.close();

			std::cout << "Finished Parsing War and Peace" << endl;

			auto end = steady_clock::now();
    		std::cout << "Parsing took "
              	<< duration_cast<milliseconds>(end - start).count()
             	<< "ms.\n";


			for (size_t i = 0; i < CountDifferentWorld.size(); i ++)
			{
				if (CountDifferentWorld[i].first == "peace")
				{
					std::cout<<"Found a total of " << CountDifferentWorld[i].second <<" fois de peace" << endl;
				}

				if (CountDifferentWorld[i].first == "war")
				{
					std::cout<<"Found a total of " << CountDifferentWorld[i].second <<" fois de war" << endl;
				}

				if (CountDifferentWorld[i].first == "toto")
				{
					std::cout<<"Found a total of " << CountDifferentWorld[i].second <<" fois de toto" << endl;
				}	
			}
			break;
		}
		
		case 3:
		{
			HashMap CountDifferentWorld(1); 
			
			while (input >> word) 					//如果还能读入词
			{								
				
				word = regex_replace ( word, re, "");			//消除标点符号和异常字符
				transform(word.begin(),word.end(),word.begin(),::tolower);	//把所有单词全部转换成小写

				CountDifferentWorld.pushBack(word);
			}
			input.close();
			
			std::cout << "Finished Parsing War and Peace" << endl;

			auto end = steady_clock::now();
    		std::cout << "Parsing took " << duration_cast<milliseconds>(end - start).count() << "ms.\n";
				
			std::cout << "Found a total of  " << CountDifferentWorld.getValue("peace") <<" fois de peace" << endl;
			std::cout << "Found a total of  " << CountDifferentWorld.getValue("war") <<" fois de war" << endl;
			std::cout << "Found a total of  " << CountDifferentWorld.getValue("toto") <<" fois de toto" << endl;
			
			vector<pair<string, int>> VecPair;


			size_t indice = 0;
			for (size_t i = 0; i < CountDifferentWorld.alloc_size(); i++)
			{
				for( auto &item : CountDifferentWorld[i])
				{
					if (item.Key != "" || item.Value != 0)
					{
						pair<string, int> NewPair;
						NewPair.first = item.Key;		//因为Map[i]里面的链表本身就是非空的，存在一个空白的HashItem
						NewPair.second = item.Value;	//我们pushBack的时候只是把新的HashItem放到了它的前面，如果不加if
						VecPair.emplace_back(NewPair);	//就会把空白值赋值出去导致错误	
						indice++;											
					}				
				}
			}
			

			std::sort(VecPair.begin(), VecPair.end(), 
				[](const pair<string, int>& a, const pair<string, int>& b)
				{
					return a.second < b.second;
				});

			/*cout << VecPair[0].first<<" "<<VecPair[0].second << endl;
			cout << VecPair[indice-1].first<<" "<<VecPair[indice-1].second << endl << endl;*/
			
			//TME3

			cout << "TME3" << endl << endl;

			//Ex 5
			vector<pair<string, int>> VecPairTme3;
			size_t indiceTME3 = 0;
			
			for (HashMap::iterator it = CountDifferentWorld.begin(); it != CountDifferentWorld.end(); ++it)
			{
				
				VecPairTme3.push_back(*it);
				indiceTME3 ++;
				
			}

			//Ex 6
			std::sort(VecPairTme3.begin(), VecPairTme3.end(), 
				[](const pair<string, int>& a, const pair<string, int>& b)
				{
					return a.second < b.second;
				});

			cout << "Les dix mots les plus frequents du livre :" << endl;
			size_t calcul = 1;
			while (calcul <= 10)
			{
				cout << VecPairTme3[indiceTME3 - calcul].first << endl;
				calcul ++;
			}

			//Ex1
			HashMap::iterator it = CountDifferentWorld.begin();
			HashMap::iterator endIt = CountDifferentWorld.end();

			size_t elementCount = CountDifferentWorld.count(it, endIt);
			cout << "La resultat de la Fonction cout est : " << elementCount << endl;
			
			//Ex2，确实只有一次，因为相同的全都放在Peace对应的Value里了
			size_t nombre_Peace = CountDifferentWorld.count_if_equal(it, endIt, "peace");
			cout<< "La nombre de peace est : " << nombre_Peace << endl;
			

			
			break;
		}

		case 4:
		{
			std::unordered_map<std::string, int> CountDifferentWorld;
			std::unordered_map<int, std::forward_list<std::string>> FrequencyMap;
			while (input >> word) 					//如果还能读入词
			{								
				word = regex_replace ( word, re, "");			//消除标点符号和异常字符
				transform(word.begin(),word.end(),word.begin(),::tolower);	//把所有单词全部转换成小写

				//Ex 7
				auto result = CountDifferentWorld.insert({word, 1});
				if (!result.second) 
				{
            		result.first->second++;		// 如果单词已经存在，增加其值
        		}
				
			}
			input.close();
			
			std::cout << "Finished Parsing War and Peace" << endl;
			auto end = steady_clock::now();
    		std::cout << "Parsing took " << duration_cast<milliseconds>(end - start).count() << "ms.\n";

			if(CountDifferentWorld.find("peace") != CountDifferentWorld.end())
				std::cout << "Found a total of  " << CountDifferentWorld.find("peace")->second <<" fois de peace" << endl;
			else
				std::cout << "Found a total of  " << 0 <<" fois de peace" << endl;
			
			if(CountDifferentWorld.find("war") != CountDifferentWorld.end())
				std::cout << "Found a total of  " << CountDifferentWorld.find("war")->second <<" fois de war" << endl;
			else
				std::cout << "Found a total of  " << 0 <<" fois de war" << endl;
			
			if(CountDifferentWorld.find("toto") != CountDifferentWorld.end())
				std::cout << "Found a total of  " << CountDifferentWorld.find("toto")->second <<" fois de toto" << endl;
			else
				std::cout << "Found a total of  " << 0 <<" fois de toto" << endl;

			//Ex 8
			for (const auto& entry : CountDifferentWorld) 
			{
        		int frequency = entry.second;
        		FrequencyMap[frequency].push_front(entry.first);
    		}

			cout << "Donnez-moi le frequence que vous souhaitez : " << endl;
			int frequence;
			cin >> frequence;

			if (FrequencyMap.find(frequence) != FrequencyMap.end()) 
			{
        		std::cout << "Mots avec frequence " << frequence << ":" << std::endl;
        		for (const std::string& word : FrequencyMap[frequence]) 
				{
            		std::cout << word << std::endl;
        		}
    		} else 
			{
        		std::cout << "Aucun mot trouvé avec fréquence " << frequence << "." << std::endl;
    		}

		}

		case 0 :
			exit;
	}

    return 0;
}
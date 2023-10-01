#include "Vector.h"
#include "VectorEx3.h"
#include "HashMap.h"

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
				for( HashMap::HashItem &item : CountDifferentWorld[i])
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
			
			cout << indice << endl;

			std::sort(VecPair.begin(), VecPair.end(), 
				[](const pair<string, int>& a, const pair<string, int>& b)
				{
					return a.second < b.second;
				});

			cout << VecPair[0].first<<" "<<VecPair[0].second << endl;
			cout << VecPair[20332].first<<" "<<VecPair[20332].second << endl;
			


			
			break;
		}

		case 0 :
			exit;
	}

    return 0;
}
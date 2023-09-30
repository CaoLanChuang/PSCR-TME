#include "Vector.h"
#include "VectorEx3.h"

int main () {

	ifstream input = ifstream("WarAndPeace.txt");		//读入文件

	auto start = steady_clock::now();					//记录开始时间
	std::cout << "Parsing War and Peace" << endl;			
	string word;										//下一个将被处理的词
	
	regex re( R"([^a-zA-Z])");							//识别异常字符的正则表达式
	
	int choice;
	std::cout << "1. Ex2" << endl;
	std::cout << "2. Ex3" << endl;
	std::cout << "0. Exit" << endl;

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
		
		case 0 :
			exit;
	}

    return 0;
}
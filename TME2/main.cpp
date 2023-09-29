#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <forward_list>


int main () {
	using namespace std;
	using namespace std::chrono;


	//TME2 Ex2
	/*
	class Vector
	{
		private:
			string *world;
			size_t sz;
			size_t alloc_sz;
		
		public:
			Vector(size_t size)
			{
				sz = 0;
				alloc_sz = size;
				world = new string[size];
			}

			~Vector()
			{
				delete[] world;
			}

			size_t size()
			{
				return sz;
			}

			bool empty()
			{
				return sz==0;
			}

			string& operator[] (size_t index)
			{
				if (index < sz)
				{
					return world[index];
				}
				else
				{
					std::cout << "Index OverRange !\n" << std::endl;
				}
			}

			const string& operator[] (size_t index) const
			{
				if (index < sz)
				{
					return world[index];
				}
				else
				{
					std::cout << "Index OverRange !\n" << std::endl;
				}
			}

			void ensureCapaticy(size_t n)
			{
				if (n > alloc_sz)
				{
					alloc_sz = std::max(2*alloc_sz, n);
					string* temp = new string[alloc_sz];
					for (size_t i = 0; i < sz; i++)
					{
						temp[i] = world[i];
					}
					delete[] world;
					world = temp;
				}
			}

			void pushBack(const string& elt)
			{
				ensureCapaticy(sz+1);
				world[sz++] = elt;
				
			}

			bool isInclude(const string &wd)
			{
				for (size_t i = 0; i < sz; i++)
				{
					if (world[i] == wd) return true;
				}
				return false;
			}	

	};
	*/

	//TME2 Ex3
	/*
	class VectorEx3
	{
		private:
			pair<string, int> *world;
			size_t sz;
			size_t alloc_sz;
		
		public:
			VectorEx3(size_t size)
			{
				sz = 0;
				alloc_sz = size;
				world = new pair<string,int>[size];
			}

			~VectorEx3()
			{
				delete[] world;
			}

			size_t size()
			{
				return sz;
			}

			bool empty()
			{
				return sz==0;
			}


			void ensureCapaticy(size_t n)
			{
				if (n > alloc_sz)
				{
					alloc_sz = std::max(2*alloc_sz, n);
					pair<string, int>* temp = new pair<string,int>[alloc_sz];

					for (size_t i = 0; i < sz; i++)
					{
						temp[i].first = world[i].first;
						temp[i].second = world[i].second;
					}
					delete[] world;
					world = temp;
				}
			}

			

			void pushBack(const string& elt)
			{
				bool isInclude = false;
				for (size_t i = 0; i < sz; i++)
				{
					if (world[i].first == elt)
					{
						world[i].second++;
						isInclude =true;
						break;
					}
				}
				
				if(isInclude == false)
				{
					ensureCapaticy(sz+1);
					world[sz].first = elt;
					world[sz].second = 1;
					sz++;
				}
				
			}

			pair<string, int>& operator[] (size_t i)
			{
				return world[i];
			}

			const pair<string, int>& operator[] (size_t i) const
			{
				return world[i];
			}

	};*/


	//TME2 Ex5
	class HashMap
	{
		struct HashItem
		{
			const string Key;
			int Value;
		};

		

		vector<std::forward_list<HashItem>> Map;

	};







	ifstream input = ifstream("WarAndPeace.txt");		//读入文件

	auto start = steady_clock::now();					//记录开始时间
	std::cout << "Parsing War and Peace" << endl;			

	//size_t nombre_lu = 0;								//计数
	
	string word;										//下一个将被处理的词
	
	regex re( R"([^a-zA-Z])");							//识别异常字符的正则表达式
	
	//TME2 Ex2
	//Vector CountDifferentWorld(1);							//创建大小为0的计数Vector

	//TME2 Ex3
	//VectorEx3 CountDifferentWorld(1);



	while (input >> word) {								//如果还能读入词
		
		word = regex_replace ( word, re, "");			//消除标点符号和异常字符
		
		transform(word.begin(),word.end(),word.begin(),::tolower);	//把所有单词全部转换成小写
		
		/* TME2 Ex2
		if (!CountDifferentWorld.isInclude(word))
		{
			CountDifferentWorld.pushBack(word);
		}*/
			
		
		//TME2 Ex3
		//CountDifferentWorld.pushBack(word);
		
		
	}
	input.close();

	std::cout << "Finished Parsing War and Peace" << endl;

	auto end = steady_clock::now();
    std::cout << "Parsing took "
              << duration_cast<milliseconds>(end - start).count()
              << "ms.\n";

    //cout << "Found a total of " << nombre_lu << " words." << endl;
	
	//TME2 Ex2
	//cout << "Found a total of " << CountDifferentWorld.size() << " words different" << endl;

	//TME2 Ex3
	/*
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

	}*/


    return 0;
}



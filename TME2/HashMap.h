
#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <forward_list>


using namespace std;
using namespace std::chrono;
//TME2 Ex5
class HashMap
{
	struct HashItem
	{
		const string Key;
		int Value;
	};
	private:
		vector<std::forward_list<HashItem>> Map;
	
	public:
		HashMap(vector<std::forward_list<HashItem>> Map) : Map(Map){}
		
	//一个vector，用hashcode作为下标(用函数hash(string)生成)，存储HashTiem类型的链表，当hashcode相同时，存储到同位置的链表的下一个
};
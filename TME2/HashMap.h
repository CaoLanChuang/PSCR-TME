
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
		HashItem(const string& key, int value) : Key(key), Value(value) {}
	};

	private:
		size_t sz;
		size_t alloc_sz;
		vector<std::forward_list<HashItem>> *Map;
	
	public:
		HashMap(size_t size)
		{
			this->alloc_sz = size;
			sz = 0;
			Map = new vector<forward_list<HashItem>>[size];
		}

		~HashMap()
		{
			delete[] Map;
		}	

		size_t size()
		{
			return this->sz;
		}

	    bool empty()
		{
			return sz==0;
		}

		forward_list<HashItem>& operator[] (size_t index)
		{
			if (index < sz)
			{
				return (*Map)[index];
			}
			else
			{
				std::cout << "Index OverRange !\n" << std::endl;
				throw std::out_of_range("Index out of range");
			}
		}

		const forward_list<HashItem>& operator[] (size_t index) const
		{
			if (index < sz)
			{
				return (*Map)[index];
			}
			else
			{
				std::cout << "Index OverRange !\n" << std::endl;
				throw std::out_of_range("Index out of range");
			}
		}

		void ensureCapacity(size_t n)
		{
    		if (n > alloc_sz)
    		{
        		size_t newAllocSize = std::max(2 * alloc_sz, n);

        		// 创建一个新的哈希表，注意这里使用了新的 alloc_sz
        		vector<forward_list<HashItem>>* temp = new vector<forward_list<HashItem>>[newAllocSize];

        		// 逐个重新插入所有现有项到新的哈希表中，并且更新成新的index
        		for (size_t i = 0; i < alloc_sz; i++)
        		{
            		for (const HashItem& item : (*Map)[i])
            		{
                		size_t newHashCode = std::hash<string>{}(item.Key);
                		size_t newIndex = newHashCode % newAllocSize;
                		(*temp)[newIndex].push_front(item);
            		}
        		}

        		// 删除旧的哈希表并使用新的哈希表
        		delete[] Map;
        		Map = temp;
        		alloc_sz = newAllocSize;
    		}
		}

		bool isInclude(const string& key)				//判断某个key是否已经在Vector里存储过
		{												//首先通过index找到Vector里对应的格子
		//随后对这个链表进行循环判断，以确定key这个string
		//是否是某个HashItem的Key
			size_t hashCode = std::hash<string>{}(key);	
			size_t index = hashCode % alloc_sz;			

			for (const HashItem& item : (*Map)[index])
			{
				if (item.Key == key)
				{
					return true;
				}
			}
			return false;
		}	

		void pushBack(const string& wd)
		{
			size_t hashCode = std::hash<string>{}(wd);
    		size_t index = hashCode % alloc_sz;

			bool isInclude = false;

			if ((*Map)[index].empty())						//如果没出现过，那么插入一个新的Item
			{		
				ensureCapacity(sz+1);						//需要增加sz的值，因为占用了一个新的空格
				(*Map)[index].push_front(HashItem(wd, 1));	
			}
			else											//如果Map[index]里已经有值了
			{
				for ( HashItem& item : (*Map)[index])		//对Vector里index这个格子里的链表进行遍历
				{											//用于确认wd对应的string是否已经作为一个HashItem存储其中
					if (item.Key == wd)						//如果是，那么直接增加它的value的值
					{
						item.Value++;
						isInclude = true;
                		break;
					}
				}
				if (isInclude == false)						//如果否，则直接插入，但是不需要增加sz的值				
				{
					(*Map)[index].push_front(HashItem(wd, 1));
				}
			}
		}

		int getValue(const string& wd) const
		{
   	 		size_t hashCode = std::hash<string>{}(wd);
    		size_t index = hashCode % alloc_sz;

    		for (const HashItem& item : (*Map)[index])		// 遍历链表，寻找匹配的HashItem
    		{
        		if (item.Key == wd)							// 找到匹配的HashItem，返回其Value
        		{
            		return item.Value;
        		}
    		}												
    		return 0;										//没有就是0
		}

};
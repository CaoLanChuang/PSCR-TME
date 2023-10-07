
#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <forward_list>



using namespace std;
using namespace std::chrono;
class HashMap
{
	private:
		struct HashItem
		{
			const string Key;
			int Value;
			HashItem(const string& key, int value) : Key(key), Value(value) {}
		};
		size_t sz;
		size_t alloc_sz;
		vector<std::forward_list<HashItem>> Map;
	
	public:
		HashMap(size_t size)
		{
			this->alloc_sz = size;
			sz = 0;
			Map = vector<forward_list<HashItem>>(size);	
		}

		~HashMap()
		{
			
		}	

		size_t size()
		{
			return this->sz;
		}

		size_t alloc_size()
		{
			return this->alloc_sz;
		}

	    bool empty()
		{
			return sz==0;
		}

		forward_list<HashItem>& operator[] (size_t index)
		{
			if (index < alloc_sz)
			{
				return Map[index];
			}
			else
			{
				std::cout << "Index OverRange !\n" << std::endl;
				throw std::out_of_range("Index out of range");
			}
		}

		const forward_list<HashItem>& operator[] (size_t index) const
		{
			if (index < alloc_sz)
			{
				return Map[index];
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
        		vector<forward_list<HashItem>> newMap(newAllocSize);	
			
        		for (size_t i = 0; i < alloc_sz; i++)
        		{	
					for (HashItem item : Map[i])
					{
						size_t newHashCode = std::hash<string>{}(item.Key);
						size_t newIndex = newHashCode % newAllocSize;
						newMap[newIndex].push_front(item);
					}
        		}
        		alloc_sz = newAllocSize;	//更改alloc_sz
				Map.resize(newAllocSize);	//扩容Vector
        		Map.swap(newMap);			//把Map和newMap里的元素全部互换，包括内存地址和下标
    		}
		}

		void pushBack(const string& wd)
		{
			size_t hashCode = std::hash<string>{}(wd);
    		size_t index = hashCode % alloc_sz;

			bool isInclude = false;					
	
			for (HashItem& item : Map[index])
    		{
        		if (item.Key == wd)						//遍历整个list，确认有没有和我们现在的string一样的
        		{
            		item.Value++;						//如果有，那么计数+1，更改flah，跳出循环
            		isInclude = true;
            		break;
        		}
    		}	
			if (!isInclude)								//如果没有，那么说明是个新的
    		{
        		Map[index].push_front(HashItem(wd, 1));	//直接加在list后面，但是不需要扩容，因为这个格子已经存在了。
				ensureCapacity(sz+1);
				sz++;
    		}
				
		}

		int getValue(const string& wd) const
		{
   	 		size_t hashCode = std::hash<string>{}(wd);
    		size_t index = hashCode % alloc_sz;

    		for (const HashItem& item : Map[index])		// 遍历链表，寻找匹配的HashItem
    		{
        		if (item.Key == wd)						// 找到匹配的HashItem，返回其Value
        		{
            		return item.Value;
        		}
    		}												
    		return 0;									//没有就是0
		}

		//TME3
		class iterator
		{
			private:
				std::vector<forward_list<HashItem>>& buckets;
				size_t vit;				//当前下标
				typename std::forward_list<HashItem>::iterator lit;

				void moveToNextNonEmptyBucket() 
				{
					if(vit < buckets.size())
					{
						++vit;
            			while (vit < buckets.size() && buckets[vit].empty()) 
						{
                			++vit;
            			}

            			if (vit < buckets.size()) 
						{
                			lit = buckets[vit].begin();
           		 		}
					}
            		
        		}
			
			public:

				iterator(std::vector<std::forward_list<HashItem>>& map, size_t currentBucket, typename std::forward_list<HashItem>::iterator currentElement) : buckets(map), vit(currentBucket), lit(currentElement) 
				{
            		// 在构造迭代器时初始化
            		if (lit == buckets[vit].end()) 
					{
                		moveToNextNonEmptyBucket();
            		}
        		}


        		iterator& operator++() 
				{
            		if (lit != buckets[vit].end()) 
					{
                		++lit;
            		}

            		while (vit < buckets.size() && lit == buckets[vit].end()) 
					{
                		moveToNextNonEmptyBucket();
            		}

            		return *this;
        		}

        		bool operator!=(const iterator& other) const 
				{
            		return vit != other.vit || lit != other.lit;
        		}

        		/*HashItem& operator*() 
				{
            		
					return *lit;
       			}*/
				
				std::pair<std::string, int> operator*() 
				{
            		return std::make_pair(lit->Key, lit->Value);
        		}
		};

		iterator begin() 
		{
        	return iterator(Map, 0, Map[0].begin());
			
    	}

    	iterator end() 
		{
        	return iterator(Map, Map.size(), Map[Map.size()-1].end());
		}

};
#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <forward_list>


using namespace std;
using namespace std::chrono;	
    
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
};
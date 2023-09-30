#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <forward_list>


using namespace std;
using namespace std::chrono;
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
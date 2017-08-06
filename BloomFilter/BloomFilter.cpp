#include<iostream>
#include"BitSet.h"
using namespace std;


//布隆过滤器
//实现思路： 位图+hash
//hash 确定位置进行定位

template<class K>
struct _HashFunc1
{
	size_t BKDRHash(const char *str)
	{
		register size_t hash = 0;
		while (size_t ch = (size_t)*str++)
		{
			hash = hash * 131 + ch;   // 也可以乘以31、131、1313、13131、131313.. 

		}
		return hash;
	}

	size_t operator()(const string &s)
	{
		return BKDRHash(s.c_str());
	}
};

template<class K>
struct _HashFunc2
{
	size_t SDBMHash(const char *str)
	{
		register size_t hash = 0;
		while (size_t ch = (size_t)*str++)
		{
			hash = 65599 * hash + ch;
			//hash = (size_t)ch + (hash << 6) + (hash << 16) - hash; 
		}
		return hash;
	}

	size_t operator()(const string &s)
	{
		return SDBMHash(s.c_str());
	}
};

template<class K>
struct _HashFunc3
{
	size_t RSHash(const char *str)
	{
		if (!*str)        
			return 0;
		register size_t hash = 1315423911;
		while (size_t ch = (size_t)*str++)
		{
			hash ^= ((hash << 5) + ch + (hash >> 2));
		}
		return hash;
	}

	size_t operator()(const string &s)
	{
		return RSHash(s.c_str());
	}
};

template<class K>
struct _HashFunc4
{
	size_t RSHash(const char *str)
	{
		register size_t hash = 0;
		size_t magic = 63689;
		while (size_t ch = (size_t)*str++)
		{
			hash = hash * magic + ch;
			magic *= 378551;
		}
		return hash;
	}

	size_t operator()(const string&s)
	{
		return RSHash(s.c_str());
	}
};

template<class K>
struct _HashFunc5
{
	size_t RSHash(const char *str)
	{
		register size_t hash = 0;
		size_t ch;
		for (long i = 0; ch = (size_t)*str++; i++)
		{
			if ((i & 1) == 0)
			{
				hash ^= ((hash << 7) ^ ch ^ (hash >> 3));
			}
			else
			{
				hash ^= (~((hash << 11) ^ ch ^ (hash >> 5)));
			}
		}
		return hash;
	}

	size_t operator()(const string &s)
	{
		return RSHash(s.c_str());
	}
};
template<class K=string,
class HashFunc1=_HashFunc1<K>,
class HashFunc2=_HashFunc2<K>,
class HashFunc3=_HashFunc3<K>,
class HashFunc4=_HashFunc4<K>,
class HashFunc5 =_HashFunc5<K >>
class BloomFilter
{
public:
	BloomFilter(size_t size)
	{
		_capacity = _GetNextPrime(size);
		_bitmap.Resize(_capacity);
	}

	void Set(const K &key)
	{
		size_t index1 = HashFunc1()(key);
		size_t index2 = HashFunc2()(key);
		size_t index3 = HashFunc3()(key);
		size_t index4 = HashFunc4()(key);
		size_t index5 = HashFunc5()(key);

		_bitmap.Set(index1%_capacity);
		_bitmap.Set(index2%_capacity);
		_bitmap.Set(index3%_capacity);
		_bitmap.Set(index4%_capacity);
		_bitmap.Set(index5%_capacity);
	}

	bool IsIn(const K&key)
	{
		size_t index1 = HashFunc1()(key);
		if (!_bitmap.Test(index1%_capacity))
		{
			return false;
		}
		size_t index2 = HashFunc2()(key);
		if (!_bitmap.Test(index2%_capacity))
		{
			return false;
		}
		size_t index3 = HashFunc3()(key);
		if (!_bitmap.Test(index3%_capacity))
		{
			return false;
		}
		size_t index4 = HashFunc4()(key);
		if (!_bitmap.Test(index4%_capacity))
		{
			return false;
		}
		size_t index5 = HashFunc5()(key);
		if (!_bitmap.Test(index5%_capacity))
		{
			return false;
		}
		return true;
	}


protected:
	unsigned long _GetNextPrime(unsigned long num)
	{
		const int _PrimeSize = 28;
		static const unsigned long _PrimeList[_PrimeSize] =
		{
			53ul, 97ul, 193ul, 389ul, 769ul,
			1543ul, 3079ul, 6151ul, 12289ul, 24593ul,
			49157ul, 98317ul, 196613ul, 393241ul, 786433ul,
			1572869ul, 3145739ul, 6291469ul, 12582917ul, 25165843ul,
			50331653ul, 100663319ul, 201326611ul, 402653189ul, 805306457ul,
			1610612741ul, 3221225473ul, 4294967291ul
		};
		size_t pos = 0;
		while (pos < _PrimeSize)
		{
			if (_PrimeList[pos] > num)
			{
				break;
			}
			++pos;
		}
		return _PrimeList[pos];
	}


private:
	BitSet _bitmap;
	size_t _capacity;

};



void Test2()
{
	BitSet bitmap(60);
	for (int i = 4; i < 10; i++)
	{
		bitmap.Set(i);
	}

	for (int i = 0; i < 20; i++)
	{
		cout << i << ":" << bitmap.Test(i) << endl;
	}

}

void Test1()
{ 
	BloomFilter<>bf(30);
	string s11("www");
	bf.Set(s11);
	bf.Set("张三");
	bf.Set("王五");
	bf.Set("李四");
	
	cout << bf.IsIn("布隆过滤器") << endl;
	cout << bf.IsIn("ww") << endl;
	cout << bf.IsIn("www") << endl;
	cout << bf.IsIn("王五") << endl;
}


int main()
{
	Test1();
	return 0;
}
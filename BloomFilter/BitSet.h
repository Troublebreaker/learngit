#include<iostream>
#include<vector>

using namespace std;

//
//1.【基础题】--实现一个位图。
//位图特点： 节省空间，适合数据密集的情况


class BitSet
{
public:
	BitSet()
		:_size(0)
	{
	}

	BitSet(size_t size)
	{
		_size = (size >> 5) + 1;
		_array.resize(_size);
	}

	bool Set(int num)
	{
		//定位到位置  进行置1
		//比特位操作  
		//位运算： & |  

		//找到对应字节，将其对应比特位进行位运算

		int index = num >> 5;
		if (index >= _size)
		{
			return false;
		}
		int bit = num % 32;  //该位置进行置1  ,
		_array[index] = _array[index] | (1 << (31 - bit));
		return true;

	}

	void Resize(size_t num)
	{
		_array.resize(num);
		_size = num;
	}
	bool ReSet(int num)
	{
		//找到对应字节，将其对应比特位进行位运算

		int index = num >> 5;
		if (index >= _size)
		{
			return false;
		}
		int bit = num % 32;  //该位置进行置0,其他位保持不变
		_array[index] = _array[index] & (~(1 << (31 - bit)));
		return true;
	}

	bool Test(int num)
	{
		int index = num >> 5;
		int bit = num % 32;  //该位置进行置0,其他位保持不变
		if (_array[index] & (1 << (31 - bit)))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

private:
	vector<int >  _array;
	size_t _size;
};


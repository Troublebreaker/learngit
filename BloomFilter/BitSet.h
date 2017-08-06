#include<iostream>
#include<vector>

using namespace std;

//
//1.�������⡿--ʵ��һ��λͼ��
//λͼ�ص㣺 ��ʡ�ռ䣬�ʺ������ܼ������


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
		//��λ��λ��  ������1
		//����λ����  
		//λ���㣺 & |  

		//�ҵ���Ӧ�ֽڣ������Ӧ����λ����λ����

		int index = num >> 5;
		if (index >= _size)
		{
			return false;
		}
		int bit = num % 32;  //��λ�ý�����1  ,
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
		//�ҵ���Ӧ�ֽڣ������Ӧ����λ����λ����

		int index = num >> 5;
		if (index >= _size)
		{
			return false;
		}
		int bit = num % 32;  //��λ�ý�����0,����λ���ֲ���
		_array[index] = _array[index] & (~(1 << (31 - bit)));
		return true;
	}

	bool Test(int num)
	{
		int index = num >> 5;
		int bit = num % 32;  //��λ�ý�����0,����λ���ֲ���
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


#pragma once
#include <vector>
#include <algorithm>
using namespace std;

template<class T>
class SortedArray
{
public:
	SortedArray();			//���캯������ʼ����С
	~SortedArray();

	void ReSort();				//����
	void Insert(const T& elem);	//����
	void Erase(int index);		//�±�ɾ��
	void Erase(const T& elem);	//Ԫ��ɾ��

	T& operator[] (int i);
	const T& operator[] (int i) const;

private:
	vector<T>* data;
	int size;
};


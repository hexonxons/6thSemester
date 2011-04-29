#include <iostream>
#include <algorithm>
#include <vector>

using std::cin;
using std::vector;
using std::cout;

struct pair
{
	int first;
	int second;
};



int compare(struct pair &left, struct pair &right)
{
	return left.second > right.second;
}

int main()
{
	// количество точек
	int num;
	int i;
	// массив точек
	struct pair *arr;
	struct pair *leftTree;
	struct pair *rightTree;
	vector<struct pair *> dataY;
	vector<struct pair *> dataX;


	cin >> num;
	arr = new struct pair[num];
	// считывание точек
	for(i = 0; i < num; ++i)
	{
		cin >> arr[i].first;
		cin >> arr[i].second;
		dataY.push_back(&arr[i]);
		dataX.push_back(&arr[i]);
	}
	// проходим начиная с max Y координаты
	


	return 0;
}
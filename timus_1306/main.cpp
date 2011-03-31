#include <iostream>
#include <queue>
#include <algorithm>
#include <iomanip>

using namespace std;

int main(int argc, char **argv)
{
	priority_queue<unsigned int> arr;
	unsigned int num;
	unsigned int i;
	unsigned int tmp;
	
	cin >> num;
	for(i = 0; i < num / 2 + 1; ++i)
	{
		cin >> tmp;
		arr.push(tmp);	
	}
	for(i = num / 2 + 1; i < num; ++i)
	{
		cin >> tmp;
		if(tmp < arr.top())
		{
			arr.pop();
			arr.push(tmp);
		}
	}
	
	if(num % 2)
	{
		cout.setf(ios::fixed);
		cout << setprecision(1) << (double) arr.top();	
	}
	else
	{
		double a = (double)arr.top() / 2;
		arr.pop();
		double b = (double)arr.top() / 2;
		cout.setf(ios::fixed);
		cout << setprecision(1) << a + b;		
	}

	return 0;
}

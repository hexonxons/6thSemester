#include <stdio.h>
#include <stdlib.h>

struct pair
{
	int first;
	int second;
};
int d = 0;

struct pair
	factorize(int num)
{
	int i;
	struct pair ret;
	ret.first = 0;
	ret.second = 0;
	for (i = 2; i < sqrt(num); ++i)
	{
		if(num % i == 0)
		{
			ret.first = i;
			ret.second = num / i;
			return ret;
		}
	}
	return ret;
};

struct pair
	reverseEuclidAlgo(int a, int b)
{
	struct pair ret;
	int q;
	int r;
	int tmpX;
	int tmpY;
	int x1 = 0;
	int x2 = 1;
	int y1 = 1;
	int y2 = 0;

	if (b == 0)
	{
		ret.first = 1;
		ret.second = 0;
		return ret;
	}
	while (b > 0)
	{
		// целая часть
		q = a / b;
		// остаток
		r = a - q * b;
		tmpX = x2 - q * x1;
		tmpY = y2 - q * y1;
		a = b;
		b = r;
		x2 = x1;
		x1 = tmpX;
		y2 = y1;
		y1 = tmpY;
	}
	ret.first = x2;
	ret.second = y2;
	d = a;
	return ret;	
}


int
	main()
{
	int numOfTests;
	int *tests;
	int i;
	struct pair fNum;

	scanf("%d", &numOfTests);
	tests = (int *) malloc (numOfTests * sizeof(int));
	for (i = 0; i < numOfTests; ++i)
	{
		scanf("%d", &tests[i]);
	}
	for (i = 0; i < numOfTests; ++i)
	{
		struct pair val;
		fNum = factorize(tests[i]);
		val = reverseEuclidAlgo(fNum.first, fNum.second);
		if (val.first >= 0)
		{
			while (val.second <= 0)
				val.second += fNum.first;
			printf("0 1 %d %d\n", fNum.first * val.first, fNum.second * val.second);
		}
		else
		{
			while (val.first <= 0)
				val.first += fNum.second;
			printf("0 1 %d %d\n", fNum.second * val.second, fNum.first * val.first);
		}
	}

	return 0;
}
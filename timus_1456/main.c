#include <stdio.h>

int phi(int n)
{
	int ret = 1, p;
	for(p = 2; p <= n / p; ++p)
	{
		if (n % p == 0)
		{
			n /= p;
			while(n % p == 0) 
			{
				n /= p;
				ret *= p;
			}
			ret *= p - 1;
		}
	}
	return n > 1 ? ret * (n - 1) : ret;
}


int main()
{
	int N;
	__int64 i;
	int mod = 0;
	scanf("%d", &N);
	for (i = 3; i < 20000; ++i)
	{
		if (N == phi(i))
		{
			printf("%d", i);
			return 0;
		}
	}
	printf("0");
	return 0;
}
#include <stdio.h>
#include <stdlib.h>


char *mergesort(char *arr, unsigned int arrSz);
char *merge(char *left, char *right, unsigned int leftSz, unsigned int rightSz);

int main(int argc, char **argv)
{
	int num;
	int i;
	char *arr;
	char *res;
	scanf("%d", &num);
	arr = (char *) malloc(num * sizeof(char));
	for(i = 0; i < num; ++i)
	{
		scanf("%c", &arr[i]);
	}
	res = mergesort(arr, num);
	for(i = 0; i < num; ++i)
		printf("%c", res[i]);
	return 0;
}

char *merge(char *left, char *right, unsigned int leftSz, unsigned int rightSz)
{
	char *result = (char *)malloc((leftSz + rightSz) * sizeof(char));
	unsigned int rightPtr = 0;
	unsigned int leftPtr = 0;
	unsigned int resPtr = 0;

	while(rightPtr < rightSz && leftPtr < leftSz)
	{
		if(left[leftPtr] <= right[rightPtr])
		{
			result[resPtr] = left[leftPtr];
			++leftPtr;
			++resPtr;
		}
		if(left[leftPtr] > right[rightPtr])
		{
			result[resPtr] = right[rightPtr];
			++rightPtr;
			++resPtr;
		}
	}
	if(leftPtr < leftSz)
	{
		unsigned int i;
		for(i = leftPtr; i < leftSz - i; ++i)
		{
			result[resPtr] = left[i];
			++resPtr;			
		}

	}

	if(rightPtr < rightSz)
	{
		unsigned int i;
		for(i = rightPtr; i < rightSz; ++i)
		{
			result[resPtr] = right[i];
			++resPtr;			
		}			
	}

	return result;
}

char *mergesort(char *arr, unsigned int arrSz)
{
	char *result = (char *)malloc(arrSz * sizeof(char));
	char *left;
	char *right;
	if(arrSz <= 1)
		return arr;
	else
	{
		int middle = arrSz/2;
		int i;
		left = (char *) malloc(middle * sizeof(char));
		right = (char *) malloc((arrSz - middle) * sizeof(char));

		for(i = 0; i < middle; ++i)
			left[i] = arr[i];

		for(i = 0; i < arrSz - middle; ++i)
			right[i] = arr[arrSz - middle + i - arrSz % 2];
		left = mergesort(left, middle);
		right = mergesort(right, arrSz - middle);
		result = merge(left, right, middle, arrSz - middle);
	}	
	return result;
}
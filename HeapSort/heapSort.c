#include <stdio.h>
#include <stdlib.h>

void Heapify(int *arr, unsigned int top, unsigned int heapSz);
void BuildMaxHeap(int *arr, unsigned int heapSz);
void HeapSort(int *arr, unsigned int sz);

int main(int argc, char **argv)
{
	int *arr = NULL;
	int arrSz;
	int i;
	
	scanf("%d", &arrSz);
	arr = malloc(arrSz * sizeof(int));
	
	if(!arr) 
	{
		printf("ERROR! Memory Alloc");
		exit(-1);
	}
	
	// read data
	for(i = 0; i < arrSz; i++)
		scanf("%d", &arr[i]);
		
	HeapSort(arr, arrSz);
		
	for(i = 0; i < arrSz; ++i)
	{
		printf("%d ", arr[i]);
	}
	free(arr);
	return 0;
}


void Heapify(int *arr, unsigned int top, unsigned int heapSz)
{
	unsigned int left;
	unsigned int right;
	unsigned int largest;
	int temp;
	
	left = (top << 1) + 1; //Left
	right = left + 1; 			//Right
	
	if(left < heapSz && arr[left] > arr[top])
		largest = left;
	else
		largest = top;
		
	if(right < heapSz && arr[right] > arr[largest])
		largest = right;
		
	if(largest != top)
	{
		temp = arr[top];
		arr[top] = arr[largest];
		arr[largest] = temp;
		Heapify(arr, largest, heapSz);
	}
}

void BuildMaxHeap(int *arr, unsigned int heapSz)
{
	unsigned int i = heapSz >> 1;
	while(i > 0)
	{
		--i;
		Heapify(arr, i, heapSz);
	}
}

void HeapSort(int *arr, unsigned int sz)
{
	unsigned int i;
	unsigned int heapSz = sz;
	int temp;
	
	BuildMaxHeap(arr, sz);
	i = sz;
	while(i > 0)
	{
		--i;
		--heapSz;
		temp = arr[i];
		arr[i] = arr[0];
		arr[0] = temp;
		Heapify(arr, 0, heapSz);
	}
}

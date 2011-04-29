#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

struct order
{
	int *arr;
	int numOfElemToDelete;
	int currentSz;
};

void removeElem(struct order *ord, int step)
{
	int tmpstp = step;

	fprintf(stdout, "%d ", (int)ord->arr[ord->numOfElemToDelete]);
	if (ord->currentSz - 1 == 0)
	{
		ord->currentSz = 0;
		return;
	}
	// копируем все от удаляемого элемента до конца
	memcpy(ord->arr + ord->numOfElemToDelete, ord->arr + ord->numOfElemToDelete + 1, (ord->currentSz - ord->numOfElemToDelete) * sizeof(int));
	ord->currentSz--;
	if (ord->currentSz >= ord->numOfElemToDelete + step)
	{
		ord->numOfElemToDelete += step - 1;
	}
	else
	{
		while (ord->numOfElemToDelete + tmpstp > ord->currentSz)
		{
			tmpstp -= ord->currentSz;
		}
		ord->numOfElemToDelete = ord->numOfElemToDelete + tmpstp - 1;
	}
}
int main()
{
	struct order *ord = (struct order *) malloc (sizeof(struct order));
	int num = 0;
	int step = 0;
	int i = 0;
	scanf("%d", &num);
	scanf("%d", &step);

	// инициализачия начального положения
	ord->arr = (int *) malloc(num * sizeof(int));
	for(i = 0; i < num; i++)
		ord->arr[i] = i + 1;
	ord->numOfElemToDelete = step - 1;
	ord->currentSz = num;

	while (ord->currentSz != 0)
	{
		removeElem(ord, step);
	}

	return 0;
}
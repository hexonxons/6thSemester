#include <stdio.h>
#include <stdlib.h>

struct pair
{
	int ID;
	int tasks;	
};

int main(int argc, char **argv)
{
	// количество команд
	int teams;
	int i;
	int maxTask = 0;
	//int last;
	int *sortArr;
	struct pair *arr, *retarr;
	
	// считывем количество команд
	scanf("%d", &teams);
	
	arr = (struct pair *) malloc((teams + 1) * sizeof(struct pair));
	retarr = (struct pair *) malloc((teams + 1)* sizeof(struct pair));

	// записываем в массив и находим максимальное
	// количество решенных задач
	for(i = 1; i < teams + 1; ++i)
	{
		scanf("%d", &arr[i].ID);
		scanf("%d", &arr[i].tasks);
		if(arr[i].tasks > maxTask)
			maxTask = arr[i].tasks;
	}
	
	// инициализируем нулями дополнительный массив
	sortArr = (int *) malloc((maxTask + 1)  * sizeof(int));
	memset(sortArr, 0, (maxTask + 1) * sizeof(int));
	
	// в i-м элементе доп. таблицы находится количество
	// команд, решивших i задач
	for(i = 1; i <= teams; i++)
		sortArr[arr[i].tasks ] += 1;

	// идем от конца доп. массива и прибавляем к текущему элементу предыдущий
	for(i = maxTask; i > 0; --i)
		sortArr[i - 1] += sortArr[i];
	
	for(i = teams; i > 0; i--)
	{
		struct pair tmpPair;
		tmpPair.ID = arr[i].ID;
		tmpPair.tasks = arr[i].tasks;
		retarr[sortArr[arr[i].tasks]] = tmpPair;
		--sortArr[arr[i].tasks];
	}

	for (i = 1; i < teams + 1; i++)
	{
		printf("%d %d\n", retarr[i].ID, retarr[i].tasks);
	}	
	
	free(arr);
	free(retarr);
	free(sortArr);
	return 0;
}
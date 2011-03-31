#include <stdlib.h>
#include <stdio.h>

struct stack
{
	struct stack *next;
	unsigned int value;
};

struct stack *pop(struct stack *ptr)
{
	struct stack* tmp;
	tmp = ptr->next;
	free(ptr);
	return tmp;
}

struct stack *push(struct stack *ptr, unsigned int value)
{
	struct stack *tmp = (struct stack *)malloc(sizeof(struct stack));
	tmp->next = ptr;
	tmp->value = value;
	return tmp;
}

int main()
{
	// входные данные
	struct stack *order;
	unsigned int num;
	unsigned int i;
	unsigned int LocalValue;
	// начало стека
	struct stack *orderBegin;
	// правильный стек
	struct stack *trueOrder = NULL;
	order = (struct stack *)malloc(sizeof(struct stack));
	order->next = NULL;

	scanf("%d", &num);
	orderBegin = order;
	for(i = 0; i < num; ++i)
	{
		struct stack *LocalStack;
		scanf("%d", &LocalValue);
		if(i + 1 < num)
		{
			LocalStack = (struct stack *)malloc(sizeof(struct stack));
			order->value = LocalValue;
			order->next = LocalStack;
			order = LocalStack;		
		}
		else
		{
			order->value = LocalValue;
			order->next = NULL;
		}
	}
	order = orderBegin;
	i = 1;
	while (1)
	{
		unsigned int currValue = order->value;
		if(i > currValue)
		{
			printf("Cheater");
			return 0;
		}
		for(i; i <= currValue; ++i)
			trueOrder = push(trueOrder, i);
		while (trueOrder != NULL && order->value == trueOrder->value)
		{
		  	order = pop(order);
			trueOrder = pop(trueOrder);
		}
		if(order == NULL && trueOrder == NULL)
		{
			printf("Not a proof");
			return 0;
		}
	}


	return 0;
}
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <conio.h>

struct pair
{
	int first;
	int second;
};

int main()
{
	struct pair size;
	struct pair start;
	struct pair finish;
	struct pair	*wallsHorizontal = NULL;
	struct pair *wallsVertical = NULL;
	unsigned char delim;
	int flag = 1;
	int i = 0;
	int X;
	int Y;

	
	scanf("%d", &size.first);
	scanf("%d", &size.second);
	scanf("%d", &start.first);
	scanf("%d", &start.second);
	scanf("%d", &finish.first);
	scanf("%d", &finish.second);
	scanf("%c", &delim);
	while(flag)
	{
		scanf("%c", &delim);
		if (!isdigit(delim))
		{	
			if (delim == 'v')
				i = 0;
			if (delim == 'h')
			{
				i = 0;
				flag = 2;
			}
			continue;
		}
		if (flag == 1)
		{
			ungetch(delim);
			wallsVertical = (struct pair *) realloc(wallsVertical, (i + 1) * sizeof(struct pair));
			scanf("%d", &X);
			scanf("%d", &Y);
			wallsVertical[i].first = X;
			wallsVertical[i].second = Y;
			++i;
		}
		if (flag == 2)
		{
			ungetch(delim);
			wallsHorizontal = (struct pair *) realloc(wallsHorizontal, (i + 1) * sizeof(struct pair));
			scanf("%d", &X);
			scanf("%d", &Y);
			wallsHorizontal[i].first = X;
			wallsHorizontal[i].second = Y;
			++i;
		}
	}

	return 0;
}
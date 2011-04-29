#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
#include <cstdio>

using std::vector;
using std::list;

struct cluster
{
	int sizeOfCluster;
	list<int> listOfVertexes;
};

struct node
{
	int first;
	int second;
};

struct MyStack
{
	struct MyStack *next;
	struct node value;
};

struct MyStack *pop(struct MyStack *ptr)
{
	struct MyStack* tmp;
	tmp = ptr->next;
	free(ptr);
	return tmp;
}

struct MyStack *push(struct MyStack *ptr, node value)
{
	struct MyStack *tmp = (struct MyStack *)malloc(sizeof(struct MyStack));
	tmp->next = ptr;
	tmp->value = value;
	return tmp;
}


vector<cluster *> data;
int *vertex;

int addEdge(int firstNode, int secondNode,  int numOfClusters)
{
	// если первая вершина не принадлежит к какому-либо кластеру
	if (vertex[firstNode] < 0)
	{
		// если ни одна из вершин не принадлежит к какому-либо кластеру
		if (vertex[secondNode] < 0)
		{
			// уменьшаем количество кластеров
			--numOfClusters;
			// создаем новый кластер с 2мя вершинами
			cluster *newCluster = new cluster;
			newCluster->sizeOfCluster = 2;
			newCluster->listOfVertexes.push_back(firstNode);
			newCluster->listOfVertexes.push_back(secondNode);
			// кладем его в вектор кластеров
			data.push_back(newCluster);
			// указываем на кластеры
			vertex[firstNode] = data.size() - 1;
			vertex[secondNode] = data.size() - 1;
		}
		// если вторая вершина лежит в каком-либо кластере
		// количество кластеров уменьшается на 1
		else
		{
			--numOfClusters;
			// увеличиваем количество вершин в кластере 2й вершины
			data[vertex[secondNode]]->sizeOfCluster++;
			// кладем 1ю вершину в кластер 2й
			data[vertex[secondNode]]->listOfVertexes.push_back(firstNode);
			vertex[firstNode] = vertex[secondNode];
		}
	}
	else
		// если нет кластера только у 2й вершины
		// количество кластеров уменьшается на 1
		if (vertex[secondNode] < 0)
		{
			--numOfClusters;
			data[vertex[firstNode]]->sizeOfCluster++;
			data[vertex[firstNode]]->listOfVertexes.push_back(secondNode);
			vertex[secondNode] = vertex[firstNode];
		}
		else
			// если обе вершины лежат в кластере
			if (vertex[secondNode] >= 0 && vertex[firstNode] >= 0)
			{
				// если они лежат в одном кластере, то добавление нового ребра не поменяет кол-ва кластеров
				// пропускаем
				if(vertex[secondNode] == vertex[firstNode]);
				else
					// если размер кластера второй вершины > размера кластера 1й вершины
					// количество кластеров уменьшится на 1
					if (data[vertex[secondNode]]->sizeOfCluster > data[vertex[firstNode]]->sizeOfCluster)
					{
						data[vertex[secondNode]]->sizeOfCluster += data[vertex[firstNode]]->sizeOfCluster;
						list <int> tmp = data[vertex[firstNode]]->listOfVertexes;
						int numOfClusterToDel = vertex[firstNode];
						list<int>::iterator It = tmp.begin();
						while(It != tmp.end())
						{
							vertex[*It] = vertex[secondNode];
							++It;
						}
						data[vertex[secondNode]]->listOfVertexes.splice(data[vertex[secondNode]]->listOfVertexes.end(), tmp);
						data[numOfClusterToDel]->listOfVertexes.clear();
						delete(data[numOfClusterToDel]);
						data[numOfClusterToDel]=NULL;
						--numOfClusters;
					}
					else
					{
						data[vertex[firstNode]]->sizeOfCluster += data[vertex[secondNode]]->sizeOfCluster;
						list <int> tmp = data[vertex[secondNode]]->listOfVertexes;
						int numOfClusterToDel = vertex[secondNode];
						list<int>::iterator It = tmp.begin();
						while(It != tmp.end())
						{
							vertex[*It] = vertex[firstNode];
							++It;
						}
						data[vertex[firstNode]]->listOfVertexes.splice(data[vertex[firstNode]]->listOfVertexes.end(), tmp);
						data[numOfClusterToDel]->listOfVertexes.clear();
						delete(data[numOfClusterToDel]);
						data[numOfClusterToDel]=NULL;
						--numOfClusters;
					}			
			}
			return numOfClusters;
}

int main(int argc, char **argv)
{
	// количество вершин
	int numOfNodes = 0;
	// количество ребер
	int numOfEdges = 0;
	// количество ребер для удаления
	int numOfEdgesToDelete = 0;
	// массив ребер
	vector<node> edges;
	// стек ребер для удаления
	MyStack *edgesToDel = (struct MyStack *) malloc(sizeof(struct MyStack));
	int numOfClusters;
	int i;

	// считываем количество вершин
	scanf("%d", &numOfNodes);
	// считываем количество ребер
	scanf("%d", &numOfEdges);
	// кладем все ребра в массив
	for(i = 0; i < numOfEdges; ++i)
	{
		node tmp;
		scanf("%d", &tmp.first);
		scanf("%d", &tmp.second);
		edges.push_back(tmp);
	}
	vertex = new int[numOfNodes];
	memset(vertex, -1, numOfNodes * sizeof(int));
	numOfClusters = numOfNodes;
	// считываем номера ребер, которые нужно будет удалить
	scanf("%d", &numOfEdgesToDelete);

	// записываем ребра для удаления
	for(i = 0; i < numOfEdgesToDelete; ++i)
	{
		int numToDel = 0;
		scanf("%d", &numToDel);

		edgesToDel = push(edgesToDel, edges.at(numToDel - 1));
		edges[numToDel - 1].first = -1;
		edges[numToDel - 1].second = -1;
	}

	// проходим по всему массиву и ищем количество компонент
	for(i = 0; i < numOfEdges; ++i)
	{
		// если текущее ребро будет удалено , то пока пропускаем его
		if(edges[numOfEdges - i - 1].first < 0)
			continue;
		int firstNode = edges[numOfEdges - i - 1].first - 1;
		int secondNode = edges[numOfEdges - i - 1].second - 1;
		numOfClusters = addEdge(firstNode, secondNode,  numOfClusters);
	}

	// Итак, мы положили все ребра и знаем количество кластеров
	// уииии!
	
	vector <int> pararam;
	for(i = 0; i < numOfEdgesToDelete; ++i)
	{
		pararam.push_back(numOfClusters);
		int firstNode = edgesToDel->value.first - 1;
		int secondNode = edgesToDel->value.second - 1;
		numOfClusters = addEdge(firstNode, secondNode,  numOfClusters);
		edgesToDel = pop(edgesToDel);		
	}

	for(i = pararam.size() - 1; i > -1; --i)
	{
		printf("%d ", pararam[i]);
	}
	return 0;
}
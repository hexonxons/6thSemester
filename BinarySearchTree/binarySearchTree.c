#include <stdio.h>
#include <stdlib.h>
#include "binarySearchTree.h"


struct treeNode *initTree(struct nodeData elem)
{
	struct treeNode* root = (struct treeNode *) malloc(sizeof(struct treeNode));
	root->parent = root;
	root->left = NULL;
	root->right = NULL;
	root->data = (struct nodeData *) malloc(sizeof(struct nodeData));
	root->data->key = elem.key;
	root->data->str = elem.str;
	return root;
}

struct treeNode *makeNode()
{
	struct treeNode* node = (struct treeNode *) malloc(sizeof(struct treeNode));
	node->parent = NULL;
	node->left = NULL;
	node->right = NULL;
	node->data = (struct nodeData *) malloc(sizeof(struct nodeData));
	return node;
}

void insertNode(struct treeNode *tree, struct nodeData elem)
{
	// если ключ >= ключа в дереве
	if (keyCompare(elem.key, tree->data->key) != -1)
	{
		// если есть дети у вершины
		if(tree->right != NULL)
			insertNode(tree->right, elem);
		else
		{
			struct treeNode *node = makeNode();			
			node->parent = tree;
			node->data->key = elem.key;
			node->data->str = elem.str;
			tree->right = node;
			return;
		}
	}
	// если ключ < ключа в дереве
	if (keyCompare(elem.key, tree->data->key) == -1)
	{
		if(tree->left != NULL)
			insertNode(tree->left, elem);			
		else
		{
			struct treeNode *node = makeNode();
			node->parent = tree;
			node->data->key = elem.key;
			node->data->str = elem.str;
			tree->left = node;
			return;
		}
	}
}

struct treeNode *findNode(struct treeNode *tree, key_t key)
{
	if(tree->right == NULL && tree->left == NULL && tree->parent == NULL)
		return NULL;
	if(keyCompare(key, tree->data->key) == 0)
		return tree;
	if(keyCompare(key, tree->data->key) > 0)
		return findNode(tree->right, key);
	if(keyCompare(key, tree->data->key) < 0)
		return findNode(tree->left, key);
	// for avoid warning in codelite
	return NULL;
}

// TODO память. valgrind
void removeNode(struct treeNode *tree, key_t key)
{
	if(tree->data == NULL)
		return;
	if(keyCompare(key, tree->data->key) == 0)
	{
		// 1 вариант: у элемента на удаление нет детей
		if(tree->left == NULL && tree ->right == NULL)
		{
			// копируем указатель на родителя
			struct treeNode *nodeParent = tree->parent;
			free(tree->data);
			free(tree);

			// обнуление ссылки у родителя
			if(nodeParent->left == tree)
				nodeParent->left = NULL;
			else
				nodeParent->right = NULL;
			return;			
		}
		// 2 вариант: если есть оба ребенка
		if (tree->left != NULL && tree->right != NULL)
		{
			struct treeNode *tempNode = tree->right;
			struct nodeData tempObj;
			
			// ищем самый левый элемент правого поддерева
			while(tempNode->left != NULL)
				tempNode = tempNode->left;
			// копировние data самого левого элемента
			tempObj.key = tempNode->data->key;
			tempObj.str = tempNode->data->str;
			//удаляем этот самый левый элемент
			removeNode(tree, tempObj.key);
			
			// заполняем элемент для удаления новыми данными. Все ссылки остаются преждними
			tree->data->str = tempObj.str;
			tree->data->key = tempObj.key;
			return;
			
		}
		// 3 вариант: нет левого ребенка
		if(tree->left == NULL)
		{
			struct treeNode *tempNode = tree->right;
			struct nodeData tempObj;

			tempObj.key = tempNode->data->key;
			tempObj.str = tempNode->data->str;

			tree->right = tempNode->right;
			tree->left = tempNode->left;
			tree->data->key = tempObj.key;
			tree->data->str = tempObj.str;

			if (tempNode->left != NULL)
			{
				tempNode->left->parent = tree;
			}
			if (tempNode->right != NULL)
			{
				tempNode->right->parent = tree;
			}
			
			free(tempNode->data);
			free(tempNode);
			return;

		}
		// 3 вариант: нет правого ребенка
		if(tree->right == NULL)
		{
			struct treeNode *tempNode = tree->left;
			struct nodeData tempObj;

			tempObj.key = tempNode->data->key;
			tempObj.str = tempNode->data->str;

			tree->right = tempNode->right;
			tree->left = tempNode->left;
			tree->data->key = tempObj.key;
			tree->data->str = tempObj.str;

			if (tempNode->left != NULL)
			{
				tempNode->left->parent = tree;
			}
			if (tempNode->right != NULL)
			{
				tempNode->right->parent = tree;
			}

			free(tempNode->data);
			free(tempNode);		
			return;
		}
	}

	if(keyCompare(key, tree->data->key) > 0)
		removeNode(tree->right, key);
	if(keyCompare(key, tree->data->key) < 0)
		removeNode(tree->left, key);

	
}


int keyCompare(key_t left, key_t right)
{
	if(left < right)
		return -1;
	else
		return left > right;
}

void memAllocErrorCheck(const char *file, const unsigned int line, const void *obj)
{
	if (obj == NULL)
		fprintf(stderr,"Error while memory allocation in %s, on line %u", file, line);
	//exit(-1);
}
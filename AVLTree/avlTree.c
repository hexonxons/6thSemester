#include <stdio.h>
#include <stdlib.h>
#include "avlTree.h"


struct treeNode *initTree(struct nodeData elem)
{
	// Выделяем память
	struct treeNode* root = (struct treeNode *) malloc(sizeof(struct treeNode));
	memAllocErrorCheck(__FILE__, __LINE__, root);
	// Родитель корня - он сам
	root->parent = root;
	root->left = NULL;
	root->right = NULL;
	root->data = (struct nodeData *) malloc(sizeof(struct nodeData));
	memAllocErrorCheck(__FILE__, __LINE__, root->data);
	root->data->key = elem.key;
	root->data->str = elem.str;
	return root;
}

struct treeNode *makeNode()
{
	// Выделяем память
	struct treeNode* node = (struct treeNode *) malloc(sizeof(struct treeNode));
	memAllocErrorCheck(__FILE__, __LINE__, node);
	// зануляем указатели на все внутренние структуры
	node->parent = NULL;
	node->left = NULL;
	node->right = NULL;
	node->data = (struct nodeData *) malloc(sizeof(struct nodeData));
	memAllocErrorCheck(__FILE__, __LINE__, node->data);
	return node;
}

int treeDepth(struct treeNode *node)
{
	int rightDepth = 0;
	int leftDepth = 0;

	if (node == NULL)
		return 0;

	leftDepth = treeDepth(node->left);
	rightDepth = treeDepth(node->right);

	if (rightDepth > leftDepth)
		return 1 + rightDepth;
	else
		return 1 + leftDepth;	
}

struct treeNode *insertNode(struct treeNode *tree, struct nodeData elem)
{
	// если ключ >= ключа в дереве
	if (keyCompare(elem.key, tree->data->key) != -1)
	{
		// если есть дети у вершины
		if(tree->right != NULL)
		{
			// вставляем в правое поддерево
			tree->right = insertNode(tree->right, elem);
			// проверяем на сбалансированность
			if (treeDepth(tree->right) - treeDepth(tree->left) == 2)
			{
				if(elem.key > tree->right->data->key)
				{
					tree = singleRightRotate(tree);
					return tree;
				}
				else
				{
					tree = doubleRightRotate(tree);			
					return tree;
				}
			}
		}
		else
		{
			// если детей нет, то создаем лист
			struct treeNode *node = makeNode();			
			node->parent = tree;
			node->data->key = elem.key;
			node->data->str = elem.str;
			tree->right = node;
			return tree;
		}
	}
	// если ключ < ключа в дереве
	if (keyCompare(elem.key, tree->data->key) == -1)
	{
		if(tree->left != NULL)
		{
			// вставляем в левое поддерево
			tree->left = insertNode(tree->left, elem);	
			// проверяем на сбалансорованность
			if (treeDepth(tree->left) - treeDepth(tree->right) == 2)
			{
				if(elem.key < tree->left->data->key)
				{
					tree = singleLeftRotate(tree);
					return tree;
				}
				else
				{
					tree = doubleLeftRotate(tree);			
					return tree;
				}
			}
		}
		else
		{
			// если детей нет, то создаем лист
			struct treeNode *node = makeNode();
			node->parent = tree;
			node->data->key = elem.key;
			node->data->str = elem.str;
			tree->left = node;
			return tree;
		}
	}
	return tree;
}

struct treeNode *findNode(struct treeNode *tree, key_t key)
{
	// если детей нет, то и искать негде
	if(tree == NULL || (tree->right == NULL && tree->left == NULL && tree->parent == NULL))
		return NULL;
	// если ключ совпал - возвращаем
	if(keyCompare(key, tree->data->key) == 0)
		return tree;
	// иначе ищем в поддеревьях
	if(keyCompare(key, tree->data->key) > 0)
		return findNode(tree->right, key);
	if(keyCompare(key, tree->data->key) < 0)
		return findNode(tree->left, key);
	// for avoid warning
	return NULL;
}

struct treeNode *removeNode(struct treeNode *tree, key_t key)
{
	if(tree == NULL)
		return tree;
	// ищем в правом или левом поддереве и удаляем
	if(keyCompare(key, tree->data->key) == 1)
	{
		tree->right = removeNode(tree->right, key);
		// проверяем на сбалансорованность
		if (treeDepth(tree->left) - treeDepth(tree->right) == 2)
		{
			if(key > tree->left->data->key)
			{
				tree = singleLeftRotate(tree);
				return tree;
			}
			else
			{
				tree = doubleLeftRotate(tree);			
				return tree;
			}
		}
	}
	if(keyCompare(key, tree->data->key) == -1)
	{
		tree->left = removeNode(tree->left, key);
		// проверяем на сбалансорованность
		if (treeDepth(tree->right) - treeDepth(tree->left) == 2)
		{
			if(key < tree->right->data->key)
			{
				tree = singleRightRotate(tree);
				return tree;
			}
			else
			{
				tree = doubleRightRotate(tree);		
				return tree;
			}
		}
	}
	// если мы нашли вершину для удаления
	if (keyCompare(key, tree->data->key) == 0)
	{
		// если у неё нет детей, то удаляем
		if (tree->left == NULL && tree->right == NULL)
		{
			if (tree->parent->left == tree)
				tree->parent->left = NULL;
			else
				tree->parent->right = NULL;

			free(tree->data);
			tree->data =NULL;
			tree->parent = NULL;
			free(tree);
			tree = NULL;
			return tree;
		}
		// если есть только правый ребенок, то мы переносим значения 
		// data из ребенка в текущий элемент, зануляем ребенка и зануляем на него ссылку.
		// так как у нас avl-дерево, то у ребенка детей быть не может
		if (tree->left == NULL && tree->right != NULL)
		{
			tree->data->key = tree->right->data->key;
			tree->data->str = tree->right->data->key;
			free(tree->right->data);
			tree->right->data = NULL;
			free(tree->right);
			tree->right = NULL;
			return tree;
		}
		// аналогично, если нет правого ребенка
		if (tree->right == NULL && tree->left != NULL)
		{
			tree->data->key = tree->left->data->key;
			tree->data->str = tree->left->data->key;
			free(tree->left->data);
			tree->left->data = NULL;
			free(tree->left);
			tree->left = NULL;
			return tree;
		}
		// если есть оба ребенка, берем самый левый элемент правого поддерева,
		// копируем его data в текущий элемент и рекурсивно удаляем его.
		else
		{
			struct treeNode *tempNode = tree->right;
			struct nodeData elem;
			// флаг. 1, если мы удаляем корневой элемент
			int flag = 0;
			if (tree->parent == tree)
				flag = 1;

			while (tempNode->left != NULL)
				tempNode = tempNode->left;

			elem.key = tempNode->data->key;
			elem.str = tempNode->data->str;
			tree = removeNode(tree, tempNode->data->key);
			tempNode = findNode(tree, key);
			tempNode->data->key = elem.key;
			tempNode->data->str = elem.str;
			if (treeDepth(tree->left) - treeDepth(tree->right) == 2)
			{
				if(key > tree->left->data->key)
				{
					tree = singleLeftRotate(tree);
					if (flag)
						tree->parent = tree;
					return tree;
				}
				else
				{
					tree = doubleLeftRotate(tree);
					if (flag)
						tree->parent = tree;
					return tree;
				}
			}
			if (treeDepth(tree->right) - treeDepth(tree->left) == 2)
			{
				if(key > tree->right->data->key)
				{
					tree = singleRightRotate(tree);
					if (flag)
						tree->parent = tree;
					return tree;
				}
				else
				{
					tree = doubleRightRotate(tree);
					if (flag)
						tree->parent = tree;
					return tree;
				}
			}
		}
	}
	return tree;
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
	// если память не выделена, выходим из программы
	if (obj == NULL)
	{
		fprintf(stderr,"Error while memory allocation in %s, on line %u", file, line);
		exit(-1);
	}
}

struct treeNode *singleLeftRotate(struct treeNode *node)
{
	struct treeNode *tempNode;
	int flag = 0;
	if (node->parent == node)
		flag = 1;
	// поворачиваем
	tempNode = node->left;
	if (flag)
		tempNode->parent = tempNode;
	else
	tempNode->parent = node->parent;
	node->left = tempNode->right;
	if(node->left != NULL)
		node->left->parent = node;

	tempNode->right = node;
	tempNode->right->parent = tempNode;
	return tempNode;
}

struct treeNode *singleRightRotate(struct treeNode *node)
{
	struct treeNode *tempNode;
	int flag = 0;
	if (node->parent == node)
		flag = 1;
	// поворачиваем
	tempNode = node->right;
	if (flag)
		tempNode->parent = tempNode;
	else
		tempNode->parent = node->parent;
	node->right = tempNode->left;
	if(node->right != NULL)
		node->right->parent = node;
	tempNode->left = node;
	tempNode->left->parent = tempNode;

	return tempNode;
}

struct treeNode *doubleLeftRotate(struct treeNode *node)
{
	node->left = singleRightRotate(node->left);
	return singleLeftRotate(node);
}

struct treeNode *doubleRightRotate(struct treeNode *node)
{
	node->right = singleLeftRotate(node->right);
	return singleRightRotate(node);
}

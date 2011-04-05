#include <stdio.h>
#include <stdlib.h>
#include "avlTree.h"

int checkBalance(struct treeNode *tree)
{
	if(tree->right != NULL && tree->left != NULL)
		return abs(tree->right->height - tree->left->height);
	if(tree->right == NULL && tree->left == NULL)
		return 0;
	if (tree->right == NULL)
		return tree->left->height + 1;
	if (tree->left == NULL)
		return tree->right->height + 1;
}

int height(struct treeNode *tree)
{
	if(tree == NULL)
		return 0;
	if(tree->right != NULL && tree->left != NULL)
		return max(tree->right->height, tree->left->height) + 1;
	if(tree->right == NULL && tree->left == NULL)
		return 0;
	if (tree->right == NULL)
		return tree->left->height + 1;
	if (tree->left == NULL)
		return tree->right->height + 1;
}

struct treeNode *initTree(struct nodeData elem)
{
	// �������� ������
	struct treeNode* root = (struct treeNode *) malloc(sizeof(struct treeNode));
	// �������� ����� - �� ���
	root->parent = root;
	// ������ ������ = 0
	root->height = 0;
	root->left = NULL;
	root->right = NULL;
	root->data = (struct nodeData *) malloc(sizeof(struct nodeData));
	root->data->key = elem.key;
	root->data->str = elem.str;
	return root;
}

struct treeNode *makeNode()
{
	// �������� ������
	struct treeNode* node = (struct treeNode *) malloc(sizeof(struct treeNode));
	// �������� ��������� �� ��� ���������� ���������
	node->parent = NULL;
	node->left = NULL;
	node->right = NULL;
	node->data = (struct nodeData *) malloc(sizeof(struct nodeData));
	return node;
}



struct treeNode *insertNode(struct treeNode *tree, struct nodeData elem)
{
	// ���� ���� >= ����� � ������
	if (keyCompare(elem.key, tree->data->key) != -1)
	{
		// ���� ���� ���� � �������
		if(tree->right != NULL)
		{
			// ��������� � ������ ���������
			tree->right = insertNode(tree->right, elem);
			tree->height = height(tree);

			// ��������� �� ������������������
			if (checkBalance(tree) == 2)
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
			// ���� ����� ���, �� ������� ����
			struct treeNode *node = makeNode();			
			node->parent = tree;
			node->data->key = elem.key;
			node->data->str = elem.str;
			node->height = 0;
			tree->right = node;
			if (tree->left != NULL)
				tree->height = max(tree->left->height, tree->right->height) + 1;
			else
				tree->height = tree->right->height + 1;
			return tree;
		}
	}
	// ���� ���� < ����� � ������
	if (keyCompare(elem.key, tree->data->key) == -1)
	{
		if(tree->left != NULL)
		{
			// ��������� � ����� ���������
			tree->left = insertNode(tree->left, elem);	
			tree->height = height(tree);
			// ��������� �� ������������������
			if (checkBalance(tree) == 2)
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
			// ���� ����� ���, �� ������� ����
			struct treeNode *node = makeNode();
			node->parent = tree;
			node->data->key = elem.key;
			node->data->str = elem.str;
			node->height = 0;
			tree->left = node;
			if (tree->right != NULL)
				tree->height = max(tree->left->height, tree->right->height) + 1;
			else
				tree->height = tree->left->height + 1;
			return tree;
		}
	}
	return tree;
}

struct treeNode *findNode(struct treeNode *tree, key_t key)
{
	// ���� ����� ���, �� � ������ �����
	if(tree == NULL || (tree->right == NULL && tree->left == NULL && tree->parent == NULL))
		return NULL;
	// ���� ���� ������ - ����������
	if(keyCompare(key, tree->data->key) == 0)
		return tree;
	// ����� ���� � �����������
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
	// ���� � ������ ��� ����� ��������� � �������
	if(keyCompare(key, tree->data->key) == 1)
	{
		tree->right = removeNode(tree->right, key);
		tree->height = height(tree);
		// ��������� �� ������������������
		if (height(tree->left)- height(tree->right) == 2)
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
		tree->height = height(tree);
		// ��������� �� ������������������
		if (height(tree->right)- height(tree->left) == 2)
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
	// ���� �� ����� ������� ��� ��������
	if (keyCompare(key, tree->data->key) == 0)
	{
		// ���� � �� ��� �����, �� �������
		if (tree->left == NULL && tree->right == NULL)
		{
			if (tree->parent->left == tree)
				tree->parent->left = NULL;
			else
				tree->parent->right = NULL;

			tree->parent->height = height(tree->parent);
			free(tree->data);
			tree->data =NULL;
			tree->parent = NULL;
			free(tree);
			tree = NULL;
			return tree;
		}
		// ���� ���� ������ ������ �������, �� �� ��������� �������� 
		// data �� ������� � ������� �������, �������� ������� � �������� �� ���� ������.
		// ��� ��� � ��� avl-������, �� � ������� ����� ���� �� �����
		if (tree->left == NULL && tree->right != NULL)
		{
			tree->data->key = tree->right->data->key;
			tree->data->str = tree->right->data->str;
			free(tree->right->data);
			tree->right->data = NULL;
			free(tree->right);
			tree->right = NULL;
			tree->height = 0;
			return tree;
		}
		// ����������, ���� ��� ������� �������
		if (tree->right == NULL && tree->left != NULL)
		{
			tree->data->key = tree->left->data->key;
			tree->data->str = tree->left->data->str;
			free(tree->left->data);
			tree->left->data = NULL;
			free(tree->left);
			tree->left = NULL;
			tree->height = 0;
			return tree;
		}
		// ���� ���� ��� �������, ����� ����� ����� ������� ������� ���������,
		// �������� ��� data � ������� ������� � ���������� ������� ���.
		else
		{
			struct treeNode *tempNode = tree->right;
			struct nodeData elem;
			// ����. 1, ���� �� ������� �������� �������
			int flag = 0;
			if (tree->parent == tree)
				flag = 1;

			while (tempNode->left != NULL)
				tempNode = tempNode->left;

			elem.key = tempNode->data->key;
			elem.str = tempNode->data->str;
			tree = removeNode(tree, tempNode->data->key);
			tree->height = height(tree);
			tempNode = findNode(tree, key);
			tempNode->data->key = elem.key;
			tempNode->data->str = elem.str;
			if (height(tree->left)- height(tree->right) == 2)
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
			if (height(tree->right)- height(tree->left) == 2)
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

struct treeNode *singleLeftRotate(struct treeNode *node)
{
	struct treeNode *tempNode;
	int flag = 0;
	if (node->parent == node)
		flag = 1;
	// ������������
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

	node->height = height(node);
	tempNode->height = height(tempNode);
	
	return tempNode;
}

struct treeNode *singleRightRotate(struct treeNode *node)
{
	struct treeNode *tempNode;
	int flag = 0;
	if (node->parent == node)
		flag = 1;
	// ������������
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

	node->height = height(node);
	tempNode->height = height(tempNode);

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

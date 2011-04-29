#pragma once



// class A - тип ключа
// class B - тип значения
template<class A, class B> class CAvlTree
{
public:
	// структура данных в дереве
	struct nodeData
	{
		A m_key; 
		B m_data;
	};

	// структура узла дерева
	struct treeNode
	{
		struct treeNode *m_pLeft;
		struct treeNode *m_pRight;
		struct treeNode *m_pParent;
		struct nodeData *m_pData;
		int height;
	};

	CAvlTree()
	{
		m_pRoot = NULL;
	}


	int insertValue(const B *elem)
	{
		nodeData *p

		insertNode(m_pRoot, elem);
	}
	



	// функция удаления элемента из дерева по ключу
	struct treeNode *removeNode(struct treeNode *tree, A key)
	{
		if(tree == NULL)
			return tree;
		// ищем в правом или левом поддереве и удаляем
		if(keyCompare(key, tree->m_pData->m_key) == 1)
		{
			tree->m_pRight = removeNode(tree->m_pRight, key);
			tree->height = height(tree);
			// проверяем на сбалансорованность
			if (height(tree->m_pLeft)- height(tree->m_pRight) == 2)
			{
				if(key > tree->m_pLeft->m_pData->m_key)
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
		if(keyCompare(key, tree->m_pData->m_key) == -1)
		{
			tree->m_pLeft = removeNode(tree->m_pLeft, key);
			tree->height = height(tree);
			// проверяем на сбалансорованность
			if (height(tree->m_pRight)- height(tree->m_pLeft) == 2)
			{
				if(key < tree->m_pRight->m_pData->m_key)
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
		if (keyCompare(key, tree->m_pData->m_key) == 0)
		{
			// если у неё нет детей, то удаляем
			if (tree->m_pLeft == NULL && tree->m_pRight == NULL)
			{
				if (tree->m_pParent->m_pLeft == tree)
					tree->m_pParent->m_pLeft = NULL;
				else
					tree->m_pParent->m_pRight = NULL;

				tree->m_pParent->height = height(tree->m_pParent);
				delete tree->m_pData;
				tree->m_pData =NULL;
				tree->m_pParent = NULL;
				delete tree;
				tree = NULL;
				return tree;
			}
			// если есть только правый ребенок, то мы переносим значения 
			// data из ребенка в текущий элемент, зануляем ребенка и зануляем на него ссылку.
			// так как у нас avl-дерево, то у ребенка детей быть не может
			if (tree->m_pLeft == NULL && tree->m_pRight != NULL)
			{
				tree->m_pData->m_key = tree->m_pRight->m_pData->m_key;
				tree->m_pData->m_data = tree->m_pRight->m_pData->m_data;
				delete tree->m_pRight->m_pData;
				tree->m_pRight->m_pData = NULL;
				delete tree->m_pRight;
				tree->m_pRight = NULL;
				tree->height = 0;
				return tree;
			}
			// аналогично, если нет правого ребенка
			if (tree->m_pRight == NULL && tree->m_pLeft != NULL)
			{
				tree->m_pData->m_key = tree->m_pLeft->m_pData->m_key;
				tree->m_pData->m_data = tree->m_pLeft->m_pData->m_data;
				delete tree->m_pLeft->m_pData;
				tree->m_pLeft->m_pData = NULL;
				delete tree->m_pLeft;
				tree->m_pLeft = NULL;
				tree->height = 0;
				return tree;
			}
			// если есть оба ребенка, берем самый левый элемент правого поддерева,
			// копируем его data в текущий элемент и рекурсивно удаляем его.
			else
			{
				struct treeNode *tempNode = tree->m_pRight;
				struct nodeData elem;
				// флаг. 1, если мы удаляем корневой элемент
				int flag = 0;
				if (tree->m_pParent == tree)
					flag = 1;

				while (tempNode->m_pLeft != NULL)
					tempNode = tempNode->m_pLeft;

				elem.m_key = tempNode->m_pData->m_key;
				elem.m_data = tempNode->m_pData->m_data;
				tree = removeNode(tree, tempNode->m_pData->m_key);
				tree->height = height(tree);
				tempNode = findNode(tree, key);
				tempNode->m_pData->m_key = elem.m_key;
				tempNode->m_pData->m_data = elem.m_data;
				if (height(tree->m_pLeft)- height(tree->m_pRight) == 2)
				{
					if(key > tree->m_pLeft->m_pData->m_key)
					{
						tree = singleLeftRotate(tree);
						if (flag)
							tree->m_pParent = tree;
						return tree;
					}
					else
					{
						tree = doubleLeftRotate(tree);
						if (flag)
							tree->m_pParent = tree;
						return tree;
					}
				}
				if (height(tree->m_pRight)- height(tree->m_pLeft) == 2)
				{
					if(key > tree->m_pRight->m_pData->m_key)
					{
						tree = singleRightRotate(tree);
						if (flag)
							tree->m_pParent = tree;
						return tree;
					}
					else
					{
						tree = doubleRightRotate(tree);
						if (flag)
							tree->m_pParent = tree;
						return tree;
					}
				}
			}
		}
		return tree;
	}



	// функция поиска данных в дереве по ключу
	struct treeNode *findNode(struct treeNode *tree, A key)
	{
		// если детей нет, то и искать негде
		if(tree == NULL || (tree->m_pRight == NULL && tree->m_pLeft == NULL && tree->m_pParent == NULL))
			return NULL;
		// если ключ совпал - возвращаем
		if(keyCompare(key, tree->m_pData->m_key) == 0)
			return tree;
		// иначе ищем в поддеревьях
		if(keyCompare(key, tree->m_pData->m_key) > 0)
			return findNode(tree->m_pRight, key);
		if(keyCompare(key, tree->m_pData->m_key) < 0)
			return findNode(tree->m_pLeft, key);
		// for avoid warning
		return NULL;
	}



	// функция сравнения ключей
	// возвращает 1, если left > right
	//			  0, если равны
	//			 -1, если left < right
	int keyCompare(A left, A right)
	{
		if(left < right)
			return -1;
		else
			return left > right;
	}



private:
	// функция вычисления хэша по строке
	int calcHash(string *pElem, const unsigned int keySz)
	{
		unsigned int i;
		unsigned int hash = 0;
		for(i = 0; i < pElem->size(); i++)
			hash += pElem->at(i);
		return hash % keySz;
	}



	// функция создания нода
	struct treeNode *makeNode()
	{
		// Выделяем память
		treeNode* node = new treeNode;
		// зануляем указатели на все внутренние структуры
		node->m_pParent = NULL;
		node->m_pLeft = NULL;
		node->m_pRight = NULL;
		node->m_pData = new nodeData;
		return node;
	}



	// функция вставки элемента в дерево
	struct treeNode *insertNode(struct treeNode *tree, struct nodeData elem)
	{
		if (tree == NULL)
		{
			// Выделяем память
			tree = new treeNode;
			// Родитель корня - он сам
			tree->m_pParent = tree;
			// высота дерева = 0
			tree->height = 0;
			tree->m_pLeft = NULL;
			tree->m_pRight = NULL;
			tree->m_pData = new nodeData;
			tree->m_pData->m_key = elem.m_key;
			tree->m_pData->m_data = elem.m_data;
			return tree;
		}
		// если ключ >= ключа в дереве
		if (keyCompare(elem.m_key, tree->m_pData->m_key) != -1)
		{
			// если есть дети у вершины
			if(tree->m_pRight != NULL)
			{
				// вставляем в правое поддерево
				tree->m_pRight = insertNode(tree->m_pRight, elem);
				tree->height = height(tree);

				// проверяем на сбалансированность
				if (checkBalance(tree) == 2)
				{
					if(elem.m_key > tree->m_pRight->m_pData->m_key)
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
				node->m_pParent = tree;
				node->m_pData->m_key = elem.m_key;
				node->m_pData->m_data = elem.m_data;
				node->height = 0;
				tree->m_pRight = node;
				if (tree->m_pLeft != NULL)
					tree->height = max(tree->m_pLeft->height, tree->m_pRight->height) + 1;
				else
					tree->height = tree->m_pRight->height + 1;
				return tree;
			}
		}
		// если ключ < ключа в дереве
		if (keyCompare(elem.m_key, tree->m_pData->m_key) == -1)
		{
			if(tree->m_pLeft != NULL)
			{
				// вставляем в левое поддерево
				tree->m_pLeft = insertNode(tree->m_pLeft, elem);	
				tree->height = height(tree);
				// проверяем на сбалансорованность
				if (checkBalance(tree) == 2)
				{
					if(elem.m_key < tree->m_pLeft->m_pData->m_key)
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
				treeNode *node = makeNode();
				node->m_pParent = tree;
				node->m_pData->m_key = elem.m_key;
				node->m_pData->m_data = elem.m_data;
				node->height = 0;
				tree->m_pLeft = node;
				if (tree->m_pRight != NULL)
					tree->height = max(tree->m_pLeft->height, tree->m_pRight->height) + 1;
				else
					tree->height = tree->m_pLeft->height + 1;
				return tree;
			}
		}
		return tree;
	}



	// функции вращения
	struct treeNode *singleLeftRotate(struct treeNode *node)
	{
		treeNode *tempNode;
		int flag = 0;
		if (node->m_pParent == node)
			flag = 1;
		// поворачиваем
		tempNode = node->m_pLeft;
		if (flag)
			tempNode->m_pParent = tempNode;
		else
			tempNode->m_pParent = node->m_pParent;
		node->m_pLeft = tempNode->m_pRight;
		if(node->m_pLeft != NULL)
			node->m_pLeft->m_pParent = node;

		tempNode->m_pRight = node;
		tempNode->m_pRight->m_pParent = tempNode;

		node->height = height(node);
		tempNode->height = height(tempNode);

		return tempNode;
	}



	struct treeNode *singleRightRotate(struct treeNode *node)
	{
		treeNode *tempNode;
		int flag = 0;
		if (node->m_pParent == node)
			flag = 1;
		// поворачиваем
		tempNode = node->m_pRight;
		if (flag)
			tempNode->m_pParent = tempNode;
		else
			tempNode->m_pParent = node->m_pParent;
		node->m_pRight = tempNode->m_pLeft;
		if(node->m_pRight != NULL)
			node->m_pRight->m_pParent = node;
		tempNode->m_pLeft = node;
		tempNode->m_pLeft->m_pParent = tempNode;

		node->height = height(node);
		tempNode->height = height(tempNode);

		return tempNode;
	}



	struct treeNode *doubleLeftRotate(struct treeNode *node)
	{
		node->m_pLeft = singleRightRotate(node->m_pLeft);
		return singleLeftRotate(node);
	}



	struct treeNode *doubleRightRotate(struct treeNode *node)
	{
		node->m_pRight = singleLeftRotate(node->m_pRight);
		return singleRightRotate(node);
	}



	// высота дерева
	int height(struct treeNode *tree)
	{
		if(tree == NULL)
			return 0;
		if(tree->m_pRight != NULL && tree->m_pLeft != NULL)
			return max(tree->m_pRight->height, tree->m_pLeft->height) + 1;
		if(tree->m_pRight == NULL && tree->m_pLeft == NULL)
			return 0;
		if (tree->m_pRight == NULL)
			return tree->m_pLeft->height + 1;
		if (tree->m_pLeft == NULL)
			return tree->m_pRight->height + 1;
	}



	// проверка сбалансированности
	int checkBalance(struct treeNode *tree);
	// корень дерева
	struct treeNode *m_pRoot;
};
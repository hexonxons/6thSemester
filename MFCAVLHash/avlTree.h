#pragma once



// class A - ��� �����
// class B - ��� ��������
template<class A, class B> class CAvlTree
{
public:
	// ��������� ������ � ������
	struct nodeData
	{
		A m_key; 
		B m_data;
	};

	// ��������� ���� ������
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
	



	// ������� �������� �������� �� ������ �� �����
	struct treeNode *removeNode(struct treeNode *tree, A key)
	{
		if(tree == NULL)
			return tree;
		// ���� � ������ ��� ����� ��������� � �������
		if(keyCompare(key, tree->m_pData->m_key) == 1)
		{
			tree->m_pRight = removeNode(tree->m_pRight, key);
			tree->height = height(tree);
			// ��������� �� ������������������
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
			// ��������� �� ������������������
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
		// ���� �� ����� ������� ��� ��������
		if (keyCompare(key, tree->m_pData->m_key) == 0)
		{
			// ���� � �� ��� �����, �� �������
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
			// ���� ���� ������ ������ �������, �� �� ��������� �������� 
			// data �� ������� � ������� �������, �������� ������� � �������� �� ���� ������.
			// ��� ��� � ��� avl-������, �� � ������� ����� ���� �� �����
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
			// ����������, ���� ��� ������� �������
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
			// ���� ���� ��� �������, ����� ����� ����� ������� ������� ���������,
			// �������� ��� data � ������� ������� � ���������� ������� ���.
			else
			{
				struct treeNode *tempNode = tree->m_pRight;
				struct nodeData elem;
				// ����. 1, ���� �� ������� �������� �������
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



	// ������� ������ ������ � ������ �� �����
	struct treeNode *findNode(struct treeNode *tree, A key)
	{
		// ���� ����� ���, �� � ������ �����
		if(tree == NULL || (tree->m_pRight == NULL && tree->m_pLeft == NULL && tree->m_pParent == NULL))
			return NULL;
		// ���� ���� ������ - ����������
		if(keyCompare(key, tree->m_pData->m_key) == 0)
			return tree;
		// ����� ���� � �����������
		if(keyCompare(key, tree->m_pData->m_key) > 0)
			return findNode(tree->m_pRight, key);
		if(keyCompare(key, tree->m_pData->m_key) < 0)
			return findNode(tree->m_pLeft, key);
		// for avoid warning
		return NULL;
	}



	// ������� ��������� ������
	// ���������� 1, ���� left > right
	//			  0, ���� �����
	//			 -1, ���� left < right
	int keyCompare(A left, A right)
	{
		if(left < right)
			return -1;
		else
			return left > right;
	}



private:
	// ������� ���������� ���� �� ������
	int calcHash(string *pElem, const unsigned int keySz)
	{
		unsigned int i;
		unsigned int hash = 0;
		for(i = 0; i < pElem->size(); i++)
			hash += pElem->at(i);
		return hash % keySz;
	}



	// ������� �������� ����
	struct treeNode *makeNode()
	{
		// �������� ������
		treeNode* node = new treeNode;
		// �������� ��������� �� ��� ���������� ���������
		node->m_pParent = NULL;
		node->m_pLeft = NULL;
		node->m_pRight = NULL;
		node->m_pData = new nodeData;
		return node;
	}



	// ������� ������� �������� � ������
	struct treeNode *insertNode(struct treeNode *tree, struct nodeData elem)
	{
		if (tree == NULL)
		{
			// �������� ������
			tree = new treeNode;
			// �������� ����� - �� ���
			tree->m_pParent = tree;
			// ������ ������ = 0
			tree->height = 0;
			tree->m_pLeft = NULL;
			tree->m_pRight = NULL;
			tree->m_pData = new nodeData;
			tree->m_pData->m_key = elem.m_key;
			tree->m_pData->m_data = elem.m_data;
			return tree;
		}
		// ���� ���� >= ����� � ������
		if (keyCompare(elem.m_key, tree->m_pData->m_key) != -1)
		{
			// ���� ���� ���� � �������
			if(tree->m_pRight != NULL)
			{
				// ��������� � ������ ���������
				tree->m_pRight = insertNode(tree->m_pRight, elem);
				tree->height = height(tree);

				// ��������� �� ������������������
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
				// ���� ����� ���, �� ������� ����
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
		// ���� ���� < ����� � ������
		if (keyCompare(elem.m_key, tree->m_pData->m_key) == -1)
		{
			if(tree->m_pLeft != NULL)
			{
				// ��������� � ����� ���������
				tree->m_pLeft = insertNode(tree->m_pLeft, elem);	
				tree->height = height(tree);
				// ��������� �� ������������������
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
				// ���� ����� ���, �� ������� ����
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



	// ������� ��������
	struct treeNode *singleLeftRotate(struct treeNode *node)
	{
		treeNode *tempNode;
		int flag = 0;
		if (node->m_pParent == node)
			flag = 1;
		// ������������
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
		// ������������
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



	// ������ ������
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



	// �������� ������������������
	int checkBalance(struct treeNode *tree);
	// ������ ������
	struct treeNode *m_pRoot;
};
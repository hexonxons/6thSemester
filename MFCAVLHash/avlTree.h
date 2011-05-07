#pragma once

#include "../includes/memalloc.h"

#ifdef _DEBUG
    #include <crtdbg.h>
    #define new new(__FILE__,__LINE__)
#endif

template<class T> class CAvlTree
{
public:
	CAvlTree()
	{
		m_pRoot = NULL;
	}

    virtual ~CAvlTree()
    {
        deleteNode(m_pRoot);
    }

    // ������� ���������
    typedef  int (Compare)(T *pElem1, T *pElem2);

    // ������� ���������� �������� � ���-�������
    T *AddElem(T *pElem, Compare compareFunction)
    {
        m_pRoot = insertNode(m_pRoot, pElem, compareFunction);
        return m_pRoot->pData;
    }

    // ������� �������� �������� �� �����
    int DelElem(T* pElem, Compare compareFunction)
    {
        m_pRoot = removeNode(m_pRoot, pElem, compareFunction);
        return 0;
    }

    int getLastError()
    {
        int retErr = m_lastError;
        m_lastError = 0;
        return retErr;
    }

    // ������� ������ �������� �� �����
    T *FindElem(T* pKeyWord, Compare compareFunction)
    {
        treeNode *fndElem = findNode(m_pRoot, pKeyWord, compareFunction);
        // ���� �� ������� ��������, ���������� NULL
        if (fndElem == NULL)
        {
            return NULL;
        }
        return fndElem->pData;
    }
	
private:
    // ��������� ���� ������
    struct treeNode
    {
        struct treeNode *pLeft;
        struct treeNode *pRight;
        struct treeNode *pParent;
        T *pData;
        int height;
    };

    // ������� �������� ����
    struct treeNode *makeNode()
    {
        // �������� ������
        treeNode* node = new treeNode;
        // �������� ��������� �� ��� ���������� ���������
        node->pParent = NULL;
        node->pLeft = NULL;
        node->pRight = NULL;
        node->pData = NULL;
        node->height = 0;
        return node;
    }
    
    // ������� ��������� ���� � ��� ���
    int deleteNode(treeNode *node)
    {
       if(node->pLeft == NULL && node->pRight == NULL)
       {
           delete node;
           node = NULL;
           return 0;
       }
       if (node->pLeft != NULL && node->pRight == NULL)
       {
           deleteNode(node->pLeft);
           delete node;
           node = NULL;
           return 0;
       }
       if(node->pRight != NULL && node->pLeft == NULL)
       {
           deleteNode(node->pRight);
           delete node;
           node = NULL;
           return 0;
       }
       if (node->pRight != NULL && node->pLeft != NULL)
       {
           deleteNode(node->pRight);
           deleteNode(node->pLeft);
           delete node;
           node = NULL;
           return 0;
       }
       return -1;
    }

    // ������� ������� �������� � ������
    struct treeNode *insertNode(struct treeNode *tree, T *pElem, Compare compareFunction)
    {
        if (tree == NULL)
        {
            tree = makeNode();
            tree->pParent = tree;
            tree->pData = pElem;
            return tree;
        }
        if (compareFunction(pElem, tree->pData) == 0)
        {
            m_lastError = 2;
            return tree;
        }
        // ���� ���� > ����� � ������
        if (compareFunction(pElem, tree->pData) != -1)
        {
            // ���� ���� ���� � �������
            if(tree->pRight != NULL)
            {
                // ��������� � ������ ���������
                tree->pRight = insertNode(tree->pRight, pElem, compareFunction);
                tree->height = height(tree);

                // ��������� �� ������������������
                if (checkBalance(tree) == 2)
                {
                    if(compareFunction(pElem,tree->pRight->pData) != -1)
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
                node->pParent = tree;
                node->pData = pElem;
                tree->pRight = node;
                if (tree->pLeft != NULL)
                    tree->height = max(tree->pLeft->height, tree->pRight->height) + 1;
                else
                    tree->height = tree->pRight->height + 1;
                return tree;
            }
        }
        // ���� ���� < ����� � ������
        if (compareFunction(pElem, tree->pData) == -1)
        {
            if(tree->pLeft != NULL)
            {
                // ��������� � ����� ���������
                tree->pLeft = insertNode(tree->pLeft, pElem, compareFunction);	
                tree->height = height(tree);
                // ��������� �� ������������������
                if (checkBalance(tree) == 2)
                {
                    if(compareFunction(pElem, tree->pLeft->pData) == -1)
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
                node->pParent = tree;
                node->pData = pElem;
                tree->pLeft = node;
                if (tree->pRight != NULL)
                    tree->height = max(tree->pLeft->height, tree->pRight->height) + 1;
                else
                    tree->height = tree->pLeft->height + 1;
                return tree;
            }
        }
        return tree;
    }

    // ������� �������� �������� �� ������ �� �����
    struct treeNode *removeNode(struct treeNode *tree, T *pElem, Compare compareFunction)
    {
        if(tree == NULL)
            return tree;
        // ���� � ������ ��� ����� ��������� � �������
        if(compareFunction(pElem, tree->pData) == 1)
        {
            tree->pRight = removeNode(tree->pRight, pElem, compareFunction);
            tree->height = height(tree);
            // ��������� �� ������������������
            if (height(tree->pLeft)- height(tree->pRight) == 2)
            {
                if(compareFunction(pElem, tree->pLeft->pData) == 1)
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
        if(compareFunction(pElem, tree->pData) == -1)
        {
            tree->pLeft = removeNode(tree->pLeft, pElem, compareFunction);
            tree->height = height(tree);
            // ��������� �� ������������������
            if (height(tree->pRight)- height(tree->pLeft) == 2)
            {
                if(compareFunction(pElem, tree->pRight->pData) == -1)
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
        if (compareFunction(pElem, tree->pData) == 0)
        {
            // ���� � �� ��� �����, �� �������
            if (tree->pLeft == NULL && tree->pRight == NULL)
            {
                if (tree->pParent->pLeft == tree)
                    tree->pParent->pLeft = NULL;
                else
                    tree->pParent->pRight = NULL;

                tree->pParent->height = height(tree->pParent);
                tree->pData = NULL;
                tree->pParent = NULL;
                delete(tree);
                tree = NULL;
                return tree;
            }
            // ���� ���� ������ ������ �������, �� �� ��������� �������� 
            // pData �� ������� � ������� �������, �������� ������� � �������� �� ���� ������.
            // ��� ��� � ��� avl-������, �� � ������� ����� ���� �� �����
            if (tree->pLeft == NULL && tree->pRight != NULL)
            {
                tree->pData = tree->pRight->pData;
                tree->pRight->pData = NULL;
                delete(tree->pRight);
                tree->pRight = NULL;
                tree->height = 0;
                return tree;
            }
            // ����������, ���� ��� ������� �������
            if (tree->pRight == NULL && tree->pLeft != NULL)
            {
                tree->pData = tree->pLeft->pData;
                tree->pLeft->pData = NULL;
                delete(tree->pLeft);
                tree->pLeft = NULL;
                tree->height = 0;
                return tree;
            }
            // ���� ���� ��� �������, ����� ����� ����� ������� ������� ���������,
            // �������� ��� pData � ������� ������� � ���������� ������� ���.
            else
            {
                struct treeNode *tempNode = tree->pRight;
                T *elem;
                // ����. 1, ���� �� ������� �������� �������
                int flag = 0;
                if (tree->pParent == tree)
                    flag = 1;

                while (tempNode->pLeft != NULL)
                    tempNode = tempNode->pLeft;

                elem = tempNode->pData;
                tree = removeNode(tree, tempNode->pData, compareFunction);
                tree->height = height(tree);
                tempNode = findNode(tree, pElem, compareFunction);
                tempNode->pData = elem;
                if (height(tree->pLeft)- height(tree->pRight) == 2)
                {
                    if(compareFunction(pElem, tree->pLeft->pData) == 1)
                    {
                        tree = singleLeftRotate(tree);
                        if (flag)
                            tree->pParent = tree;
                        return tree;
                    }
                    else
                    {
                        tree = doubleLeftRotate(tree);
                        if (flag)
                            tree->pParent = tree;
                        return tree;
                    }
                }
                if (height(tree->pRight)- height(tree->pLeft) == 2)
                {
                    if(compareFunction(pElem, tree->pRight->pData) == 1)
                    {
                        tree = singleRightRotate(tree);
                        if (flag)
                            tree->pParent = tree;
                        return tree;
                    }
                    else
                    {
                        tree = doubleRightRotate(tree);
                        if (flag)
                            tree->pParent = tree;
                        return tree;
                    }
                }
            }
        }
        return tree;
    }



    // ������� ������ ������ � ������ �� �����
    struct treeNode *findNode(struct treeNode *tree, T *key, Compare compareFunction)
    {
        // ���� ����� ���, �� � ������ �����
        if(tree == NULL)
            return NULL;
        // ���� ���� ������ - ����������
        if(compareFunction(key, tree->pData) == 0)
            return tree;
        // ����� ���� � �����������
        if(compareFunction(key, tree->pData) == 1)
            return findNode(tree->pRight, key, compareFunction);
        if(compareFunction(key, tree->pData) == -1)
            return findNode(tree->pLeft, key, compareFunction);
        // for avoid warning
        return NULL;
    }


    // ������� ��������
	struct treeNode *singleLeftRotate(struct treeNode *node)
	{
		treeNode *tempNode;
		int flag = 0;
		if (node->pParent == node)
			flag = 1;
		// ������������
		tempNode = node->pLeft;
		if (flag)
			tempNode->pParent = tempNode;
		else
			tempNode->pParent = node->pParent;
		node->pLeft = tempNode->pRight;
		if(node->pLeft != NULL)
			node->pLeft->pParent = node;

		tempNode->pRight = node;
		tempNode->pRight->pParent = tempNode;

		node->height = height(node);
		tempNode->height = height(tempNode);

		return tempNode;
	}

	struct treeNode *singleRightRotate(struct treeNode *node)
	{
		treeNode *tempNode;
		int flag = 0;
		if (node->pParent == node)
			flag = 1;
		// ������������
		tempNode = node->pRight;
		if (flag)
			tempNode->pParent = tempNode;
		else
			tempNode->pParent = node->pParent;
		node->pRight = tempNode->pLeft;
		if(node->pRight != NULL)
			node->pRight->pParent = node;
		tempNode->pLeft = node;
		tempNode->pLeft->pParent = tempNode;

		node->height = height(node);
		tempNode->height = height(tempNode);

		return tempNode;
	}



	struct treeNode *doubleLeftRotate(struct treeNode *node)
	{
		node->pLeft = singleRightRotate(node->pLeft);
		return singleLeftRotate(node);
	}



	struct treeNode *doubleRightRotate(struct treeNode *node)
	{
		node->pRight = singleLeftRotate(node->pRight);
		return singleRightRotate(node);
	}



	// ������ ������
	int height(struct treeNode *tree)
	{
		if(tree == NULL)
			return 0;
		if(tree->pRight != NULL && tree->pLeft != NULL)
			return max(tree->pRight->height, tree->pLeft->height) + 1;
		if(tree->pRight == NULL && tree->pLeft == NULL)
			return 0;
		if (tree->pRight == NULL)
			return tree->pLeft->height + 1;
		if (tree->pLeft == NULL)
			return tree->pRight->height + 1;
	}



	// �������� ������������������
    int checkBalance(struct treeNode *tree)
    {
        if(tree->pRight != NULL && tree->pLeft != NULL)
            return abs(tree->pRight->height - tree->pLeft->height);
        if(tree->pRight == NULL && tree->pLeft == NULL)
            return 0;
        if (tree->pRight == NULL)
            return tree->pLeft->height + 1;
        if (tree->pLeft == NULL)
            return tree->pRight->height + 1;
    }

	// ������ ������
	struct treeNode *m_pRoot;
    // ��������� ������
    int m_lastError;
    // ��������� treeNode
    CBasicDataBase<treeNode> m_NodeStorage;
};

#ifdef _DEBUG
    #undef new
#endif
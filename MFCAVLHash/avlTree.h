#pragma once

#include "../includes/memalloc.h"
#include "../includes/banned.h"

// ������������ ����
const unsigned int KEY_SZ = 3;

template<class T> class CAvlTree
{
public:
	CAvlTree()
	{
		m_pRoot = NULL;
	}

    // ������� ���������
    typedef  int (Compare)(T *pElem1, T *pElem2);
    // ������� ���������� ���� �� ������
    typedef int (CalcHash)(T *pElem);

    // ������� ���������� �������� � ���-�������
    T *AddElem(T *pElem, Compare compareFunction, CalcHash calcHashFunction)
    {
        m_pRoot = insertNode(m_pRoot, pElem, compareFunction, calcHashFunction);
        return m_pRoot->pData;
    }

    // ������� �������� �������� �� �����
    int DelElem(T* pElem, Compare compareFunction, CalcHash calcHashFunction)
    {
        m_pRoot = removeNode(m_pRoot, pElem, compareFunction, calcHashFunction);
        return 0;
    }

    int getLastError()
    {
        int retErr = m_lastError;
        m_lastError = 0;
        return retErr;
    }

    // ������� ������ �������� �� �����
    T *FindElem(T* pKeyWord, Compare compareFunction, CalcHash calcHashFunction)
    {
        treeNode *fndElem = findNode(m_pRoot, pKeyWord, calcHashFunction);
        external *pTmp;
        // ���� �� ������� ��������, ���������� NULL
        if (fndElem == NULL)
        {
            return NULL;
        }
        pTmp = fndElem->pNext;
        // ���� ������� �������� - ����, ���������� ���
        if (compareFunction(fndElem->pData, pKeyWord) == 0)
        {
            return fndElem->pData;
        }
        if (pTmp == NULL)
        {
            return NULL;
        }
        // ���� �������� ������ ��������� �� ������� ������ � �����
        while (compareFunction(pTmp->pData, pKeyWord) != 0)
        {
            pTmp = pTmp->pNext;
            if (pTmp == NULL)
            {
                return NULL;
            }
        }
        return pTmp->pData;
    }


	
private:
    // ��������� ��� ������������ ����
    struct external
    {
        external *pNext;
        T *pData;
    };

    // ��������� ���� ������
    struct treeNode
    {
        struct treeNode *pLeft;
        struct treeNode *pRight;
        struct treeNode *pParent;
        external *pNext;
        T *pData;
        int height;
        unsigned int hash;
    };

    // ������� �������� ����
    struct treeNode *makeNode()
    {
        treeNode node;
        // �������� ��������� �� ��� ���������� ���������
        node.pParent = NULL;
        node.pLeft = NULL;
        node.pRight = NULL;
        node.pData = NULL;
        node.pNext = NULL;
        node.height = 0;
        return m_NodeStorage.Add(node);
    }

    // ������� ������� �������� � ������
    struct treeNode *insertNode(struct treeNode *tree, T *pElem, Compare compareFunction, CalcHash calcHashFunction)
    {
        unsigned int curhash = calcHashFunction(pElem) % KEY_SZ;
        if (tree == NULL)
        {
            tree = makeNode();
            // ������ ������ = 0
            tree->pData = pElem;
            tree->hash = curhash;
            tree->pParent = tree;
            return tree;
        }
        // ���� ���� == ����� � ������
        if (curhash == tree->hash)
        {
            external *pTmp = tree->pNext;
            external *pPrevPtr;
            external newElem;

            // ���� external ��������� ��� ��� � � tree ����� ����� ��������
            if (compareFunction(tree->pData, pElem) == 0)
            {
                m_lastError = 2;
                return tree;
            }

            // ���� external ��������� ��� ���
            if (pTmp == NULL)
            {
                newElem.pData = pElem;
                newElem.pNext = NULL;
                tree->pNext = m_ExtStorage.Add(newElem);
                return tree;
            }

            while (pTmp != NULL)
            {
                if (compareFunction(pTmp->pData, pElem) == 0)
                {
                    m_lastError = 2;
                    return tree;
                }
                pPrevPtr = pTmp;
                pTmp = pTmp->pNext;
            }
            newElem.pData = pElem;
            newElem.pNext = NULL;
            pPrevPtr->pNext = m_ExtStorage.Add(newElem);
            return tree;
        }

        // ���� ���� > ����� � ������
        if (curhash > tree->hash)
        {
            // ���� ���� ���� � �������
            if(tree->pRight != NULL)
            {
                // ��������� � ������ ���������
                tree->pRight = insertNode(tree->pRight, pElem, compareFunction, calcHashFunction);
                tree->height = height(tree);

                // ��������� �� ������������������
                if (checkBalance(tree) == 2)
                {
                    if(curhash > tree->pRight->hash)
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
                node->height = 0;
                tree->pRight = node;
                tree->pRight->hash = curhash;
                if (tree->pLeft != NULL)
                    tree->height = max(tree->pLeft->height, tree->pRight->height) + 1;
                else
                    tree->height = tree->pRight->height + 1;
                return tree;
            }
        }
        // ���� ���� < ����� � ������
        if (curhash < tree->hash)
        {
            if(tree->pLeft != NULL)
            {
                // ��������� � ����� ���������
                tree->pLeft = insertNode(tree->pLeft, pElem, compareFunction, calcHashFunction);	
                tree->height = height(tree);
                // ��������� �� ������������������
                if (checkBalance(tree) == 2)
                {
                    if(curhash < tree->pLeft->hash)
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
                node->pParent = tree;
                node->pData = pElem;
                tree->pLeft = node;
                tree->pLeft->hash = curhash;
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
    struct treeNode *removeNode(struct treeNode *tree, T *pElem, Compare compareFunction, CalcHash calcHashFunction)
    {
        unsigned int curhash = calcHashFunction(pElem) % KEY_SZ;
        if(tree == NULL)
            return tree;
        // ���� � ������ ��� ����� ��������� � �������
        if(curhash > tree->hash)
        {
            tree->pRight = removeNode(tree->pRight, pElem, compareFunction, calcHashFunction);
            tree->height = height(tree);
            // ��������� �� ������������������
            if (height(tree->pLeft)- height(tree->pRight) == 2)
            {
                if(curhash > tree->pLeft->hash)
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
        if(curhash < tree->hash)
        {
            tree->pLeft = removeNode(tree->pLeft, pElem, compareFunction, calcHashFunction);
            tree->height = height(tree);
            // ��������� �� ������������������
            if (height(tree->pRight)- height(tree->pLeft) == 2)
            {
                if(curhash < tree->pRight->hash)
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
        if (curhash == tree->hash)
        {
            // ���� ������ ���. ������ ���, �� �������, ��� �� �������� avl-������
            if (tree->pNext == NULL)
            {
                // ���� � �� ��� �����, �� �������
                if (tree->pLeft == NULL && tree->pRight == NULL)
                {
                   /* if (tree->pParent->pLeft == tree)
                        tree->pParent->pLeft = NULL;
                    else
                        tree->pParent->pRight = NULL;

                    tree->pParent->height = height(tree->pParent); */
                    tree = NULL;
                    return tree;
                }
                // ���� ���� ������ ������ �������, �� �� ��������� �������� 
                // data �� ������� � ������� �������, �������� ������� � �������� �� ���� ������.
                // ��� ��� � ��� avl-������, �� � ������� ����� ���� �� �����
                if (tree->pLeft == NULL && tree->pRight != NULL)
                {
                    tree->pData = tree->pRight->pData;
                    tree->pRight = NULL;
                    tree->height = 0;
                    return tree;
                }
                // ����������, ���� ��� ������� �������
                if (tree->pRight == NULL && tree->pLeft != NULL)
                {
                    tree->pData = tree->pLeft->pData;
                    tree->hash = tree->pLeft->hash;
                    tree->pLeft = NULL;
                    tree->height = 0;
                    return tree;
                }
                // ���� ���� ��� �������, ����� ����� ����� ������� ������� ���������,
                // �������� ��� data � ������� ������� � ���������� ������� ���.
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
                    tree = removeNode(tree, tempNode->pData, compareFunction, calcHashFunction);
                    tree->height = height(tree);
                    tempNode = findNode(tree, pElem, calcHashFunction);
                    tempNode->pData = elem;
                    if (height(tree->pLeft)- height(tree->pRight) == 2)
                    {
                        if(curhash > tree->pLeft->hash)
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
                        if(curhash > tree->pRight->hash)
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
            else
            {
                if (compareFunction(tree->pData, pElem) == 0)
                {
                    external *pTmp = tree->pNext->pNext;
                    tree->pData = tree->pNext->pData;
                    tree->pNext = pTmp;
                }
                else
                {
                    external *pTmp = tree->pNext;   //  != NULL
                    external *pPrev;
                    while (compareFunction(pTmp->pData, pElem) != 0)
                    {
                        pPrev = pTmp;
                        pTmp = pTmp->pNext;
                        if(pTmp == NULL)
                        {
                            return tree;
                        }
                    }
                }
            }
        }
        return tree;
    }



    // ������� ������ ������ � ������ �� �����
    struct treeNode *findNode(struct treeNode *tree, T *key, CalcHash calcHashFunction)
    {
        unsigned int curHash = calcHashFunction(key) % KEY_SZ;
        // ���� ����� ���, �� � ������ �����
        if(tree == NULL)
            return NULL;
        // ���� ���� ������ - ����������
        if(curHash ==  tree->hash)
            return tree;
        // ����� ���� � �����������
        if(curHash > tree->hash)
            return findNode(tree->pRight, key, calcHashFunction);
        if(curHash < tree->hash)
            return findNode(tree->pLeft, key, calcHashFunction);
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
    // ��������� external
    CBasicDataBase<external> m_ExtStorage;
    // ��������� treeNode
    CBasicDataBase<treeNode> m_NodeStorage;
};
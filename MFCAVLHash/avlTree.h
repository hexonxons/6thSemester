#pragma once

#include "../includes/memalloc.h"
#include "../includes/banned.h"

// максимальный ключ
const unsigned int KEY_SZ = 2;

template<class T> class CAvlTree
{
public:
	CAvlTree()
	{
		m_pRoot = NULL;
	}

    // функция сравнения
    typedef  int (Compare)(T *pElem1, T *pElem2);
    // функция вычисления хэша по строке
    typedef int (CalcHash)(T *pElem);

    // функция добавления элемента в хэш-таблицу
    T *AddElem(T *pElem, Compare compareFunction, CalcHash calcHashFunction)
    {
        m_pRoot = insertNode(m_pRoot, pElem, compareFunction, calcHashFunction);
        return m_pRoot->pData;
    }

    // функция удаления элемента по ключу
    int DelElem(T* pElem, Compare compareFunction, CalcHash calcHashFunction)
    {
        return 0;
    }

    int getLastError()
    {
        int retErr = m_lastError;
        m_lastError = 0;
        return retErr;
    }

    // функция поиска элемента по ключу
    T *FindElem(T* pKeyWord, Compare compareFunction, CalcHash calcHashFunction)
    {
    }


	
private:
    // структура для совпадающего хэша
    struct external
    {
        external *pNext;
        T *m_pData;
    };

    // структура узла дерева
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

    // функция создания нода
    struct treeNode *makeNode()
    {
        treeNode node;
        // зануляем указатели на все внутренние структуры
        node.pParent = NULL;
        node.pLeft = NULL;
        node.pRight = NULL;
        node.pData = NULL;
        node.pNext = NULL;
        node.height = 0;
        return m_NodeStorage.Add(node);
    }

    // функция вставки элемента в дерево
    struct treeNode *insertNode(struct treeNode *tree, T *pElem, Compare compareFunction, CalcHash calcHashFunction)
    {
        unsigned int curhash = calcHashFunction(pElem) % KEY_SZ;
        if (tree == NULL)
        {
            tree = makeNode();
            // высота дерева = 0
            tree->pData = pElem;
            tree->hash = curhash;
            tree->pParent = tree;
            return tree;
        }
        // если ключ == ключу в дереве
        if (curhash == tree->hash)
        {
            external *pTmp = tree->pNext;
            external *pPrevPtr;
            external newElem;

            // если external элементов еще нет и в tree лежит такое значение
            if (compareFunction(tree->pData, pElem) == 0)
            {
                m_lastError = 2;
                return tree;
            }

            // если external элементов еще нет
            if (pTmp == NULL)
            {
                newElem.m_pData = pElem;
                newElem.pNext = NULL;
                tree->pNext = m_ExtStorage.Add(newElem);
                return tree;
            }

            while (pTmp != NULL)
            {
                if (compareFunction(pTmp->m_pData, pElem) == 0)
                {
                    m_lastError = 2;
                    return tree;
                }
                pPrevPtr = pTmp;
                pTmp = pTmp->pNext;
            }
            newElem.m_pData = pElem;
            newElem.pNext = NULL;
            pPrevPtr->pNext = m_ExtStorage.Add(newElem);
            return tree;
        }

        // если ключ > ключа в дереве
        if (curhash > tree->hash)
        {
            // если есть дети у вершины
            if(tree->pRight != NULL)
            {
                // вставляем в правое поддерево
                tree->pRight = insertNode(tree->pRight, pElem, compareFunction, calcHashFunction);
                tree->height = height(tree);

                // проверяем на сбалансированность
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
                // если детей нет, то создаем лист
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
        // если ключ < ключа в дереве
        if (curhash < tree->hash)
        {
            if(tree->pLeft != NULL)
            {
                // вставляем в левое поддерево
                tree->pLeft = insertNode(tree->pLeft, pElem, compareFunction, calcHashFunction);	
                tree->height = height(tree);
                // проверяем на сбалансорованность
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
                // если детей нет, то создаем лист
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

    // функция удаления элемента из дерева по ключу
    struct treeNode *removeNode(struct treeNode *tree, T key)
    {
        if(tree == NULL)
            return tree;
        // ищем в правом или левом поддереве и удаляем
        if(keyCompare(key, tree->pData->m_key) == 1)
        {
            tree->pRight = removeNode(tree->pRight, key);
            tree->height = height(tree);
            // проверяем на сбалансорованность
            if (height(tree->pLeft)- height(tree->pRight) == 2)
            {
                if(key > tree->pLeft->pData->m_key)
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
        if(keyCompare(key, tree->pData->m_key) == -1)
        {
            tree->pLeft = removeNode(tree->pLeft, key);
            tree->height = height(tree);
            // проверяем на сбалансорованность
            if (height(tree->pRight)- height(tree->pLeft) == 2)
            {
                if(key < tree->pRight->pData->m_key)
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
        if (keyCompare(key, tree->pData->m_key) == 0)
        {
            // если у неё нет детей, то удаляем
            if (tree->pLeft == NULL && tree->pRight == NULL)
            {
                if (tree->pParent->pLeft == tree)
                    tree->pParent->pLeft = NULL;
                else
                    tree->pParent->pRight = NULL;

                tree->pParent->height = height(tree->pParent);
                delete tree->pData;
                tree->pData =NULL;
                tree->pParent = NULL;
                delete tree;
                tree = NULL;
                return tree;
            }
            // если есть только правый ребенок, то мы переносим значения 
            // data из ребенка в текущий элемент, зануляем ребенка и зануляем на него ссылку.
            // так как у нас avl-дерево, то у ребенка детей быть не может
            if (tree->pLeft == NULL && tree->pRight != NULL)
            {
                tree->pData->m_key = tree->pRight->pData->m_key;
                tree->pData->m_data = tree->pRight->pData->m_data;
                delete tree->pRight->pData;
                tree->pRight->pData = NULL;
                delete tree->pRight;
                tree->pRight = NULL;
                tree->height = 0;
                return tree;
            }
            // аналогично, если нет правого ребенка
            if (tree->pRight == NULL && tree->pLeft != NULL)
            {
                tree->pData->m_key = tree->pLeft->pData->m_key;
                tree->pData->m_data = tree->pLeft->pData->m_data;
                delete tree->pLeft->pData;
                tree->pLeft->pData = NULL;
                delete tree->pLeft;
                tree->pLeft = NULL;
                tree->height = 0;
                return tree;
            }
            // если есть оба ребенка, берем самый левый элемент правого поддерева,
            // копируем его data в текущий элемент и рекурсивно удаляем его.
            else
            {
                struct treeNode *tempNode = tree->pRight;
                struct nodeData elem;
                // флаг. 1, если мы удаляем корневой элемент
                int flag = 0;
                if (tree->pParent == tree)
                    flag = 1;

                while (tempNode->pLeft != NULL)
                    tempNode = tempNode->pLeft;

                elem.m_key = tempNode->pData->m_key;
                elem.m_data = tempNode->pData->m_data;
                tree = removeNode(tree, tempNode->pData->m_key);
                tree->height = height(tree);
                tempNode = findNode(tree, key);
                tempNode->pData->m_key = elem.m_key;
                tempNode->pData->m_data = elem.m_data;
                if (height(tree->pLeft)- height(tree->pRight) == 2)
                {
                    if(key > tree->pLeft->pData->m_key)
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
                    if(key > tree->pRight->pData->m_key)
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



    // функция поиска данных в дереве по ключу
    struct treeNode *findNode(struct treeNode *tree, T key)
    {
        // если детей нет, то и искать негде
        if(tree == NULL || (tree->pRight == NULL && tree->pLeft == NULL && tree->pParent == NULL))
            return NULL;
        // если ключ совпал - возвращаем
        if(keyCompare(key, tree->pData->m_key) == 0)
            return tree;
        // иначе ищем в поддеревьях
        if(keyCompare(key, tree->pData->m_key) > 0)
            return findNode(tree->pRight, key);
        if(keyCompare(key, tree->pData->m_key) < 0)
            return findNode(tree->pLeft, key);
        // for avoid warning
        return NULL;
    }


    // функции вращения
	struct treeNode *singleLeftRotate(struct treeNode *node)
	{
		treeNode *tempNode;
		int flag = 0;
		if (node->pParent == node)
			flag = 1;
		// поворачиваем
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
		// поворачиваем
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



	// высота дерева
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



	// проверка сбалансированности
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

	// корень дерева
	struct treeNode *m_pRoot;
    // последняя ошибка
    int m_lastError;
    // хранилище external
    CBasicDataBase<external> m_ExtStorage;
    // хранилище treeNode
    CBasicDataBase<treeNode> m_NodeStorage;
};
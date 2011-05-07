#pragma once

#include "../includes/memalloc.h"

#ifdef _DEBUG
	#include <crtdbg.h>
	#define new new(__FILE__,__LINE__)
#endif



template<class T> class CHashTable
{
public:
    // функция сравнения
    typedef int (*Compare)(T *pElem1, T *pElem2);
    // функция вычисления хэша по строке
    typedef int (*CalcHash)(T *pElem);

	struct STableElem
	{
		struct STableElem *pNext;
		T *pElem;
	};

	CHashTable(unsigned int keySz, CalcHash calcHashFunction, Compare compareFunction)
	{
        m_keySz = keySz;
		m_pTable = new STableElem[m_keySz];
		if (!m_pTable)
			m_lastError = -1;
		else
		{
			// зануляем таблицу
			memset(m_pTable, 0, m_keySz * sizeof(struct STableElem));
			m_lastError = 0;
		}
        m_numOfCollissions = 0;
        m_calcHashFunction = calcHashFunction;
        m_compareFunction = compareFunction;
	}

	virtual ~CHashTable()
	{
		unsigned int i;
		STableElem *ptr;
		STableElem *pPrev;
		if(m_lastError != -1)
		{
			for(i = 0; i < m_keySz; ++i)
			{
				if(m_pTable[i].pNext != NULL)
				{
					pPrev = m_pTable[i].pNext;
					ptr = pPrev->pNext;
					while(ptr != NULL)
					{
						delete pPrev;
						pPrev = ptr;
						ptr = ptr->pNext;
					}
					delete pPrev;
				}
			}
			delete[] m_pTable;
		}
		m_pTable = NULL;		
	}

	

	// функция добавления элемента в хэш-таблицу
	//	Код возврата дублирует переменную m_lastError в классе
	//	Получение ошибки - getLastError()
	//	m_lastError = 2 - элемент существует
	//	m_lastError = 0 - все прошло нормально
	//
	int AddElem(T *pElem)
	{
		unsigned int hash = 0;
		// указатель, куда надо вставлять
		STableElem *ptr = NULL;
		STableElem *pTemp = NULL;

		hash = m_calcHashFunction(pElem) % m_keySz;
		// Если по этому индексу ничего не лежит
		if(m_pTable[hash].pElem == NULL)
			m_pTable[hash].pElem = pElem;
		else
		{

			pTemp = &m_pTable[hash];
			ptr = m_pTable[hash].pNext;
			// если такой ключ уже есть, то возвращаем 2
			if(m_compareFunction(pTemp->pElem, pElem) == 0)
			{
				m_lastError = 2;
				return 2;
			}
			while(ptr != NULL)
			{
				// если такой ключ уже есть, то возвращаем 2
				if(m_compareFunction(ptr->pElem, pElem) == 0)
				{
					m_lastError = 2;
					return 2;
				}
				ptr = ptr->pNext;
				pTemp = pTemp->pNext;
			}
            // увеличиваем число коллизий
            ++m_numOfCollissions;
			// добавление в массив указаделей дополнительных выделенных кусков памяти
			//m_ptrToExternalPointers.Add(ptrToIns);
			pTemp->pNext = new STableElem;
			pTemp->pNext->pNext = NULL;
			pTemp->pNext->pElem = pElem;
		}
		return 0;
	}

	// функция удаления элемента по ключу
	//	Код возврата дублирует переменную m_lastError в классе
	//	Получение ошибки - getLastError()
	//	m_lastError = 2 - элемента не существует
	//	m_lastError = 0 - все прошло нормально
	//
	int DelElem(T* pElem)
	{
		unsigned int hash = 0;
		STableElem *ptr = NULL;
		STableElem *pTemp = NULL;

		hash = m_calcHashFunction(pElem) % m_keySz;
		// получаем указатель на элемент в таблице с хэшем, равным полученному
		pTemp = &m_pTable[hash];
		ptr = m_pTable[hash].pNext;
		if (pTemp->pElem == NULL )
		{
			return 2;
		}

		if (m_compareFunction(pTemp->pElem, pElem) != 0)
		{
			if(ptr == NULL)
			{
				m_lastError = 2;
				return 2;
			}
			while (m_compareFunction(ptr->pElem, pElem) != 0)
			{
				pTemp = ptr;
				ptr = ptr->pNext;
				if (ptr == NULL)
				{
					m_lastError = 2;
					return 2;
				}
			}
			pTemp->pNext = ptr->pNext;
			pTemp->pElem = ptr->pElem;
			delete ptr;
			ptr = NULL;
			return 0;
		}
		else
		{
			if (m_pTable[hash].pNext != NULL)
			{
				ptr = m_pTable[hash].pNext;
				m_pTable[hash].pElem = ptr->pElem;
				m_pTable[hash].pNext = ptr->pNext;
				delete ptr;
				ptr = NULL;
			}
			else
			{
				m_pTable[hash].pElem = NULL;
				m_pTable[hash].pNext = NULL;
			}
			return 0;
		}

		return -1;
	}

	int getLastError()
	{
		int retErr = m_lastError;
		m_lastError = 0;
		return retErr;
	}

	// функция поиска элемента по ключу
	T *FindElem(T* pKeyWord)
	{
		int i = 0;
		int hash = m_calcHashFunction(pKeyWord) % m_keySz;
		STableElem *pCurrent = &m_pTable[hash];
		int count = 1;
		if (pCurrent->pElem == NULL)
			return NULL;

		while (m_compareFunction(pCurrent->pElem, pKeyWord))
		{
			pCurrent = pCurrent->pNext;
			if (pCurrent == NULL)
				return NULL;
		}
		return pCurrent->pElem;
	}

private:
	STableElem *m_pTable;
	int m_lastError;
    int m_numOfCollissions;
    unsigned int m_keySz;
    CalcHash m_calcHashFunction;
    Compare m_compareFunction;
};

#ifdef _DEBUG
    #undef new
#endif
#pragma once
#include <iostream>
#include <functional>
#include "memAlloc.h"
using std::string;

// максимальный ключ
const unsigned int KEY_SZ = 100;

template<class T> class CHashTable
{
public:
	struct STableElem
	{
		struct STableElem *m_pPrev;
		struct STableElem *m_pNext;
		T *m_pElem;
		T *m_pKey;
	};

	CHashTable()
	{
		m_pTable = new STableElem[KEY_SZ];
		memset(m_pTable, 0, KEY_SZ * sizeof(struct STableElem));
		m_lastError = 0;
	}

	virtual ~CHashTable()
	{
		delete[] m_pTable;
	}

	// функция сравнения
	typedef  int (Compare)(T *pElem1, T *pElem2);
	// функция добавления элемента в хэш-таблицу
	// keyWord - ключевой элемент. По нему считается хэш
	// также он добавляется в хранилище
	// element - значение, соответствующее keyWord
	int AddElem(STableElem *pElemToAdd, Compare compareFunction)
	{
		unsigned int curHash = 0;
		STableElem *pointerToInsert;
		STableElem *pTemp;
		curHash = calcHash(pElemToAdd->m_pKey, KEY_SZ);
		if(m_pTable[curHash].m_pElem == NULL)
			m_pTable[curHash] = *pElemToAdd;
		else
		{
			pTemp = &m_pTable[curHash];
			pointerToInsert = m_pTable[curHash].m_pNext;
			// если такой ключ уже есть, то возвращаем 2
			if(compareFunction(pTemp->m_pKey, pElemToAdd->m_pKey) == 0)
			{
				m_lastError = 2;
				return 2;
			}
			while(pointerToInsert != NULL)
			{
				pointerToInsert = pointerToInsert->m_pNext;
				// если такой ключ уже есть, то возвращаем 2
				if(pTemp->m_pKey == pElemToAdd->m_pKey)
				{
					m_lastError = 2;
					return 2;
				}
				pTemp = pTemp->m_pNext;
			}
			pointerToInsert = pElemToAdd;
			// добавление в массив указаделей дополнительных выделенных кусков памяти
			//m_ptrToExternalPointers.Add(ptrToIns);
			pointerToInsert->m_pPrev  = pTemp;
			pointerToInsert->m_pNext = NULL;
			pTemp->m_pNext = pointerToInsert;
		}
		return 0;
	}

	int getLastError()
	{
		int retErr = m_lastError;
		m_lastError = 0;
		return retErr;
	}

	// функция поиска элемента по ключу
	T *FindElem(T* pKeyWord, Compare compareFunction)
	{
		int i = 0;
		int hash = calcHash(pKeyWord, KEY_SZ);
		STableElem *pCurrent = &m_pTable[hash];
		int count = 1;
		if (pCurrent->m_pElem == NULL)
			return NULL;

		while (compareFunction(pCurrent->m_pKey, pKeyWord))
		{
			pCurrent = pCurrent->m_pNext;
			if (pCurrent == NULL)
				return NULL;
		}
		return pCurrent->m_pElem;
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
	// хэш таблица
	struct STableElem *m_pTable;
	// Хранилище
	CBasicDataBase<T> m_memStorage;
	// Хранилище указателей на дополнительно выделенные элементы
	// в случае коллизии
	//CArray <STableElem *> m_ptrToExternalPointers;
	int m_lastError;
};
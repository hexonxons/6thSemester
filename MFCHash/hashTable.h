#pragma once
#include <iostream>
#include <functional>
#include "memAlloc.h"
using std::string;

// ������������ ����
const unsigned int KEY_SZ = 2;

template<class T> class CHashTable
{
public:
	struct STableElem
	{
		struct STableElem *pNext;
		T *pElem;
	};

	CHashTable()
	{
		m_pTable = new STableElem[KEY_SZ];
		if (!m_pTable)
			m_lastError = -1;
		else
		{
			memset(m_pTable, 0, KEY_SZ * sizeof(struct STableElem));
			m_lastError = 0;
		}
	}

	virtual ~CHashTable()
	{
		delete[] m_pTable;
	}

	// ������� ���������
	typedef  int (Compare)(T *pElem1, T *pElem2);
	// ������� ���������� ���� �� ������
	typedef int (CalcHash)(T *pElem);

	// ������� ���������� �������� � ���-�������
	int AddElem(T *pElemToAdd, Compare compareFunction, CalcHash calcHashFunction)
	{
		unsigned int curHash = 0;
		// ���������, ���� ���� ���������
		STableElem *pointerToInsert;
		STableElem *pTemp;
		STableElem elem;

		elem.pElem = pElemToAdd;
		elem.pNext = NULL;

		curHash = calcHashFunction(pElemToAdd) % KEY_SZ;
		if(m_pTable[curHash].pElem == NULL)
			m_pTable[curHash].pElem = pElemToAdd;
		else
		{
			pTemp = &m_pTable[curHash];
			pointerToInsert = m_pTable[curHash].pNext;
			// ���� ����� ���� ��� ����, �� ���������� 2
			if(compareFunction(pTemp->pElem, pElemToAdd) == 0)
			{
				m_lastError = 2;
				return 2;
			}
			while(pointerToInsert != NULL)
			{
				pointerToInsert = pointerToInsert->pNext;
				// ���� ����� ���� ��� ����, �� ���������� 2
				if(compareFunction(pTemp->pElem, pElemToAdd) == 0)
				{
					m_lastError = 2;
					return 2;
				}
				pTemp = pTemp->pNext;
			}
			pointerToInsert = m_externalPointersStorage.Add(elem);
			// ���������� � ������ ���������� �������������� ���������� ������ ������
			//m_ptrToExternalPointers.Add(ptrToIns);
			pTemp->pNext = pointerToInsert;
		}
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
		int i = 0;
		int hash = calcHashFunction(pKeyWord) % KEY_SZ;
		STableElem *pCurrent = &m_pTable[hash];
		int count = 1;
		if (pCurrent->pElem == NULL)
			return NULL;

		while (compareFunction(pCurrent->pElem, pKeyWord))
		{
			pCurrent = pCurrent->pNext;
			if (pCurrent == NULL)
				return NULL;
		}
		return pCurrent->pElem;
	}
private:
	
	STableElem *m_pTable;
	CBasicDataBase<STableElem> m_externalPointersStorage;
	// ��������� ���������� �� ������������� ���������� ��������
	// � ������ ��������
	//CArray <STableElem *> m_ptrToExternalPointers;
	int m_lastError;
};
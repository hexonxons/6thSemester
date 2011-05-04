#pragma once
#include "../includes/memalloc.h"

// ������������ ����
const unsigned int KEY_SZ = 100;

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
			// �������� �������
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
	int AddElem(T *pElem, Compare compareFunction, CalcHash calcHashFunction)
	{
		unsigned int hash = 0;
		// ���������, ���� ���� ���������
		STableElem *ptr;
		STableElem *pTemp;
		STableElem elem;

		elem.pElem = pElem;
		elem.pNext = NULL;

		hash = calcHashFunction(pElem) % KEY_SZ;
		if(m_pTable[hash].pElem == NULL)
			m_pTable[hash].pElem = pElem;
		else
		{
			pTemp = &m_pTable[hash];
			ptr = m_pTable[hash].pNext;
			// ���� ����� ���� ��� ����, �� ���������� 2
			if(compareFunction(pTemp->pElem, pElem) == 0)
			{
				m_lastError = 2;
				return 2;
			}
			while(ptr != NULL)
			{
				ptr = ptr->pNext;
				// ���� ����� ���� ��� ����, �� ���������� 2
				if(compareFunction(pTemp->pElem, pElem) == 0)
				{
					m_lastError = 2;
					return 2;
				}
				pTemp = pTemp->pNext;
			}
			ptr = m_externalPointersStorage.Add(elem);
			// ���������� � ������ ���������� �������������� ���������� ������ ������
			//m_ptrToExternalPointers.Add(ptrToIns);
			pTemp->pNext = ptr;
		}
		return 0;
	}

	// ������� �������� �������� �� �����
	int DelElem(T* pElem, Compare compareFunction, CalcHash calcHashFunction)
	{
		unsigned int hash;
		STableElem *ptr;
		STableElem *pTemp;

		hash = calcHashFunction(pElem) % KEY_SZ;
		// �������� ��������� �� ������� � ������� � �����, ������ �����������
		pTemp = &m_pTable[hash];
		if (pTemp->pElem == NULL)
		{
			return 2;
		}

		if (compareFunction(pTemp->pElem, pElem) != 0)
		{
			ptr = m_pTable[hash].pNext;
			while (compareFunction(ptr->pElem, pElem) != 0)
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
			return 0;
		}
		if (compareFunction(pTemp->pElem, pElem) == 0)
		{
			if (m_pTable[hash].pNext != NULL)
			{
				m_pTable[hash].pElem = m_pTable[hash].pNext->pElem;
				m_pTable[hash].pNext = m_pTable[hash].pNext->pNext;
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
#pragma once
#include "../includes/memalloc.h"

#ifdef _DEBUG
	#include <crtdbg.h>
	#define new new(__FILE__,__LINE__)
#endif


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
			// �������� �������
			memset(m_pTable, 0, KEY_SZ * sizeof(struct STableElem));
			m_lastError = 0;
		}
	}

	virtual ~CHashTable()
	{
		int i;
		STableElem *ptr;
		STableElem *pPrev;
		if(m_lastError != -1)
		{
			for(i = 0; i < KEY_SZ; ++i)
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

	// ������� ���������
	typedef  int (Compare)(T *pElem1, T *pElem2);
	// ������� ���������� ���� �� ������
	typedef int (CalcHash)(T *pElem);

	// ������� ���������� �������� � ���-�������
	//	��� �������� ��������� ���������� m_lastError � ������
	//	��������� ������ - getLastError()
	//	m_lastError = 2 - ������� ����������
	//	m_lastError = 0 - ��� ������ ���������
	//
	int AddElem(T *pElem, Compare compareFunction, CalcHash calcHashFunction)
	{
		unsigned int hash = 0;
		// ���������, ���� ���� ���������
		STableElem *ptr = NULL;
		STableElem *pTemp = NULL;

		hash = calcHashFunction(pElem) % KEY_SZ;
		// ���� �� ����� ������� ������ �� �����
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
				// ���� ����� ���� ��� ����, �� ���������� 2
				if(compareFunction(ptr->pElem, pElem) == 0)
				{
					m_lastError = 2;
					return 2;
				}
				ptr = ptr->pNext;
				pTemp = pTemp->pNext;
			}
			// ���������� � ������ ���������� �������������� ���������� ������ ������
			//m_ptrToExternalPointers.Add(ptrToIns);
			pTemp->pNext = new STableElem;
			pTemp->pNext->pNext = NULL;
			pTemp->pNext->pElem = pElem;
		}
		return 0;
	}

	// ������� �������� �������� �� �����
	//	��� �������� ��������� ���������� m_lastError � ������
	//	��������� ������ - getLastError()
	//	m_lastError = 2 - �������� �� ����������
	//	m_lastError = 0 - ��� ������ ���������
	//
	int DelElem(T* pElem, Compare compareFunction, CalcHash calcHashFunction)
	{
		unsigned int hash = 0;
		STableElem *ptr = NULL;
		STableElem *pTemp = NULL;

		hash = calcHashFunction(pElem) % KEY_SZ;
		// �������� ��������� �� ������� � ������� � �����, ������ �����������
		pTemp = &m_pTable[hash];
		ptr = m_pTable[hash].pNext;
		if (pTemp->pElem == NULL )
		{
			return 2;
		}

		if (compareFunction(pTemp->pElem, pElem) != 0)
		{
			if(ptr == NULL)
			{
				m_lastError = 2;
				return 2;
			}
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
	//CBasicDataBase<STableElem> m_externalPointersStorage;
	// ��������� ���������� �� ������������� ���������� ��������
	// � ������ ��������
	//CArray <STableElem *> m_ptrToExternalPointers;
	int m_lastError;
};
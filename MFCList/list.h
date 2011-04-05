#pragma once

template<class T> class CMyList
{
private:
	struct listElem 
	{
		T *pElem;
		listElem *pNext;
		listElem *pPrev;
	};
public:
	class Iterator
	{
	public:
		Iterator(listElem *elem)
		{
			m_listElem = elem;
		}

		T *Next()
		{
			m_listElem = m_listElem->pNext;
			if (m_listElem != NULL)
				return m_listElem->pElem;
			return NULL;
		}

		T *Current()
		{
			return m_listElem->pElem;
		}

		bool isValid()
		{
			return !(m_listElem == NULL);
		}
	private:
		Iterator():
		   m_listElem(NULL)
		   {
		   }
		listElem *m_listElem;
	};

	CMyList():
	  m_pBegin(NULL),
	  m_pEnd(NULL),
	  m_pCurrent(NULL)
	{
	}

	virtual ~CMyList()
	{
		while (DelFromEnd() != -1)
		{};
	}
	// ���������� -1 ��� ������
	int AddToBegin(T *elem)
	{
		// ���� �������� ���, �� ������ ���� ������� � ���������
		if(m_pBegin == NULL)
		{
			m_pBegin = new listElem;
			if (!m_pBegin)
				return -1;
			m_pBegin->pElem = elem;
			m_pBegin->pPrev = NULL;
			m_pBegin->pNext = NULL;
			// ������ ������� � �������� ���������
			m_pEnd = m_pBegin;
		}
		else
		{
			listElem *newElem = new listElem;
			if (!newElem)
				return -1;
			newElem->pElem = elem;
			newElem->pPrev = NULL;
			newElem->pNext = m_pBegin;
			m_pBegin = newElem;
		}
		return 0;
	}
	// ���������� -1 ��� ������
	int AddToEnd(T *elem)
	{
		// ���� �������� ���
		if(m_pEnd == NULL)
		{
			m_pEnd = new listElem;
			if (!m_pEnd)
				return -1;
			m_pEnd->pElem = elem;
			m_pEnd->pPrev = NULL;
			m_pEnd->pNext = NULL;
			// ������ ������� � �������� ���������
			m_pBegin = m_pEnd;
		}
		else
		{
			listElem *newElem = new listElem;
			if (!newElem)
				return -1;
			newElem->pElem = elem;
			newElem->pPrev = m_pEnd;
			m_pEnd->pNext = newElem;
			newElem->pNext = NULL;
			m_pEnd = newElem;
		}
		return 0;
	}

	int DelFromBegin()
	{
		if(m_pBegin == NULL)
			return -1;
		if(m_pBegin->pNext != NULL)
		{
			m_pBegin = m_pBegin->pNext;
			free(m_pBegin->pPrev);
			m_pBegin->pPrev = NULL;
		}
		else
		{
			free(m_pBegin);
			m_pBegin = NULL;
			m_pEnd = NULL;
		}
		return 0;
	}

	int DelFromEnd()
	{
		if(m_pEnd == NULL)
			return -1;
		if(m_pEnd->pPrev != NULL)
		{
			m_pEnd = m_pEnd->pPrev;
			free(m_pEnd->pNext);
			m_pEnd->pNext = NULL;
		}
		else
		{
			free(m_pEnd);
			m_pEnd = NULL;
			m_pBegin = NULL;
		}
		return 0;
	}

	Iterator Begin()
	{
		return Iterator(m_pBegin);
	}

private:
	// ��������� �� ������
	listElem *m_pBegin;
	// ��������� �� �����
	listElem *m_pEnd;
	// ��������� �� ������� �������
	listElem *m_pCurrent;
};
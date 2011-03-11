#pragma once
#include <string>
using std::string;

// ������ �����
const int BLOCK_SIZE = 60;

typedef struct
{
//	struct data *next;
//	struct data *prev;
	string s;
	int num;
} data;

template<class T> class CBasicDataBase
{
private:
	// ��������� ����� ������
	struct Storage
	{
		T		*pData;
		Storage *pNext;
		Storage *pPrev;
	};

public:
	class Iterator
	{
	public:
		Iterator(Storage *pData, unsigned int blockCount, unsigned int elemCount)
		{
			m_pData = pData;
			m_posInBlock = 0;
			m_blockCount = blockCount;
			m_blockNum = 0;
			m_elemCount = elemCount;
		}

		T *Next()
		{
			++m_posInBlock;
			if (m_posInBlock == BLOCK_SIZE && m_pData->pNext != NULL)
			{
				m_posInBlock = 0;
				m_pData = m_pData->pNext;
				++m_blockNum;
			}
			return m_pData->pData + m_posInBlock;
		}

		T *Current()
		{
			return m_pData->pData + m_posInBlock;
		}

		bool isValid()
		{
			return(m_blockNum * BLOCK_SIZE + m_posInBlock != m_elemCount);
		}
	private:
		Iterator():
			m_pData(NULL),
			m_posInBlock(0),
			m_blockCount(0),
			m_blockNum(0),
			m_elemCount(0)

		{
		}
		Storage *m_pData;
		// ������� ������� � �����
		unsigned int m_posInBlock;
		// ���������� ������
		unsigned int m_blockCount;
		// ����� �������� �����
		unsigned int m_blockNum;
		// ���������� ��������� � �����
		unsigned int m_elemCount;

	};

	// �����������
	CBasicDataBase():
	   m_pData(NULL),
	   m_pBegin(0),
	   m_freeBlockNum(0),
	   m_blockCount(0),
	   m_elemCount(0)
	{
	}

	// ����������
	virtual ~CBasicDataBase()
	{
		// ������������ �� ����� ��������� ����
		while (m_pData->pNext != NULL)
			m_pData = m_pData->pNext;
		// ������������ �� ���� ����� � ������� �������
		while(m_pData->pPrev)
		{
			m_pData = m_pData->pPrev;
			if(m_pData->pNext->pData)
			{
				delete[] m_pData->pNext->pData;
				m_pData->pNext->pData = NULL;
			}
			delete m_pData->pNext;
			m_pData->pNext = NULL;
		}
		delete[] m_pData->pData;
		m_pData->pData = NULL;
		delete m_pData;
		m_pData = NULL;
		m_pBegin = NULL;
	}
	// ������� ���������� �������� � ����
	void Add(T& elem)
	{
		// ���� ���������� ��������� ������ � ������ ����� == 0
		if(m_freeBlockNum == 0)
		{
			// ���� ��� ������ ����, �� �������� �� ���� ������
			if (m_blockCount == 0)
			{
				m_pData = new Storage;
				m_pBegin = m_pData;
				if(m_pData)
				{
					// �������� ������ � ����� �� BLOCK_SIZE ���������
					m_pData->pData = new T[BLOCK_SIZE];
					m_pData->pNext = NULL;
					m_pData->pPrev = NULL;
					// �������� ��� �������� ��� ���������
					m_freeBlockNum = BLOCK_SIZE;
					// ����������� ����� ������ 
					++m_blockCount;
				}
			}
			else
			{
				// ���� �� ������ ����, �� �������� ������ �� ��������� ����
				m_pData->pNext = new Storage;
				// ������ ��������� �����
				m_pData->pNext->pPrev = m_pData;
				// ������������ ��������� �� ����� ����
				m_pData = m_pData->pNext;
				// �������� ������ � ����� �� BLOCK_SIZE ���������
				m_pData->pData = new T[BLOCK_SIZE];
				m_pData->pNext = NULL;
				// �������� ��� �������� ��� ���������
				m_freeBlockNum = BLOCK_SIZE;
				// ����������� ����� ������ 
				++m_blockCount;

			}			
		}
		// ���� � ������� ����� ���� ��������� �����, �� ���������
		m_pData->pData[BLOCK_SIZE - m_freeBlockNum] = elem;
		--m_freeBlockNum;
		++m_elemCount;
	}

	Iterator Begin()
	{
		return Iterator(m_pBegin, m_blockCount, m_elemCount);
	}

private:
	// ��������� �� ������� ����
	Storage *m_pData;
	// ��������� �� ������
	Storage *m_pBegin;
	// ���������� ��������� ���� � �����
	unsigned int m_freeBlockNum;
	// ���������� ������
	unsigned int m_blockCount;
	// ���������� ��������� � �����
	unsigned int m_elemCount;
};
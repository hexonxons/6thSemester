#pragma once

// ������ �����
const int BLOCK_SIZE = 3;

template<class T> class CBasicDataBase
{
private:
	// ��������� ����� ������
	struct Storage
	{
		T		*m_pStorageData;
		Storage *m_pStorageNext;
		Storage *m_pStoragePrev;
	};

public:
	class Iterator
	{
	public:
		Iterator(Storage *pData, unsigned int elemCount)
		{
			m_pData = pData;
			m_posInBlock = 0;
			m_blockNum = 0;
			m_elemCount = elemCount;
		}

		T *Next()
		{
			++m_posInBlock;
			if (m_posInBlock == BLOCK_SIZE && m_pData->m_pStorageNext != NULL)
			{
				m_posInBlock = 0;
				m_pData = m_pData->m_pStorageNext;
				++m_blockNum;
			}
			return m_pData->m_pStorageData + m_posInBlock;
		}

		T *Current()
		{
			return m_pData->m_pStorageData + m_posInBlock;
		}

		int isValid()
		{
			return(m_blockNum * BLOCK_SIZE + m_posInBlock != m_elemCount);
		}
	private:
		Iterator():
			m_pData(NULL),
			m_posInBlock(0),
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
	   m_elemCount(0),
	   m_blockCount(0)
	{
	}

	// ����������
	virtual ~CBasicDataBase()
	{
		if (m_pData != NULL)
		{
			// ������������ �� ����� ��������� ����
			while (m_pData->m_pStorageNext != NULL)
				m_pData = m_pData->m_pStorageNext;
			// ������������ �� ���� ����� � ������� �������
			while(m_pData->m_pStoragePrev)
			{
				m_pData = m_pData->m_pStoragePrev;
				if(m_pData->m_pStorageNext->m_pStorageData)
				{
					delete[] m_pData->m_pStorageNext->m_pStorageData;
					m_pData->m_pStorageNext->m_pStorageData = NULL;
				}
				delete m_pData->m_pStorageNext;
				m_pData->m_pStorageNext = NULL;
			}
			delete[] m_pData->m_pStorageData;
			m_pData->m_pStorageData = NULL;
			delete m_pData;
			m_pData = NULL;
			m_pBegin = NULL;
		}
	}
	// ������� ���������� �������� � ����
	T* Add(T& elem)
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
					m_pData->m_pStoragePrev = NULL;
				}
			}
			else
			{
				// ���� �� ������ ����, �� �������� ������ �� ��������� ����
				m_pData->m_pStorageNext = new Storage;
				// ������ ��������� �����
				m_pData->m_pStorageNext->m_pStoragePrev = m_pData;
				// ������������ ��������� �� ����� ����
				m_pData = m_pData->m_pStorageNext;

			}		
			// �������� ������ � ����� �� BLOCK_SIZE ���������
			m_pData->m_pStorageData = new T[BLOCK_SIZE];
			m_pData->m_pStorageNext = NULL;
			// �������� ��� �������� ��� ���������
			m_freeBlockNum = BLOCK_SIZE;
			// ����������� ����� ������ 
			++m_blockCount;

		}
		// ���������
		m_pData->m_pStorageData[BLOCK_SIZE - m_freeBlockNum] = elem;
		--m_freeBlockNum;
		++m_elemCount;
		return &elem;
	}

	Iterator Begin()
	{
		return Iterator(m_pBegin, m_elemCount);
	}

	T* LastValuePointer()
	{
		unsigned int counter = m_elemCount;
		if (m_blockCount > 1)
			counter = m_elemCount - (m_blockCount - 1) * BLOCK_SIZE;
		return m_pData->m_pStorageData + counter - 1;
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
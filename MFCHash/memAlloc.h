#pragma once

// размер блока
const int BLOCK_SIZE = 3;

template<class T> class CBasicDataBase
{
private:
	// структура блока данных
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
		// текущая позиция в блоке
		unsigned int m_posInBlock;
		// количество блоков
		unsigned int m_blockCount;
		// номер текущего блока
		unsigned int m_blockNum;
		// количество элементов в листе
		unsigned int m_elemCount;

	};

	// конструктор
	CBasicDataBase():
	   m_pData(NULL),
	   m_pBegin(0),
	   m_freeBlockNum(0),
	   m_elemCount(0),
	   m_blockCount(0)
	{
	}

	// деструктор
	virtual ~CBasicDataBase()
	{
		if (m_pData != NULL)
		{
			// перематываем на самый последний блок
			while (m_pData->m_pStorageNext != NULL)
				m_pData = m_pData->m_pStorageNext;
			// возвращаемся на блок назад и удаляем текущий
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
	// функция добавления элемента в лист
	T* Add(T& elem)
	{
		// если количество свободных яйчеек в данном блоке == 0
		if(m_freeBlockNum == 0)
		{
			// если это первый блок, то выделяем на него память
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
				// если не первый блок, то выделяем память на следующий блок
				m_pData->m_pStorageNext = new Storage;
				// делаем указатель назад
				m_pData->m_pStorageNext->m_pStoragePrev = m_pData;
				// переставляем указатель на новый блок
				m_pData = m_pData->m_pStorageNext;

			}		
			// выделяем память в блоке на BLOCK_SIZE элементов
			m_pData->m_pStorageData = new T[BLOCK_SIZE];
			m_pData->m_pStorageNext = NULL;
			// помечаем все элементы как свободные
			m_freeBlockNum = BLOCK_SIZE;
			// увеличиваем число блоков 
			++m_blockCount;

		}
		// добавляем
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
	// указатель на текущий блок
	Storage *m_pData;
	// указатель на начало
	Storage *m_pBegin;
	// количество свободных мест в блоке
	unsigned int m_freeBlockNum;
	// количество блоков
	unsigned int m_blockCount;
	// количество элементов в листе
	unsigned int m_elemCount;
};
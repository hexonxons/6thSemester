#pragma once
#include <string>
using std::string;

// размер блока
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
	// структура блока данных
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
	   m_blockCount(0),
	   m_elemCount(0)
	{
	}

	// деструктор
	virtual ~CBasicDataBase()
	{
		// перематываем на самый последний блок
		while (m_pData->pNext != NULL)
			m_pData = m_pData->pNext;
		// возвращаемся на блок назад и удаляем текущий
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
	// функция добавления жлемента в лист
	void Add(T& elem)
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
					// выделяем память в блоке на BLOCK_SIZE элементов
					m_pData->pData = new T[BLOCK_SIZE];
					m_pData->pNext = NULL;
					m_pData->pPrev = NULL;
					// помечаем все элементы как свободные
					m_freeBlockNum = BLOCK_SIZE;
					// увеличиваем число блоков 
					++m_blockCount;
				}
			}
			else
			{
				// если не первый блок, то выделяем память на следующий блок
				m_pData->pNext = new Storage;
				// делаем указатель назад
				m_pData->pNext->pPrev = m_pData;
				// переставляем указатель на новый блок
				m_pData = m_pData->pNext;
				// выделяем память в блоке на BLOCK_SIZE элементов
				m_pData->pData = new T[BLOCK_SIZE];
				m_pData->pNext = NULL;
				// помечаем все элементы как свободные
				m_freeBlockNum = BLOCK_SIZE;
				// увеличиваем число блоков 
				++m_blockCount;

			}			
		}
		// если в текущем блоке есть свободное место, то добавляем
		m_pData->pData[BLOCK_SIZE - m_freeBlockNum] = elem;
		--m_freeBlockNum;
		++m_elemCount;
	}

	Iterator Begin()
	{
		return Iterator(m_pBegin, m_blockCount, m_elemCount);
	}

private:
	// указатель на текущий блок
	Storage *m_pData;
	// указатель на начало
	Storage *m_pBegin;
	// количество свододных мест в блоке
	unsigned int m_freeBlockNum;
	// количество блоков
	unsigned int m_blockCount;
	// количество элементов в листе
	unsigned int m_elemCount;
};
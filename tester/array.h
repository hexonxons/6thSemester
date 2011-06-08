#ifndef __ARRAY_H__
#define __ARRAY_H__

#include "../includes/banned.h"
#include "../includes/memalloc.h"

#ifdef _DEBUG
    #include <crtdbg.h>
    #define new new(__FILE__,__LINE__)
#endif

template <class T> class CArr
{
public:
    // функция сравнения
    typedef int (*Compare)(T *pElem1, T *pElem2);

    CArr(Compare compareFunction)
    {
        m_compareFunction = compareFunction;
        m_pos = 0;
        m_pArr = NULL;
    }

    virtual ~CArr()
    {
        if (m_pArr != NULL)
        {
            delete m_pArr;
            m_pArr = NULL;
        }
    }

    int Init(unsigned int sz)
    {
        m_pArr = new T *[sz];
        memset(m_pArr, 0, sz * sizeof(T *));
        m_isSort = 0;
        return 0;
    }

    int destroy()
    {
        if (m_pArr != NULL)
        {
            delete m_pArr;
            m_pArr = NULL;
        }
        m_pArr = NULL;
        m_pos = 0;
        return 0;
    }

    int AddElem(T *pElem)
    {
        m_pArr[m_pos] = pElem;
        ++m_pos;
        return 0;
    }

    T *FindElem(T *pElem)
    {
        T *pRet;
        if(m_isSort == 0)
        {
            MergeSort(m_pArr, m_pArr + m_pos);
            m_isSort = 1;
        }
        pRet = BinSearch(m_pArr, pElem, 0, m_pos);
        return pRet;
    }

private:
    void msort(T **first, T **last, T **buf)
    {
        // если не один элемент
        if(first + 1 < last)
        {
            T **mid = first + (last - first) / 2;

            msort(first, mid, buf);
            msort(mid, last, buf);

            T **pLeft = first;
            T **pRight = mid;
            T **ptr = buf;

            while(pLeft != mid && pRight != last)
            {
                if(m_compareFunction(*pLeft, *pRight) < 0)
                    *ptr++ = *pLeft++;
                else
                    *ptr++ = *pRight++;
            }
            while(pRight != last)
                *ptr++ = *pRight++;
            while(pLeft != mid)
                *ptr++ = *pLeft++;

            for(ptr = buf; first != last;)
                *first++ = *buf++;
        }
    }

    void MergeSort(T **first, T **last)
    {
        if(first + 1 < last)
        {
            static T **temp;
            temp = new T*[last - first];
            msort(first, last, temp);
            delete[] temp;
        }
    }

    T *BinSearch(T **A, T *key,  int low, int high)
    {
        int mid = (low + high) / 2;
        if(low == m_pos)
            return NULL;
        if (m_compareFunction(*(A + mid), key) == 0)
            return A[mid];
        if (low >= high)
            return NULL;
        if (m_compareFunction(*(A + mid), key) < 0)
            return BinSearch(A,key,mid+1,high);
        if (m_compareFunction(*(A + mid), key) > 0)
            return BinSearch(A,key,low,mid-1);
    }

    Compare m_compareFunction;
    T **m_pArr;
    unsigned int m_pos;
    int m_isSort;

};

#ifdef _DEBUG
    #undef new
#endif

#endif  // __ARRAY_H__
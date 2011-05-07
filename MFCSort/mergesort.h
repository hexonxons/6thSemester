#pragma once

template<typename T>
void msort(T *first, T *last, T *buf, CompareSort1Type cmpFunc)
{
    // если не один элемент
    if(first + 1 < last)
    {
        T *mid = first + (last - first) / 2;

        msort(first, mid, buf, cmpFunc);
        msort(mid, last, buf, cmpFunc);

        T *pLeft = first;
        T *pRight = mid;
        T *ptr = buf;

        while(pLeft != mid && pRight != last)
        {
            if(cmpFunc(*pLeft, *pRight) < 0)
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

template<typename T>
void MergeSort(T *first, T *last, CompareSort1Type cmpFunc)
{
    if(first + 1 < last)
    {
        static T *temp;
        temp = new T[last - first];
        msort(first, last, temp, cmpFunc);
        delete[] temp;
    }
}
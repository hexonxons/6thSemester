#include "StdAfx.h"
#include "hashtable.h"
#include <string.h>

CHashTable::CHashTable()
{
    // выделяем память на таблицы
    m_pHashAdress = new CHashElem[10000];
    memset(m_pHashAdress, 0, 10000 * sizeof(CHashElem));
    m_pHashClient = new CHashElem[10000];
    memset(m_pHashClient, 0, 10000 * sizeof(CHashElem));
    m_pHashStuff = new CHashElem[10000];
    memset(m_pHashStuff, 0, 10000 * sizeof(CHashElem));

    m_pExternalElements = new CHashElem[10000];
    m_last = 0;
}

CHashTable::~CHashTable()
{
    delete m_pHashAdress;
    delete m_pHashClient;
    delete m_pHashStuff;
    delete m_pExternalElements;
}

int CHashTable::calcHash(CString *pElem)
{
    unsigned int hash = 0;
    for (int i = 0; i < pElem->GetLength(); ++i)
        hash += (*pElem)[i];
    return hash % 10000;
}

void CHashTable::add(CQuad *elem)
{
    int hashAdress = calcHash(elem->adress);
    int hashClient = calcHash(elem->client);
    int hashStuff = calcHash(elem->stuff);
    // Указатель на последний элемент с подсчитанным хешем
    CHashElem *lastElem = NULL;

    if (m_pHashAdress[hashAdress].pElem == NULL)
    {
        m_pHashAdress[hashAdress].pElem = elem;
        m_pHashAdress[hashAdress].pNext = NULL;
    }
    else
    {
        m_pExternalElements[m_last].pElem = elem;
        m_pExternalElements[m_last].pNext = NULL;
        lastElem = &m_pHashAdress[hashAdress];
        while (lastElem->pNext != NULL)
            lastElem = lastElem->pNext;

        lastElem->pNext = &m_pExternalElements[m_last];
        ++m_last;
    }

    if (m_pHashClient[hashClient].pElem == NULL)
    {
        m_pHashClient[hashClient].pElem = elem;
        m_pHashClient[hashClient].pNext = NULL;
    }
    else
    {
        m_pExternalElements[m_last].pElem = elem;
        m_pExternalElements[m_last].pNext = NULL;
        lastElem = &m_pHashClient[hashClient];
        while (lastElem->pNext != NULL)
            lastElem = lastElem->pNext;

        lastElem->pNext = &m_pExternalElements[m_last];
        ++m_last;
    }

    if (m_pHashStuff[hashStuff].pElem == NULL)
    {
        m_pHashStuff[hashStuff].pElem = elem;
        m_pHashStuff[hashStuff].pNext = NULL;
    }
    else
    {
        m_pExternalElements[m_last].pElem = elem;
        m_pExternalElements[m_last].pNext = NULL;
        lastElem = &m_pHashStuff[hashStuff];
        while (lastElem->pNext != NULL)
            lastElem = lastElem->pNext;

        lastElem->pNext = &m_pExternalElements[m_last];
        ++m_last;
    }

}

CHashTable::CHashElem *CHashTable::findClient(CString client)
{
    int hash = calcHash(&client);
    if (m_pHashClient[hash].pElem == NULL)
        return NULL;
    return &m_pHashClient[hash];
}


CHashTable::CHashElem *CHashTable::findAdress(CString adress)
{
    int hash = calcHash(&adress);
    if (m_pHashAdress[hash].pElem == NULL)
        return NULL;
    return &m_pHashAdress[hash];
}


CHashTable::CHashElem *CHashTable::findStuff(CString stuff)
{
    int hash = calcHash(&stuff);
    if (m_pHashStuff[hash].pElem == NULL)
        return NULL;
    return &m_pHashStuff[hash];
}

void CHashTable::del(CQuad *elem)
{
    int hashAdress = calcHash(elem->adress);
    int hashClient = calcHash(elem->client);
    int hashStuff = calcHash(elem->stuff);
    CHashElem *ptrToCurElem;
    CHashElem *prevPtr;

    // удаление из таблицы адресов
    if (m_pHashAdress[hashAdress].pElem == NULL)
        return;
    else
    {
        ptrToCurElem = &m_pHashAdress[hashAdress];
        prevPtr = ptrToCurElem;
        while(true)
        {
            if(strcmp(*(ptrToCurElem->pElem->stuff), *(elem->stuff)) != 0)
            {
                prevPtr = ptrToCurElem;
                ptrToCurElem = ptrToCurElem->pNext;
                if (ptrToCurElem == NULL)
                    return;
            }
            else
            {
                 if(strcmp(*(ptrToCurElem->pElem->client), *(elem->client)) == 0)
                 {
                     // если удаляем элемент, лежащий в самой таблице, а не снаружи
                    if (ptrToCurElem == &m_pHashAdress[hashAdress])
                    {
                        if(ptrToCurElem->pNext == NULL)
                        {
                            ptrToCurElem->pElem = NULL;
                        }
                        else
                        {
                            ptrToCurElem->pElem = ptrToCurElem->pNext->pElem;
                            ptrToCurElem->pNext = ptrToCurElem->pNext->pNext;
                        }
                        break;
                    }
                    prevPtr->pNext = ptrToCurElem->pNext;
                    break;
                 }
            }
        }
    }

    // удаление из таблицы заказчиков
    ptrToCurElem = &m_pHashClient[hashClient];
    prevPtr = ptrToCurElem;
    while(true)
    {
        if(strcmp(*(ptrToCurElem->pElem->stuff), *(elem->stuff)) != 0)
        {
            prevPtr = ptrToCurElem;
            ptrToCurElem = ptrToCurElem->pNext;
            if (ptrToCurElem == NULL)
                return;
        }
        else
        {
             if(strcmp(*(ptrToCurElem->pElem->adress), *(elem->adress)) == 0)
             {
                // если удаляем элемент, лежащий в самой таблице, а не снаружи
                if (ptrToCurElem == &m_pHashClient[hashClient])
                {
                    if(ptrToCurElem->pNext == NULL)
                    {
                        ptrToCurElem->pElem = NULL;
                    }
                    else
                    {
                        ptrToCurElem->pElem = ptrToCurElem->pNext->pElem;
                        ptrToCurElem->pNext = ptrToCurElem->pNext->pNext;
                    }
                    break;
                }
                prevPtr->pNext = ptrToCurElem->pNext;
                break;
             }
        }
    }
    
    // удаление из таблицы заказов
    ptrToCurElem = &m_pHashStuff[hashStuff];
    prevPtr = ptrToCurElem;
    while(true)
    {
        if(strcmp(*(ptrToCurElem->pElem->adress), *(elem->adress)) != 0)
        {
            prevPtr = ptrToCurElem;
            ptrToCurElem = ptrToCurElem->pNext;
            if (ptrToCurElem == NULL)
                return;
        }
        else
        {
             if(strcmp(*(ptrToCurElem->pElem->client), *(elem->client)) == 0)
             {
                // если удаляем элемент, лежащий в самой таблице, а не снаружи
                if (ptrToCurElem == &m_pHashStuff[hashStuff])
                {
                    if(ptrToCurElem->pNext == NULL)
                    {
                        ptrToCurElem->pElem = NULL;
                    }
                    else
                    {
                        ptrToCurElem->pElem = ptrToCurElem->pNext->pElem;
                        ptrToCurElem->pNext = ptrToCurElem->pNext->pNext;
                    }
                    break;
                }
                prevPtr->pNext = ptrToCurElem->pNext;
                break;
             }
        }
    }
}
#include "StdAfx.h"
#include "queue.h"

CQueue::CQueue()
{
    m_pTop = new CQueueElem;
    m_pTop->pNext = NULL;
    m_pTop->pData = NULL;
}

CQueue::~CQueue()
{
    CQueueElem *pElemToDel = m_pTop;
    CQueueElem *pNextElemToDel = m_pTop->pNext;
    if (m_pTop->pData == NULL)
        return;
    while (pNextElemToDel != NULL)
    {
        // удаляем строки
        delete pElemToDel->pData->client;
        pElemToDel->pData->client = NULL;
        delete pElemToDel->pData->adress;
        pElemToDel->pData->adress = NULL;
        delete pElemToDel->pData->stuff;
        pElemToDel->pData->stuff = NULL;

        // удаляем саму CQuad структуру
        delete pElemToDel->pData;
        pElemToDel->pData = NULL;
        // Удаляем CQueueElem структуру
        delete pElemToDel;
        // переставляем указатели
        pElemToDel = pNextElemToDel;
        pNextElemToDel = pElemToDel->pNext;
    }
    // удаляем строки
    delete pElemToDel->pData->client;
    pElemToDel->pData->client = NULL;
    delete pElemToDel->pData->adress;
    pElemToDel->pData->adress = NULL;
    delete pElemToDel->pData->stuff;
    pElemToDel->pData->stuff = NULL;

    delete pElemToDel->pData;
    pElemToDel->pData = NULL;
    delete pElemToDel;
    pElemToDel = NULL;
}

CQuad *CQueue::add(CQuad elem)
{
    if (m_pTop->pData == NULL)
    {
        m_pTop->pData = new CQuad;
        *(m_pTop->pData) = elem;
        m_pLast = m_pTop;
    }
    else
    {
        CQueueElem *tmp = new CQueueElem;
        tmp->pData = new CQuad;
        tmp->pNext = NULL;
        *(tmp->pData) = elem;
        m_pLast->pNext = tmp;
        m_pLast = tmp;
    }
    return m_pLast->pData;
}
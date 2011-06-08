#pragma once

class CQueue
{
public:
    CQueue();
    ~CQueue();
    CQuad *add(CQuad elem);
private:
    struct CQueueElem
    { 
        CQuad *pData;
        CQueueElem *pNext;
    };
    CQueueElem *m_pTop;
    CQueueElem *m_pLast;
};
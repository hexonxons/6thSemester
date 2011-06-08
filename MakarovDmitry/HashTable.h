#pragma once

class CHashTable
{
public:
    struct CHashElem
    {
        CQuad *pElem;
        CHashElem *pNext;
    };

    CHashTable(void);
    ~CHashTable(void);
    int calcHash(CString *);
    void add(CQuad *);
    void del(CQuad *);

    CHashElem *findClient(CString);
    CHashElem *findAdress(CString);
    CHashElem *findStuff(CString);
private:
    // хэш - таблицы для адресов, клиентов, заказов
    CHashElem *m_pHashAdress;
    CHashElem *m_pHashClient;
    CHashElem *m_pHashStuff;

    // массив указателей на дополнительные ячейки в хэш-таблице
    CHashElem *m_pExternalElements;
    // номер последнего добавленного в ячейки элемента
    int m_last;
};

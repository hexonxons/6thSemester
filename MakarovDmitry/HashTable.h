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
    // ��� - ������� ��� �������, ��������, �������
    CHashElem *m_pHashAdress;
    CHashElem *m_pHashClient;
    CHashElem *m_pHashStuff;

    // ������ ���������� �� �������������� ������ � ���-�������
    CHashElem *m_pExternalElements;
    // ����� ���������� ������������ � ������ ��������
    int m_last;
};

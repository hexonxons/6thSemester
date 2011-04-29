// SortTemplate.cpp: implementation of the CSortTemplate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "sort.h"
#include "SortTemplate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

Sort1 *CreateNewSort1Element(const char *pszFIO, const char *pszDate)
{
    Sort1 *pNew = new Sort1();
    if(!pNew)
        return NULL;
    pNew->FIO  = pszFIO;
    pNew->Date = pszDate;
    return pNew;
}

void AddElementToList(Sort1 *pElement, CListBox &List)
{
    CString s;
    s.Format("%s, %s", pElement->FIO, pElement->Date);
    List.AddString(s);
}

void ClearSort1Array(Sort1Array &array)
{
    for(int i = 0; i < array.GetSize(); i++)
    {
        delete array[i];
        array[i] = NULL;
    }
    array.RemoveAll();
}

void DebugPrintElement(Sort1 *pElement)
{
    CString s;
    s.Format("%s, %s\n", pElement->FIO, pElement->Date);
    TRACE(s);
}

void DebugPrintElement(Sort1Array &array)
{
    CString s;
    s.Format("Число элементов массива - %d\nЭлементы массива:\n", array.GetSize());
    TRACE(s);

    for(int i = 0; i < array.GetSize(); i++)
    {
        s.Format("\t%s, %s\n", array[i]->FIO, array[i]->Date);
        TRACE(s);
    }
    TRACE("Конец массива");
}

int CompareSort1FIO(Sort1 *pElem1, Sort1 *pElem2)
{
    return pElem1->FIO.CollateNoCase(pElem2->FIO);
}
int CompareSort1Date(Sort1 *pElem1, Sort1 *pElem2)
{
	return pElem1->Date.CollateNoCase(pElem2->Date);
}
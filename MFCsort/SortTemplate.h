// SortTemplate.h: interface for the CSortTemplate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SORTTEMPLATE_H__680DB19B_866C_40E5_A479_ABF004CDBD1B__INCLUDED_)
#define AFX_SORTTEMPLATE_H__680DB19B_866C_40E5_A479_ABF004CDBD1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

/// Струкрута состоящая из Ф.И.О. и Дата рождения
struct Sort1
{
    CString FIO;    ///< Ф.И.О.
    CString Date;   ///< Дата рождения
};

/// определение массива элементов Sort1
typedef  CArray<Sort1 *, Sort1 *> Sort1Array; 

/// Создать новый элемент Sort1
Sort1 *CreateNewSort1Element(const char *pszFIO, const char *pszDate);

/// Добавить элемен в список
void AddElementToList(Sort1 *pElement, CListBox &List);

/// Очистка массива элементов Sort1
void   ClearSort1Array(Sort1Array &array);

/// Печать элемента в отладочный протокол
void DebugPrintElement(Sort1 *pElement);

/// Печать массива элементов в отладочный протокол
void DebugPrintElement(Sort1Array &array);

/** \brief функция сравнения двух элементов Sort1 по Ф.И.О.
Возвращаемые значения:
    1  - первый элемент меньше второго
    0  - элементы равны
    -1 - первый элемент больше второго

\return
    \li 1  - pElem1 меньше pElem2
    \li 0  - элементы pElem1 и pElem2 равны
    \li -1 - pElem1 больше pElem2
*/
int CompareSort1FIO(Sort1 *pElem1, Sort1 *pElem2); 


/** \brief функция сравнения двух элементов Sort1 по дате рождения
Возвращаемые значения:
    1  - первый элемент меньше второго
    0  - элементы равны
    -1 - первый элемент больше второго

\return
    \li 1  - pElem1 меньше pElem2
    \li 0  - элементы pElem1 и pElem2 равны
    \li -1 - pElem1 больше pElem2
*/
int CompareSort1Date(Sort1 *pElem1, Sort1 *pElem2); 

/** \brief функция сравнения двух элементов Sort1 по дню (календарному) рождения
Возвращаемые значения:
    1  - первый элемент меньше второго
    0  - элементы равны
    -1 - первый элемент больше второго

\return
    \li 1  - pElem1 меньше pElem2
    \li 0  - элементы pElem1 и pElem2 равны
    \li -1 - pElem1 больше pElem2
*/
int CompareSort1DayBird(Sort1 *pElem1, Sort1 *pElem2); 


//--------------------------
//  На будущее
//--------------------------

/// Определение типа сравнения элементов
typedef  int (CompareSort1Type)(Sort1 *pElem1, Sort1 *pElem2);

#endif // !defined(AFX_SORTTEMPLATE_H__680DB19B_866C_40E5_A479_ABF004CDBD1B__INCLUDED_)

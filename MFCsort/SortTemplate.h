// SortTemplate.h: interface for the CSortTemplate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SORTTEMPLATE_H__680DB19B_866C_40E5_A479_ABF004CDBD1B__INCLUDED_)
#define AFX_SORTTEMPLATE_H__680DB19B_866C_40E5_A479_ABF004CDBD1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

/// ��������� ��������� �� �.�.�. � ���� ��������
struct Sort1
{
    CString FIO;    ///< �.�.�.
    CString Date;   ///< ���� ��������
};

/// ����������� ������� ��������� Sort1
typedef  CArray<Sort1 *, Sort1 *> Sort1Array; 

/// ������� ����� ������� Sort1
Sort1 *CreateNewSort1Element(const char *pszFIO, const char *pszDate);

/// �������� ������ � ������
void AddElementToList(Sort1 *pElement, CListBox &List);

/// ������� ������� ��������� Sort1
void   ClearSort1Array(Sort1Array &array);

/// ������ �������� � ���������� ��������
void DebugPrintElement(Sort1 *pElement);

/// ������ ������� ��������� � ���������� ��������
void DebugPrintElement(Sort1Array &array);

/** \brief ������� ��������� ���� ��������� Sort1 �� �.�.�.
������������ ��������:
    1  - ������ ������� ������ �������
    0  - �������� �����
    -1 - ������ ������� ������ �������

\return
    \li 1  - pElem1 ������ pElem2
    \li 0  - �������� pElem1 � pElem2 �����
    \li -1 - pElem1 ������ pElem2
*/
int CompareSort1FIO(Sort1 *pElem1, Sort1 *pElem2); 


/** \brief ������� ��������� ���� ��������� Sort1 �� ���� ��������
������������ ��������:
    1  - ������ ������� ������ �������
    0  - �������� �����
    -1 - ������ ������� ������ �������

\return
    \li 1  - pElem1 ������ pElem2
    \li 0  - �������� pElem1 � pElem2 �����
    \li -1 - pElem1 ������ pElem2
*/
int CompareSort1Date(Sort1 *pElem1, Sort1 *pElem2); 

/** \brief ������� ��������� ���� ��������� Sort1 �� ��� (������������) ��������
������������ ��������:
    1  - ������ ������� ������ �������
    0  - �������� �����
    -1 - ������ ������� ������ �������

\return
    \li 1  - pElem1 ������ pElem2
    \li 0  - �������� pElem1 � pElem2 �����
    \li -1 - pElem1 ������ pElem2
*/
int CompareSort1DayBird(Sort1 *pElem1, Sort1 *pElem2); 


//--------------------------
//  �� �������
//--------------------------

/// ����������� ���� ��������� ���������
typedef  int (CompareSort1Type)(Sort1 *pElem1, Sort1 *pElem2);

#endif // !defined(AFX_SORTTEMPLATE_H__680DB19B_866C_40E5_A479_ABF004CDBD1B__INCLUDED_)

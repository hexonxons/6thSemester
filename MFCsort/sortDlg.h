// sortDlg.h : header file
//

#if !defined(AFX_SORTDLG_H__B51B0A3A_C5A1_4A7E_BD7B_4BE71060C928__INCLUDED_)
#define AFX_SORTDLG_H__B51B0A3A_C5A1_4A7E_BD7B_4BE71060C928__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SortTemplate.h"
/////////////////////////////////////////////////////////////////////////////
// CSortDlg dialog

class CSortDlg : public CDialog
{
// Construction
public:
    CSortDlg(CWnd* pParent = NULL); // standard constructor

// Dialog Data
    //{{AFX_DATA(CSortDlg)
    enum { IDD = IDD_SORT_DIALOG };
    CListBox    m_InputList;    ///< Список отображения входных элементов
    CListBox    m_OutputList;   ///< Список отображения сортированного состояния
    CString         m_FIO;      ///< Значение поля ввода фамилии
    COleDateTime    m_Date;     ///< Значение поля ввода даты
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSortDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    Sort1Array m_Array; ///< Массив для хранения данных ввода

    HICON m_hIcon;


    // Generated message map functions
    //{{AFX_MSG(CSortDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnSort();
    virtual void OnOK();
    afx_msg void OnAdd();
    afx_msg void OnClearList();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
	int m_sortMethod;
public:
	afx_msg void OnCbnSelchangeCombo1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SORTDLG_H__B51B0A3A_C5A1_4A7E_BD7B_4BE71060C928__INCLUDED_)

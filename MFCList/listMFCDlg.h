// listMFCDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "../includes/memalloc.h"
#include "list.h"

struct Data
{
	CString m_FIO;
	CString m_Purchase;
};

// CListMFCDlg dialog
class CListMFCDlg : public CDialog
{
// Construction
public:
	CListMFCDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_LISTMFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CListBox m_OutListBox;
	CString m_sPurchase;
	CString m_sFio;
	// класс с хранением данных
	CBasicDataBase<Data> m_memStorage;
	// класс списка
	CMyList<Data> m_list;
	// итератор на последний добавленный в хранилище элемент
	//CBasicDataBase<CString>::Iterator m_IterToEnd;
public:
	afx_msg void OnBnClickedListbtn();
	afx_msg void OnBnClickedAddbegbtn();
	afx_msg void OnBnClickedDebugstoragebtn();
	afx_msg void OnBnClickedAddendbtn();
	afx_msg void OnBnClickedDelbegbtn();
	afx_msg void OnBnClickedDelendbtn();
};

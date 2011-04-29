// MFCHashDlg.h : header file
//

#pragma once
#include "hashTable.h"
#include "afxwin.h"



// CMFCHashDlg dialog
class CMFCHashDlg : public CDialog
{
// Construction
public:
	CMFCHashDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MFCHASH_DIALOG };

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
public:
	afx_msg void OnBnClickedBtnadd();
	afx_msg void OnBnClickedBtnfind();
private:
	CString m_Key;
	CString m_Value;
	CString m_SearchValue;
	// хэш ничего не знает о том, где лежат данные
	// хэш - hashTable.h
	// а это - интерфейс. Ему знать можно
	CBasicDataBase<CHashTable<string>::STableElem> m_memStorage; 
	// а это - хранилище для строчек. 
	// CHashTable<string>::STableElem * имеет внутри поля
	// T *elem	и T *key;
	// и на них тоже надо выделять память. Мой memman не умеет выделять память на внутренние элементы =(
	// как я понимаю, надо делать так.
	CBasicDataBase<string> m_stringStorage; 
	CHashTable<string> table;
	CListBox m_OutputList;
};

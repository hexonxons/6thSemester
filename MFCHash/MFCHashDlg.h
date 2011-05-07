// MFCHashDlg.h : header file
//

#pragma once
#include "hashTable.h"
#include "afxwin.h"

struct Data
{
	CString key;
	CString value;
};

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
	CListBox m_OutputList;
	CString m_DelValue;
	// хэш ничего не знает о том, где лежат данные
	CBasicDataBase<CHashTable<Data>::STableElem> m_HashStorage; 
	// а это - хранилище для строчек. 
	CBasicDataBase<CString> m_stringStorage; 
	// Хранилище
	CBasicDataBase<Data> m_DataStorage;
	CHashTable<Data> table;
public:
	afx_msg void OnBnClickedBtndel();
};

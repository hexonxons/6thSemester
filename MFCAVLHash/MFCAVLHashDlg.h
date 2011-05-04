// MFCAVLHashDlg.h : header file
//

#pragma once
#include "avltree.h"
#include "../includes/memalloc.h"
#include "afxwin.h"

struct Data
{
	CString key;
	CString value;
};
// CMFCAVLHashDlg dialog
class CMFCAVLHashDlg : public CDialog
{
// Construction
public:
	CMFCAVLHashDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MFCAVLHASH_DIALOG };

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
	CAvlTree<Data> tree;
	CString m_Key;
	CString m_Value;
	CString m_SearchValue;
	CListBox m_OutputList;
    // Хранилище
    CBasicDataBase<Data> m_DataStorage;
public:
    afx_msg void OnBnClickedBtnadd();
    afx_msg void OnBnClickedBtnfind();
private:
    CString m_DelValue;
public:
    afx_msg void OnBnClickedBtndel();
};

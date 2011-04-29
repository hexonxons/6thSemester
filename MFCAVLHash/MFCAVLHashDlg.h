// MFCAVLHashDlg.h : header file
//

#pragma once
#include "avltree.h"
#include "afxwin.h"

struct data
{
	CString m_key;
	CString m_value;
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
	CAvlTree<int, data> tree;
	CString m_keyword;
	CString m_value;
	CString m_searchKey;
	CListBox m_outputList;
};

// MakarovDmitryDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "hashtable.h"
#include "queue.h"


// CMakarovDmitryDlg dialog
class CMakarovDmitryDlg : public CDialog
{
// Construction
public:
	CMakarovDmitryDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MAKAROVDMITRY_DIALOG };

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
    CString m_client;
    CString m_stuff;
    CString m_adress;
    int m_num;
    CString m_clientFind;
    CString m_stuffFind;
    CString m_adressFind;
    CListBox m_output;
    CHashTable m_hash;
    CQueue m_queue;
public:
    afx_msg void OnBnClickedBtnadd();
    afx_msg void OnBnClickedBtnfnd();
    afx_msg void OnBnClickedBtndel();
};

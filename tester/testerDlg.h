// testerDlg.h : header file
//

#pragma once
#include "lab.h"

// CTesterDlg dialog
class CTesterDlg : public CDialog
{
// Construction
public:
	CTesterDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TESTER_DIALOG };

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
    afx_msg void OnBnClickedBtnfill();
    afx_msg void OnBnClickedBtnfind();
    afx_msg void OnBnClickedBtninit();
    afx_msg void OnBnClickedBtnrandfind();
    afx_msg void OnBnClickedBtndestroy();
private:
    CTester<CString> tester;
    unsigned int m_numOfTests;
    double m_avlTime;
    double m_hashTime;
    unsigned int m_collisionCount;
    int m_success;
    double m_storageTime;
    double m_arrayTime;
};

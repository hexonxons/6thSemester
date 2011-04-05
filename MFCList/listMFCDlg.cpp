// listMFCDlg.cpp : implementation file
//
#include "stdafx.h"
#include "listMFC.h"
#include "listMFCDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CListMFCDlg dialog




CListMFCDlg::CListMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CListMFCDlg::IDD, pParent)
	, m_sPurchase(_T(""))
	, m_sFio(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CListMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ORDERLST, m_OutListBox);
	DDX_Text(pDX, IDC_PURCHASE, m_sPurchase);
	DDX_Text(pDX, IDC_FIO, m_sFio);
}

BEGIN_MESSAGE_MAP(CListMFCDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_LISTBTN, &CListMFCDlg::OnBnClickedListbtn)
	ON_BN_CLICKED(IDC_ADDBEGBTN, &CListMFCDlg::OnBnClickedAddbegbtn)
	ON_BN_CLICKED(IDC_DEBUGSTORAGEBTN, &CListMFCDlg::OnBnClickedDebugstoragebtn)
	ON_BN_CLICKED(IDC_ADDENDBTN, &CListMFCDlg::OnBnClickedAddendbtn)
	ON_BN_CLICKED(IDC_DELBEGBTN, &CListMFCDlg::OnBnClickedDelbegbtn)
	ON_BN_CLICKED(IDC_DELENDBTN, &CListMFCDlg::OnBnClickedDelendbtn)
END_MESSAGE_MAP()


// CListMFCDlg message handlers

BOOL CListMFCDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CListMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CListMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CListMFCDlg::OnBnClickedAddbegbtn()
{
	if(!UpdateData(true))
		return;
	if(m_sFio.IsEmpty())
	{
		MessageBox(L"Вы забыли заполнить поле ФИО", L"Error", MB_ICONERROR | MB_OK);
		return;
	}
	if(m_sPurchase.IsEmpty())
	{
		MessageBox(L"Вы забыли заполнить поле заказа!", L"Error", MB_ICONERROR | MB_OK);
		return;
	}
	// добавляем в хранилище
	m_memStorage.Add(m_sFio + ' ' + m_sPurchase);
	m_list.AddToBegin(m_memStorage.End().Current());
	// очистка полей ввода
	m_sFio.Empty();
	m_sPurchase.Empty();
	UpdateData(false);
}

void CListMFCDlg::OnBnClickedAddendbtn()
{
	if(!UpdateData(true))
		return;
	if(m_sFio.IsEmpty())
	{
		MessageBox(L"Вы забыли заполнить поле ФИО", L"Error", MB_ICONERROR | MB_OK);
		return;
	}
	if(m_sPurchase.IsEmpty())
	{
		MessageBox(L"Вы забыли заполнить поле заказа!", L"Error", MB_ICONERROR | MB_OK);
		return;
	}
	// добавляем в хранилище
	m_memStorage.Add(m_sFio + ' ' + m_sPurchase);
	m_list.AddToEnd(m_memStorage.End().Current());
	// очистка полей ввода
	m_sFio.Empty();
	m_sPurchase.Empty();
	UpdateData(false);
}

void CListMFCDlg::OnBnClickedDelbegbtn()
{
	m_list.DelFromBegin();
}

void CListMFCDlg::OnBnClickedDelendbtn()
{
	m_list.DelFromEnd();
}

void CListMFCDlg::OnBnClickedDebugstoragebtn()
{
	m_OutListBox.ResetContent();
	for(CBasicDataBase<CString>::Iterator i = m_memStorage.Begin(); i.isValid(); i.Next())
	{
		CString *str = i.Current();
		m_OutListBox.AddString(*str);
		UpdateData(false);
	}
}

void CListMFCDlg::OnBnClickedListbtn()
{
	m_OutListBox.ResetContent();
	for(CMyList<CString>::Iterator i = m_list.Begin(); i.isValid(); i.Next())
	{
		CString *str = i.Current();
		m_OutListBox.AddString(*str);
		UpdateData(false);
	}
}

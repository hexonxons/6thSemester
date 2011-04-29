// MFCHashDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFCHash.h"
#include "MFCHashDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCHashDlg dialog




CMFCHashDlg::CMFCHashDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMFCHashDlg::IDD, pParent)
	, m_Key(_T(""))
	, m_Value(_T(""))
	, m_SearchValue(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCHashDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_KEYWORD, m_Key);
	DDX_Text(pDX, IDC_VALUE, m_Value);
	DDX_Text(pDX, IDC_KEYSEARCH, m_SearchValue);
	DDX_Control(pDX, IDC_LSTOUT, m_OutputList);
}

BEGIN_MESSAGE_MAP(CMFCHashDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTNADD, &CMFCHashDlg::OnBnClickedBtnadd)
	ON_BN_CLICKED(IDC_BTNFIND, &CMFCHashDlg::OnBnClickedBtnfind)
END_MESSAGE_MAP()


// CMFCHashDlg message handlers

BOOL CMFCHashDlg::OnInitDialog()
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

void CMFCHashDlg::OnPaint()
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
HCURSOR CMFCHashDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int compare(string *pLeft, string *pRight)
{
	return strcmp(pLeft->c_str(), pRight->c_str());
}
void CMFCHashDlg::OnBnClickedBtnadd()
{

	string key;
	string value;
	CHashTable<string>::STableElem newElem;

	if(!UpdateData(TRUE))
		return;

	if(m_Key.IsEmpty())
	{
		AfxMessageBox("Вы забыли заполнить поле ключа!");
		return;
	}
	if(m_Value.IsEmpty())
	{
		AfxMessageBox("Вы забыли заполнить поле значения!");
		return;
	}
	key = m_Key.GetString();
	value = m_Value.GetString();
	newElem.m_pNext = NULL;
	newElem.m_pPrev = NULL;
	// выделяем память на строчки структуры
	m_stringStorage.Add(key);
	newElem.m_pKey = m_stringStorage.LastValuePointer();
	m_stringStorage.Add(value);
	newElem.m_pElem = m_stringStorage.LastValuePointer();
	// выделили память на структуру
	m_memStorage.Add(newElem);

	table.AddElem(m_memStorage.LastValuePointer(), compare);
	if (table.getLastError() == 2)
	{
		AfxMessageBox("Значение с таким ключом уже существует!");
		return;
	}
	m_Key.Empty();
	m_Value.Empty();
	UpdateData(FALSE);
}

void CMFCHashDlg::OnBnClickedBtnfind()
{
	int i = 0;
	string key;
	string *pRetValue;
	if(!UpdateData(TRUE))
		return;

	if(m_SearchValue.IsEmpty())
	{
		AfxMessageBox("Вы забыли заполнить поле ключа для поиска!");
		return;
	}
	m_OutputList.ResetContent();
	key = m_SearchValue.GetString();
	pRetValue = table.FindElem(&key, compare);
	m_SearchValue.Empty();
	if (pRetValue != NULL)
		m_OutputList.AddString(pRetValue->c_str());	
	else
		m_OutputList.AddString("0 elem found.");
	UpdateData(FALSE);
}

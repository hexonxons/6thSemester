// MFCAVLHashDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFCAVLHash.h"
#include "MFCAVLHashDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCAVLHashDlg dialog



int compare(Data *pLeft, Data *pRight)
{
    return strcmp(pLeft->key, pRight->key);
}

// функция вычисления хэша по строке
int calcHash(Data *pElem)
{
    int i;
    unsigned int hash = 0;
    for(i = 0; i < pElem->key.GetLength(); i++)
        hash += pElem->key[i];
    return hash;
}

CMFCAVLHashDlg::CMFCAVLHashDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMFCAVLHashDlg::IDD, pParent)
	, m_Key(_T(""))
	, m_Value(_T(""))
	, m_searchKey(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCAVLHashDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_KEYWORD, m_Key);
	DDX_Text(pDX, IDC_VALUE, m_Value);
	DDX_Text(pDX, IDC_KEYSEARCH, m_searchKey);
	DDX_Control(pDX, IDC_LIST, m_outputList);
}

BEGIN_MESSAGE_MAP(CMFCAVLHashDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BTNADD, &CMFCAVLHashDlg::OnBnClickedBtnadd)
END_MESSAGE_MAP()


// CMFCAVLHashDlg message handlers

BOOL CMFCAVLHashDlg::OnInitDialog()
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

void CMFCAVLHashDlg::OnPaint()
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
HCURSOR CMFCAVLHashDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMFCAVLHashDlg::OnBnClickedBtnadd()
{
    Data elem;

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


    elem.key = m_Key;
    elem.value = m_Value;
    // выделяем память на структуру данных
    //m_DataStorage.Add(elem);

    tree.AddElem(m_DataStorage.Add(elem), compare, calcHash);
    if (tree.getLastError() == 2)
    {
        AfxMessageBox("Значение с таким ключом уже существует!");
        return;
    }
    m_Key.Empty();
    m_Value.Empty();
    UpdateData(FALSE);
}

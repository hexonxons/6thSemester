// MakarovDmitryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MakarovDmitry.h"
#include "MakarovDmitryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMakarovDmitryDlg dialog




CMakarovDmitryDlg::CMakarovDmitryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMakarovDmitryDlg::IDD, pParent)
    , m_client(_T(""))
    , m_stuff(_T(""))
    , m_adress(_T(""))
    , m_num(0)
    , m_clientFind(_T(""))
    , m_stuffFind(_T(""))
    , m_adressFind(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMakarovDmitryDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_CLIENT, m_client);
    DDV_MaxChars(pDX, m_client, 80);
    DDX_Text(pDX, IDC_STUFF, m_stuff);
    DDV_MaxChars(pDX, m_stuff, 80);
    DDX_Text(pDX, IDC_ADRESS, m_adress);
    DDV_MaxChars(pDX, m_adress, 80);
    DDX_Text(pDX, IDC_NUM, m_num);
    DDV_MinMaxInt(pDX, m_num, 1, 600);
    DDX_Text(pDX, IDC_CLIENTFIND, m_clientFind);
    DDV_MaxChars(pDX, m_clientFind, 80);
    DDX_Text(pDX, IDC_STUFFFIND, m_stuffFind);
    DDV_MaxChars(pDX, m_stuffFind, 80);
    DDX_Text(pDX, IDC_ADRESSFIND, m_adressFind);
    DDV_MaxChars(pDX, m_adressFind, 80);
    DDX_Control(pDX, IDC_OUTPUT, m_output);
}

BEGIN_MESSAGE_MAP(CMakarovDmitryDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BTNADD, &CMakarovDmitryDlg::OnBnClickedBtnadd)
    ON_BN_CLICKED(IDC_BTNFND, &CMakarovDmitryDlg::OnBnClickedBtnfnd)
    ON_BN_CLICKED(IDC_BTNDEL, &CMakarovDmitryDlg::OnBnClickedBtndel)
END_MESSAGE_MAP()


// CMakarovDmitryDlg message handlers

BOOL CMakarovDmitryDlg::OnInitDialog()
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

void CMakarovDmitryDlg::OnPaint()
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
HCURSOR CMakarovDmitryDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMakarovDmitryDlg::OnBnClickedBtnadd()
{
    CString client;
    CString stuff;
    CString adress;
    CQuad data;

    if(!UpdateData(TRUE))
        return;
    if(m_client.IsEmpty() || m_adress.IsEmpty() || m_stuff.IsEmpty())
    {
        AfxMessageBox("Вы забыли заполнить поля для вставки!");
        return;
    }

    data.client = new CString;
    data.stuff = new CString;
    data.adress = new CString;

    *(data.client) = m_client;
    *(data.adress) = m_adress;
    *(data.stuff) = m_stuff;
    data.num = m_num;

    m_hash.add(m_queue.add(data));

    m_client.Empty();
    m_adress.Empty();
    m_stuff.Empty();
    m_num = 1;

    UpdateData(FALSE);
}


void CMakarovDmitryDlg::OnBnClickedBtnfnd()
{
    m_output.ResetContent();
    CHashTable::CHashElem *pArr;
    if(!UpdateData(TRUE))
        return;
    if (!m_clientFind.IsEmpty())
    {
        pArr = m_hash.findClient(m_clientFind);
    }
    else
        if (!m_stuffFind.IsEmpty())
        {
            pArr = m_hash.findStuff(m_stuffFind);
        }
        else
            if (!m_adressFind.IsEmpty())
            {
                pArr = m_hash.findAdress(m_adressFind);
            }
            else
            {
                AfxMessageBox("Вы забыли заполнить поля для поиска!");
                return;
            }

    
    if (pArr == NULL)
    {
        m_output.AddString("0 elem found.");
    }
    else
    {
        while (pArr != NULL)
        {
            m_output.AddString(*(pArr->pElem->client) + " " + *(pArr->pElem->stuff) + " " + *(pArr->pElem->adress));
            pArr = pArr->pNext;
        }
    }

    m_clientFind.Empty();
    m_stuffFind.Empty();
    m_adressFind.Empty();
    UpdateData(FALSE);
}


void CMakarovDmitryDlg::OnBnClickedBtndel()
{
    CString client;
    CString stuff;
    CString adress;
    CQuad data;

    if(!UpdateData(TRUE))
        return;

    data.client = new CString;
    data.stuff = new CString;
    data.adress = new CString;

    *(data.client) = m_clientFind;
    *(data.adress) = m_adressFind;
    *(data.stuff) = m_stuffFind;
    data.num = m_num;

    m_hash.del(m_queue.add(data));

    m_clientFind.Empty();
    m_adressFind.Empty();
    m_stuffFind.Empty();
    m_num = 1;

    UpdateData(FALSE);
}

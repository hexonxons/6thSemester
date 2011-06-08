// testerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tester.h"
#include "testerDlg.h"
#include <string>
#include <locale>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const char alphabet[] = {"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ‡·‚„‰Â∏ÊÁËÈÍÎÏÌÓÔÒÚÛÙıˆ˜¯˘˙˚¸˝˛ˇ¿¡¬√ƒ≈®∆«»… ÀÃÕŒœ–—“”‘’÷◊ÿŸ⁄€‹›ﬁﬂ -_1234567890"};

// CTesterDlg dialog

// ÙÛÌÍˆËˇ ‚˚˜ËÒÎÂÌËˇ ı˝¯‡ ÔÓ ÒÚÓÍÂ
int calcHash(CString *pElem)
{
    unsigned int i;
    unsigned int hash = 0;
    std::string str = pElem->GetString();
    for(i = 0; i < str.size(); i++)
        hash += str[i];
    return hash;
}

int compare(CString *pLeft, CString *pRight)
{
    //OutputDebugString("\r\n" + *pLeft + " " + *pRight + "\r\n");
    return (*pLeft).Compare(*pRight);
}

CString gen(int length)
{
    // Heh...
    srand((unsigned int)time(0) % (rand() + 15));
    int i;
    CString arr;
    for (i = 0; i < length; ++i)
    {
        int index = rand() % 131;
        arr += alphabet[index];
    }
    return arr;
}

CTesterDlg::CTesterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTesterDlg::IDD, pParent)
    , tester(calcHash, compare, gen)
    , m_numOfTests(0)
    , m_avlTime(0)
    , m_hashTime(0)
    , m_collisionCount(0)
    , m_success(0)
    , m_storageTime(0)
    , m_arrayTime(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTesterDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_NUMTESTS, m_numOfTests);
    DDX_Text(pDX, IDC_AVLTIME, m_avlTime);
    DDV_MinMaxDouble(pDX, m_avlTime, 0, 1000000);
    DDX_Text(pDX, IDC_HASHTIME, m_hashTime);
    DDV_MinMaxDouble(pDX, m_hashTime, 0, 1000000);
    DDX_Text(pDX, IDC_NUMCOLLISION, m_collisionCount);
    DDX_Text(pDX, IDC_HASHSUCC, m_success);
    DDV_MinMaxInt(pDX, m_success, 0, 1000000);
    DDX_Text(pDX, IDC_ATORAGETIME, m_storageTime);
    DDV_MinMaxDouble(pDX, m_storageTime, 0, 1000000);
    DDX_Text(pDX, IDC_ARRTIME, m_arrayTime);
	DDV_MinMaxDouble(pDX, m_arrayTime, 0, 1000000000);
}

BEGIN_MESSAGE_MAP(CTesterDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BTNFILL, &CTesterDlg::OnBnClickedBtnfill)
    ON_BN_CLICKED(IDC_BTNFIND, &CTesterDlg::OnBnClickedBtnfind)
    ON_BN_CLICKED(IDC_BTNINIT, &CTesterDlg::OnBnClickedBtninit)
    ON_BN_CLICKED(IDC_BTNRANDFIND, &CTesterDlg::OnBnClickedBtnrandfind)
    ON_BN_CLICKED(IDC_BTNDESTROY, &CTesterDlg::OnBnClickedBtndestroy)
END_MESSAGE_MAP()


// CTesterDlg message handlers

BOOL CTesterDlg::OnInitDialog()
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

void CTesterDlg::OnPaint()
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
HCURSOR CTesterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTesterDlg::OnBnClickedBtninit()
{
    if(!UpdateData(TRUE))
        return;

    if(m_numOfTests == 0)
    {
        AfxMessageBox("¬‚Â‰ËÚÂ ÌÂÌÛÎÂ‚ÓÂ ÁÌ‡˜ÂÌËÂ");
        return;
    }
    tester.fillStruct(m_numOfTests);
    m_storageTime = tester.getStorageTime();
    m_hashTime = 0;
    m_arrayTime = 0;
    m_avlTime = 0;
    m_success = 0;
    m_collisionCount = 0;
    UpdateData(FALSE);
}

void CTesterDlg::OnBnClickedBtnfill()
{
    if(!UpdateData(TRUE))
    {
        return;
    }
    tester.testInsert();
    m_avlTime = tester.getAvlTime();
    m_hashTime = tester.getHashTime();
    m_arrayTime = tester.getArrayTime();
    m_collisionCount = tester.getCollisionCount();
    m_storageTime = 0;
    m_success = 0;
    UpdateData(FALSE);
}

void CTesterDlg::OnBnClickedBtnfind()
{
    if(!UpdateData(TRUE))
        return;
    tester.testFind();
    m_avlTime = tester.getAvlTime();
    m_hashTime = tester.getHashTime();
    m_arrayTime = tester.getArrayTime();
    m_success = tester.getSuccess();
    m_storageTime = 0;
    m_collisionCount = 0;
    UpdateData(FALSE);
}


void CTesterDlg::OnBnClickedBtnrandfind()
{
    if(!UpdateData(TRUE))
        return;
    tester.testRandomFind();
    m_avlTime = tester.getAvlTime();
    m_hashTime = tester.getHashTime();
    m_arrayTime = tester.getArrayTime();
    m_success = tester.getSuccess();
    m_storageTime = 0;
    m_collisionCount = 0;
    UpdateData(FALSE);
}

void CTesterDlg::OnBnClickedBtndestroy()
{
    if(!UpdateData(TRUE))
        return;
    tester.testDestroy();
    m_avlTime = tester.getAvlTime();
    m_hashTime = tester.getHashTime();
    m_arrayTime = tester.getArrayTime();
    m_storageTime = tester.getStorageTime();
    m_storageTime = 0;
    m_collisionCount = 0;
    m_success = 0;
    UpdateData(FALSE);
}

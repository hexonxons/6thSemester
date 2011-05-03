// sortDlg.cpp : implementation file
//

#include "stdafx.h"
#include "sort.h"
#include "sortDlg.h"
#include "perfcounter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// определение функции сортировки
void Sort(Sort1Array &array, CompareSort1Type cmpFunc);

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSortDlg dialog

CSortDlg::CSortDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSortDlg::IDD, pParent)
	, m_sortMethod(0)
{
	//{{AFX_DATA_INIT(CSortDlg)
	m_FIO = _T("");
	m_Date = COleDateTime::GetCurrentTime();
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSortDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSortDlg)
	DDX_Control(pDX, IDC_LIST2, m_OutputList);
	DDX_Control(pDX, IDC_LIST1, m_InputList);
	DDX_Text(pDX, IDC_FIO, m_FIO);
	DDV_MaxChars(pDX, m_FIO, 120);
	DDX_DateTimeCtrl(pDX, IDC_DATE, m_Date);
	//}}AFX_DATA_MAP
	DDX_CBIndex(pDX, IDC_COMBO1, m_sortMethod);
}

BEGIN_MESSAGE_MAP(CSortDlg, CDialog)
	//{{AFX_MSG_MAP(CSortDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SORT, OnSort)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_CLEAR_LIST, OnClearList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSortDlg message handlers

BOOL CSortDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSortDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSortDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSortDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSortDlg::OnOK() 
{	
	ClearSort1Array(m_Array);
	CDialog::OnOK();
}

/// Нажатие на кнопку "Очистить"
void CSortDlg::OnClearList() 
{
   if(AfxMessageBox("Вы уверены, что хотите очистить список", MB_YESNO) == IDYES)
    {
		m_InputList.ResetContent();
		m_OutputList.ResetContent();
		ClearSort1Array(m_Array);
	}
}

/// Нажатие на кнопку "Добавить"
void CSortDlg::OnAdd() 
{
    if(!UpdateData(TRUE))
        return;

    if(m_FIO.IsEmpty())
    {
		AfxMessageBox("Вы забыли заполнить поле!");
        return;
    }
    
    CString Date = m_Date.Format("%Y.%m.%d");
	
    Sort1 *pNew = CreateNewSort1Element(m_FIO, Date);
    if(!pNew)
    {
		AfxMessageBox("Ошибка выделения памяти");
        return;
    }

	m_Array.Add(pNew);
	AddElementToList(pNew, m_InputList);
    m_FIO.Empty();
    m_Date = COleDateTime::GetCurrentTime();

    UpdateData(FALSE);
}

/// Нажатие на кнопку "Сортировать"
void CSortDlg::OnSort() 
{
	if(!UpdateData(TRUE))
        return;
	if(m_sortMethod == 0)
		Sort(m_Array, CompareSort1FIO);
	if(m_sortMethod == 1)
		Sort(m_Array, CompareSort1Date); 
	m_OutputList.ResetContent();

	for (int i = 0; i < m_Array.GetSize(); ++i)
	{
		m_OutputList.AddString(m_Array.GetAt(i)->FIO + " " + m_Array[i]->Date);
	}
}

int CompareByFIO (Sort1 *left, Sort1 *right)
{
	return left->FIO.CompareNoCase(right->FIO) < 0;
}

int CompareByDate(Sort1 *left, Sort1 *right)
{
	return left->Date.CompareNoCase(right->Date) < 0;
}


Sort1 **merge(Sort1 **left, Sort1 **right, unsigned int leftSz, unsigned int rightSz, CompareSort1Type compareFunc)
{
	Sort1 **result = (Sort1 **)malloc((leftSz + rightSz) * sizeof(Sort1 *));
	unsigned int rightPtr = 0;
	unsigned int leftPtr = 0;
	unsigned int resPtr = 0;

	while(rightPtr < rightSz && leftPtr < leftSz)
	{
		if(compareFunc(left[leftPtr], right[rightPtr]) < 0)
		{
			result[resPtr] = left[leftPtr];
			++leftPtr;
			++resPtr;
		}
		else
		{
			result[resPtr] = right[rightPtr];
			++rightPtr;
			++resPtr;
		}
	}
	if(leftPtr < leftSz)
	{
		unsigned int i;
		for(i = leftPtr; i < leftSz; ++i)
		{
			result[resPtr] = left[i];
			++resPtr;			
		}

	}

	if(rightPtr < rightSz)
	{
		unsigned int i;
		for(i = rightPtr; i < rightSz; ++i)
		{
			result[resPtr] = right[i];
			++resPtr;			
		}			
	}
	return result;
}

Sort1 **mergesort(Sort1 **arr, unsigned int arrSz, CompareSort1Type compareFunc)
{
	Sort1 **result;
	Sort1 **left;
	Sort1 **right;
	Sort1 **tmpLeft;
	Sort1 **tmpRight;
	if(arrSz <= 1)
		return arr;
	else
	{
		int middle = arrSz/2;
		int i;
		left = (Sort1 **) malloc(middle * sizeof(Sort1 *));
		right = (Sort1 **) malloc((arrSz - middle) * sizeof(Sort1 *));
		if(arrSz % 2 != 0)
		{
			for(i = 0; i < middle; ++i)
				left[i] = arr[i];

			for(i = 0; i < middle + 1; ++i)
				right[i] = arr[middle + i];
		}
		else
			for(i = 0; i < middle; ++i)
			{
				left[i] = arr[i];
				right[i] = arr[arrSz - middle + i];
			}
		tmpLeft = mergesort(left, middle, compareFunc);
		tmpRight = mergesort(right, arrSz - middle, compareFunc);
		result = merge(tmpLeft, tmpRight, middle, arrSz - middle, compareFunc);
		if(tmpLeft != left)
			free(tmpLeft);
		if(tmpRight != right)
			free(tmpRight);

		free(left);
		left = NULL;
		free(right);
		right = NULL;
		tmpLeft = NULL;
		tmpRight = NULL;
	}	
	return result;
}

///Сортировка
void Sort(Sort1Array &array, CompareSort1Type cmpFunc)
{
	int i;
	int sz = array.GetSize();
	Sort1 **a = mergesort(array.GetData(), sz, cmpFunc);
	array.RemoveAll();

	for(i = 0; i < sz; ++i)
		array.Add(a[i]);
	free(a);
	a = NULL;
}

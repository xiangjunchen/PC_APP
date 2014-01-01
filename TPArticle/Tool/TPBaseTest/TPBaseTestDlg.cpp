
// TPBaseTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TPBaseTest.h"
#include "TPBaseTestDlg.h"
#include "TPHtmlCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CTPBaseTestDlg dialog




CTPBaseTestDlg::CTPBaseTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTPBaseTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pHtmlCtrl = NULL;
}

void CTPBaseTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTPBaseTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CTPBaseTestDlg::OnBnClickedButton1)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CTPBaseTestDlg message handlers 
#define CInt64Array CArray<int,int >

void TP_GetOffsetDiv(CString &sHtml)
{
	CString sDivStart = _T("a");
	CString sDivEnd   = _T("b");
	CInt64Array		aStart;
	int iMark  = sHtml.Find(_T("a"),0);
	int iStart = iMark;
	int iEnd   = iMark;
	int iLast = 0;
	CString sOut ;
	while(iEnd >= 0)
	{
		iEnd = sHtml.Find(sDivEnd, iEnd);
		iStart = 0;
		while(iMark < iEnd)
		{
			iStart = sHtml.Find(sDivStart, iStart);
			if(iStart < 0 || iStart > iEnd)	break;

			BOOL bFind = FALSE;
			for (int l = 0; l < aStart.GetSize(); l++)
			{
				if(aStart[l]==iStart)
				{
					bFind = TRUE;
					break;
				}
			}
			if(!bFind)
				iLast = iStart;

			iStart ++;
		}
		if(iLast <= iMark)
			break;

		iEnd++;
		aStart.Add(iLast);
	}
	sOut = sHtml.Mid(iMark, iEnd - iMark+1);
	return ;
}
BOOL CTPBaseTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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
// 	CString sText = _T("<div ---><div><div></div><div></div></div>over");
// 	int iLeng = sText.GetLength();
// 	CString sHtml = _T("aaabbaababbbecbfcd");
// 	TP_GetOffsetDiv(sHtml);

	//CString sPath = TP_GetWindowsSysPath(TP_WINDOWSPATH_PERSONAL);
//	AdjustHtml();

	CDC *pDc = GetDC();
	CRect rtAritcleBack = CRect(10,10,100,100);
	GetDlgItem(IDC_STATIC_HTML)->GetWindowRect(rtAritcleBack);
	ScreenToClient(rtAritcleBack);
	pDc ->DrawText(_T("C:\\1ssssssssss.bmp"),rtAritcleBack,DT_SINGLELINE|DT_LEFT|DT_VCENTER|DT_CALCRECT);
	pDc->MoveTo(CPoint(rtAritcleBack.left,rtAritcleBack.top));
	pDc->LineTo(CPoint(rtAritcleBack.right,rtAritcleBack.bottom));
	CDC *pDC = GetDC();

	ReleaseDC(pDc);

	CPaintDC dc(this); 
	CRect rtClient;
	GetClientRect(&rtClient);
	dc.FillSolidRect(rtClient,RGB(255,0,0));

// 	CPaintDC dc(this); 
// 	dc.FillSolidRect(rtAritcleBack,RGB(255,0,0));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTPBaseTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
void CTPBaseTestDlg::AdjustHtml()
{

	CRect rc;
	GetDlgItem(IDC_STATIC_HTML)->GetWindowRect(&rc);
	this->ScreenToClient(&rc);

	if(NULL == m_pHtmlCtrl)
	{
		m_pHtmlCtrl = new CTPHtmlCtrl;
		m_pHtmlCtrl->Create(NULL,						 // class name
			NULL,										 // title
			(WS_CHILD | WS_VISIBLE ),					 // style
			rc,											 // rectangle
			this,									     // parent
			IDC_STATIC_HTML,										 // control ID
			NULL);									 // frame/doc context not used
	}
	else
	{
		m_pHtmlCtrl->MoveWindow(rc,FALSE);
	}
	m_pHtmlCtrl->Navigate2(_T("\\about.htm"));

}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTPBaseTestDlg::OnPaint()
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
		CPaintDC dc(this); 
		CRect rtClient;
		GetClientRect(&rtClient);
		dc.FillSolidRect(rtClient,RGB(255,0,0));

		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTPBaseTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTPBaseTestDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CWnd *pWNd = GetDlgItem(IDC_STATIC_HTML);
	CRect rtThis ;
	GetWindowRect(&rtThis);
	m_pHtmlCtrl->ShowWindow(SW_HIDE);
	if(pWNd)
	{
		CRect rtHtml;
		pWNd->GetWindowRect(&rtHtml);
		this->ScreenToClient(&rtHtml);
		pWNd->MoveWindow(rtHtml.left, rtHtml.top, rtThis.Width(), rtHtml.Height());
	}
	AdjustHtml();
	m_pHtmlCtrl->ShowWindow(SW_SHOW);
}

void CTPBaseTestDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CWnd *pWNd = GetDlgItem(IDC_STATIC_HTML);
	CRect rtThis ;
	GetWindowRect(&rtThis);
	if(m_pHtmlCtrl)
	{
		//m_pHtmlCtrl->ShowWindow(SW_HIDE);
		if(pWNd)
		{
			CRect rtHtml;
			pWNd->GetWindowRect(&rtHtml);
			this->ScreenToClient(&rtHtml);
			pWNd->MoveWindow(rtHtml.left, rtHtml.top, rtThis.Width()-40, rtHtml.Height());
		}
		AdjustHtml();
		//m_pHtmlCtrl->ShowWindow(SW_SHOW);
	}


	// TODO: Add your message handler code here
}

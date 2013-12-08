
// TPArticleBroadCast.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "TPArticleBroadCast.h"
#include "MainFrm.h"
#include "TPImgPreviewWnd.h"
#include "TPArticleMainDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTPArticleBroadCastApp

BEGIN_MESSAGE_MAP(CTPArticleBroadCastApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CTPArticleBroadCastApp::OnAppAbout)
END_MESSAGE_MAP()


// CTPArticleBroadCastApp construction

CTPArticleBroadCastApp::CTPArticleBroadCastApp()
{

	m_bHiColorIcons = TRUE;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CTPArticleBroadCastApp object

CTPArticleBroadCastApp theApp;


// CTPArticleBroadCastApp initialization

BOOL CTPArticleBroadCastApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));



	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	//CMainFrame* pFrame = new CMainFrame;
	//if (!pFrame)
	//	return FALSE;
	//m_pMainWnd = pFrame;
	//// create and load the frame with its resources
	//pFrame->LoadFrame(IDR_MAINFRAME,
	//	WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
	//	NULL);
	//
	//// The one and only window has been initialized, so show and update it
	//pFrame->ShowWindow(SW_SHOW);
	//pFrame->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand

	TP_InitCtrl();
  	CTPArticleMainDlg dlg;
  	m_pMainWnd = &dlg;
  	INT_PTR nResponse = dlg.DoModal();

	return FALSE;
}


// CTPArticleBroadCastApp message handlers




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

// App command to run the dialog
void CTPArticleBroadCastApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CTPArticleBroadCastApp customization load/save methods

void CTPArticleBroadCastApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CTPArticleBroadCastApp::LoadCustomState()
{
}

void CTPArticleBroadCastApp::SaveCustomState()
{
}

// CTPArticleBroadCastApp message handlers




int CTPArticleBroadCastApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
	TP_ReleaseCtrl();
	return CWinAppEx::ExitInstance();
}

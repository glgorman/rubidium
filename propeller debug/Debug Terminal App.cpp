// P2 Graphical Debug Terminal.cpp : Defines the class behaviors for the application.
//
#include "stdafx.h"
#include "globalunit.h"
#include "P2 Graphical Debug Terminal.h"
#include "MainFrm.h"
#include "DebugFrame.h"
#include "P2 Graphical Debug TerminalDoc.h"
#include "signal view.h"
#include "ComThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDebugTerminalApp

BEGIN_MESSAGE_MAP(CDebugTerminalApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CDebugTerminalApp::OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_TOOLS_DETECTCPUTYPE, &CDebugTerminalApp::OnToolsDetectHardware)
	ON_COMMAND(ID_TOOLS_DISPLAYCPUTYPES, &CDebugTerminalApp::OnToolsDisplaycputypes)
	ON_COMMAND(ID_TOOLS_DETECTCOMPORTS, &CDebugTerminalApp::OnToolsDetectcomports)
END_MESSAGE_MAP()

// CDebugTerminalApp construction

CDebugTerminalApp::CDebugTerminalApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CDebugTerminalApp object

CDebugTerminalApp theApp;

// CDebugTerminalApp initialization


BOOL CDebugTerminalApp::InitInstance()
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
	CWinApp::InitInstance();
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}
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
	SetRegistryKey(_T("P2 Graphical Debug Terminal"));
	LoadStdProfileSettings(8);  // Load standard INI file options (including MRU)
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views

	CMultiDocTemplate *pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_P2_TERMINAL_EDIT_TYPE,
		RUNTIME_CLASS(CTerminalDoc),
		RUNTIME_CLASS(CDebugFrame), // custom MDI child frame
		RUNTIME_CLASS(CTerminalEditView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	pDocTemplate = new CMultiDocTemplate(IDR_P2_GRAPHICAL_DEBUG_TYPE,
		RUNTIME_CLASS(CTerminalDoc),
		RUNTIME_CLASS(CDebugFrame), // custom MDI child frame
		RUNTIME_CLASS(CSignalView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

// Dispatch commands specified on the command line.  Will return FALSE if
// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
// FIXME - this is causing a strange assertion in Debug mode?
#if 0
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
#endif

	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();	
	StartComThread();
	StartTerminalThread();
	return TRUE;
}

int CDebugTerminalApp::ExitInstance()
{
	StopTerminalThread();
	StopComThread();
	Sleep(100);
	return 0;
}

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
void CDebugTerminalApp::OnAppAbout()
{ 
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CDebugTerminalApp message handlers
void CDebugTerminalApp::StartTerminalThread ()
{
	m_pTerminalThread = (CTerminalThread*)AfxBeginThread
		(RUNTIME_CLASS(CTerminalThread),NULL,NULL);
}

void CDebugTerminalApp::StartComThread ()
{
	m_pComThread = (CComThread*)AfxBeginThread
		(RUNTIME_CLASS(CComThread),NULL,NULL);
}

void CDebugTerminalApp::StopTerminalThread ()
{
	m_pTerminalThread->PostThreadMessage(WM_QUIT,0,0);
}

void CDebugTerminalApp::StopComThread ()
{
	m_pComThread->PostThreadMessage(WM_QUIT,0,0);
}

void CDebugTerminalApp::OnToolsDetectcomports()
{
	int status;
	status = AfxMessageBox (_T("Press OK to detect COM ports."),IDOK);
	if (status!=IDOK)
		return;
	else {
		CWaitCursor wc;
		CComThread::EnumerateComPorts (m_pComThread);
		Sleep (500);
	}
}

void CDebugTerminalApp::OnToolsDetectHardware()
{
	int status;
	status = AfxMessageBox (_T("Press OK Check CPU on detected COM ports."),IDOK);
	if (status!=IDOK)
		return;
	else {
		CWaitCursor wc;
		CComThread::FindHardware(m_pComThread);
		Sleep (500);
	}
}

void CDebugTerminalApp::OnToolsDisplaycputypes()
{
	bool result;
	CWaitCursor wc;					
	LRESULT status = AfxMessageBox (_T("Press OK to verify CPU type."),IDOK);
	if (status==IDCANCEL)
		return;

	result = CComThread::GetHardwareVersion(m_pComThread);
}



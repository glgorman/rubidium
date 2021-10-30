// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "afxmt.h"
#include <iostream>

#include "../Frame Lisp/defines.h"
#include "../Frame Lisp/symbol_table.h"
#include "../Frame Lisp/btreetype.h"
#include "../Frame Lisp/node_list.h"
#include "../Frame Lisp/text_object.h"
#include "../Frame Lisp/scripts.h"
#include "../Frame Lisp/frames.h"
#include "../Frame Lisp/frames1.h"
#include "../Frame Lisp/extras.h"
#include "globalunit.h"

#include "P2 Graphical Debug Terminal.h"
#include "ComThread.h"
#include "Upload Progress.h"
#include "ComThreadProgress.h"
#include "PortSettingsPage.h"
#include "Audioinput.h"
#include "AudioOutputPage.h"
#include "MidiConfiguration.h"
#include "OscilloscopeSettings.h"
#include "TestTonePage.h"
#include "DebugFrame.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString CMainFrame::getCurrentTime ()
{
	CString strTime;
	int ihour,imin,isec,imsec;
	struct _SYSTEMTIME theTime;
	GetSystemTime (&theTime);

	ihour = theTime.wHour;
	imin  = theTime.wMinute;
	isec  = theTime.wSecond;
	imsec = theTime.wMilliseconds;

	strTime.Format (_T("%02d:%02d:%02d.%03d "),ihour,imin,isec,imsec);
	return strTime;
}

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_MESSAGE(WM_SET_VERSION_STRING1,OnSetVersionString1)
	ON_MESSAGE(WM_SET_VERSION_STRING2,OnSetVersionString2)
	ON_MESSAGE(WM_RECIEVE_SERIAL_DATA,OnRxSerialString)
	ON_MESSAGE(WM_RECIEVE_DEBUG_DATA,OnRxDebugString)
	ON_COMMAND(ID_TERMINAL_TEXTMODE, &CMainFrame::OnTerminalTextmode)
	ON_COMMAND(ID_DEBUG_OPEN_SERIAL,&CMainFrame::OnConnectSerial)
	ON_COMMAND(ID_DEBUG_CLOSE_SERIAL,&CMainFrame::OnCloseSerial)
	ON_COMMAND(ID_START_TELNET_SERVER,&CMainFrame::OnStartTelnetServer)
	ON_COMMAND(ID_TOOLS_SEND_RESET, &CMainFrame::OnToolsSendhardwarereset)
	ON_COMMAND(ID_PROJECT_PREFERENCES, &CMainFrame::OnProjectPreferences)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_frame->m_debugstr = new debug_stream;
	lframe::set_global();
	command_proc::pFrame = m_frame;
	command_proc::reset_debug_symbols();
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

void CMainFrame::OnTerminalTextmode()
{
	CDebugFrame *pFrame = (CDebugFrame*)MDIGetActive ();
	if (pFrame==NULL) {	
		AfxMessageBox (_T("There is no active view"),IDOK);
		return;
	}	
	CView *pView = (CView *)pFrame->GetActiveView ();
	CTerminalDoc *pDoc =  (CTerminalDoc*) pView->GetDocument ();

	CDebugTerminalApp *pApp;
	pApp = (CDebugTerminalApp*)AfxGetApp();
	CComThread* pComThread = (CComThread*) pApp->m_pComThread;
	pComThread->PostThreadMessage (WM_START_DEBUG,0,0);
}

// This is done in CMainFrame so that the ComThread does not
// need to know anything about which window is currenlty
// active. Thus all received data passes though this 
// function other then data which is exchanged with the
// remote device during hardware detection, port configuration
// etc.

LRESULT CMainFrame::OnRxDebugString (WPARAM w, LPARAM l)
{
	LRESULT res;
	res = 0;

	CDebugFrame *pFrame = (CDebugFrame*)MDIGetActive ();
	if (pFrame==NULL)
		return -1;

	CView *pView = (CView *)pFrame->GetActiveView ();
	CTerminalDoc *pDoc =  (CTerminalDoc*) pView->GetDocument ();
	pView->PostMessage (WM_UPDATE_TEXT,w,l);
	return res;
}

LRESULT CMainFrame::OnRxSerialString (WPARAM w, LPARAM l)
{
	LRESULT res;
	res = 0;
	CDebugFrame *pFrame = (CDebugFrame*)MDIGetActive ();
	if (pFrame==NULL) {
		return -1;
	}
	CView *pView = (CView *)pFrame->GetActiveView ();
	CTerminalDoc *pDoc =  (CTerminalDoc*) pView->GetDocument ();
	
	LPARAM L2 = lframe::preprocess_data ((char*)w);
	command_proc::parse_debug_string();
//	send parsed message to correct view. 
	pView->PostMessage (WM_UPDATE_TEXT,w,l);
	return res;
}

LRESULT CMainFrame::OnSetVersionString1 (WPARAM w, LPARAM l)
{
	int portId;
	CPropertySheet s;
	s.SetWizardMode();
	CComThreadProgress page1;
	CPortSettingsPage page2;
	CUploadProgress page3;
	s.AddPage(&page1);
	s.AddPage(&page2);

	ASSERT(w);
	CString msg;
	CString *pStr = (CString*) w; 
	portId = (int)l;
	msg.Format(_T("ComThread::FindHardware returned: %s\n Found on COM%d:\n"),(*pStr),portId);
	m_str_tracing.Append(msg);
	page1.m_progress_text.Append(m_str_tracing);
	s.DoModal();
	delete pStr;
	return 0;
}

LRESULT CMainFrame::OnSetVersionString2 (WPARAM w, LPARAM l)
{
	LPCSTR pStr = (LPCSTR) w;
	CString version_string, msg;;
	ASSERT(w);
	version_string = CString(pStr);
	msg.Format(_T("ComThread::GetCPUInfo returned: %s\n"),version_string);
	AfxMessageBox(msg,IDOK);
	return 0;
}

void CMainFrame::OnConnectSerial()
{
// retrieve a pointer to the com thread.
	CDebugTerminalApp *pApp;
	pApp = (CDebugTerminalApp*)AfxGetApp();
	CComThread* pComThread = (CComThread*) pApp->m_pComThread;
	CString str1;
	int portNum;
	UINT res;
	WPARAM w;
	portNum = pComThread->GetComPort();
	if (portNum<1)
	{
		AfxMessageBox(_T("NO PORT assigned."),IDOK);
		return;
	}
	else
	{
		str1.Format (_T("Connecting to P2 on COM%d"),portNum);
		res = AfxMessageBox(str1,IDOK);
		w = (WPARAM) portNum;
		pComThread->PostThreadMessage(WM_CREATE_SESSION,w,0);
	}
}

void CMainFrame::OnCloseSerial()
{
// retrieve a pointer to the com thread.
	CDebugTerminalApp *pApp;
	pApp = (CDebugTerminalApp*)AfxGetApp();
	CComThread* pComThread = (CComThread*) pApp->m_pComThread;
	CString str1;
	int portNum;
	WPARAM w;
	portNum = pComThread->GetComPort();
	w = (WPARAM) portNum;
	pComThread->PostThreadMessage(WM_CLOSE_SESSION,w,0);
}

void CMainFrame::OnStartTelnetServer()
{
// retrieve a pointer to the terminal thread.
	CDebugTerminalApp *pApp;
	pApp = (CDebugTerminalApp*)AfxGetApp();
	CTerminalThread *pThread;
	pThread = pApp->m_pTerminalThread;
	pThread->PostThreadMessage(WM_CREATE_SESSION,0,0);
}


void CMainFrame::OnToolsSendhardwarereset()
{
	CDebugTerminalApp *pApp;
	pApp = (CDebugTerminalApp*)AfxGetApp();
	CComThread* pComThread = (CComThread*) pApp->m_pComThread;
	pComThread->PostThreadMessage(WM_SEND_RESET,0,0);
}

void CMainFrame::OnProjectPreferences()
{
	static bool m_bIsActive = false;
	if (m_bIsActive==true)
		return;

	CDebugTerminalApp *pApp;
	pApp = (CDebugTerminalApp*)AfxGetApp();
	CComThread* pComThread = (CComThread*) pApp->m_pComThread;

	m_bIsActive = true;
	CPropertySheet s;
	CPortSettingsPage page0;
	CAudioInput page1;
	CAudioOutputPage page2;
	CMidiConfiguration page3;
	CTestTonePage page4;

	s.SetWizardMode();
	s.AddPage(&page0);
	s.AddPage(&page1);
	s.AddPage(&page2);
	s.AddPage(&page3);
	s.AddPage(&page4);
	s.SetOwner(this);
	s.DoModal();

	WPARAM w1 = (WPARAM) page0.m_port_num;
	WPARAM w2 = (WPARAM) page0.m_baud_rate;
	pComThread->PostThreadMessage(WM_SET_PORT_NUM,w1,0);
	pComThread->PostThreadMessage(WM_SET_BAUD_RATE,w2,0);

// allow message processing to continue
// while blocking this function from exiting!
#if 0
	s.Create();
	CMainFrame::RunModalLoop();
#endif  
	m_bIsActive = false;
}

 // ComThread.cpp : implementation file
//
#include "stdafx.h"
#include "windows.h"
#include "globalunit.h"
#include "P2 Graphical Debug Terminal.h"
#include "../Frame Lisp/telnet.h"
#include "ComThread.h"
#include "ComThreadProgress.h"
#include "PortSettingsPage.h"
#include "Upload Progress.h"
#include "MainFrm.h"

extern int port_speeds [];


IMPLEMENT_DYNCREATE(CComThread, CWinThread)

BEGIN_MESSAGE_MAP(CComThread, CWinThread)
	ON_THREAD_MESSAGE(WM_FIND_HARDWARE,OnFindHardware)
	ON_THREAD_MESSAGE(WM_GET_HARDWARE_VERSION,OnGetHeardwareVersion)
	ON_THREAD_MESSAGE(WM_GET_VERSION_STRING,OnGetVersionString)
	ON_THREAD_MESSAGE(WM_LOAD_HARDWARE,OnLoadHardware)
	ON_THREAD_MESSAGE(WM_SET_PORT_NUM,OnSetPortNum)
	ON_THREAD_MESSAGE(WM_SET_BAUD_RATE,OnSetBaudRate)
	ON_THREAD_MESSAGE(WM_CREATE_SESSION,OnCreateSession)
	ON_THREAD_MESSAGE(WM_CLOSE_SESSION,OnCloseSession)
	ON_THREAD_MESSAGE(WM_TRANSMIT_DATA,OnTransmitData)
	ON_THREAD_MESSAGE(WM_TRANSMIT_CHAR,OnTransmitData)
	ON_THREAD_MESSAGE(WM_SEND_RESET,OnSendResetCmd)
	ON_THREAD_MESSAGE(WM_START_DEBUG,OnStartDebug)
END_MESSAGE_MAP()

CComThread::CComThread()
{
	m_portId=0;
	m_iBytesTransmitted=0;
	m_iBytesRecieved=0;
	m_pages_allocated = 0;
	m_fileBuffer = NULL;
	m_baud_rate = DEFAULT_BAUD_RATE;
#ifdef HAS_CONSOLE
//	m_dos_mode = false;
	AllocConsole();
#endif
}

CComThread::~CComThread()
{
}

size_t CComThread::get_page_size()
{
	SYSTEM_INFO sSysInfo;
	GetSystemInfo(&sSysInfo);
	DWORD dwPageSize = sSysInfo.dwPageSize;
	return dwPageSize;
}

LPVOID CComThread::allocate_buffers(size_t size)
{
	BOOL b_valid;
	LPVOID	p_addr;
	size_t allocation = size;
	p_addr = (LPVOID) VirtualAlloc(NULL,allocation,
    MEM_COMMIT,PAGE_READWRITE|PAGE_NOCACHE);
	ASSERT (p_addr!=NULL);
	b_valid = (BOOL) VirtualLock (p_addr,allocation);
	return p_addr;
}

BOOL CComThread::InitInstance()
{
#if 0
	AfxMessageBox (_T("Com Thread Started"));
#endif
	LPVOID  p_addr;
	m_iPageSize = get_page_size();
	size_t pages_needed = (MAX_BUFFERS*(TxBuffSize+RxBuffSize))/m_iPageSize;
	if ((TxBuffSize+RxBuffSize)%m_iPageSize>0)
		pages_needed++;
	p_addr = allocate_buffers(m_iPageSize*pages_needed);
	m_comunit.config_buffers(p_addr);
	m_pages_allocated = pages_needed;

	return TRUE;
}

int CComThread::Run()
{
	int res;
	res = CWinThread::Run();
	return res;
}

void CComThread::poll_rx ()
{
	DWORD dwStatus;
	if(!m_comunit.is_active())
		return;

	try {
	dwStatus = WaitForSingleObjectEx
		(m_comunit.m_hEventRx,COM_THREAD_POLL_INTERVAL,TRUE);

	if ((dwStatus==WAIT_OBJECT_0)
		||(m_comunit.m_bOverlapped==false))
	{	
		ResetEvent(m_comunit.m_hEventRx);
		m_comunit.read_buffer();
	}}
	catch (_com_error err)
	{
		CComThread::OutputDebugString
			("\r\nFIRST CHANCE Exception in CComThread::poll_rx");
	}
}

BOOL CComThread::OnIdle (LONG lCount)
{
	if(m_comunit.is_active())
	{
		CComThread::poll_rx();
	}
	lCount++;
	CWinThread::OnIdle (lCount);
	return true;
}

int CComThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

// CComThread message handlers
/////////////////////////////////////////////////////
//
// Step 1:  The application posts a WM_FIND_HARDWARE
// message to this thread; and then returns to doing
// application related stuff, like handling OnDraw
// commands in the GUI.
//
// Step 2:  This thread receives a WM_FIND_HARDWARE
// message so that it knows to call the Find
// hardware method in the prop_serial object.
//
// Step 3:  When the prop_serial object is done
// (hopefully sucessfully) - we can post a message
// back to the application main window.
//
// In the meantime - the communit is free to post
// messages on its own to the main window - so
// as to update things like "status text" or
// progress bars, etc.

void CComThread::OnSendResetCmd (WPARAM, LPARAM)
{
	m_comunit.reset_hardware();
}

void CComThread::FindHardware(CComThread *pThread)
{
	pThread->PostThreadMessage(WM_FIND_HARDWARE,1,0);
}

void CComThread::OnFindHardware (WPARAM cmd, LPARAM)
{
	bool result;
	CString *strVersion = new CString;
	result = m_comunit.detect_hardware();
	if (result==true)
	{
		(*strVersion) = m_comunit.m_strVersion;
		int port = m_comunit.GetComPortNumber();
		WPARAM  wparam = (WPARAM) strVersion;
		AfxGetApp()->m_pMainWnd->
			PostMessage(WM_SET_VERSION_STRING1,
			wparam,(LPARAM)port);
	}
}

// proxy call from CDebugTermingalDoc.
// Set the file name and then post the actual message
// to the thread ...
void CComThread::LoadHardwareAndRun(CComThread *pThread,CString strFile)
{
	pThread->m_strFileName = strFile;
	CPropertySheet s;
	s.SetWizardMode();
	s.SetTitle (_T("P2 Binary Upload Manager"));

	CPortSettingsPage page1;
	CUploadProgress page2;
	s.AddPage(&page1);
	s.AddPage(&page2);

	CString msg;
	msg.Format(_T("Preparing to Upload ...  \r\n%s"),strFile);
	CMainFrame *pMain = (CMainFrame*)(AfxGetApp()->GetMainWnd());
	pMain->m_str_tracing.Append(msg);
	page2.m_progress_text.Append(msg);
	msg = _T("\r\nPress NEXT then UPLOAD to continue with upload (or CANCEL)");
	page2.m_progress_text.Append(msg);
	page1.m_pThread = pThread;
	page2.m_pThread = pThread;
	pThread->m_pNotify = &page2;
	BOOL res = s.DoModal();
	pThread->m_pNotify = NULL;
}

// now in the actual thread.  Does the CString contain the
// correct file name to upload?

void CComThread::import_file ()
{
#define SECTOR_SIZE (512)
#define	INCREMENT (SECTOR_SIZE/sizeof(DWORD))

	if (m_fileBuffer==NULL)
		m_fileBuffer = allocate_buffers(MEGABYTE);
	ASSERT(m_fileBuffer);
	int i;
	CFile f;
	CFileException ex;
	UINT flags;
	LPVOID buff;
	flags = CFile::modeRead;
	if (m_strFileName.GetLength()==0)
		return;
	i = 0;
	buff = m_fileBuffer;
	CComThread::OutputDebugString ("\r\nReading File");
	f.Open (m_strFileName,flags,&ex); 
	while (f.Read (buff,512))
	{
#if 0
		CComThread::OutputDebugString ("\r\nReading sector %d",i);
#endif
		buff = (LPVOID) (((DWORD*)buff)+INCREMENT);
		i++;
	}
	f.Close ();
	m_comunit.m_obj_file_size = SECTOR_SIZE*i;
	CComThread::OutputDebugString ("\r\n%d sectors read",i);
}

void CComThread::OnLoadHardware (WPARAM,LPARAM)
{
	CComThread::import_file ();
	size_t sz = m_comunit.m_obj_file_size;
	m_comunit.m_port_speed = m_baud_rate;
	m_comunit.load_hardware (m_fileBuffer,sz);
}

void CComThread::OnCreateSession (WPARAM w, LPARAM)
{
	m_comunit.set_overlapped (true);
	bool result;
	try {
		result = m_comunit.open_com();
		CComThread::OutputDebugString
			("\r\nCComThread::CreateSession ... SUCCESS!");
		m_comunit.open_forth ();
	}
	catch (_com_error err)
	{
		result = false;
		CComThread::OutputDebugString
			("\r\nCComThread::CreateSession ... FAILURE!");
	}
}

void CComThread::OnCloseSession (WPARAM, LPARAM)
{
	m_comunit.close_com();
	Sleep(2000);
}

////////////////////////////////////////////////////////////

CString CComThread::get_com_string(int id)
{
	CString result;
	result.Format(_T("COM%d"),id);
	return result;
}

// every key press in a terminal view is sent to
// this thread, whether there is an active
// connection or not.

void CComThread::OnTransmitData (WPARAM w,LPARAM)
{
	unsigned char c = (unsigned char) w;
	if (m_comunit.is_active())
	{
		m_comunit.tx_char (c);
		m_comunit.tx_comm();
	}
}

void CComThread::OutputDebugString (const char *str)
{
	WPARAM w;
	LPARAM l;
	CWinApp *pApp;
	pApp = AfxGetApp();
	CMainFrame *pMain = (CMainFrame*)pApp->m_pMainWnd;
#if 0
//	TODO - put in a system for various debug levels
//	like typedef enum { NONE, LACONIC, MEDIUM, VERBOSE, }
//	debug_level;
	if (m_debug==false)
		return;
#endif
	w = (WPARAM) _strdup(str);
	l = 0;
#ifdef HAS_CONSOLE
	DWORD dwWritten;
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	WriteFile(hOut,(LPVOID)str,(DWORD)strlen(str),&dwWritten,NULL);
#endif
	if (pMain!=NULL)
	{
	  pMain->PostMessage(WM_RECIEVE_DEBUG_DATA,w,l);
	}
	else
		delete (char*)w;
//	Sleep(1);
}

void CComThread::ConsoleDebugOutput (const char *str, int val)
{
#ifdef HAS_CONSOLE
	size_t len = strlen (str);
	char *p = new char [len+16];
	sprintf_s (p,len+15,str,val);
	DWORD dwWritten;
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	WriteFile(hOut,(LPVOID)p,(DWORD)strlen(p),&dwWritten,NULL);
	delete p;
#endif
}

void CComThread::OutputDebugString (const char *str, char *str2)
{
	size_t len = strlen (str)+strlen(str2);
	char *p = new char [len+16];
	sprintf_s (p,len+15,str,str2);
	WPARAM w = (WPARAM) p;
	LPARAM l;
	CWinApp *pApp;
	pApp = AfxGetApp();
	CMainFrame *pMain = (CMainFrame*)pApp->m_pMainWnd;
	l = 0;
#ifdef HAS_CONSOLE
	DWORD dwWritten;
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	WriteFile(hOut,(LPVOID)p,(DWORD)strlen(p),&dwWritten,NULL);
#endif
	if (pMain!=NULL)
	{
	  pMain->PostMessage(WM_RECIEVE_DEBUG_DATA,w,l);
	}
	else
		delete p;
	Sleep (1);
}

void CComThread::OutputDebugString (const char *str, int val)
{
	size_t len = strlen (str);
	char *p = new char [len+16];
	sprintf_s (p,len+15,str,val);
	WPARAM w = (WPARAM) p;
	LPARAM l;
	CWinApp *pApp;
	pApp = AfxGetApp();
	CMainFrame *pMain = (CMainFrame*)pApp->m_pMainWnd;
	l = 0;
#ifdef HAS_CONSOLE
	DWORD dwWritten;
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	WriteFile(hOut,(LPVOID)p,(DWORD)strlen(p),&dwWritten,NULL);
#endif
	if (pMain!=NULL)
	{
	  pMain->PostMessage(WM_RECIEVE_DEBUG_DATA,w,l);
	}
	else
		delete p;
	Sleep (1);
}

void CComThread::UpdateProgress (size_t bytes, size_t total)
{
	CDebugTerminalApp* pApp;
	pApp = (CDebugTerminalApp*)AfxGetApp();
	CComThread *pThread = pApp->m_pComThread;
	WPARAM w = (WPARAM) bytes;
	LPARAM l = (LPARAM) total;
	CPropertyPage *notify;
	notify = pThread->m_pNotify; 
	if (notify!=NULL)
		notify->PostMessage (WM_SET_UPLOAD_PROGRESS,w,l);
}

void CComThread::EnumerateComPorts(CComThread* pThread)
{
	pThread->PostThreadMessage(WM_FIND_HARDWARE,0,0);
}


bool CComThread::GetHardwareVersion(CComThread *pThread)
{
	bool result;
	if (pThread==NULL)
		return false;
	pThread->PostThreadMessage(WM_GET_HARDWARE_VERSION,0,0);
	result = true;
	return result;
}

void CComThread::OnGetHeardwareVersion (WPARAM,LPARAM)
{
	m_comunit.set_version_string ();
}

// the application, or a document within the application
// has requested that the application level copy of 
// the version string be updated.  Todo - handdle 
// multiple P2 chips connected over Wifi, or multiple
// com ports - and make this information avaiable in
// a dialog; similar to the way we do MidiEnnumerateDevices
// and the MidiGetCaps, or RasEnum devices, etc.

void CComThread::OnGetVersionString (WPARAM w,LPARAM)
{
	int portNum = (int)w;
	CString str;
	LPCSTR pStr;
	m_comunit.detect_hardware();
	str = m_comunit.m_strVersion;
	pStr = (LPCSTR) &str;
	CWinApp *pApp = AfxGetApp();
	CWnd *pWnd = pApp->GetMainWnd();
	pWnd->SendMessage (WM_SET_VERSION_STRING1,(WPARAM)pStr,0);
}

void CComThread::OnSetPortNum(WPARAM w,LPARAM)
{
	int portNum = (int) w;
	m_portId = portNum;
	m_comunit.m_iPortId = m_portId;
}

void CComThread::OnSetBaudRate(WPARAM w,LPARAM)
{
	m_baud_rate = (int)w;
	m_comunit.m_port_speed = m_baud_rate;
	m_comunit.reset_dcb (&m_comunit.m_CommDCB,false);
}

void CComThread::OnStartDebug(WPARAM w, LPARAM)
{
#if 0
	m_comunit.start_debug();
	propeller_adc fakep2;
	fakep2.main ();
#endif
}

int CComThread::GetComPort(void)
{
	int portNumber = m_comunit.GetComPortNumber();
	return portNumber;
}


CString CComThread::GetPortStatus(void)
{
	CString str;
	return str;
}


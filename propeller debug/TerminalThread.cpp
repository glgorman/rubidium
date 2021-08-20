
#include "stdafx.h"
#include "../Frame Lisp/telnet.h"
#include "globalunit.h"
#include "P2 Graphical Debug Terminal.h"
#include "ComThread.h"

IMPLEMENT_DYNCREATE(CTerminalThread, CWinThread)

BEGIN_MESSAGE_MAP(CTerminalThread, CWinThread)
	ON_THREAD_MESSAGE(WM_CREATE_SESSION,CreateSession)
	ON_THREAD_MESSAGE(WM_CLOSE_SESSION,CloseSession)
	ON_THREAD_MESSAGE(WM_TRANSMIT_STRING,TransmitString)
	ON_THREAD_MESSAGE(WM_TRANSMIT_DATA,OnTransmitData)
	ON_THREAD_MESSAGE(WM_TRANSMIT_CHAR,OnTransmitData)
END_MESSAGE_MAP()

CTerminalThread::CTerminalThread()
{
	m_iBytesTransmitted=0;
	m_iBytesRecieved=0;
}

CTerminalThread::~CTerminalThread()
{

}

BOOL CTerminalThread::OnIdle (LONG lCount)
{
	if (m_bConnected)
		CTerminalThread::ReadBuffer(NULL,NULL);
	lCount++;
	CWinThread::OnIdle (lCount);
	Sleep (50);
	return true;
}

int CTerminalThread::Run()
{
	int res;
	res = CWinThread::Run();
	return res;
}

BOOL CTerminalThread::InitInstance()
{
	m_server = NULL;
	m_bConnected = false;
	return true;
}

int CTerminalThread::ExitInstance()
{
	return 0;
}
		
void CTerminalThread::CreateSession (WPARAM,LPARAM)
{
	m_status = false;
#if 0
	size_t tbytes = sizeof(TelnetServer);
	static char ts[sizeof(TelnetServer)];
	m_server = new ((void*)ts) TelnetServer;
#endif
	m_server = new TelnetServer;

	AfxMessageBox (_T("Attempting to create Telnet Server"),IDOK);
	BOOL result = m_server->InitInstance();
	m_server->WaitForConnection();
	while (m_status==false) 
	{
		m_status = m_server->authenticate();
	}
	m_bConnected = true;
}

void CTerminalThread::CloseSession (WPARAM,LPARAM)
{
	ASSERT(false);
}

void CTerminalThread::ReadBuffer (WPARAM,LPARAM)
{
	int status;
	char buffer [256];
	char *msg_buf;
	status = m_server->getstr (buffer);
	if (status==0)
		return;
	msg_buf = _strdup(buffer);
	AfxGetApp()->m_pMainWnd->
	PostMessage(WM_RECIEVE_SERIAL_DATA,
		(WPARAM)msg_buf,0);
}

void CTerminalThread::TransmitString (WPARAM w, LPARAM)
{
	char *str;
	str = (char *) w;
	m_server->putText (str);
}

void CTerminalThread::OnTransmitData (WPARAM w, LPARAM)
{
	char str[256];
	str[0]=(char)w;
	str[1]=0;
	if (m_server==NULL)
		return;
	else
		m_server->putText (str);
}

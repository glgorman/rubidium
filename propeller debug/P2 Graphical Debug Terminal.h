// P2 Graphical Debug Terminal.h : main header file for the P2 Graphical Debug Terminal application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "messages.h"
#include "compatibility.h"
#include "debugdisplayunit.h"
#include "propserial.h"

class CComThread;
class CTerminalThread;

class CDebugTerminalApp: public CWinApp
{
protected:
	void StartTerminalThread ();
	void StopTerminalThread ();
	void StartComThread ();
	void StopComThread ();

public:
	CTerminalThread *m_pTerminalThread;
	CComThread *m_pComThread;

public:
	CDebugTerminalApp();
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnAppAbout();
	afx_msg void OnToolsDetectHardware();
	afx_msg void OnToolsDisplaycputypes();
	afx_msg void OnToolsDetectcomports();
};


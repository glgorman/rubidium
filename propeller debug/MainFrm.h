// MainFrm.h : interface of the CMainFrame class
//
#pragma once

class lframe;
class debug_stream;

class CMainFrame: public CMDIFrameWnd
{
public:
	lframe *m_frame;
	debug_stream *m_debugstr;

	CString m_str_version, m_str_tracing;
	DECLARE_DYNAMIC(CMainFrame)

public:
	static CString getCurrentTime ();
	CMainFrame();
	virtual ~CMainFrame();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSetVersionString1 (WPARAM,LPARAM);
	afx_msg LRESULT OnSetVersionString2 (WPARAM,LPARAM);
	afx_msg LRESULT OnTraceDebugMessage (WPARAM,LPARAM);
	afx_msg LRESULT OnRxDebugString (WPARAM,LPARAM);
	afx_msg LRESULT OnRxSerialString (WPARAM,LPARAM);
	DECLARE_MESSAGE_MAP()

public:
// these functions are associated with menu items
// thus they take no parameters.
	afx_msg void OnTerminalTextmode();
	afx_msg void OnStartTelnetServer();
	afx_msg void OnConnectSerial();
	afx_msg void OnCloseSerial();
	afx_msg void OnToolsSendhardwarereset();
	afx_msg void OnProjectPreferences();
};



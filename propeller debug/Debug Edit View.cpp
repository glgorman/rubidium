// TerminalView.cpp : implementation file
//

#include "stdafx.h"
#include "globalunit.h"
#include "P2 Graphical Debug Terminal.h"
#include "signal view.h"
#include "comthread.h"

// CTerminalEditView

IMPLEMENT_DYNCREATE(CTerminalEditView, CEditView)

CTerminalEditView::CTerminalEditView()
{
	
}

CTerminalEditView::~CTerminalEditView()
{
}

BEGIN_MESSAGE_MAP(CTerminalEditView, CEditView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_PALETTECHANGED()
	ON_WM_QUERYNEWPALETTE()
	ON_WM_TIMER ()
	ON_WM_KEYDOWN ()
	ON_WM_KEYUP ()
	ON_WM_CHAR ()
//	ON_WM_MOUSEMOVE()
//	ON_WM_MOUSEWHEEL()
//	ON_WM_RBUTTONDOWN()
//	ON_WM_LBUTTONDOWN()
//	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_FILE_PRINT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CEditView::OnFilePrintPreview)
	ON_MESSAGE (WM_UPDATE_TEXT,OnRxString)
END_MESSAGE_MAP()

// CTerminalEditView diagnostics

#ifdef _DEBUG
void CTerminalEditView::AssertValid() const
{
	CEditView::AssertValid();
}

#ifndef _WIN32_WCE
void CTerminalEditView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif
#endif //_DEBUG

BOOL CTerminalEditView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	BOOL bPreCreated = CEditView::PreCreateWindow(cs);
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// Enable word-wrapping
	return bPreCreated;
}

void CTerminalEditView::OnInitialUpdate()
{
	m_connected = false;
	m_local_echo = true;
	m_double_echo = false;
	CString newTextLine (_T("Propeler P2 Debug Terminal 0.1\r\n>"));
	CEdit &theEdit = GetEditCtrl();
	theEdit.SetSel(-1,-1,FALSE);
	theEdit.ReplaceSel(newTextLine);
}

LRESULT CTerminalEditView::OnRxString (WPARAM w, LPARAM l)
{
	m_connected = true;;
	m_double_echo = false;
	m_local_echo = false;
	LRESULT res = 0;
	char *rx_buffer = (char*)w;
	CString msg(rx_buffer);
	CString newTextLine (_T("\r\nNOT CONNECTED\r\n>"));

	CEdit &theEdit = GetEditCtrl();
	theEdit.SetSel(-1,-1,FALSE);
	theEdit.ReplaceSel(msg);
	delete rx_buffer;
	return res;
}

BOOL CTerminalEditView::OnEraseBkgnd(CDC* pDC) 
{
// true means we did the erasure and dont want
// the framework to call the base class.  This
// should be set to true if we are doing double
// buffering and all updates are actually done
// by blitting.
	BOOL result = false;
	return result;
// Unfortunately, the CEditview class will
// call the default OnErase method no matter what
// we do - so just go ahead and return false,
// which allows the base class method to run
#if 0
	COLORREF background_color = COLOR::dark_green;
	CRect	update_region;
	GetClientRect (&update_region);
	BOOL m_defaultErase, result;
	m_defaultErase=false;
	if (m_defaultErase==false)
	{
		SETCOLOR _(pDC,background_color);
		pDC->Rectangle(update_region);
		//	m_update = true;
		result=true;
	}
#endif
	return result;
}

afx_msg void CTerminalEditView::OnChar (UINT nchar, UINT rep_count, UINT flags)
{
	CDebugTerminalApp* pApp;
	pApp = (CDebugTerminalApp*)AfxGetApp();
	CComThread *pThread = pApp->m_pComThread;
	CTerminalThread *tThread = pApp->m_pTerminalThread;
	ASSERT(pThread);

//	m_double_echo = false;
	m_connected = true;
	if (m_connected)
	{
// user pressed a key - send this key to the P2
// via the serial port, also via Telbet for debugging

		pThread->PostThreadMessage(WM_TRANSMIT_DATA,
			(WPARAM)nchar,(LPARAM)flags);
		tThread->PostThreadMessage(WM_TRANSMIT_CHAR,
			(WPARAM)nchar,(LPARAM)flags);
		if (nchar==13)
		tThread->PostThreadMessage(WM_TRANSMIT_CHAR,
			(WPARAM)10,(LPARAM)flags);
	}
	if (m_local_echo)
		CEditView::OnChar (nchar,rep_count,flags);
	
	if (m_double_echo)
		CEditView::OnChar (nchar,rep_count,flags);
}

afx_msg void CTerminalEditView::OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case 37: // left arrow
//		pan_camera_left ();
		break;

	case 38: // up arrow
//		tilt_camera_up ();
		break;

	case 39: // right arrow
//		pan_camera_right ();
		break;

	case 40: // down arrow
//		tilt_camera_down ();
		break;
	}
	Invalidate (TRUE);
}

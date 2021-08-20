// ChildFrm.cpp : implementation of the CDebugFrame class
//
#include "stdafx.h"
#include "globalunit.h"
#include "P2 Graphical Debug Terminal.h"
#include "signal view.h"
#include "MainFrm.h"
#include "DebugFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDebugFrame

IMPLEMENT_DYNCREATE(CDebugFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CDebugFrame, CMDIChildWnd)
END_MESSAGE_MAP()


// CDebugFrame construction/destruction

CDebugFrame::CDebugFrame()
{
// surprize!  Microsoft does not do this for us!
// apparently there is no requirement that a Window
// actually be associated with any sort of document.
	m_pDoc = NULL;
}

CDebugFrame::~CDebugFrame()
{

}

BOOL CDebugFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

CSignalView *CDebugFrame::SetGraphMode (GRAPHMODE::mode graphMode)
{
	CSignalView *pView = NULL;
	CDebugTerminalApp *pApp = (CDebugTerminalApp*)AfxGetApp ();
	CMainFrame *pMainFrame = (CMainFrame*)pApp->GetMainWnd ();
	CMDIChildWnd* pFrame = pMainFrame->MDIGetActive ();
	if (pFrame!=NULL) {
		pView = (CSignalView*) pFrame->GetActiveView ();
	}
	else
		AfxMessageBox (_T("Please open a new view"));

	if (pView!=NULL)
	{	
		m_pDoc = (CTerminalDoc*)pView->GetDocument ();
		m_graphMode = graphMode;
#if 0	
		scene *pScene = &pView->m_scene;
		pScene->pRenderProc = NULL;
		pScene->m_objects.resize(1);
#endif
	}
//	SetTitle (_T("This is a test."));
	return pView;
}

// CDebugFrame diagnostics

#ifdef _DEBUG
void CDebugFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CDebugFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG


// CDebugFrame message handlers

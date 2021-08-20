 // P2 Graphical Debug TerminalView.cpp : implementation of the CSignalView class
//
#include "stdafx.h"
#include "globalunit.h"
#include "P2 Graphical Debug Terminal.h"
#include "P2 Graphical Debug TerminalDoc.h"
#include "signal view.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IDT_RENDER 1234

// access standard colora by
// storing them in an array

const COLORREF color [] = 
{
	COLOR::black,
	COLOR::blue,
	COLOR::red,
	COLOR::magenta,
	COLOR::green,
	COLOR::cyan,
	COLOR::yellow,
	COLOR::gray,
	COLOR::white,
};

IMPLEMENT_DYNCREATE(CSignalView,INHERIT_FROM)

BEGIN_MESSAGE_MAP(CSignalView,INHERIT_FROM)
	ON_COMMAND(ID_FILE_PRINT, &INHERIT_FROM::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &INHERIT_FROM::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &INHERIT_FROM::OnFilePrintPreview)
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
	ON_COMMAND(ID_TERMINAL_DEBUGMODE, &CSignalView::OnTerminalDebugmode)
	ON_COMMAND(ID_TERMINAL_DISASSEMBLER, &CSignalView::OnTerminalDisassembler)
	ON_COMMAND(ID_TERMINAL_HTMLVIEW, &CSignalView::OnTerminalHtmlview)
	ON_COMMAND(ID_TERMINAL_TEXTMODE, &CSignalView::OnTerminalTextmode)
	ON_COMMAND(ID_TERMINAL_OSCILLOSCOPESETTINGS, &CSignalView::OnTerminalOscilloscope)
	ON_COMMAND(ID_TERMINAL_POLARPLOT,&CSignalView::OnTerminalPolarPlot)
	ON_COMMAND(ID_TERMINAL_SMITHPLOT,&CSignalView::OnTerminalSmithPlot)
	ON_COMMAND(ID_TERMINAL_XYPLOTTER, &CSignalView::OnTerminalXyplotter)
	ON_MESSAGE (WM_UPDATE_TEXT,OnRxString)
END_MESSAGE_MAP()


// CSignalView construction/destruction
CSignalView::CSignalView()
{
	m_debug_trace=false;
}

CSignalView::~CSignalView()
{

}

BOOL CSignalView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.style |= WS_CLIPSIBLINGS|WS_CLIPCHILDREN; 
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);
	return CView::PreCreateWindow(cs);
}

int CSignalView::OnCreate (LPCREATESTRUCT lpCreateStruct) 
{
	TRACE0("OnCreate\r\n") ;
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
    CClientDC dc(this);
	PIXELFORMATDESCRIPTOR pfd ;
	memset(&pfd,0, sizeof(PIXELFORMATDESCRIPTOR)) ;
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);   
    pfd.nVersion = 1 ;                           // Version number
	pfd.dwFlags =  PFD_DOUBLEBUFFER |        // Use double buffer
	               PFD_SUPPORT_OPENGL |          // Use OpenGL
	               PFD_DRAW_TO_WINDOW ;          // Pixel format is for a window.
		pfd.iPixelType = PFD_TYPE_RGBA ;
    pfd.cColorBits = 24;                         // 8-bit color
	pfd.cDepthBits = 32 ;					 // 32-bit depth buffer
    pfd.iLayerType = PFD_MAIN_PLANE ;            // Layer type

    int nPixelFormat = ChoosePixelFormat(dc.m_hDC, &pfd);
		if (nPixelFormat == 0)
		{
		TRACE("ChoosePixelFormat Failed %d\r\n",GetLastError()) ;
		return -1 ;
	}
	TRACE("Pixel Format %d\r\n", nPixelFormat) ;
    BOOL bResult = SetPixelFormat(dc.m_hDC, nPixelFormat, &pfd);
	if (!bResult)
	{
		TRACE("SetPixelFormat Failed %d\r\n",GetLastError()) ;
		return -1 ;
	}
#ifdef HAVE_OPEN_GL
	m_hrc = wglCreateContext(dc.m_hDC);
	if (!m_hrc)
	{
		TRACE("wglCreateContext Failed %x\r\n", GetLastError()) ;
		return -1;
	}
	const GLubyte *glu_version = gluGetString (GLU_VERSION);
	TRACE ("GLU version %s\n",glu_version);
	CreateRGBPalette(dc.m_hDC) 
#endif
	return 0;
}

void CSignalView::OnInitialUpdate()
{
	m_idtRender = SetTimer(IDT_RENDER,16,NULL);
	m_dbgDoc.create ((CWnd*)this);
	Invalidate(TRUE);
	m_update = true;
}

BOOL CSignalView::OnEraseBkgnd(CDC* pDC) 
{
	COLORREF background_color = COLOR::dark_red;
	CRect	update_region;
	GetClientRect (&update_region);
	BOOL result = true;
	m_defaultErase=true;
	if (m_defaultErase==false)
	{
		SETCOLOR _(pDC,background_color);
		pDC->Rectangle(update_region);
		//	m_update = true;
	}
	return result;
}

void CSignalView::OnSize(UINT nType, int cx, int cy) 
{
	bool m_bOpenGL=false;
	CView::OnSize(nType, cx, cy);
	Invalidate(TRUE);
	if (m_bOpenGL==true)
	{
		if ((cx<=0)||(cy<=0)) 
			return;
	}
}

void CSignalView::OnTimer (UINT timer)
{	 
//	m_camera.timestep (0);
	switch (timer)
	{	
	case IDT_RENDER:
//		if (m_bOpenGL==false)
		if (m_update==true)
			Invalidate (TRUE);
		break;

	default:
		CWnd::OnTimer (timer);
		break;
	}
}

// CSignalView printing

BOOL CSignalView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSignalView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSignalView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

// CSignalView diagnostics

#ifdef _DEBUG
void CSignalView::AssertValid() const
{
	CView::AssertValid();
}

void CSignalView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTerminalDoc* CSignalView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTerminalDoc)));
	return (CTerminalDoc*)m_pDocument;
}
#endif //_DEBUG

// CSignalView message handlers
afx_msg void CSignalView::OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags)
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

	case 66: // b
//		move_camera_backward ();
		break;

	case 68: // d
//		move_camera_down ();
		break;

	case 70: // f
//		move_camera_forward ();
		break;

	case 76: // l
//		move_camera_left ();
		break;

	case 82: // r
//		move_camera_right ();
		break;

	case 85: // u
//		move_camera_up ();
		break;

	default:
		break;
	}
//	TRACE_CURRENT_TIME ();
//	TRACE ("CCameraView::OnKeyDown\n");
	Invalidate (TRUE);
}

afx_msg void CSignalView::OnKeyUp (UINT nChar, UINT nRepCnt, UINT nFlags)
{

}

void CSignalView::OnDrawGDI (CDC *dc)
{
	CTerminalDoc *pDoc = GetDocument();
	dc->SelectObject(&pDoc->m_font);

	m_dbgDoc.set_dc(dc);
	m_dbgDoc.clear_bitmap ();
//	m_dbgDoc.update_display ();
}

void CSignalView::OnDraw(CDC *dc) 
{
	HENHMETAFILE m_hm=NULL;
	ENHMETAHEADER	mheader;
	CDC offscreen;
	CDC	*render = NULL;
	char *fname = NULL;
	int w,h;
	CBitmap displayBmp;
//	DWORD dBlitMode;

	bool m_bInverse=false;
	bool m_buffer=true;
	bool m_bIsPrinting=false;
	bool m_use_metafile=false;
	CRect	m_update_region;
	
	if (dc->m_bPrinting==TRUE)
	{
		int hres, vres;
		hres = dc->GetDeviceCaps(HORZRES);
		vres = dc->GetDeviceCaps(VERTRES);	
		m_use_metafile=false;
		w = hres;
		h = vres;
		m_update_region = CRect (0,0,w,h);
		m_update=true;
		m_bIsPrinting = true;
		render = dc;
	}
	else
	{
		GetClientRect (&m_update_region);
		w = m_update_region.Width();
		h = m_update_region.Height();
		m_bIsPrinting = false;
	}
//	m_camera.set_window_area (m_update_region);
	if ((m_use_metafile==true)&&(m_update==true))
	{
		if (m_hm!=NULL)
		{
			DeleteEnhMetaFile (m_hm);
			m_hm = NULL;
		}	 
		GetEnhMetaFileHeader(m_hm,sizeof(mheader),&mheader);
		m_update = false;
	}
	if ((m_buffer==true)&&(dc->m_bPrinting==FALSE))
	{	
		displayBmp.CreateCompatibleBitmap (dc,w,h);
		offscreen.CreateCompatibleDC (dc);
		offscreen.SelectObject (&displayBmp);
		render = &offscreen;
	}
	if (dc->m_bPrinting==FALSE)
	{
		CBrush *pBrush = dc->GetCurrentBrush ();
		CBrush bkgrnd (COLOR::dark_blue);
		render->FillRect (&m_update_region,&bkgrnd);
	}
	if (m_use_metafile==true)
	{
		PlayEnhMetaFile (render->m_hDC,m_hm,&m_update_region);
	}
	else
	{
		OnDrawGDI (dc);
		dc->SetBkColor (COLOR::black);
		dc->SetTextColor (COLOR::yellow);
		dc->DrawText (m_description,m_update_region,DT_CENTER);
		dc->DrawText (m_text,m_update_region,DT_LEFT);

#if 0
		SmithChart S(COLOR::yellow);
		S.RenderProc(render_context,this);
		dc->SetBkColor (COLOR::black);
#endif	
	}
	m_update = false;
/*	if ((m_buffer==true)&&(dc->m_bPrinting==FALSE))
	{
		int x,y;
		CRect &r = m_update_region;
		x = r.left;
		y = r.top;
		w = r.Width ();
		h = r.Height ();
		dBlitMode = (m_bInverse==false?SRCCOPY:NOTSRCCOPY);		
		dc->BitBlt (x,y,w,h,render_context,x,y,dBlitMode);
	}
*/
}

LRESULT CSignalView::OnRxString (WPARAM w, LPARAM l)
{
	int tick = GetTickCount();
	double dtick = ((double) tick)/1000.0;
	LRESULT res = 0;
	char *rx_buffer = (char*)w;
	CString msg(rx_buffer);
	CString tickStr;
	tickStr.Format(_T("%6.3lf: "),dtick);
	delete rx_buffer;
	if (m_debug_trace==false)
		return res;

	m_text.Append(tickStr);
	m_text.Append (msg);
	m_text.Append (_T("\r\n"));
	int sz = m_text.GetLength(); 
	if (sz>2048)
		m_text = m_text.Right(8200);
	m_update = true;
	return res;
}

void CSignalView::OnTerminalDebugmode()
{
	// TODO: Add your command handler code here
	m_description = _T("This will be the (RAW) Debug Terminal Mode.");
	m_dbgDoc.set_display_type (dis_term);
	m_update=true;
	m_debug_trace = true;
}

void CSignalView::OnTerminalDisassembler()
{
	m_description = _T("This will be the Disassembler Window.");
	m_dbgDoc.set_display_type (dis_term);
	m_update=true;
	m_debug_trace = false;
}

void CSignalView::OnTerminalHtmlview()
{
	m_description = _T("This will be an HTML compatible view.");
	m_dbgDoc.set_display_type (dis_term);
	m_update=true;
	m_debug_trace = false;
}

void CSignalView::OnTerminalTextmode()
{
	m_description = _T("This will be a Text Mode Window.");
	m_dbgDoc.set_display_type (dis_term);
	m_update=true;
	m_debug_trace = true;
}

void CSignalView::OnTerminalOscilloscope()
{
	m_description = _T("");
	m_dbgDoc.set_display_type (dis_scope);
	m_update=true;
	m_debug_trace = false;
}

void CSignalView::OnTerminalPolarPlot()
{
	m_description = _T("");
	m_dbgDoc.set_display_type (dis_scope_rt);
	m_update=true;
	m_debug_trace = false;
}

void CSignalView::OnTerminalSmithPlot()
{
	m_description = _T("This is a Smith Chart");
	m_dbgDoc.set_display_type (dis_smith);
	m_update=true;
	m_debug_trace = false;
}

void CSignalView::OnTerminalXyplotter()
{
	m_description = _T("");
	m_dbgDoc.set_display_type (dis_scope_xy);
	m_update=true;
	m_debug_trace = false;
}

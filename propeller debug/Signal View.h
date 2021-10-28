  // P2 Graphical Debug TerminalView.h : interface of the CSignalView class
//
#pragma once

#include <complex>

#define PI (3.14159265358979)
#define MATH_TYPE	float
#define INHERIT_FROM CView
class CSignalView;
class CTerminalDoc;

typedef enum note_type
{
	SIXTEENTH,
	EIGTH,
	QUARTER,
	HALF,
	WHOLE
};

class SETCOLOR 
{
#define THICKNESS 2

private:
	CPen	*m_pOldPen, m_pen;
	CBrush	*m_pOldBrush, m_brush;
	CDC		*m_pCDC;

public:	
	SETCOLOR (CDC *pCDC, COLORREF c)
	{
		int thickness = THICKNESS;
		m_pCDC = pCDC;
		m_pen.CreatePen (PS_SOLID,thickness,c);
		m_brush.CreateSolidBrush (c);
		m_pOldPen = m_pCDC->SelectObject (&m_pen);
		m_pOldBrush = m_pCDC->SelectObject (&m_brush);
	}
	SETCOLOR (CDC *pCDC, COLORREF c1, COLORREF c2)
	{
		int thickness = THICKNESS;
		m_pCDC = pCDC;
		m_pen.CreatePen (PS_SOLID,thickness,c1);
		m_brush.CreateSolidBrush (c2);
		m_pOldPen = m_pCDC->SelectObject (&m_pen);
		m_pOldBrush = m_pCDC->SelectObject (&m_brush);
	}
	~SETCOLOR ()
	{
		m_pCDC->SelectObject (m_pOldPen);
		m_pCDC->SelectObject (m_pOldBrush);
	}
};

#if 0
class SETCOLOR 
{
#define THICKNESS 2

private:
	CPen	*m_pOldPen, m_pen;
	CBrush	*m_pOldBrush, m_brush;
//	camera	*m_pCamera;
	CDC		*m_pCDC;

public:
#if 0
	SETCOLOR (camera &a, COLORREF c1, COLORREF c2)
	{
		m_pCamera = &a;
		m_pCDC = m_pCamera->m_graph;
		m_pen.CreatePen (PS_SOLID,THICKNESS,c1);
		m_brush.CreateSolidBrush (c2);
		m_pOldPen = m_pCDC->SelectObject (&m_pen);
		m_pOldBrush = m_pCDC->SelectObject (&m_brush);
	}
#endif
	SETCOLOR (CDC *pCDC, COLORREF c, int thickness=THICKNESS)
	{
		m_pCDC = pCDC;
		m_pen.CreatePen (PS_SOLID,thickness,c);
		m_brush.CreateSolidBrush (c);
		m_pOldPen = m_pCDC->SelectObject (&m_pen);
		m_pOldBrush = m_pCDC->SelectObject (&m_brush);
	}
#if 0
	SETCOLOR (camera &a, COLORREF c)
	{
		m_pCamera = &a;
		m_pCDC = m_pCamera->m_graph;
		m_pen.CreatePen (PS_SOLID,THICKNESS,c);
		m_brush.CreateSolidBrush (c);
		m_pOldPen = m_pCDC->SelectObject (&m_pen);
		m_pOldBrush = m_pCDC->SelectObject (&m_brush);
	};
#endif
	~SETCOLOR ()
	{
		m_pCDC->SelectObject (m_pOldPen);
		m_pCDC->SelectObject (m_pOldBrush);
	}
};
#endif

class FFT_PLOT
{
	CFont font;
	CDC* m_graph;
	UINT m_height, m_width;
	int NUMBER_OF_STAVVES_PER_PAGE;
	int NUMBER_OF_MEASURES_PER_LINE;
	int NUMBER_OF_NOTES_PER_MEASURE;

protected:
	void plot_text(int x, int y, char *msg);
	void plot_note (int midi, MATH_TYPE time, note_type type);

public:
	FFT_PLOT();
	void set_dc (CDC *pdc,  CSignalView *where);
	void plot_grand_staff ();
	void plot_notes ();
};

class SmithChart
{
private:
	CDC *dc;
	void DrawCircle (int x0, int y0, int r);
	void DrawSection (int x0, int y0, int r);
	COLORREF color;
	int	m_height, m_width;

public:
	SmithChart (COLORREF c);
	void OnDraw ();
	static int RenderProc (CDC*, CSignalView*);
//	complex<double> refl (complex<double> imped);
//	complex<double> imped (complex<double> refl);	
};

class CSignalView:public INHERIT_FROM
{
private:
	CRect	m_update_region;
	CString m_description;
	CString m_text;
	BOOL m_defaultErase;
	bool m_update;
	bool m_debug_trace;
	UINT_PTR m_idtRender;
	TDebugDisplayDoc m_dbgDoc;

protected: // create from serialization only
	CSignalView();
	DECLARE_DYNCREATE(CSignalView)

public:
	CTerminalDoc* GetDocument() const;
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	virtual void OnInitialUpdate();
	virtual void OnDrawGDI (CDC *dc);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CSignalView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//	afx_msg BOOL OnQueryNewPalette();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
//	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg void OnEditCopy();
	afx_msg void OnTimer (UINT idTimer);
//	afx_msg void OnChar (UINT,UINT,UINT);
	afx_msg void OnKeyDown (UINT,UINT,UINT);
	afx_msg void OnKeyUp (UINT,UINT,UINT);
	afx_msg LRESULT OnRxString (WPARAM, LPARAM);

//	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
//	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//	afx_msg BOOL OnPreparePrinting(CPrintInfo *pInfo);
//	afx_msg void OnCameraMotion();
//	afx_msg void OnFilePrintPreview();
//	afx_msg void OnFilePrint();
//	afx_msg void OnMenuViewOpengl();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTerminalDebugmode();
	afx_msg void OnTerminalDisassembler();
	afx_msg void OnTerminalHtmlview();
	afx_msg void OnTerminalOscilloscope();
	afx_msg void OnTerminalPolarPlot();
	afx_msg void OnTerminalSmithPlot();
	afx_msg void OnTerminalTextmode();
	afx_msg void OnTerminalXyplotter();
	afx_msg void OnTerminalMusicalnotation();
};

#ifndef _DEBUG  // debug version in P2 Graphical Debug TerminalView.cpp
inline CTerminalDoc* CSignalView::GetDocument() const
   { return reinterpret_cast<CTerminalDoc*>(m_pDocument); }
#endif

class CTerminalEditView: public CEditView
{
private:
	bool m_connected;
	bool m_local_echo;
	bool m_double_echo;
	CString m_description;

protected: // create from serialization only
	CTerminalEditView();
	DECLARE_DYNCREATE(CTerminalEditView)

// Attributes
public:
	CTerminalDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
//	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
//	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
//	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
//	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CTerminalEditView();
	virtual void OnInitialUpdate();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	afx_msg LRESULT OnRxString (WPARAM,LPARAM);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags);

//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEditCopy();
	afx_msg void OnChar (UINT,UINT,UINT);
//	afx_msg void OnKeyDown (UINT,UINT,UINT);
//	afx_msg void OnKeyUp (UINT,UINT,UINT);
 
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
};



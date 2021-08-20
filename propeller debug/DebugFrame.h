
#pragma once

class CTerminalDoc;
class CSignalView;

namespace GRAPHMODE
{
enum mode
{
	DEFAULT,
	MESH,
	NO_CHANGE,
	NONE,
	OSCILLOSCOPE,
	PLOT2D,
	RASTERIZER,
	SMITH,
	SPECTRUM,
	TERRAIN,
	WIREFRAME,
};
}

class CDebugFrame: public CMDIChildWnd
{
protected:
	CTerminalDoc *m_pDoc;
	GRAPHMODE::mode m_graphMode;

	DECLARE_DYNCREATE(CDebugFrame)
public:
	CDebugFrame();

// Attributes
public:

// Operations
public:

// Overrides
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	CSignalView *SetGraphMode (GRAPHMODE::mode graphMode);

// Implementation
public:
	virtual ~CDebugFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

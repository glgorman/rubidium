
#include "stdafx.h"
#include "globalunit.h"
#include "compatibility.h"
#include "debugdisplayunit.h"
#include "signal view.h"

static char *TypeName[] = {"Scope","Scope_XY","Scope_RT","Plot","Term",NULL};

#define IGNORE_BORDER_ICONS		1

char    PGLOBAL::DebugDisplayType[DebugDisplayLimit];
command_id	PGLOBAL::DebugDisplayValue[DebugDisplayLimit];

channel_config::channel_config ()
{
	vLow  = -((int)(0x80000000));
	vHigh = (0x7FFFFFFF);
	vBase = 0;
}

TDebugDisplayDoc::TDebugDisplayDoc ()
{
	//inherited CreateNew(AOwner);
#ifndef IGNORE_BORDER_ICONS
	BorderIcons = biSystemMenu;
	BorderStyle = bsDialog;
#endif

//	Font.Charset = DEFAULT_CHARSET;
	memset(&m_lf, 0, sizeof(LOGFONT));
	m_lf.lfHeight = 12;
	wcscpy_s (m_lf.lfFaceName,32,_T("MS Sans Serif"));
}

void TDebugDisplayDoc::Free ()
{
	ASSERT(false);
}

void TDebugDisplayDoc::SetDefaults()
{
	int i;
	i = sizeof(TDebugDisplayDoc);
	vWidth = 256;
	vHeight = 256;
	vRange = 0x7FFFFFFF;
	vSamples = 256;
	m_vTriggerChannel = -1;
	m_vBackColor = DefaultBackColor;
	m_vGridColor = DefaultGridColor;
	m_vLineColor = DefaultLineColor;
	m_vFillColor = DefaultFillColor;
	m_vTextColor = DefaultTextColor;
	m_vLineSize = DefaultLineSize;
	m_vTextSize = DefaultTextSize;
	m_vTextStyle = DefaultTextStyle;
	vCols = DefaultCols;
	vRows = DefaultRows;
	vLogScale = false;
	m_vUpdate = false;
	vOffsetX = 0;
	vOffsetY = 0;
	vPixelX = 0;
	vPixelY = 0;
	vCol = 0;
	vRow = 0;
	SamplePtr = 0;
	SamplePop = 0;
	for (i=0;i<=TopChannel;i++)
		m_ch[i].m_vColor = DefaultScopeColors[i];
	for (i=0;i<=TopChannel;i++)
		m_ch[i].m_vLabel = "";
}

void TDebugDisplayDoc::ResetDisplays ()
{
	ASSERT(false);
}
	
void TDebugDisplayDoc::create(CWnd *p)
{
	m_pView = (CView*) p;
	SetDefaults();
	// Set up display bitmaps
	m_pdc = m_pView->GetDC();
	m_pixel_format = pf24bit;

	// Set font properties
	CRect rect (0,0,0,0);
	m_pdc->DrawText(_T("X"),rect,DT_CALCRECT);
	m_cWidth = rect.Width();
	m_cHeight = rect.Height();

	m_display_type = dis_scope;
	CaptionStr = CString(PGLOBAL::IntToStr(PGLOBAL::DebugDisplayValue[1]));
	CaptionStr.Append(CString("-"));
	CaptionStr.Append(CString(TypeName[m_display_type]));
	Caption = CaptionStr;
	 
	ptr = 2;
	switch (m_display_type)
	{
		case dis_scope: 
			configure_scope();
			break;
		case dis_scope_xy:
			Configure_scope_xy();
			break;
		case dis_scope_rt:
			Configure_scope_xy();
			break;
		case dis_plot:
			Configure_plot();
			break;
		case dis_term:
			Configure_term();
			break;
		default:
			break;
	}
}

////////////////////////
//  Display Routines  //
////////////////////////

void TDebugDisplayDoc::SetSize(int MarginX, int MarginY)
{
	m_vMarginX = MarginX;
	m_vMarginY = MarginY;
	m_client_width = m_vMarginX*2+vWidth;
	m_client_height = m_vMarginY*2+vHeight;
	vScale = ((_int64(vWidth) << (-1+20+32))/vRange) >> 20;
	clear_bitmap();
}

void TDebugDisplayDoc::clear_bitmap()
{
	int i, x, y;
	COLORREF color;
	string s;
	CDC *pDC = m_pdc;
	
	CRect rect;
	m_pView->GetClientRect(&rect);
	m_client_width = rect.Width();
	m_client_height = rect.Height();
	vHeight = m_client_height;
	vWidth = m_client_width;
	Caption = CaptionStr;

	m_vMarginX = 12;
	m_vMarginY = 12;

	CPen pgrid;
	CPen pens[16];
	CBrush brushes [16];
	pgrid.CreatePen(PS_SOLID,1,COLORREF(m_vGridColor));
	for (i=0;i<=TopChannel;i++)
	{
		color = (m_ch[i].m_vColor&0x00FCFCFC)>>1;
		pens[i].CreatePen(PS_DOT,2,color);
	}
	brushes[0].CreateSolidBrush(COLORREF(m_vBackColor));
	brushes[1].CreateSolidBrush(COLORREF(m_vGridColor));
	pDC->SetBkColor(m_vBackColor);
	pDC->SetTextColor(m_vTextColor);
	pDC->SelectObject(&pgrid);
	pDC->SelectObject(&(brushes[0]));
	pDC->FillRect(&rect,&(brushes[0]));
		
	if ((m_display_type==dis_scope_xy)||(m_display_type==dis_scope_rt))
	{
	// draw grid pattern for polar plot
		pDC->SelectObject(&pgrid); //m_vGridColor;
		pDC->Ellipse(m_vMarginX,m_vMarginY,vWidth-m_vMarginX+1,vHeight-m_vMarginY+1);
		pDC->MoveTo(vWidth/2,m_vMarginY);
		pDC->LineTo(vWidth/2,vHeight-m_vMarginY);
		pDC->MoveTo(m_vMarginX,vHeight/2);
		pDC->LineTo(vWidth-m_vMarginX,vHeight/2);
		
		if (m_display_type==dis_scope_xy) 
			s = CString("(X,Y)");
		else
			s = CString("(Rho,Theta)");

		rect = CRect(vWidth/2,0,vWidth/2,0);
		pDC->DrawText(s,&rect,DT_CALCRECT);
		pDC->DrawText(s,&rect,DT_LEFT);

		s = CString("r=");
		s.Append(PGLOBAL::IntToStr(vRange));
		if (vLogScale)
			s.Append(CString(" log scale"));

		rect = CRect(vWidth/2,m_cHeight,vWidth/2,m_cHeight);
		pDC->DrawText(s,&rect,DT_CALCRECT);
		pDC->DrawText(s,&rect,DT_LEFT);
	}
	if (m_display_type==dis_smith)
	{
		SmithChart S(COLOR::yellow);
		S.RenderProc(pDC,(CSignalView*)m_pView);
	}
	
	switch (m_display_type)
	{
	case dis_scope:
//		pDC->SelectObject(&(brushes[0]));
		pDC->Rectangle(m_vMarginX,m_vMarginY,vWidth-m_vMarginX,vHeight-m_vMarginY);

		// draw gridlines
		for (i=0;i<=TopChannel;i++)
		if (m_ch[i].vGrid!=0)
		{
			pDC->SetTextColor(m_ch[i].m_vColor);
			pDC->SelectObject(&(pens[i]));
			y = vHeight-(m_vMarginY+m_ch[i].vBase);
			if ((m_ch[i].vGrid&0x01)!=0)
			{
				y--;
				pDC->MoveTo(m_vMarginX, y);
				pDC->LineTo(vWidth-m_vMarginX, y);
			}
			else if ((m_ch[i].vGrid&0x02)!=0)
			{
				y -= m_ch[i].vTall;
				pDC->MoveTo(m_vMarginX, y);
				pDC->LineTo(vWidth-m_vMarginX, y);
			}
			if ((m_ch[i].vGrid&0x04)!=0)
			{
				y-= m_cHeight;
				//Font.Style = NULL;
				if (m_ch[i].vLow<m_ch[i].vHigh)
					x = m_ch[i].vLow;
				else
					x = m_ch[i].vHigh;
				if (x>=0) {
					s = CString("+");
					s.Append(PGLOBAL::IntToStr(x));
				}
				else 
					s = PGLOBAL::IntToStr(x);

				rect = CRect(0,0,0,0);
				pDC->DrawText(s,&rect,DT_CALCRECT);
				rect+=CPoint(m_vMarginX,y-1);
				pDC->DrawText(s,&rect,DT_LEFT);
			}
			else if ((m_ch[i].vGrid&0x08)!=0)
			{
				y = vHeight-(m_vMarginY+m_ch[i].vBase+m_ch[i].vTall);
				//Font.Style = NULL;
				if (m_ch[i].vLow<m_ch[i].vHigh)
					x = m_ch[i].vHigh;
				else
					x = m_ch[i].vLow;
				if (x>=0)
				{
					s = CString("+");
					s.Append(PGLOBAL::IntToStr(x));
				}
				else
					s = PGLOBAL::IntToStr(x);

				rect = CRect(0,0,0,0);
				pDC->DrawText(s,&rect,DT_CALCRECT);
				rect+=CPoint(m_vMarginX,y);
				pDC->DrawText(s,&rect,DT_LEFT);
			}
		}
		// draw trigger indicator

		if (vTriggered)
		{
			i = (m_ch[m_vTriggerChannel].m_vColor&0x00FEFEFE) >> 1;
			pens[i].CreatePen(PS_DOT,1,m_ch[i].m_vColor);
			pDC->SelectObject(&(pens[i]));
			vToggle = !vToggle;
			if (vToggle)
			{
				pens[i].CreatePen(PS_SOLID,1,m_ch[i].m_vColor);
				pDC->SelectObject(&(pens[i]));
			}
			else
			{
				pens[i].CreatePen(PS_SOLID,1,m_vBackColor);
				pDC->SelectObject(&(pens[i]));
			}
			x = m_client_width-m_vMarginX-PGLOBAL::Round(float(vTriggerOffset+1)/vSamples*vWidth);
			pDC->MoveTo(x, m_vMarginY);
			pDC->LineTo(x, m_vMarginY+vHeight);
		}

		// draw channel names
		//Font.Style = fsBold|fsItalic;
		x = m_vMarginX;
		y = m_cHeight/2;
		for (i=0;i<=TopChannel;i++) 
		if (m_ch[i].m_vLabel!="")
		{
			pDC->SetTextColor(m_ch[i].m_vColor);
			rect = CRect(0,0,0,0);
			pDC->DrawText(m_ch[i].m_vLabel,&rect,DT_CALCRECT);
			if (x+rect.Width()>m_vMarginX+vWidth)
			{
				x = m_vMarginX;
				y = y+vHeight-m_vMarginY;
			}
			rect+=CPoint(x,y);
			pDC->DrawText(m_ch[i].m_vLabel,&rect,DT_LEFT);
			x = x+rect.Width()+m_cWidth*2;
		}
		break;

	case dis_scope_xy:
	case dis_scope_rt:
		
		// draw channel names
		// Style = fsBold|fsItalic;
		for (i=0;i<=TopChannel;i++)
		if (m_ch[i].m_vLabel!="")
		{
			rect = CRect(0,0,0,0);
			pDC->SetTextColor(m_ch[i].m_vColor);
			pDC->DrawText(m_ch[i].m_vLabel,&rect,DT_CALCRECT);
			if ((i&2)==0)
				x = m_cWidth;
			else
				x = m_client_width-m_cWidth-rect.Width();
			if (i<4)
				y = m_cHeight;
			else
				y = m_client_height-m_cHeight*2-rect.Height();
			if ((i&1)!=0)
				y = y+m_cHeight;

			rect+=CPoint(x,y);
			pDC->DrawText(m_ch[i].m_vLabel,&rect,DT_CALCRECT);
			pDC->DrawText(m_ch[i].m_vLabel,&rect,DT_LEFT);
		}
		break;
	
	default:
		break; 
	}
#if 0
	render_cursor (_T("DEBUG"));
#endif
}

void TDebugDisplayDoc::TermChr(char c)
{
	int x, y;
	CDC *pdc = m_pdc;
	CRect r1, r2;
	CBrush b1, b2;
	HGDIOBJ old_brush;
	pdc->SetTextColor (m_vTextColor);
	pdc->SetBkColor (m_vTextBackColor);
	b1.CreateSolidBrush(COLORREF(m_vTextBackColor));
	b2.CreateSolidBrush (COLORREF(m_vBackColor));
	old_brush = pdc->SelectObject(b1);

	if (c!=(char)13)
	{
		x = m_vMarginX+vCol*m_cWidth;
		y = m_vMarginY+vRow*m_cHeight;
		r1 = CRect(x,y,x,y);
		pdc->DrawText(CString(c),&r1,DT_CALCRECT);
		pdc->DrawText(CString(c),&r1,DT_LEFT);
	}
	if (vCol!=vCols-1)
	{
		vCol++;
		return;
	}
	r1 = CRect(m_vMarginX, m_vMarginY, m_vMarginX+vCols*m_cWidth, m_vMarginY+(vRows-1)*m_cHeight);
	r2 = CRect(r1.left, r1.top+m_cHeight, r1.right, r1.bottom+m_cHeight);
	m_rc[0].CopyRect(r1, m_rc[0], r2);
	r1 = CRect(r1.left, r1.bottom, r1.right, r2.bottom);
	pdc->FillRect(&r1,&b2);
	vCol = 0;
	pdc->SelectObject(old_brush);
}

bool TDebugDisplayDoc::NextElement(int Element)
{
	bool result;
	if (PGLOBAL::DebugDisplayType[ptr]==Element)
	{
		m_val = (command_id) PGLOBAL::DebugDisplayValue[ptr];
		ptr++;
		result = true;
	} 
	else
		result = false;
	return result;
}

void TDebugDisplayDoc::Show ()
{
	return;
	m_pView->ShowWindow(SW_SHOW);
}

void TDebugDisplayDoc::render_cursor (CString str)
{
	CPen cPen1, cPen2;
	int StrW, StrH, W,X,Y,H;
	int Quadrant;
	int TextX, TextY, CursX, CursY;

	StrW = m_cWidth;
	StrH = m_cHeight-16;
	W = StrW+16;
	H = m_cHeight;
	X = 100;
	Y = 100;

	if (X>=m_client_width/2) 
		Quadrant = 1;
	else
		Quadrant = 0;
	if (Y>=m_client_height/2)
		Quadrant = Quadrant || 0x2;

	switch(Quadrant)
	{
	case 0:
		TextX = 16;
		TextY = 16;
		CursX = 9;
		CursY = 9;
		break;
	case 1:
		TextX = 0;
		TextY = 16;
		CursX = W-9;
		CursY = 9;
		break;
	case 2:
		TextX = 16;
		TextY = 0;
		CursX = 9;
		CursY = H-9;
		break;
	case 3:
		TextX = 0;
		TextY = 0;
		CursX = W-9;
		CursY = H-9;
		break;
	default:
		break;
	}
	CDC *pDC = m_pdc;
	cPen1.CreatePen (PS_SOLID,2,COLORREF(m_vBackColor^(0xFFFFFF)));
	cPen2.CreatePen (PS_SOLID,2,RGB(0,0,0));

	pDC->FillSolidRect(&CRect(0, 0, m_cWidth, m_cHeight),
		RGB (0,0,0));

	// Clear mask bitmap
	pDC->FillSolidRect(&CRect(0, 0, m_cWidth, m_cHeight),
		RGB(255,255,255));

	// if (text present, add to bitmaps
	if (str!="")
	{
		// Draw text on color bitmap
//		CursorColor.Font.Color = m_vBackColor^(0xFFFFFF);
//		CursorColor.m_textRect = CRect(TextX, TextY, TextX+StrW, TextY+StrH);
		// Draw text rectangle on mask bitmap
		pDC->SelectObject(&cPen1);
		pDC->FillSolidRect(CRect(TextX, TextY, TextX+StrW, TextY+StrH),
			RGB (0,0,0));
	}

	// Draw cross on color bitmap
	pDC->SelectObject(&cPen1);
	pDC->MoveTo(CursX+0, CursY-8);
	pDC->LineTo(CursX+0, CursY+8);
	pDC->MoveTo(CursX-8, CursY+0);
	pDC->LineTo(CursX+8, CursY+0);

	// Draw cross on mask bitmap	
	pDC->SelectObject(&cPen2);
	pDC->MoveTo(CursX+0, CursY-8);
	pDC->LineTo(CursX+0, CursY+8);
	pDC->MoveTo(CursX-8, CursY+0);
	pDC->LineTo(CursX+8, CursY+0);

	// Set up cursor
	CursorInfo.xHotSpot = CursX;
	CursorInfo.yHotSpot = CursY;
}

void TDebugDisplayDoc::FormMouseMove(int X, int Y)
{
	const int DebugCursor=5;    // user cursors are positive, system cursors are negative
	string Str;
	double Rf, Tf, Xf, Yf;
	_int64 ScaledX, ScaledY, Bias;
	switch (m_display_type)
	{
	case dis_scope:
		if ((X>=m_vMarginX)&&(X<m_vMarginX+vWidth)&&(Y>=m_vMarginY)&&(Y<m_vMarginY+vHeight))
			Str = PGLOBAL::IntToStr(X-m_vMarginX)+','+PGLOBAL::IntToStr((vHeight+m_vMarginY-1)-Y);
		else
			Str = "";
		break;

	case dis_scope_xy:
		ScaledX = X-m_client_width/2;
		ScaledY = m_client_width/2-Y;
		if (vLogScale)
		{
			Rf = PGLOBAL::Power(2, PGLOBAL::Hypot(ScaledX, ScaledY)/(vWidth/2)*PGLOBAL::Log2(vRange));
			Tf = PGLOBAL::ArcTan2(ScaledX, ScaledY);
			PGLOBAL::SinCos(Tf, Xf, Yf);
			ScaledX = PGLOBAL::Round(Rf*Xf);
			ScaledY = PGLOBAL::Round(Rf*Yf);
		}
		else
		{
			Bias = vWidth/vRange/4;            // Bias centers values spanning multiple pixels
			if (ScaledX<0) 
				ScaledX-=Bias; 
			else
				ScaledX+=Bias;
			if (ScaledY<0)
				ScaledY-=Bias;
			else 
				ScaledY+=Bias;
			ScaledX = ScaledX << (32/vScale);
			ScaledY = ScaledY << (32/vScale);
		}
		Str = PGLOBAL::IntToStr(ScaledX);
		Str.Append(CString(","));
		Str.Append(CString(PGLOBAL::IntToStr(ScaledY)));
		break;

	case dis_scope_rt:
		ScaledX = X-m_client_width/2;
		ScaledY = m_client_width/2-Y;
		Rf = PGLOBAL::Hypot(ScaledX, ScaledY);
		if (vLogScale)
			Rf = PGLOBAL::Power(2, Rf/(vWidth/2)*PGLOBAL::Log2(vRange));
		else
			Rf = Rf*0x100000000/vScale;
		Tf = PGLOBAL::ArcTan2(ScaledY, ScaledX)/(3.1415926535*2);
		ScaledX = PGLOBAL::Round(Rf);
		ScaledY = PGLOBAL::Round(Tf*0x100000000)&0xFFFFFFFF;
		Str = PGLOBAL::IntToStr(ScaledX);
		Str.Append(CString(",0x"));
		Str.Append(PGLOBAL::IntToHex(ScaledY));
		break;

	case dis_plot:
		Str = PGLOBAL::IntToStr(X-vOffsetX);
		Str.Append(CString(","));
		Str.Append(PGLOBAL::IntToStr(m_client_height-1-(Y-vOffsetY)));
		break;

	case dis_term:
		if ((X>=m_vMarginX)&&(X<m_vMarginX+m_cWidth*vCols)
			&&(Y>=m_vMarginY)&&(Y<m_vMarginY+m_cHeight*vRows))
			Str = PGLOBAL::IntToStr((X-m_vMarginX)/m_cWidth)+','+PGLOBAL::IntToStr((Y-m_vMarginY)/m_cHeight);
		else
			Str = "";
		break;

	default:
		break;
	}
	render_cursor (Str);
}

void TDebugDisplayDoc::FormMove()
{
	//  inherited;
	Caption = PGLOBAL::IntToStr(m_left);
	Caption.Append(CString(", "));
	Caption.Append(PGLOBAL::IntToStr(m_top));
}

/////////////////////////////
//  Display Configuration  //
/////////////////////////////

void TDebugDisplayDoc::configure_scope()
{
	int i;
// set defaults
	if ((vDotSize==0)&&(m_vLineSize==0))
		vDotSize = 1;

	for (i=0;i<=TopChannel;i++)
	{
		m_ch[i].vTall = vHeight;
		m_ch[i].vBase = (i*vHeight)/TopChannel;
		m_ch[i].vGrid = 0x05; // baseline with value
//		m_ch[i].vGrid = 0x06; // centered with value
		m_ch[i].m_vLabel.Format (_T("Ch:%02d"),i);
	}
	m_vTriggerChannel = -1;
	vTriggerArm = -1;
	vTriggerFire = 0;
	vTriggerOffset = vWidth/2;
	vTriggered = false;
// set initial form size
	SetSize(m_cWidth, m_cHeight*2);
	// Set unique defaults
	vDotSize = 0;
	
// process any parameters
	while (NextElement(ele_key))
	{
		switch (m_val)
		{
		case key_title:
			if (NextElement(ele_str))
				CaptionStr = char(m_val);
			break;

		case key_pos:
			if (NextElement(ele_num))
				m_left = m_val;
			if (NextElement(ele_num))
				m_top = m_val;
			break;

		case key_size:
			if (NextElement(ele_num))
				vWidth = PGLOBAL::InRange(m_val, scope_wmin, scope_wmax);
			if (NextElement(ele_num))
				vHeight = PGLOBAL::InRange(m_val, scope_hmin, scope_hmax);
			break;

		case key_samples:
			if (NextElement(ele_num))
				vSamples = PGLOBAL::InRange(m_val, 16, 2048);
			break;

		case key_dotsize:
			if (NextElement(ele_num))
				vDotSize = PGLOBAL::InRange(m_val, 0, 7);
			break;

		case key_linesize:
			if (NextElement(ele_num)) 
				m_vLineSize = PGLOBAL::InRange(m_val, 0, 7);
			break;

		case key_backcolor:
			if (NextElement(ele_num))
				m_vBackColor = PGLOBAL::WinRGB(m_val);
			break;

		case key_gridcolor:
			if (NextElement(ele_num))
				m_vGridColor = PGLOBAL::WinRGB(m_val);
			break;

		case key_update:
			m_vUpdate = true;
			break;

		default:
			break;
		}
	}
}

void TDebugDisplayDoc::Configure_scope_xy()
{
	int i;

	// Set unique defaults
	vDotSize = 1;
	// process any parameters
	while (NextElement(ele_key))
	{
		switch (m_val) {

		case key_title:
			if (NextElement(ele_str)) 
				CaptionStr = char(m_val);
			break;

		case key_pos:
			if (NextElement(ele_num))
				m_left = m_val;
			if (NextElement(ele_num))
				m_top = m_val; 
			break;

		case key_size:
			if (NextElement(ele_num))
				vWidth = PGLOBAL::InRange(m_val*2, scope_xy_wmin, scope_xy_wmax);
			else
				break;
			vHeight = vWidth;
			break;

		case key_range:
			if (NextElement(ele_num))
				vRange = PGLOBAL::InRange(m_val, 2,0x7FFFFFFF);
			// minimum 2 to prevent logscale divide-by-zero
			break;

		case key_samples:
			if (NextElement(ele_num)) 
				vSamples = PGLOBAL::InRange(m_val, 0, XY_Sets);
			break;

		case key_dotsize:
			if (NextElement(ele_num))
				vDotSize = PGLOBAL::InRange(m_val, 1, 6);
			break;

		case key_label:
			for (i=0;i<=TopChannel;i++)
				if (NextElement(ele_str))
					m_ch[i].m_vLabel = char(m_val);
			break;

		case key_color:
			for (i=0;i<=TopChannel;i++) 
				if (NextElement(ele_num))
					m_ch[i].m_vColor = PGLOBAL::WinRGB(m_val);
			break;

		case key_backcolor:
			if (NextElement(ele_num))
				m_vBackColor = PGLOBAL::WinRGB(m_val);
			break;

		case key_gridcolor:
			if (NextElement(ele_num))
				m_vGridColor = PGLOBAL::WinRGB(m_val);
			break;

		case key_logscale:
			vLogScale = true;
			break;

		case key_update:
			m_vUpdate = true;
			break;

		default:
			break;
		}
		// Set initial form size
		SetSize(m_cHeight*2, m_cHeight*2);
	}
}

void TDebugDisplayDoc::Configure_plot ()
{
	// Process any parameters
	while (NextElement(ele_key))
	switch (m_val)
	{
		case key_title:
			if (NextElement(ele_str))
				CaptionStr = char(m_val);
			break;

		case key_pos:
			if (NextElement(ele_num))
				m_left = m_val;
			if (NextElement(ele_num))
				m_top = m_val;
			break;

		case key_size:
			if (NextElement(ele_num))
				vWidth = PGLOBAL::InRange(m_val, plot_wmin, plot_wmax);
			if (NextElement(ele_num))
				vHeight = PGLOBAL::InRange(m_val, plot_hmin, plot_hmax);
			break;

		case key_offset:
			if (NextElement(ele_num))
				vOffsetX = m_val;
			else
				break;
			if (NextElement(ele_num))
				vOffsetY = m_val;
			break;

		case key_update:
			m_vUpdate = true;
			break;

		default:
			break;
	}
	// Set initial form size
	SetSize(0, 0);
}

void TDebugDisplayDoc::Configure_term()
{
	int i;
	// Set unique defaults
	m_vTextSize = FontSize;
	for (i=0;i<8;i++)
		m_ch[i].m_vColor = DefaultTermColors[i];

	// Process any parameters
	while (NextElement(ele_key))
	switch (m_val)
	{
		case key_title:
			if (NextElement(ele_str)) 
				CaptionStr = char(m_val);
			break;

		case key_pos:
			if (NextElement(ele_num))
				m_left = m_val;
			else
				break;;
			if (NextElement(ele_num))
				m_top = m_val;
			break;

		case key_size:
			if (NextElement(ele_num))
				vCols = PGLOBAL::InRange(m_val, term_colmin, term_colmax);
			else
				break;

			if (NextElement(ele_num))
				vRows = PGLOBAL::InRange(m_val, term_rowmin, term_rowmax);
			break;

		case key_textsize:
			if (NextElement(ele_num))
				m_vTextSize = PGLOBAL::InRange(m_val, 6, 200);
			break;

		case key_textcolor:
			for (i=0;i<=TopChannel;i++)
				if (NextElement(ele_num))
					m_ch[i].m_vColor = PGLOBAL::WinRGB(m_val);
				else
					break;
			break;

		case key_backcolor:
			if (NextElement(ele_num))
				m_vBackColor = PGLOBAL::WinRGB(m_val);
			break;

		case key_update:
			m_vUpdate = true;
			break;

		default:
			break;
	}

	// Set initial colors
	m_vTextColor = m_ch[0].m_vColor;
	m_vTextBackColor = m_ch[1].m_vColor;
	// Modify bitmap text metrics
	m_lf.lfHeight = m_vTextSize;
	CRect rect (0,0,0,0);
	m_pdc->DrawText(_T("X"),&rect,DT_CALCRECT);
	m_cWidth = rect.Width();
	m_cHeight = rect.Height();
	// Set initial form size
	vWidth = vCols*m_cWidth;
	vHeight = vRows*m_cHeight;
	SetSize(m_cWidth/2, m_cWidth/2);
}

//////////////////////
//  Display Update  //
//////////////////////

CPoint *TDebugDisplayDoc::Slice(CPoint *, int)
{
	CPoint *res = NULL;
	ASSERT(false);
	return res;
}

void TDebugDisplayDoc::update_display()
{
	ptr = 1;
	switch (m_display_type)
	{
	case dis_scope:
		Update_scope ();
		break;

	case dis_smith:
	case dis_scope_xy:
	case dis_scope_rt:
		Update_scope_xy ();
		break;

	case dis_plot:
		Update_plot();
		break;

	case dis_term:
		Update_term();
		break;

	default:
		ASSERT(false);
		break;
	}
}

void TDebugDisplayDoc::render_scope(int i)
{
	int j,k,mode,offset,v,x,y;
	double fScale;
	clear_bitmap();
	CPen pens[16];
	CDC *pDC = m_pdc;
	if ((vDotSize>0)&&(m_vLineSize==0))
		mode = 0;
	else if ((vDotSize==0)&&(m_vLineSize > 0))
		mode = 1;
	else
		mode = 2;
	
	for (j=i-1;j>=0;j--)
	switch (mode)
	{
	case 0:
		pens[j].CreatePen (PS_DOT,vDotSize+1,m_ch[j].m_vColor);
		break;
	case 1:
		pens[j].CreatePen (PS_SOLID,m_vLineSize,m_ch[j].m_vColor);
		break;
	case 2:
		pens[j].CreatePen (PS_DASHDOT,m_vLineSize+1,m_ch[j].m_vColor);
		break;
	default:
		pens[j].CreatePen (PS_DASHDOT,m_vLineSize,m_ch[j].m_vColor);
		break;
	}

	for (j=i-1;j>=0;j--)
	{
		pDC->SelectObject(&(pens[j]));
		if (m_ch[j].vHigh==m_ch[j].vLow)
			fScale = 0;          // prevent divide by zero
		else //FIME -- need 64bit abs here?
			fScale = ((m_ch[j].vTall-1)/abs((_int32)(m_ch[j].vHigh-m_ch[j].vLow)));
		if (m_ch[j].vHigh>m_ch[j].vLow)
			offset = m_ch[j].vLow;
		else
			offset = m_ch[j].vHigh;
		
		for (k=SamplePop-1;k>=0;k--)
		{
			v = Y_SampleBuff[((SamplePtr-(k+1))&&Y_PtrMask)*Y_SetSize+j];
			x = m_client_width-(m_vMarginX+PGLOBAL::Round((k+1)/vSamples*vWidth));
			y = m_client_height-(1+m_vMarginY+m_ch[j].vBase+PGLOBAL::Round((v-offset)*fScale));
			if (k==SamplePop-1)
				pDC->MoveTo(x ,y);
			else
				pDC->LineTo(x ,y);
		}
	}
}

void TDebugDisplayDoc::Update_scope ()
{
	string s;
	int vIndex = 0;
	int i, samp[Y_SetSize];
	_int64 v;
	while (!NextElement(ele_key))
	{
		if (NextElement(ele_key))
		switch (m_val)
		{
			case key_trigger:
				vArmed = false;
				if (NextElement(ele_num)) 
					m_vTriggerChannel = PGLOBAL::InRange(m_val, -1, 7);
				else
					break;
				if (NextElement(ele_num))
					vTriggerArm = m_val;
				else
					break;;
				if (NextElement(ele_num))
					vTriggerFire = m_val;
				else
					break;
				if (NextElement(ele_num))
					vTriggerOffset  = PGLOBAL::InRange(m_val, 0, vSamples-1);
				break;

			case key_clear:
				clear_bitmap();
				SamplePop = 0;
				break;

			case key_update:
//	TODO - Notify Windoze that we want
//	an update
				break;

			case key_save:
				if (NextElement(ele_str))
					s = char(m_val);
				else
					break;
				save_to_file(s,".bmp");
				break;
			default:
				break;
		}
		else if (NextElement(ele_str))
		{
			if (vIndex!=Channels) 
				vIndex++;
			m_ch[vIndex-1].m_vLabel = char(m_val);
			if (NextElement(ele_num))
				m_ch[vIndex-1].vHigh = m_val;
			else
				continue;
			if (NextElement(ele_num))
				m_ch[vIndex-1].vLow = m_val;
			else
				continue;
			if (NextElement(ele_num))
				m_ch[vIndex-1].vBase = m_val;
			else
				continue;
			if (NextElement(ele_num))
				m_ch[vIndex-1].vGrid = m_val;
			else 
				continue;
			if (NextElement(ele_num))
				m_ch[vIndex-1].m_vColor = PGLOBAL::WinRGB(m_val);
			else
				continue;
		}
		else
		{
				// Get channel samples
			for (i=0;i<=TopChannel;i++)
				if (NextElement(ele_num))
					samp[i] = m_val;
				else
					break;

				if (i==0)
					break;

				memcpy (&(Y_SampleBuff[SamplePtr*Y_SetSize]),samp,Y_SetSize<<2);
				SamplePtr = (SamplePtr+1)&&Y_PtrMask;
				if (SamplePop<vSamples)
					SamplePop++;
				// if (trigger enabled, only update on trigger condition
				vTriggered = false;
				if ((m_vTriggerChannel>=0))
				{
					if (SamplePop!=vSamples)
						continue;         // if (sample buffer not full, exit
					v = Y_SampleBuff[((SamplePtr-vTriggerOffset-1)&&Y_PtrMask)*Y_SetSize+m_vTriggerChannel];
					if (vArmed)
					{
					if (vTriggerFire>=vTriggerArm)
					{
						if (v>=vTriggerFire)
							vTriggered = true;
					}
					else
					{
						if (v<=vTriggerFire)
							vTriggered = true;
					}
				}
				else
				{
					if (vTriggerFire>=vTriggerArm)
					{
						if (v<=vTriggerArm)
							vArmed = true;
					}
					else
					{
						if (v>=vTriggerArm)
							vArmed = true;
					}
				}
				if (vTriggered)
					vArmed = false;
				if (!vTriggered)
					break;
			}
			render_scope(i);
				// Update display
		}
	}
}

bool TDebugDisplayDoc::plot_xy (int *samples, int i)
{
	int j, k, opa;
	if (vSamples==0)
	{
		for (j=i-1;j>=0;j--)
			PlotPoint((char)255, m_ch[j].m_vColor, samples[(j>>1)+0],samples[(j>>1)+1]);
		return false;
	}
	memcpy(&(XY_SampleBuff[SamplePtr*XY_SetSize]),samples,XY_SetSize<<2);
	SamplePtr = (SamplePtr+1)&&XY_PtrMask;
	if (SamplePop<vSamples)
		SamplePop++;
	
	clear_bitmap();
	for (j=i-1;i>=0;i--)
	for (k=SamplePop-1;k>=0;k--)
	{
		ptr = ((SamplePtr-k-1)&&XY_PtrMask)*XY_SetSize+j*2;
		opa = 255-(k*255/vSamples);
		PlotPoint(opa, m_ch[j].m_vColor, XY_SampleBuff[ptr+0], XY_SampleBuff[ptr+1]);
	}
	return true;
}

void TDebugDisplayDoc::Update_scope_xy()
{
	int ch;
	int samp[XY_SetSize];
	string s;
	while (!NextElement(ele_key))
	{
		if (!NextElement(ele_key))
		{
			for (ch=0;ch<=TopChannel;ch++)
			{
				if (NextElement(ele_num))
					samp[(ch>>1)+0] = m_val;
				else
					break;
				if (NextElement(ele_num))
					samp[(ch>>1)+1] = m_val;
				else
					break;
			}
			if (ch==0)
				continue;
			if (plot_xy (samp, ch)==false)
				continue;
		}
		else switch (m_val)
		{
		case key_clear:
			clear_bitmap();
			SamplePop = 0;
			break;

		case key_update:
		// TODO notify windozw to update
			break;

		case key_save:
			if (NextElement(ele_str))
				s = char(m_val);
			else
				break;
			save_to_file(s,".bmp");
			break;

		default:
			break;
		}
	}
}

void TDebugDisplayDoc::Update_plot()
{
	int t1, t2, t3, t4, v, i;
	CPoint p[16];
	CRect rect, r, r1;
	CString s;
	CBrush brush;
	CPen pens[3];
	CDC *dc = m_pdc;
	pens[0].CreatePen(PS_SOLID,m_vLineSize,m_vLineColor);
	pens[1].CreatePen(PS_SOLID,1,m_vLineColor);
	pens[2].CreatePen(PS_SOLID,1,m_vTextColor);
	brush.CreateSolidBrush(COLORREF(m_vFillColor));
	CPen *oldPen = dc->SelectObject(&(pens[0]));
	CBrush *oldBrush = dc->SelectObject(&brush);

	while (!NextElement(ele_key))
	{
		if (!NextElement(ele_key))
		{
			if (!NextElement(ele_num))
				continue;
			
			t1 = m_val;
			if (!NextElement(ele_num))
				continue;
			
			t2= m_val;			
			dc->MoveTo(vOffsetX+t1,m_client_height-(vOffsetY+t2));
			dc->LineTo(vOffsetX+t1,m_client_height-(vOffsetY+t2));
			vPixelX = t1;
			vPixelY = t2;
		}
		else switch (m_val)
		{
		case key_to:
		if (!NextElement(ele_num))
			break;
		t1 = m_val;
		if (!NextElement(ele_num))
			break;
		t2 = m_val;
		dc->MoveTo(vOffsetX+vPixelX,m_client_height-(vOffsetY+vPixelY));
		dc->LineTo(vOffsetX+t1,m_client_height-(vOffsetY+t2));
		vPixelX = t1;
		vPixelY = t2;
		break;

	case key_backcolor:
		if (NextElement(ele_num))
			m_vBackColor = m_val;
		break;

	case key_linesize:
		if (NextElement(ele_num))
			m_vLineSize = PGLOBAL::InRange(m_val, 1, 16);
		break;

	case key_linecolor:
		if (NextElement(ele_num))
			m_vLineColor = m_val;
		break;

	case key_fillcolor:
		if (NextElement(ele_num))
			m_vFillColor = m_val;
		break;

// RECT/RECTFILL/OVAL/OVALFILL left bottom right top
	case key_rect:
	case key_rectfill:
	case key_oval:
	case key_ovalfill:
		v = m_val;  
		if (NextElement(ele_num))
			t1 = m_val;
		break;
		if (NextElement(ele_num))
			t2 = m_val;
		break;
		if (NextElement(ele_num))
			t3 = m_val;
		break;
		if (NextElement(ele_num))
			t4 = m_val;
		break;
		r = CRect(vOffsetX+t1,m_client_height-(vOffsetY+t2),
			vOffsetX+t3,m_client_height-(vOffsetY+t4));

		switch (v)
		{
		case key_rect:
			dc->Rectangle(&r);
			break;

		case key_rectfill:
			dc->FillRect(&r,&brush);
			break;

		case key_oval:
			dc->Ellipse(&r);
			break;

		case key_ovalfill:
			dc->Ellipse(&r);
			break;

		default:
			break;
		}
		break;

	case key_poly:
	case key_polyfill:
		for (i=0;i<16;i++)
		{
			if (NextElement(ele_num))
				p[i].x = vOffsetX+m_val;
			else 
				break;
			if (NextElement(ele_num))
				p[i].y = m_client_height-(vOffsetY+m_val);
			else 
				break;
		}
		if (i<3)
			break;

		dc->SelectObject (&(pens[1]));
		m_rc[0].Polygon(Slice(p,i),i);
		break;

	case key_textsize:
		if (NextElement(ele_num))
			m_vTextSize = PGLOBAL::InRange(m_val, 6, 200);
		break;

	case key_textcolor:
		if (NextElement(ele_num))
			m_vTextColor = m_val;
		break;

	case key_textstyle:
		if (NextElement(ele_num))
			m_vTextStyle = m_val;
		break;

	case key_text:  // TEXT x y 'string'
		if (NextElement(ele_num))
			t1 = m_val;
		else
			break;
		if (NextElement(ele_num)) 
			t2 = m_val;
		else 
			break;
		if (NextElement(ele_str))
			s  = char(m_val);
		else
			break;		
#if 0
		//	TFontStyles fs = 0;
		if ((vTextStyle&&0x01)!=0)
			Include(fs, fsBold);
		if ((vTextStyle&&0x02)!=0)
			Include(fs, fsItalic);
		if ((vTextStyle&&0x04)!=0)
			Include(fs, fsUnderline);
		if ((vTextStyle&&0x08)!=0)
			Include(fs, fsStrikeOut);
		Font.Style = fsNormal;
		Font.Size = vTextSize;
#endif
		m_pdc->DrawText(s,&rect,DT_CALCRECT);
		t3 = rect.Width();
		t4 = rect.Height();
		switch ((m_vTextStyle&0x30)>>4)
		{
		case 0:
		case 1:
			t1-=(t3/2);
			break;
		case 3:
			t1-=t3;
			break;
		default:
			break;
		}
		switch ((m_vTextStyle&0xC0)>>6)
		{
		case 0:
		case 1:
			t2+=(t4/2);
			break;
		case 2:
			t2+=t4;
			break;
		default:
			break;
		}
		r1 = CRect
			(vOffsetX+t1,m_client_height-(vOffsetY+t2),
			vOffsetX+t1,m_client_height-(vOffsetY+t2));
		dc->DrawText(s,&r1,DT_CALCRECT);
		dc->SelectObject (&(pens[2])); //m_vTextColor;
		dc->DrawText(s,&r1,DT_LEFT);
		break;

	case key_clear:
		clear_bitmap();
		break;
	
	case key_update:
	// notify windoze to update
		break;
	
	case key_save:
		if (NextElement(ele_str))
			s = char(m_val);
		else
			break;
		save_to_file(s,".bmp");
		break;
	
	default:
		break;
		}
	}
	dc->SelectObject (oldPen);
	dc->SelectObject (oldBrush);
}

void TDebugDisplayDoc::Update_term ()
{
	int i, j;
	string s;
	while (!NextElement(ele_key))
	{
		if (!NextElement(ele_key))
		{
			if (!NextElement(ele_num))
			switch (m_val)
			{
				case 0:              // clear screen&&home
					clear_bitmap();
					vCol = 0;
					vRow = 0;
					break;
				case 1:              // home
					vCol = 0;
					vRow = 0;
					break;
				case 2:              // set column
					if (NextElement(ele_num))
						vCol = PGLOBAL::InRange(m_val,0,vCols-1);
					break;
				case 3:              // set row
					if (NextElement(ele_num))
						vRow = PGLOBAL::InRange(m_val,0,vRows-1);
					break;
				case 4:
				case 5:
				case 6:
				case 7:           // set colors
					m_vTextColor = m_ch[(m_val-4)*2+0].m_vColor;
					m_vTextBackColor = m_ch[(m_val-4)*2+1].m_vColor;
					break;
				case 8:              // backspace
					if (vCol!=0)
						vCol--;
					else
						vCol = vCols-1;
					if (vRow!=0) 
						vRow--;
					break;
				case 9:              // tab
					while ((vCol&0x7)!=0)
						TermChr(' ');
					break;
				case 13:
					TermChr(char(m_val));
					break;
				default:
					if ((m_val>=32)&&(m_val<256))
						TermChr(char(m_val));
					break;
			}
			else if (NextElement(ele_str))
			{             // string
				j = strlen((char*)(m_val));
				if (j!=0)
					for (i=0;i<j;i++)
						TermChr(((char*)m_val)[i]);
			}
		}
		else switch (m_val)
		{
		case key_clear:        // clear screen&&home
			clear_bitmap();
			vCol = 0;
			vRow = 0;
			break;

		case key_update:       // update bitmap
		// notify windoze to update
			break;

		case key_save:         // save bitmap
			if (NextElement(ele_str))
				s = char(m_val);
			else
				break;

			save_to_file(s,".bmp");
			break;

		default:
			break;
		}
	} // while
}

void TDebugDisplayDoc::PlotPoint(char Opacity, int Color, int X, int Y)
{
	double Rf, Tf, Xf, Yf;
	switch (m_display_type)
	{
	case dis_scope_xy:
		if (vLogScale)
		{
			Rf = PGLOBAL::Hypot(X, Y);
			if (Rf!=0)
				Rf = (PGLOBAL::Log2(Rf)/PGLOBAL::Log2(vRange))*(vWidth/2);
			Tf = PGLOBAL::ArcTan2(X, Y);
			PGLOBAL::SinCos(Tf, Xf, Yf);
			X = PGLOBAL::Round(Rf*Xf);
			Y = PGLOBAL::Round(Rf*Yf);
		}
		else
		{
			X = (_int64(X)*vScale-(0x80000000)) >> 32;
			Y = (_int64(Y)*vScale+(0x80000000)) >> 32;
		}
		break;

	case dis_scope_rt:
		if (vLogScale)
		{
			Rf = X;
			if (Rf!=0)
				Rf = (PGLOBAL::Log2(Rf)/PGLOBAL::Log2(vRange))*(vWidth/2);
		}
		else
			Rf = (_int64(X)*vScale+0x80000000) >> 32;
		Tf = Pi/2-Y/0x100000000*Pi*2;
		PGLOBAL::SinCos(Tf, Xf, Yf);
		X = PGLOBAL::Round(Rf*Xf);
		Y = PGLOBAL::Round(Rf*Yf);
		break;

	default:
		break;
	}

	// make sure X,Y within display boundaries
	X = (m_client_width>>1)+X;
	Y = (m_client_height>>1)-Y;
	if ((X<vDotSize-1)||(X>m_client_width-vDotSize)||
		(Y<vDotSize-1)||(Y>m_client_height-vDotSize))
		return;
	// alpha-bl} pixel
	pOpacity = Opacity;
	pColor = Color;

	switch (vDotSize)
	{
	case 1:
		BlendPixels(X, Y, 1);
		break;

	case 2:
		BlendPixels(X-0, Y-1, 1);
		BlendPixels(X-1, Y+0, 3);
		BlendPixels(X-0, Y+1, 1);
		break;

	case 3:
		BlendPixels(X-1, Y-2, 3);
		BlendPixels(X-2, Y-1, 5);
		BlendPixels(X-2, Y+0, 5);
		BlendPixels(X-2, Y+1, 5);
		BlendPixels(X-1, Y+2, 3);
		break;

	case 4:
		BlendPixels(X-1, Y-3, 3);
		BlendPixels(X-2, Y-2, 5);
		BlendPixels(X-3, Y-1, 7);
		BlendPixels(X-3, Y+0, 7);
		BlendPixels(X-3, Y+1, 7);
		BlendPixels(X-2, Y+2, 5);
		BlendPixels(X-1, Y+3, 3);
		break;

	case 5:
		BlendPixels(X-2, Y-4, 5);
		BlendPixels(X-3, Y-3, 7);
		BlendPixels(X-4, Y-2, 9);
		BlendPixels(X-4, Y-1, 9);
		BlendPixels(X-4, Y+0, 9);
		BlendPixels(X-4, Y+1, 9);
		BlendPixels(X-4, Y+2, 9);
		BlendPixels(X-3, Y+3, 7);
		BlendPixels(X-2, Y+4, 5);
		break;

	case 6:
		BlendPixels(X-1, Y-5, 3);
		BlendPixels(X-3, Y-4, 7);
		BlendPixels(X-4, Y-3, 9);
		BlendPixels(X-4, Y-2, 9);
		BlendPixels(X-5, Y-1, 11);
		BlendPixels(X-5, Y+0, 11);
		BlendPixels(X-5, Y+1, 11);
		BlendPixels(X-4, Y+2, 9);
		BlendPixels(X-4, Y+3, 9);
		BlendPixels(X-3, Y+4, 7);
		BlendPixels(X-1, Y+5, 3);
		break;

	default:
		break;
	}
}

void TDebugDisplayDoc::BlendPixels(int X, int Y, int Count)
{
	char *Line;
	Line = m_rc[0].scan_line(Y);
	X = X*3;
	while (Count>0)
	{
		Line[X+0] = ((Line[X+0]*(pOpacity ^ 0xFF)+((pColor >>16)&0xFF)*pOpacity)+0xFF) >> 8;
		Line[X+1] = ((Line[X+1]*(pOpacity ^ 0xFF)+((pColor >>8)&0xFF)*pOpacity)+0xFF) >> 8;
		Line[X+2] = ((Line[X+2]*(pOpacity ^ 0xFF)+((pColor >>4)&0xFF)*pOpacity)+0xFF) >> 8;
		X+=3;
		Count--;
	}
}





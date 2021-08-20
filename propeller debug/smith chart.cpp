
#include "stdafx.h"
#include "globalunit.h"
#include "P2 Graphical Debug Terminal.h"
#include "P2 Graphical Debug TerminalDoc.h"
#include "signal view.h"

SmithChart::SmithChart (COLORREF c)
{
	color = c;
	m_height = 480;
	m_width =  640;
}

void SmithChart::DrawCircle (int x0, int y0, int r)
{
	MATH_TYPE theta,c,s;
	int x,y;
	dc->MoveTo (x0+r,y0);
	const double twoPi = PI*2;
	for (theta=MATH_TYPE(0);theta<MATH_TYPE(twoPi);theta+=(MATH_TYPE)0.1)
	{
		c=cos (theta);
		s=sin (theta);
		x = (int)(r*c+x0);
		y = (int)(r*s+y0);
		dc->LineTo (x,y);
	}
}

void SmithChart::DrawSection (int x0, int y0, int r)
{
	const double twoPi = PI*2;
	double step,theta,c,s,rmax,w2,h2;
	int x,y;
	w2 = m_width/2.0;
	h2 = m_height/2.0;
	rmax = (h2*h2);
	if (r>2*h2)
		step = 0.001;
	else
		step = 0.01;
	int sign;
	if (y0>m_height/2)
		sign = -1;
	else
		sign = 1;

	dc->MoveTo ((int)(w2+h2),(int)h2);
	for (theta=0.1;theta<(twoPi);theta+=step)
	{
		c=cos (theta);
		s=sin (theta);
		x = (int)(-r*s+x0);
		y = (int)(sign*r*c+y0);
		if ((x-w2)*(x-w2)+(y-h2)*(y-h2)> rmax)
			break;
		dc->LineTo (x,y);
	}
}

int SmithChart::RenderProc (CDC *pdc,  CSignalView *where)
{
	CRect bounds;
	SmithChart chart (COLOR::yellow);
	chart.dc = pdc;
	where->GetClientRect (&bounds);
	chart.m_height = bounds.Height ();
	chart.m_width = bounds.Width ();
	chart.OnDraw ();
	return 0;
}

void SmithChart::OnDraw ()
{
	MATH_TYPE x0,y0,r;
	char *text = "This is a Smith Chart. Smith Charts are used in electrical engineering.";
	MATH_TYPE R,I;
	CRect plot (0,490,640,520);
	SETCOLOR _(dc,color);

	//	plot constant R circles for various values of
//	reflection on the real axiz

	y0 = MATH_TYPE(m_height)/2;
	for (R=MATH_TYPE(-1);R<MATH_TYPE(0);R+=MATH_TYPE(0.05))
	{
		r = abs (R*m_height/2);
		x0 = MATH_TYPE(m_width)/2+MATH_TYPE(m_height)/2 - r;
		DrawCircle (int(x0),int(m_height/2),(int)r);
	}
	MATH_TYPE step = float(1.2);
	for (I=1/32.0;I<16.0;I*=step)
	{
		if (I>1)
			step = MATH_TYPE(1.3);
		if (I>4)
			step = MATH_TYPE(1.7);
		r = I*m_height/2;
		x0 = MATH_TYPE(m_width)/2+MATH_TYPE(m_height)/2;
		y0 = I*m_height/2;
		DrawSection ((int)x0,int(m_height/2+y0),(int)r);
		DrawSection ((int)x0,int(m_height/2-y0),(int)r);
	}
	
}

#if 0
complex<double> SmithChart::refl (complex<double> imped)
{
	complex<double> result;
	result = (imped-1.0)/(imped+1.0);
	return result;
}

complex<double> SmithChart::imped (complex<double> refl)
{
	complex<double> result;
	result = (1.0+refl)/(1.0-refl);
	return result;
}
#endif

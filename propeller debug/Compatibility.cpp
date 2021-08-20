#include "stdafx.h"
#include "globalunit.h"
#include "debugunit.h"
#include <math.h>

#define pi		(3.1415926535897932384626)
#define	log2	(0.6931471805599453094172)

void PGLOBAL::Min(int &Value, int Target)
{
  if (Value<Target)
	Value = Target;
}

void PGLOBAL::Max(int &Value, int Target)
{
  if (Value>Target)
		Value = Target;
}

bool PGLOBAL::Within(int Value, int Min, int Max)
{
	bool result;
  if ((Value>=Min)&&(Value<=Max))
		result = true;
	else
		result = false;
	return result;
}

bool PGLOBAL::IsWordChr(char c)
{
	bool result;
	if (((c>='0')&&(c<='9'))||((c>='A')&&(c<='Z'))
		||((c>='a')&&(c<='z'))||(c=='_'))
		result = true;
	else
		result = false;
	return result;
}

DWORD PGLOBAL::WinRGB(int Color)
{
	DWORD result;
	result = (Color&0xFF0000>>16)|(Color&0x00FF00)|(Color&0x0000FF<<16);
	return result;
}

int	PGLOBAL::Smaller(int x, int y)
{
	int result;
  if (x<y)
		result = x;
	else
		result = y;
  return result;
}

double PGLOBAL::Log2(double x)
{
	double result;
	result = log(x)*(1/log2);
	return result;
}

double PGLOBAL::Power(double x1, double x2)
{
	double result;
	result = pow(x1,x2);
	return result;
}

double PGLOBAL::ArcTan2(long int x, long int y)
{
	double result;
	if (abs(x)>abs(y))
		result = atan(double(y)/double(x));
	else
	{
		result = atan(double(x)/double(y));
		result = (pi*0.5)-result;
	}
	return result;
}

double PGLOBAL::Hypot(double x1, double x2)
{
	double result;
	result = sqrt(x1*x1+x2*x2);
	return result;
}

CString PGLOBAL::IntToHex(int x)
{
	CString result;
	result.Format (_T("%08x"),x);
	return result;
}

CString PGLOBAL::IntToStr(_int64 x)
{
	CString result;
	result.Format (_T("%ld"),x);
	return result;
}

bool PGLOBAL::InRange (int a, int b, int c)
{
	if ((a>b)&&(a<c))
		return true;
	else
		return false;
}

void PGLOBAL::SinCos(const double &Tf, double &Xf, double &Yf)
{
	Yf=sin(Tf);
	Xf=cos(Tf);
}

char *render_context::scan_line(int)
{
	char *result;
	result = NULL;
	ASSERT(false);
	return result;
}

void render_context::Polygon(CPoint *pt1, int n)
{
	CDC *pdc = NULL;
	CPoint *pt2;
	pt2 = new CPoint[n];
	int i;
	for (i=0;i<n;i++)
		pt2[i]=pt1[i];
	pdc->Polygon(pt2,n);
	delete pt2;
}

void render_context::CopyRect (CRect &, render_context &, CRect &)
{
#if 0
	ASSERT(false);
#endif
}

void render_context::resize_display(TDebugDisplayDoc &DisplayForm)
{
	int ColIndent;
	int RowIndent;	
	ColIndent = m_cWidth>>1;
	RowIndent = m_cHeight>>2;
	// Compute columns and rows
	m_cols = (DisplayForm.m_client_width-ColIndent)/m_cWidth;
	m_rows = (DisplayForm.m_client_height-RowIndent*2)/m_cHeight;
	PGLOBAL::Min(m_cols, 0);
	PGLOBAL::Min(m_rows, 0);

	m_col = 0;
}
 
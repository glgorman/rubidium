
#define string CString
class TDebugDisplayDoc;

namespace COLOR
{
const COLORREF black = RGB (0,0,0);
const COLORREF red = RGB (255,0,0);
const COLORREF dark_red = RGB (64,0,0);
const COLORREF green = RGB (0,255,0);
const COLORREF dark_green = RGB (0,64,0);
const COLORREF dark_blue = RGB (0,0,64);
const COLORREF blue = RGB (0,0,255);
const COLORREF yellow = RGB (255,255,0);
const COLORREF cyan = RGB (0,255,255);
const COLORREF magenta = RGB (255,0,255);
const COLORREF white = RGB (255,255,255);
const COLORREF gray = RGB (64,64,64);
const COLORREF mirror_gray = RGB (128,128,128);
const COLORREF light_green = RGB (0,64,0);
extern COLORREF randColor ();
}

namespace PGLOBAL
{
	extern char    DebugDisplayType[DebugDisplayLimit];
	extern command_id	DebugDisplayValue[DebugDisplayLimit];

	void	Min(int &Value, int Target);
	void	Max(int &Value, int Target);
	bool	Within(int Value, int Min, int Max);
	bool	InRange(int Value, int Min, int Max);
	bool	IsWordChr(char);
	int		Smaller(int x, int y);
	double	Log2(double x);
	double	Power(double x1, double x2);
	double	ArcTan2(long int, long int);
	double	Hypot(double x1, double x2);
	void	SinCos(const double &p1, double &p2, double &p3);
	inline int Round(double f);
	CString IntToStr(_int64 x);
	CString IntToHex(int x);
	DWORD WinRGB(int Color);
}

#define Pi (3.14159265358979)

#define Channels        8
#define TopChannel      (Channels-1)
#define Y_SetSize        Channels
#define Y_Sets           (0x1<<11)
#define Y_PtrMask        (Y_Sets-1)
#define XY_Elements     2
#define XY_SetSize       (Channels*XY_Elements)
#define XY_Sets          (1<<9)
#define XY_PtrMask       (XY_Sets-1)

#define clLime           RGB(0x00,0xff,0x00)
#define clRed            RGB(0xff,0x00,0x00)
#define clBlue           RGB(0x00,0x3f,0xff)
#define clYellow         RGB(0xff,0xff,0x00)
#define clMagenta        RGB(0xff,0x00,0xff)
#define clAqua           RGB(0x00,0xff,0xff)
#define clOrange         RGB(0xff,0x7f,0x00)
#define clOlive          RGB(0x8F,0x8f,0x00)
#define clWhite          RGB(0xff,0xff,0xff)
#define clBlack          RGB(0x00,0x00,0x00)
#define clGrey           RGB(0x80,0x80,0x80)

#define DefaultBackColor      clBlack
#define DefaultGridColor      clGrey
#define DefaultLineColor      clLime
#define DefaultFillColor      clBlue
#define DefaultTextColor      clYellow
#define DefaultColor          clRed

#define DefaultLineSize       1
#define DefaultDotSize        1
#define DefaultTextSize       10
#define DefaultTextStyle      0
#define DefaultCols           80
#define DefaultRows           25

#define scope_wmin         32
#define scope_wmax       2048
#define scope_hmin         32
#define scope_hmax       2048
#define scope_xy_wmin      32
#define scope_xy_wmax    2048
#define plot_wmin          32
#define plot_wmax        2048
#define plot_hmin          32
#define plot_hmax        2048

#define term_colmin         1
#define term_colmax       256
#define term_rowmin         1
#define term_rowmax       128

#define pf24bit			(NULL)
#define fsNormal		(NULL)
#define fsBold			(NULL)
#define fsItalic		(NULL)
#define fsStrikeOut		(NULL)
#define fsUnderline		(NULL)
#define psSolid			(PS_SOLID)
#define psDot 			(PS_DOT)
#define bsSolid			(NULL)

typedef enum display_type
{ 
	dis_scope,
	dis_scope_xy,
	dis_scope_rt,
	dis_plot,
	dis_term,
	dis_midi,
	dis_smith,
	dis_grand_staff,
};

typedef enum command_type
{
	ele_end=0,
	ele_dis=1,	
	ele_nam=2,
	ele_key=3,
	ele_num=4,
	ele_str=5,
};

const int DefaultScopeColors[] = 
{
	clLime,
	clRed,
	clAqua,
	clYellow,
	clMagenta,
	clBlue,
	clOrange,
	clOlive
};

const int DefaultTermColors[] = 
{
	clOrange,
	clBlack,
	clYellow,
	clBlack,
	clAqua,
	clBlack,
	clLime,
	clBlack
};

inline int PGLOBAL::Round(double f)
{
	int result;
	result = (int)(f+0.5);
	return result;
}

class render_context
{
public:
	CBitmap m_bmp;
	int m_col;

protected:
	CWnd	*pWnd;
	int m_cHeight;
	int m_cWidth;
	int	m_cols;
	int m_rows;

public:
	void resize_display(TDebugDisplayDoc &);
	void SetDC(CDC *ptr);
	void Polygon(CPoint *, int);
	void CopyRect (CRect &, render_context &, CRect &);
	void save_to_file (CString, char *);
	char *scan_line(int);
};

class CCursorInfo
{
public:
	int xHotSpot, yHotSpot;
	CRect	m_textRect;
};

class channel_config
{
public:
	COLORREF m_vColor;
	string m_vLabel;
//	_int64 vHigh;
//	_int64 vLow;
	int vHigh;
	int vLow;
	int vTall;
	int vBase;
	int vGrid;
	channel_config ();
};

class TDebugDisplayDoc
{
protected:
#if 0
	int Y_SampleBuff[Y_Sets*Y_SetSize];
	int XY_SampleBuff[XY_Sets*XY_SetSize];
#endif
	int *Y_SampleBuff;
	int *XY_SampleBuff;
	int SamplePtr;
	int SamplePop;
	CView *m_pView;
	LOGFONT m_lf;

public:
	display_type m_display_type;
	CDC	*m_pdc;
	render_context m_rc[2];
	int m_client_width;
	int m_client_height;
	int m_cWidth;
	int m_cHeight;

public:
	TDebugDisplayDoc ();
	void create(CWnd *p);
	void set_dc (CDC *pdc)
	{
		m_pdc = pdc;
	}
	void clear_bitmap ();
	void update_display ();
	void set_display_type (display_type arg)
	{
		m_display_type = arg;
	}
	void Show();
	void Free();

private:
	void save_to_file(CString, char *);
	void render_cursor (CString str);
	bool plot_xy (int *, int);
	void render_scope(int i);
	void configure_scope();
	void Configure_plot();
	void Configure_term();
	void Configure_scope_xy();
	void Update_scope ();
	void Update_scope_xy ();
	void Update_plot ();
	void Update_term ();
	void form_create ();
	void FormPaint ();
	void FormMouseMove (int X, int Y);
	void FormMove (); 
	void FormClose ();
	void SetDefaults();
	void SetSize (int MarginX, int MarginY);
	void ResetDisplays();
	void PlotPoint (char Opacity, int Color, int X, int Y);
	void BlendPixels (int X, int Y, int Count);
	void TermChr(char c);
	bool  NextElement (int Element);
	CPoint *Slice(CPoint *, int);

private:
	command_id m_val;
	COLORREF m_vBackColor;
	COLORREF m_vGridColor;
	COLORREF m_vLineColor;
	COLORREF m_vFillColor;
	COLORREF m_vTextColor;
	COLORREF m_vTextBackColor;
	channel_config m_ch[Channels];

	string Caption;
	string CaptionStr;
	string FontName;
	CCursorInfo	CursorInfo;
	int m_left, m_top;
	int FontSize;
	int vWidth, vHeight;
	bool vArmed;
	bool vTriggered;
	bool vToggle;
	bool m_vUpdate;
	bool vLogScale;
	int m_vTriggerChannel;
	int vTriggerArm;
	int vTriggerFire;
	int vTriggerOffset;
	int m_vMarginX, m_vMarginY;
	int vRange;
	int vSamples;
	int vOffsetX;
	int vOffsetY;
	int vPixelX;
	int vPixelY;
	int m_vLineSize;
	int vDotSize;
	int m_pixel_format;
	int m_vTextSize;
	int m_vTextStyle;
	int vCols;
	int vRows;
	int vCol;
	int vRow;
	_int64 vScale;
	int ptr;
	// used by blend pizels/
	unsigned char pOpacity;
	int pColor;
};

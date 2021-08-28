
typedef enum command_id
{
	key_title=0,
	key_pos=1,
	key_size=2,
	key_range=3,
	key_offset=4,
	key_samples=5,
	key_trigger=6,
	key_label=7,
	key_color=8,
	key_backcolor=9,
	key_gridcolor=10,
	key_dotsize=11,
	key_logscale=12,
	key_clear=13,
	key_save=14,
	key_update=15,
	key_linesize=16,
	key_linecolor=17,
	key_fillcolor=18,
	key_to=19,
	key_rect=20,
	key_rectfill=21,
	key_oval=22,
	key_ovalfill=23,
	key_poly=24,
	key_polyfill=25,
	key_textsize=26,
	key_textcolor=27,
	key_textstyle=28,
	key_text=29,
};

#define	kBackSpace    8
#define	kDelete       46
#define	kLeft         37
#define	kRight        39
#define	kUp           38
#define	kDown         40
#define	kHome         36
#define	kEnd          35
#define	kPageUp       33
#define	kPageDown     34
#define	kSpace        32
#define	kTab          9
#define	kEnter        13
#define	kEsc          27
#define	HubLimit		0x80000
#define	DebugDisplayLimit   100

// Assembly interface
// replaced with similar functionality
// implemented in frame_lisp

class lframe;

namespace ASM386
{
	extern char m_debugStr [256];
	

	extern LPVOID P2InitStruct();
	extern void P2Compile1();
	extern void P2Compile2();
	extern void P2InsertInterpreter();
	extern void P2InsertDebugger();
	extern void P2InsertFlashLoader();
//	extern void P2ResetDebugSymbols();
//	extern void P2ParseDebugString();
};

typedef enum
{
	SCOPE,SCOPE_XY,SCOPE_RT,PLOT,TERM,TITLE,POS,SIZE1,
	RANGE,OFFSET,SAMPLES,TRIGGER,LABEL,COLOR1,BACKCOLOR,
	GRIDCOLOR,DOTSIZE,LOGSCALE,CLEAR,SAVE,UPDATE,LINESIZE,
	LINECOLOR,FILLCOLOR,TO,RECT1,RECTFILL,OVAL,OVALFILL,
	POLY,POLYFILL,TEXTSIZE,TEXTCOLOR,TEXTSTYLE,TEXT,NULL1
}	debug_id;

typedef void (*dbg_proc0[])();
typedef void (*dbg_proc1[])(void*);
typedef enum { add=0, sub=1, mult=2, divi=3, }  opcode;
typedef double (*ftable[])(const double&, const double&);

class command_proc
{
public:
	static lframe *pFrame;

protected:
	static void (*debug_exec[36])();
	static void FSCOPE ();
	static void FSCOPE_XY();
	static void FSCOPE_RT ();
	static void FPLOT ();
	static void FTERM ();
	static void FTITLE ();
	static void FPOS ();
	static void FSIZE1 ();
	static void FRANGE ();
	static void FOFFSET ();
	static void FSAMPLES ();
	static void FTRIGGER ();
	static void FLABEL ();
	static void FCOLOR ();
	static void FBACKCOLOR ();
	static void FGRIDCOLOR ();
	static void FDOTSIZE ();
	static void FLOGSCALE ();
	static void FCLEAR ();
	static void FSAVE ();
	static void FUPDATE ();
	static void FLINESIZE ();
	static void FLINECOLOR ();
	static void FFILLCOLOR ();
	static void FTO ();
	static void FRECT1 ();
	static void FRECTFILL ();
	static void FOVAL ();
	static void FOVALFILL ();
	static void FPOLY ();
	static void FPOLYFILL ();
	static void FTEXTSIZE ();
	static void FTEXTCOLOR ();
	static void FTEXTSTYLE ();
	static void FTEXT ();
	static void FNULL1 ();

public:
	static char *debug_tokens[];
	static void reset_debug_symbols();
	static void parse_debug_string();
	void exec(debug_id);
};




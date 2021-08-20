
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
	extern lframe *pFrame;

	extern LPVOID P2InitStruct();
	extern void P2Compile1();
	extern void P2Compile2();
	extern void P2InsertInterpreter();
	extern void P2InsertDebugger();
	extern void P2InsertFlashLoader();
	extern void P2ResetDebugSymbols();
	extern void P2ParseDebugString();
};





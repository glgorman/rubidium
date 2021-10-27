
#include "stdafx.h"
#include "globalunit.h"
#include "P2 Graphical Debug Terminal.h"
#include "P2 Graphical Debug TerminalDoc.h"
#include "signal view.h"

#define NUMBER_OF_STAVES_PER_PAGE (4)
#define NUMBER_OF_STAVES_PER_PAGE (4)
#define NUMBER_OF_STAVES_PER_PAGE (4)

#pragma warning (disable:4267)

namespace MIDI
{
	char *modes[] = {"Ionian","Dorian","Phrygian","Lydian",
		"Myxolydian","Aoelian","Locrian",NULL};
	char *note_names1[] = {"C","C#","D","D#","E","F","F#","G","G#","A","A#","B",NULL};
	char *note_names2[] = {"C","Db","D","Eb","E","F","Gb","G","Ab","A","Bb","B",NULL};
	int tomidi (char *note, int octave);
//	Eight lousy stinking quarter notes!
	char *pachelbel[] = {"D","A","B","F#","G","D","G","A",NULL};
};

// exmample MIDI::tomidi ("C#",4) should
// equal 61.  Middle C would be 60, etc.

int MIDI::tomidi (char *note, int octave)
{
	int i, result, offset;
	offset = 0;
	for (i=0;i<12;i++)
	{
		if ((strcmp(note,note_names1[i])==0)
			||(strcmp(note,note_names2[i])==0))
		{
			offset = i;
			break;
		}
	}
	result = 12*octave+offset;
	return offset;
}

void FFT_PLOT::set_dc (CDC *pdc,  CSignalView *view)
{
	CRect rect;
	m_graph = pdc;
	view->GetClientRect(&rect);
	m_width = rect.Width();
	m_height = rect.Height();
}

void FFT_PLOT::plot_text(int x, int y, int sz, char *msg)
{
	font.CreateFont(sz, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _T("Sonata"));
	CDC *graph = m_graph;
	CGdiObject *pOldFont = graph->SelectObject(&font); // save the current font
	graph->SetTextColor (COLOR::blue);
	CRect text_pos;	
	text_pos.left = x;
	text_pos.top = y;
	text_pos.right = 0;
	text_pos.bottom = 0;
	graph->DrawText ((const CString)msg,text_pos,DT_LEFT|DT_CALCRECT);
	graph->DrawText ((const CString)msg,text_pos,DT_LEFT);
//	graph->SelectObject(&pOldFont);
}

void FFT_PLOT::plot_note (int midi, MATH_TYPE time, note_type type)
{
	CDC *graph = m_graph;
	SETCOLOR _(graph,COLOR::black,COLOR::black);
	graph->SetBkMode(TRANSPARENT);
	
	int x0,x1,y0,y5,y2;
	int xp,dx;
	MATH_TYPE phi;
	int octave, index, vpos;
	octave = midi/12-1;
	index = midi%12;
	bool flat_scale = true;
	char *note_name;
	if (flat_scale==true)
		note_name = MIDI::note_names2[index];
	else
		note_name = MIDI::note_names1[index];
	bool is_accidental;
	
	char note_root = note_name[0];
	is_accidental = (strlen(note_name)==2?true:false);
	if (index>4)
		index++;
	if (flat_scale&&is_accidental)
		index++;
	vpos = 7*octave+32+index/2;
	int stem_height = 6;
	float y_scale = 0.02;
	// 60 = middle MIDI middle C.
	phi = (60-vpos)/2.0*y_scale*m_width;
	x0 = 0.10*m_width;
	x1 = 0.96*m_width;
	xp = time*(x1-x0)+x0;
	dx = 0.008*m_width;
	y0 = y_scale*(0+2)*m_width;
	y5 = y_scale*(10+2)*m_width;
	y2 = phi+(y0+y5)/2;
	int font_size = 3*m_width/128;
	graph->Ellipse (xp-dx,
		y2-dx,xp+dx,y2+dx);
//	if (is_accidental)
//		plot_text(xp-2*dx,y2-2*dx,font_size,&note_name[1]);
	if (type==QUARTER)
	{
		graph->MoveTo (xp+dx,y2);
		graph->LineTo (xp+dx,y2-stem_height*dx);
	}
}

FFT_PLOT::FFT_PLOT()
{
	NUMBER_OF_STAVVES_PER_PAGE = 4;
	NUMBER_OF_MEASURES_PER_LINE = 4;
	NUMBER_OF_NOTES_PER_MEASURE = 4;
}

void FFT_PLOT::plot_notes ()
{
	int i,j,note_index;
	MATH_TYPE offset;
	note_type type = QUARTER;
	int span = NUMBER_OF_MEASURES_PER_LINE*NUMBER_OF_NOTES_PER_MEASURE;
	for (i=0;i<NUMBER_OF_MEASURES_PER_LINE;i++)
	for (j=0;j<NUMBER_OF_NOTES_PER_MEASURE;j++)
	{
		note_index = i*NUMBER_OF_NOTES_PER_MEASURE+j;
		offset = float(note_index)/span;
		plot_note (48+note_index,offset,type);
		plot_note (60+note_index,offset,type);
//	plot_note (51+note_index,offset,type);
//	plot_note (55+note_index,offset,type);
	}
}

void FFT_PLOT::plot_grand_staff ()
{
//	char treble_clef[] = {0xF0,0x9D,0x84,0x9E,0};
	char treble_clef[] = {0xD8,0x34,0xDD,0x1E,0};	
//	char treble_clef[] = "this is a test";
	
	CDC *graph = m_graph;
	graph->SetBkColor(COLOR::white);
	SETCOLOR _(graph,COLOR::black);
	int i,j,k,x0,x1,y0,y1,y5;
	int note_x, note_y;
	int page_offset;

	MATH_TYPE y_scale = 0.02;
	MATH_TYPE offset, epsilon;
	for (k=0;k<NUMBER_OF_STAVES_PER_PAGE;k++)
	{
		x0 = 0.05*m_width;
		x1 = 0.95*m_width;
		page_offset = k*y_scale*(14+2)*m_width;
		y0 = y_scale*(0+2)*m_width+page_offset;
		y1 = y_scale*(4+2)*m_width+page_offset;
		y5 = y_scale*(10+2)*m_width+page_offset;		
		offset = 0;
//	treble cleff
//	plot_text(x0,(y0+y5)>>1,40,treble_clef);
		for (i=0;i<=NUMBER_OF_MEASURES_PER_LINE;i++)
		{
			epsilon = i/float(NUMBER_OF_MEASURES_PER_LINE);
			graph->MoveTo (CPoint((x1-x0)*epsilon+x0,y0));
			graph->LineTo (CPoint((x1-x0)*epsilon+x0,y1));
		}
		graph->MoveTo (CPoint((x1-x0)*0.01+x0,y0));
		graph->LineTo (CPoint((x1-x0)*0.01+x0,y1));
	// horizontal lines
		for (i=0;i<5;i++)
		{
			y0 = y_scale*(i+2)*m_width+page_offset;
			graph->MoveTo (CPoint(x0,y0));
			graph->LineTo (CPoint(x1,y0));
		}

// bass cleff
		y0 = y_scale*(6+2)*m_width+page_offset;
		for (i=0;i<=NUMBER_OF_MEASURES_PER_LINE;i++)
		{
	// vertical lines
			epsilon = i/float(NUMBER_OF_MEASURES_PER_LINE);
			graph->MoveTo (CPoint((x1-x0)*epsilon+x0,y0));
			graph->LineTo (CPoint((x1-x0)*epsilon+x0,y5));
		}
		graph->MoveTo (CPoint((x1-x0)*0.01+x0,y0));
		graph->LineTo (CPoint((x1-x0)*0.01+x0,y5));
		for (i=7;i<12;i++)
		{
	//horizontal lines
			y0 = y_scale*(i+1)*m_width+page_offset;
			graph->MoveTo (CPoint(x0,y0));
			graph->LineTo (CPoint(x1,y0));
		}
		for (i=0;i<=NUMBER_OF_MEASURES_PER_LINE;i++)
		{
	// vertical lines that  connect bass and treble
	// clefs - redundant code - but it works, and will
	// to modify this when figuring out how to write
	// lyrics between staves, for example.
			epsilon = i/float(NUMBER_OF_MEASURES_PER_LINE);
			graph->MoveTo (CPoint((x1-x0)*epsilon+x0,y1));
			graph->LineTo (CPoint((x1-x0)*epsilon+x0,y5));
		}
//	plot G clef
//	TODO
//	plot F clef
//	todo
	}
}
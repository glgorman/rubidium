
#include "stdafx.h"
#include "globalunit.h"
#include "debugunit.h"


//////////////////////
//  Event Routines  //
//////////////////////

void propeller_debug::Close()
{
	ASSERT(false);
}

void propeller_debug::FormKeyDown(short &Key)
{
	if (Key==kEsc)
		Close();
}

void propeller_debug::FormClose()
{
	CloseDisplays();
}

////////////////////////
//  Display Routines  //
////////////////////////

// Reset display variables
void propeller_debug::ResetDisplays()
{
#if 0
	TDebugDisplayDoc dbgDoc;
	CloseDisplays();
#ifdef HAVE_ASSEMNLY
	ASM386::P2ResetDebugSymbols();
#endif
	DisplayStrFlag=false;
	m_rc.resize_display (dbgDoc);
#endif
}

// Close any open display windows
void propeller_debug::CloseDisplays()
{
	int index;
	TDebugDisplayDoc *ptr;
	while (DebugDisplayCount>0)
	{
		DebugDisplayCount--;
		index = DebugDisplayCount;
		ptr = DisplayForms[index];
		ptr[DebugDisplayCount].Free();
	}
}

// Resize display

// Receive character
void propeller_debug::NewLine ()
{
	ASSERT(false);
}

void propeller_debug::NewChr (char x)
{
	ASSERT(false);
}

void propeller_debug::char_in (unsigned char x)
{
	HWND h;
	CString s;
	// FIXME - where does the come from?
	int DisplayStrLen = 0;
	if ((x==0x60)&&(!DisplayStrFlag))
	{
		DisplayStrFlag = true;
		DisplayStrLen = 0;
	}
	else if (DisplayStrFlag)
	{
		if (DisplayStrLen<255)
		{
			if (x!=13)
			{
//				DebugDisplayStr[DisplayStrLen] = x;
				DisplayStrLen = DisplayStrLen + 1;
			}
			else
			{
//				DebugDisplayStr[DisplayStrLen] = 0;
#ifdef HAVE_ASSEMNLY
				ASM386::P2ParseDebugString();
#endif
				DisplayStrFlag = false;
				// start new debug display?
				if (PGLOBAL::DebugDisplayType[0]==1)
				{
#if 0
					DisplayForms[DebugDisplayCount-1] = TDebugDisplayDoc::create(NULL);
#endif
					ASSERT(false);
					SetFocus(h);     // return focus to this form
				}
				else
					// update existing debug display?
					if (PGLOBAL::DebugDisplayType[0]==2)
						DisplayForms[PGLOBAL::DebugDisplayValue[0]]->update_display();
			}
		}
		else if (x==13)
			DisplayStrFlag = false;
	}
	if ((x>=0x20)&(x<=0x7F))
		NewChr(x);
	else if (x==13)
		NewLine();
	else if (x==9)
		do {
			NewChr(0x20);
		}
		while((m_rc.m_col&7)!=0);
}




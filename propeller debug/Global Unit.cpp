
#include "stdafx.h"
#include "afxmt.h"
#include <iostream>
#include <vector>
#include "../Frame Lisp/defines.h"
#include "../Frame Lisp/symbol_table.h"
#include "../Frame Lisp/btreetype.h"
#include "../Frame Lisp/node_list.h"
#include "../Frame Lisp/text_object.h"
#include "../Frame Lisp/scripts.h"
#include "../Frame Lisp/frames.h"
#include "../Frame Lisp/frames1.h"
#include "../Frame Lisp/extras.h"
#include "globalunit.h"

#define quote(X) #X
typedef void (*debug_cmd_proc) (void);

lframe *command_proc::pFrame;

char ASM386::m_debugStr [256];

#define PI (3.14159265358979323)

char *command_proc::debug_tokens[] = 
{
	"SCOPE","SCOPE_XY","SCOPE_RT","PLOT","TERM","TITLE","POS","SIZE",
	"RANGE","OFFSET","SAMPLES","TRIGGER","LABEL","COLOR","BACKCOLOR",
	"GRIDCOLOR","DOTSIZE","LOGSCALE","CLEAR","SAVE","UPDATE","LINESIZE",
	"LINECOLOR","FILLCOLOR","TO","RECT","RECTFILL","OVAL","OVALFILL",
	"POLY","POLYFILL","TEXTSIZE","TEXTCOLOR","TEXTSTYLE","TEXT",NULL
};

template <class TypeName>
class lambda
{
protected:
	TypeName m_data;

public:
	lambda<TypeName>(TypeName val) { m_data = val; };
	TypeName *operator() (void) { return &m_data; };
};

dbg_proc0 command_proc::debug_exec =
{
	&command_proc::FSCOPE,
	&command_proc::FSCOPE_XY,
	&command_proc::FSCOPE_RT,
	&command_proc::FPLOT,
	&command_proc::FTERM,
	&command_proc::FTITLE,
	&command_proc::FPOS,
	&command_proc::FSIZE1,
	&command_proc::FRANGE,
	&command_proc::FOFFSET,
	&command_proc::FSAMPLES,
	&command_proc::FTRIGGER,
	&command_proc::FLABEL,
	&command_proc::FCOLOR,
	&command_proc::FBACKCOLOR,
	&command_proc::FGRIDCOLOR,
	&command_proc::FDOTSIZE,
	&command_proc::FLOGSCALE,
	&command_proc::FCLEAR,
	&command_proc::FSAVE,
	&command_proc::FUPDATE,
	&command_proc::FLINESIZE,
	&command_proc::FLINECOLOR,
	&command_proc::FFILLCOLOR,
	&command_proc::FTO,
	&command_proc::FRECT1,
	&command_proc::FRECTFILL,
	&command_proc::FOVAL,
	&command_proc::FOVALFILL,
	&command_proc::FPOLY,
	&command_proc::FPOLYFILL,
	&command_proc::FTEXTSIZE,
	&command_proc::FTEXTCOLOR,
	&command_proc::FTEXTSTYLE,
	&command_proc::FTEXT,
	&command_proc::FNULL1,
};

void command_proc::reset_debug_symbols()
{
	command_proc pdbg;
	frame &f = *(pFrame->get());
	symbol_table *t=NULL;
	t = f.cons(command_proc::debug_tokens)->sort();
	pdbg.exec(NULL1);
}

void  command_proc::parse_debug_string()
{
//	node_list<char*> N0;
//	text_object T1;
//	TEST::node_test2(N0);
	CWnd *pWnd = AfxGetApp()->GetMainWnd();
}

namespace TEST
{
char *str[]=
{
	"It was a dark and stormy night.",
	"And as the swamp thing staggered from the crypt, ",
	"Suddenly there was a need for words!",
	"Alas, even now as the vultures began to circle overhead",
	"It seemed ever more unlikely that I would be able to",
	"SCOPE out a safe place for the mothership to land",
	"let alone, find an oasis.  The plot thickens ... ",
	NULL,
};
	void node_test1(node_list<char*> &);
	void node_test2(node_list<char*> &);
};

void TEST::node_test1(node_list<char*> &N1)
{
	char m_debugStr[256];
//	node<char*> *s0 = lambda<node<char*>>("test");

	strcpy(m_debugStr,quote(It was a dark and stormy night.));
	node<char*> *s1  = new node<char*>(strdup(m_debugStr));
	strcpy(m_debugStr,"And as the swamp thing staggered from the crypt, ");
	node<char*> *s2 = new node<char*>(strdup(m_debugStr));
	strcpy(m_debugStr,"Suddenly there was a need for words!");
	node<char*> *s3 = new node<char*>(strdup(m_debugStr));
	strcpy(m_debugStr,"Alas, even now as the vultures began to circle overhead");
	node<char*> *s4  = new node<char*>(strdup(m_debugStr));
	strcpy(m_debugStr,"It seemed ever more unlikely that I would be able to");
	node<char*> *s5  = new node<char*>(strdup(m_debugStr));
	strcpy(m_debugStr,"SCOPE out a safe place for the mothership to land");
	node<char*> *s6  = new node<char*>(strdup(m_debugStr));
	strcpy(m_debugStr,"let alone, find an oasis.  The plot thickens ... ");
	node<char*> *s7  = new node<char*>(strdup(m_debugStr));
	N1.concat(s1);
	N1.concat(s2);
	N1.concat(s3);
	N1.concat(s4);
	N1.concat(s5);
	N1.concat(s6);
	N1.concat(s7);
}

void TEST::node_test2(node_list<char*> &N1)
{
	std::vector<node<char*>> s;
	s.resize(7);
	int n=0;
	while (str[n]!=NULL)
	{
	 s[n] = node<char*>(str[n]);
	 N1.concat(&s[n++]);
	}
}

void command_proc::exec(debug_id id)
{
	(*debug_exec[id])();
};

void command_proc::FSCOPE()
{
	
}

void command_proc::FSCOPE_XY()
{
	
}

void command_proc::FSCOPE_RT()
{
	
}

void command_proc::FPLOT()
{
	
}

void command_proc::FTERM()
{
	
}

void command_proc::FTITLE()
{
	
}

void command_proc::FPOS()
{
	
}

void command_proc::FSIZE1()
{
	
}

void command_proc::FRANGE()
{
	
}

void command_proc::FOFFSET()
{
	
}

void command_proc::FSAMPLES()
{
	
}

void command_proc::FTRIGGER()
{
	
}

void command_proc::FLABEL()
{
	
}

void command_proc::FCOLOR()
{
	
}

void command_proc::FBACKCOLOR()
{
	
}

void command_proc::FGRIDCOLOR()
{
	
}

void command_proc::FDOTSIZE()
{
	
}

void command_proc::FLOGSCALE()
{
	
}

void command_proc::FCLEAR()
{
	
}

void command_proc::FSAVE()
{
	
}

void command_proc::FUPDATE()
{
	
}

void command_proc::FLINESIZE()
{
	
}

void command_proc::FLINECOLOR()
{
	
}

void command_proc::FFILLCOLOR()
{
	
}

void command_proc::FTO()
{
	
}

void command_proc::FRECT1()
{
	
}

void command_proc::FRECTFILL()
{
	
}

void command_proc::FOVAL()
{
	
}

void command_proc::FOVALFILL()
{
	
}

void command_proc::FPOLY()
{
	
}

void command_proc::FPOLYFILL()
{
	
}

void command_proc::FTEXTSIZE()
{
	
}

void command_proc::FTEXTCOLOR()
{
	
}

void command_proc::FTEXTSTYLE()
{
	
}

void command_proc::FTEXT()
{
	
}

void command_proc::FNULL1()
{
	
}


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

lframe *ASM386::pFrame;

char ASM386::m_debugStr [256];

char *debug_tokens[] = 
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

debug_cmd_proc cmd_map [255] =
{	
	&ASM386::P2ResetDebugSymbols,
	&ASM386::P2ParseDebugString,
};


void  ASM386::P2ResetDebugSymbols()
{
	frame &f = *(pFrame->get());
	symbol_table *t=NULL;
	t = f.cons(debug_tokens)->sort();

}

void  ASM386::P2ParseDebugString()
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







// keywords.cpp
#include "stdafx.h"
#include "defines.h"
#include <cstring>
#include <ctype.h>
#include "defines.h"
#include "symbol_table.h"
#include "btreetype.h"
#include "language.h"
#include "node_list.h"
#include "keywords.h"

extern char *separators[];
#pragma warning (disable: 4996)

void to_upper (char* str)
{
	char *p;
	if ((p=str)!=NULL)
	while (*p!='\0')
	{
		*p = toupper(*p);
		p++;
	}
}	

void to_lower (char* str)
{
	char *p;
	if ((p=str)!=NULL)
	while (*p!='\0')
	{
		*p = toupper(*p);
		p++;
	}
}

char *gdi_commands[]=
{
	"MoveTo","LineTo","BitBlt","DrawCircle",
	"DrawEllipse","DrawRect","FillRect",
	"SelectPen","SelectBrush","BeginPath",
	"EndPath","StrokePath","StrokeAndFillPath",
	"DrawPolygon","DrawArc",NULL
};

char *c_keywords[]=
{
// preprocessor
	"#define","#inclue","#ifdef","#endif",
	"pragma:","#if","#else",
// types
	"register","volatile","static",
	"external","signed","unsigned",
	"long","short","struct","union",
	"char","double","enum","float","int",
	"struct","void","explicit","inline",
// conditional
	"if","else","case","switch",
// orogram flow
	"for","goto","while","do",
	"continue","break","return",
//	C++
	"public","protected","private"
	"class","template","try","throw",
	"catch","except","operator",
	"new","delete","static_cast",
	"dynamic_cast","reinterpret_cast",
	"virtual",
	NULL
};

char *pascal_keywords[] =
{
// program organization
	"program","unit","library",
	"procedure","function",
	"interface","uses",
	"const","var","declaration",
	"implementation",
// program flow
	"begin","end","repeat","until",
	"for","to","downto",
	"while","do","with","if",
	"then","else","and","or","not",
	"break","continue",
	"case","otherwise",
// logical and math
	"div","shr","shl",":=","==",
	"<>",">","<",">=","<=",
// data types
	"record","type","packed","array",
	"integer","real","extended",
	"boolean","bit","short"
	"true","false","word",
	NULL
};


char *lisp_keywords[] =
{
	"car","cdr","cons","eval",
	"cond","defun","append","quote",
	"list","lambda",NULL
};

#if 0
char *delimiters [] = 
{
	".","!","?","#",NULL
};
#endif
#if 0
char *separators [] = 
{
	".", ",", ":", ";", "!", "?","\n","\t","#",NULL
};
#endif

typedef struct 
{
	char *key;
	void (*proc_addr)();
	int params;
} dispatch_entry;


bool key_word::is_type (char *what, char **type_list)
{
	int count = 0;
	char *possible, isThis [128];
	if (what==NULL)
		return false;
	strcpy (isThis,what);
	to_lower (isThis);
	for (;;) {
		possible = type_list [count];
		if (possible==NULL)
			break;
		int test = strcmp (possible,isThis);
		if (test == 0)
			return true;
		count++;
	}
	return false;
};

bool key_word::is_seperator (char *is_in)
{
	bool result; 
	result = is_type (is_in,separators);
	return result;
}

bool key_word::is_c (char *is_in)
{
	bool result; 
	result = is_type (is_in,c_keywords);
	return result;
}

bool key_word::is_pascal (char *is_in)
{
	bool result; 
	result = is_type (is_in,pascal_keywords);
	return result;
}

bool key_word::is_numeric (char *is_in)
{
	char theChar;
	bool result = true;
	unsigned int n, j = strlen (is_in);
	for (n=0;n<j;n++)
	{
		theChar = is_in [n];
		if ((theChar<'0')||(theChar>'9')) {
			result=false;
			break;
		}
	}
	if (j==1) {
		theChar = is_in [0];
		if ((theChar=='+')||
		(theChar=='-')||
		(theChar=='*')||
		(theChar=='=')||
		(theChar=='/'))
		result = true;
	}
	return result;
}

bool key_word::is_opcode (char *is_in)
{
	char theChar;
	bool result = false;
	unsigned  j = strlen (is_in);
	if (j==1) {
		theChar = is_in [0];
		if ((theChar=='+')||
		(theChar=='-')||
		(theChar=='*')||
		(theChar=='=')||
		(theChar=='/'))
		result = true;
	}
	return result;
}

source_type key_word::identify_program (char *ascii)
{
	char *isThis = ascii;
	source_type result;	
	if (strcmp(isThis," ")==0)
		result = whitespace;
	else if (is_pascal (isThis)==true)
		result = reserved_pascal;
	else if (is_c (isThis)==true)
		result = reserved_c;
	else if (is_c (isThis)==true)
		result = reserved_lisp;
	else
		result = unknown2;

	return result;
}

void key_word::pToName (char *result, language aType)
{
	switch (aType)
	{
		case reserved_c:
			strcpy (result,"C/C++");
			break;
		case reserved_pascal:
			strcpy (result,"Pascal");
			break;
		case reserved_lisp:
			strcpy (result,"Lisp");
			break;
		case reserved_fortran:
			strcpy (result,"Lisp");
			break;
		default:
			break;
	}
}

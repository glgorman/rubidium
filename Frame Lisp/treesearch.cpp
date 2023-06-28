
#include "stdafx.h"
#include "afxmt.h"
#include <iostream>
#include "language.h"
#include "../Frame Lisp/framelisp.h"
#include "insymbol.h"
#include "../Frame Lisp/objects.h"

#ifdef PASCAL_COMPILER
// for functions needing identifer
// ans structure classes - todo - move
// into dedicated headers
#include "../p-system compiler/compilerdata.h"
#include "../p-system compiler/declarationpart.h"
#include "../p-system compiler/bodypart.h"
#include "../p-system compiler/compiler.h"
#endif

// #define DEBUG_SEARCH

// DEFINED IN COMPILER.H

#if 0
typedef enum _SYMBOL
{
	IDENT,COMMA,COLON,SEMICOLON,LPARENT,RPARENT,DOSY,TOSY,
	DOWNTOSY,ENDSY,UNTILSY,OFSY,THENSY,ELSESY,BECOMES,LBRACK,
	RBRACK,ARROW,PERIOD,BEGINSY,IFSY,CASESY,REPEATSY,WHILESY,
	FORSY,WITHSY,GOTOSY,LABELSY,CONSTSY,TYPESY,VARSY,PROCSY,
	FUNCSY,PROGSY,FORWARDSY,INTCONST,REALCONST,STRINGCONST,
	NOTSY,MULOP,ADDOP,RELOP,SETSY,PACKEDSY,ARRAYSY,RECORDSY,  
	FILESY,OTHERSY,LONGCONST,USESSY,UNITSY,INTERSY,IMPLESY,
	EXTERNLSY,SEPARATSY,MAXSYMBOL
}	SYMBOL;

typedef enum _OPERATOR
{
	MUL,RDIV,ANDOP,IDIV,IMOD,PLUS,MINUS,OROP,LTOP,LEOP,
	GEOP,GTOP,NEOP,EQOP,INOP,NOOP,MAXOPERATOR,
} OPERATOR;	
#endif

namespace pascal0
{
key_info operators[] = 
{
	key_info(":=",SYMBOLS::BECOMES,SYMBOLS::NOOP),
	key_info("(*",SYMBOLS::COMMENTSY,SYMBOLS::NOOP),
	key_info("*)",SYMBOLS::SEPARATSY,SYMBOLS::NOOP),
	key_info("(",SYMBOLS::LPARENT,SYMBOLS::NOOP),
	key_info(")",SYMBOLS::RPARENT,SYMBOLS::NOOP),
	key_info("{",SYMBOLS::COMMENTSY,SYMBOLS::NOOP),	
	key_info("}",SYMBOLS::SEPARATSY,SYMBOLS::NOOP),
	key_info("..",SYMBOLS::COLON,SYMBOLS::NOOP),
	key_info(".",SYMBOLS::PERIOD,SYMBOLS::NOOP),
	key_info(";",SYMBOLS::SEMICOLON,SYMBOLS::NOOP),
	key_info(",",SYMBOLS::COMMA,SYMBOLS::NOOP),
	key_info(":",SYMBOLS::COLON,SYMBOLS::NOOP),
	key_info("^",SYMBOLS::ARROW,SYMBOLS::NOOP),
	key_info("[",SYMBOLS::LBRACK,SYMBOLS::NOOP),
	key_info("]",SYMBOLS::RBRACK,SYMBOLS::NOOP),	
	key_info("+",SYMBOLS::ADDOP,SYMBOLS::PLUS),
	key_info("-",SYMBOLS::ADDOP,SYMBOLS::MINUS),
	key_info("*",SYMBOLS::MULOP,SYMBOLS::MUL),
	key_info("/",SYMBOLS::MULOP,SYMBOLS::RDIV),
	key_info("=",SYMBOLS::RELOP,SYMBOLS::EQOP),
	key_info("<>",SYMBOLS::RELOP,SYMBOLS::NEOP),
	key_info(">=",SYMBOLS::RELOP,SYMBOLS::GEOP),
	key_info("<=",SYMBOLS::RELOP,SYMBOLS::LEOP),
	key_info(">",SYMBOLS::RELOP,SYMBOLS::GTOP),	
	key_info("<",SYMBOLS::RELOP,SYMBOLS::LTOP),
	key_info("\'",SYMBOLS::STRINGCONST,SYMBOLS::NOOP),
	key_info("",SYMBOLS::OTHERSY,SYMBOLS::NOOP),
};

key_info keywords [] =
{
	key_info("DO",SYMBOLS::DOSY,SYMBOLS::NOOP),
	key_info("WITH",SYMBOLS::WITHSY,SYMBOLS::NOOP),
	key_info("IN",SYMBOLS::SETSY,SYMBOLS::INOP),
	key_info("TO",SYMBOLS::TOSY,SYMBOLS::NOOP),
	key_info("GOTO",SYMBOLS::GOTOSY,SYMBOLS::NOOP),
	key_info("SET",SYMBOLS::SETSY,SYMBOLS::NOOP),
	key_info("DOWNTO",SYMBOLS::DOWNTOSY,SYMBOLS::NOOP),
	key_info("LABEL",SYMBOLS::LABELSY,SYMBOLS::NOOP),
	key_info("PACKED",SYMBOLS::PACKEDSY,SYMBOLS::NOOP),
	key_info("END",SYMBOLS::ENDSY,SYMBOLS::NOOP),
	key_info("CONST",SYMBOLS::CONSTSY,SYMBOLS::NOOP),
	key_info("ARRAY",SYMBOLS::ARRAYSY,SYMBOLS::NOOP),
	key_info("UNTIL",SYMBOLS::UNTILSY,SYMBOLS::NOOP),
	key_info("TYPE",SYMBOLS::TYPESY,SYMBOLS::NOOP),
	key_info("RECORD",SYMBOLS::RECORDSY,SYMBOLS::NOOP),
	key_info("OF",SYMBOLS::OFSY,SYMBOLS::NOOP),
	key_info("VAR",SYMBOLS::VARSY,SYMBOLS::NOOP),
	key_info("FILE",SYMBOLS::FILESY,SYMBOLS::NOOP),
	key_info("THEN",SYMBOLS::THENSY,SYMBOLS::NOOP),
	key_info("PROCEDURE",SYMBOLS::PROCSY,SYMBOLS::NOOP),
	key_info("USES",SYMBOLS::USESSY,SYMBOLS::NOOP),
	key_info("ELSE",SYMBOLS::ELSESY,SYMBOLS::NOOP),
	key_info("FUNCTION",SYMBOLS::FUNCSY,SYMBOLS::NOOP),
	key_info("UNIT",SYMBOLS::UNITSY,SYMBOLS::NOOP),
	key_info("BEGIN",SYMBOLS::BEGINSY,SYMBOLS::NOOP),
	key_info("PROGRAM",SYMBOLS::PROGSY,SYMBOLS::NOOP),
	key_info("INTERFACE",SYMBOLS::INTERSY,SYMBOLS::NOOP),
	key_info("IF",SYMBOLS::IFSY,SYMBOLS::NOOP),
	key_info("SEGMENT",SYMBOLS::SEPARATSY,SYMBOLS::NOOP),
	key_info("IMPLEMENTATION",SYMBOLS::IMPLESY,SYMBOLS::NOOP),
	key_info("CASE",SYMBOLS::CASESY,SYMBOLS::NOOP),
	key_info("FORWARD",SYMBOLS::FORWARDSY,SYMBOLS::NOOP),
	key_info("EXTERNAL",SYMBOLS::EXTERNLSY,SYMBOLS::NOOP),
	key_info("REPEAT",SYMBOLS::REPEATSY,SYMBOLS::NOOP),
	key_info("NOT",SYMBOLS::NOTSY,SYMBOLS::NOOP),
	key_info("OTHERWISE",SYMBOLS::OTHERSY,SYMBOLS::NOOP),
	key_info("WHILE",SYMBOLS::WHILESY,SYMBOLS::NOOP),
	key_info("AND",SYMBOLS::RELOP,SYMBOLS::ANDOP),
	key_info("DIV",SYMBOLS::MULOP,SYMBOLS::IDIV),
	key_info("MOD",SYMBOLS::MULOP,SYMBOLS::IMOD),
	key_info("FOR",SYMBOLS::FORSY,SYMBOLS::NOOP),
	key_info("OR",SYMBOLS::RELOP,SYMBOLS::OROP),
};
};

namespace treetype
{
	char *operators[] = 
	{
		"~","!","%","&","&&","|","||","*","-","+","/",
		"<>",">=","<=","==",
	};
	char *keywords[] = 
	{
		"DO","WITH","IN","TO","GOTO","SET","DOWNTO","LABEL",
		"PACKED","END","CONST","ARRAY","UNTIL","TYPE","RECORD",
		"OF","VAR","FILE","THEN","PROCEDURE","USES",
		"ELSE","FUNCTION","UNIT","BEGIN","PROGRAM","INTERFACE",
		"IF","SEGMENT","IMPLEMENTATION","CASE","FORWARD","EXTERNAL",
		"REPEAT","NOT","OTHERWISE","WHILE","AND","DIV","MOD",
		"FOR","OR",NULL
	};
	char *klass_names [] =
	{
		"NONE",
		"TYPES",
		"KONST",
		"FORMALVARS",
		"ACTUALVARS",
		"FIELD",
		"PROC1",
		"FUNC",
		"MODULE"
	};
	char *struct_forms [] =
	{
		"UNDEFINED",
		"SCALAR",
		"SUBRANGE",
		"POINTER",
		"LONGINT",
		"POWER",
		"ARRAYS",
		"RECORDS",
		"FILES",
		"TAGFLD",
		"VARIANT2"
	};
	frame m_pFrame;
	symbol_table *m_keywords;
	pascal0::key_info *get_key_info (int index);
	void struct_info (const CTP node, STP stp);
	void printnode (const CTP &n1, size_t &N, IDCLASS, bool verbose);
	void PRINTNODE1(const CTP &n1, size_t &N);
	void printleaf (const CTP node, IDCLASS target);
	void INDENT_NEWLINE (int i);
	void ADJUST_LINE(size_t &I);
};

void treetype::symbol_dump (const CTP &n1, int i, IDCLASS ftype)
{
	writeln (output,"Identifiers Level = ",i);
#ifdef PASCAL_COMPILER
	treetype::printtree("Tree: ",n1, i, ftype,false);
#endif
	writeln (output);
}

void treetype::TRAP1 (char *str1, char *str2)
{
	if (strcmp(str1,str2)==0)
		ASSERT(false);
}

void treetype::TRAP2 (char *str1, char *str2, CTP LCP0)
{
#ifdef PASCAL_COMPILER
	if (strcmp(str1,str2)==0)
		treetype::printtree1(LCP0);
#endif
}

void treetype::TRAP3 (char *str1, char *str2)
{
//	writeln(output);
	if (strcmp(str1,str2)==0)
	{
		::write(output,"PASCALCOMPILER::SEARCHID: ");
		writeln(output,"Searching for: \"",str1,"\" in SETOFIDS ");
	}
}

void treetype::reset_symbols()
{
	frame &f = treetype::m_pFrame;
	symbol_table *t=NULL;
	// very dangerous.  does it work?
	t = f.cons(keywords);
	t->sort(t->_unique);
 	m_keywords = t;
}

int treetype::compare (ALPHA &str1, ALPHA &str2)
{
	char c1, c2;
	int i,j,k,l;
	int test = 0;
	i = str1.length();
	j = str2.length();
	if (i>j)
		l=i;
	else
		l=j;

	for (k=0;k<l;k++)
	{
		test = 0;
		c1 = tolower(str1[k]);
		c2 = tolower(str2[k]);
// some of the symbols have trailing spaces, which means
// that one string will have a terminatinng null while
// the other will have a trailing space, but only when
// the lengths are unequal.
		if ((c1==0)&&(c2==32)
			||(c2==0)&&(c1==32))
			break;
// perfectly safe to compare the terminating zero with
// any other characther in the regular fashion.
		if (c1!=c2)
		{
			test = (c1>c2?1:-1);
			break;
		}
	}
	if (((i<j)&&(test==0))&&(str2[k]!=32))
		test=-1;

	return test;
}

pascal0::key_info *treetype::get_key_info (int index)
{
	pascal0::key_info *result;
	ASSERT((index>=0)&&(index<=42));
	result = &pascal0::keywords [index];
	return result;
}

#ifdef PASCAL_COMPILER
int treetype::idsearch(const CTP& n1, CTP& n2, ALPHA &str)
{
#ifdef DEBUG_TREESEARCH
	write(output,"treetype(CTP& n1, CTP& n2,\"",(char*)str,"\")");
#endif
//	TRAP1 ("SIZE",str);

	CTP ptr = n1;
	bool found = false;
	bool quit = false;
	int test;
	int i,j,k,l;
	
	do {
		test = compare (str,ptr->NAME);
		if ((test<0)&&(ptr->LLINK()!=NULL))
			ptr = ptr->LLINK();
		else if ((test>0)&&(ptr->RLINK()!=NULL))
			ptr = ptr->RLINK();
		else if (test==0)
		{
			found=true;
			quit = true;
		}
		else if ((test<0)&&(ptr->LLINK()==NULL))
			quit = true;
		else if ((test>0)&&(ptr->RLINK()==NULL))
			quit = true;
	}
	while (quit==false);
	n2 = ptr;
#if DEBUG_TREESEARCH
	writeln (output," returning ",test);
#endif
	return test;
}
#endif

int treetype::keysearch(int pos, char *&str)
{
//	SEARCH::RESET_SYMBOLS();
#ifdef DEBUG_SEARCH
	write(output,"\nSEARCH::IDSEARCH --> ");
#endif

	size_t i, len, sz;
	bool found = false;
	int syid = -1;
	pascal0::key_info result;
	pascal0::key_info *kp = pascal0::keywords;
	char c1, buf[32];
	char *str1=str+pos;
	for (len=0;len<32;len++)
	{
		c1=str1[len];
		if (!chartypes::ident.in(c1))
			break;
	}
	memcpy(buf,str1,len);
	buf[len]=0;

#ifdef DEBUG_SEARCH
	writeln (output,"searching for \"",buf,"\"");
#endif

	symbol_table &T = *treetype::m_keywords;
	sz = T.size();
	token *t;
	for (i=0;i<sz;i++)
	{
		t = &T[(int)i];
		len = strlen(t->ascii);
		if (strcmp(buf,t->ascii)==0)
		{
			result.ID = t->ascii;
			found = true;
			syid = t->m_index;
			break;
		}
	}
#ifdef DEBUG_SEARCH
	if (found==true)
		writeln(output,"FOUND \"",result.ID,"\"");
	else
		writeln(output,"NOT FOUND \"",buf,"\"");
#endif
	return syid;
}

void treetype::ADJUST_LINE(size_t &I)
{
	if (I>120){
//		write(output,"[",(int)I,"]");
		writeln(output);
		write(output,"\t");
		I=0;
	}
}

void treetype::INDENT_NEWLINE (int i)
{
	int j;
	writeln(output);
	for (j=0;j<i;j++)
	  write(output,"--");
}

#ifdef PASCAL_COMPILER
void treetype::struct_info (const CTP node, STP stp)
{
	char *fname = NULL;
	STP IDTYPE;
	if (node!=NULL)
		IDTYPE = node->IDTYPE;
	else
		IDTYPE = stp;

	STP eltype = NULL;
	STP recvar = NULL;
	STRUCTFORM form1;
	STRUCTFORM form2;
	int i;
	int lex = 1;
	do {
		if (IDTYPE!=NULL)
		{
			form1 = IDTYPE->form();
			eltype = IDTYPE->ELTYPE;
			recvar = IDTYPE->RECVAR;
		}
		else
			form1 = UNDEFINED;

		if (lex>0)
		{
			for (i=0;i<lex;i++)
				write(output,"----");
			write (output,">");
		}
		writeln (output," STRUCTFORM = <",struct_forms[form1],">");
		if (form1==POINTER)
		{
			IDTYPE = eltype;
			if (eltype!=NULL)
			{
				form2 = eltype->form();
				fname = struct_forms[form2]; 
			}

			for (i=0;i<lex;i++)
				write(output,"----");
			write (output,">");

			writeln (output," ELETYPE->form = <",fname,">");
		}
		else if (form1==ARRAYS)
		{
			object::arrays obj2 = static_cast<object::arrays> (IDTYPE);

			eltype = obj2.AELTYPE;
			STP index_type = obj2.INXTYPE;
			form2 = eltype->form();
			fname = struct_forms[form2];
			
			for (i=0;i<lex;i++)
				write(output,"----");
			write (output,">");

			writeln (output," AELETYPE->form = <",fname,">");
			if (index_type!=NULL)
			{
				form2 = index_type->form();
				fname = struct_forms[form2];
			}
			else
				fname = NULL;

			for (i=0;i<lex;i++)
				write(output,"----");
			write (output,">");

			write (output," INXTYPE->form = <",fname,">");
			if ((index_type!=NULL)&&(form2==SUBRANGE))
			{
				int min, max;
				min = (int)index_type->MIN.IVAL;
				max = (int)index_type->MAX.IVAL;
				write (output, " MIN = ",min,", MAX = ",max);
			}
			writeln (output);
			IDTYPE = eltype;
		}
		else if (form1==RECORDS)
		{
			IDTYPE = IDTYPE->RECVAR;
			if (stp->FSTFLD!=NULL)
			{
				CTP FSTFLD;
				FSTFLD = stp->FSTFLD;
				for (i=0;i<lex;i++)
					write(output,"----");
				write (output,">");
				writeln (output," FSTFLD: \"", FSTFLD->NAME,"\"");
//				printleaf (stp->FSTFLD,stp->FSTFLD->KLASS);
			}
			if (IDTYPE!=NULL)
			{
				form2 = IDTYPE->form ();
				fname = struct_forms[form2]; 
			}
			else
				fname = NULL;

			for (i=0;i<lex;i++)
				write(output,"----");
			write (output,">");
			writeln (output," RECVAR->form = <",fname,">");	
		}
		else
			IDTYPE = NULL;
		lex++;
	}
	while (IDTYPE!=NULL);
}
#endif

#ifdef PASCAL_COMPILER
void treetype::printleaf (const CTP node, IDCLASS target)
{
	IDCLASS id = node->KLASS;
	STP IDTYPE = node->IDTYPE;
	if (id!=target)
		return;

	write (output,"\"",node->NAME,"\",");
	write (output," KLASS = <",klass_names[id],">");
	if (id==KONST)
		write (output,", IVAL = <",node->VALUES.IVAL,">");

	writeln(output);
	struct_info (node,IDTYPE);
}

void treetype::printnode
(
	const CTP &n1,
	size_t &N,
	IDCLASS ftype,
	bool verbose
)
{
	CTP node;
	node = n1;
//	ADJUST_LINE(N);
//	INDENT_NEWLINE((int)N);
	N++;
	if (verbose==true)
		write(output,"NODE(");
//	N+=5;
	if (node->LLINK()!=NULL)
	{
		printnode (node->LLINK(),N,ftype,verbose);			
	}
	else
	{
//		N += 3;
		if (verbose==true)
		  write(output,"nil,");
	}
	printleaf (node,ftype);
	
//	N+= (strlen(node->NAME)+4);
	if (node->RLINK()!=NULL)
	{
		printnode (node->RLINK(),N,ftype,verbose);	
	}
	else
	{
		if (verbose==true)
		  write(output,"nil,");
//		N+=3;
	}
	if (verbose==true)
	  write(output,")");
	N--;
//	++N;
}

void treetype::PRINTNODE1(const CTP &n1, size_t &N)
{
	CTP node;
	node = n1;

	if (node->LLINK()!=NULL)
	{
		PRINTNODE1(node->LLINK(),N);			
	}
	else
	{
		write(output,"<null>");
	}
	write (output,",\"",node->NAME,"\",");
	if (node->RLINK()!=NULL)
	{
		PRINTNODE1(node->RLINK(),N);	
	}
	else
	{
		write(output,"null");
//		N+=3;l
	}
//	write(output,")");
	N--;
//	++N;
}

void treetype::printtree
(
	char *tag,
	const CTP &n1,
	int i,
	IDCLASS ftype,
	bool verbose
)
{
	CTP node;
//	size_t J  = 5;
	size_t lex = 0;
	node = n1;
	writeln (output,tag);
	if (node==NULL)
	{
		writeln(output,"TREE = NULL");
	return;
	}
//	write(output,"TREE(");
	if (node->LLINK()!=NULL)
	{
		printnode (node->LLINK(),lex,ftype,verbose);	
	}
	else if (verbose==true)
		write(output,"nil,");

	printleaf (node,ftype);
//	write (output,"\"",node->NAME,"\",");
	if (node->RLINK()!=NULL)
	{
		printnode (node->RLINK(),lex,ftype,verbose);	
	}
	else if (verbose==true)
	{
		write(output,"nil,");
		writeln(output,")");
	}
}

void treetype::printtree1(const CTP &n1)
{
	CTP node;
	STP	ptr = n1->IDTYPE;
	size_t lex = 0;
	node = n1;
	if (node==NULL)
	{
	writeln(output,"TREE = NULL");
	return;
	}
	writeln(output,"TREE: IDTYPE = ",(DWORD)ptr);
	if (node->LLINK()!=NULL)
	{
		PRINTNODE1(node->LLINK(),lex);	
	}
	else
		write(output,"null");

	write (output,",\"",node->NAME,"\",");
	if (node->RLINK()!=NULL)
	{
		PRINTNODE1(node->RLINK(),lex);	
	}
	else
		write(output,"nulll");
	writeln(output);
}
#endif
 // key_lists.cpp

#include "stdafx.h"
#include "defines.h"
#include <cstring>
#include <ctype.h>
#include "defines.h"
#include "symbol_table.h"
#include "btreetype.h"
#include "language.h"
#include "node_list.h"
#include "text_object.h"
#include "key_list.h"

#ifdef PASCAL
extern bool is_digit (char aChar);
#endif

key_list::key_list ()
{
//	wordCount = 0;
	m_nList.m_nBegin = NULL;
	m_nList.m_nPos = NULL;
	m_sList.m_pData = NULL;
	m_sList.m_pNext = NULL;
	m_bEnd = true;
}

key_list::key_list (text_object &theInput)
{
	s_node<char*,grammar::gtype> temp;
	grammar::gtype m_typeid;
	m_typeid = grammar::unknown1;
	char *ascii;
	theInput.rewind ();
	while (theInput.end_of_text()==false) {
		theInput.get (ascii);
//	m_typeid = temp.check (ascii);
		if (m_typeid==grammar::unknown1)
			append (ascii); }
	rewind ();
}

void key_list::rewind()
{
	text_object::rewind();
}

key_list::key_list (node_list<node_str> *m_pList)
{
	grammar::gtype what_type;
	what_type = grammar::unknown1;
	node<node_str> *the_word;
	char *ascii;
	the_word = m_pList->m_nBegin;
	while (m_pList->m_nPos!=NULL)
	{		
		if (what_type==grammar::unknown1) {
			the_word = m_nPos;
			ascii = the_word->data().ascii();
			append (ascii); }
		if (m_nPos!=NULL)
			m_nPos=&the_word->next();
	}
	rewind ();
}

key_list::key_list (char *m_pText)
{
#if 0
	language theType;
#endif
	s_node<char*,grammar::gtype> temp;
	int length, start = 0;
	char *copy = NULL;
	if (m_pText!=NULL) {
		length = strlen (m_pText);
		copy = new char [length+1]; }
	else return;
	while (m_pText [start]!=0) {
		int n = 0;		
		while (m_pText[start]==' ')
			start++;
		if (isalpha(m_pText[start])) do
			copy [n++] = m_pText [start++];
		while (isalpha(m_pText[start]));
		else if (isdigit(m_pText[start])) do
			copy [n++] = m_pText [start++];
		while (isdigit(m_pText[start]));
		else if (m_pText[start]!=0) do
			copy [n++] = m_pText [start++];
		while (!isalnum(m_pText[start])
			&&(m_pText[start]!=0)
			&&(m_pText[start]!=' '));
		copy [n]=0;
		
#if 0
		if (*copy!=0) 
		{
			theType = temp.check ();
			if (theType==unknown1) {
				if (m_nList==NULL)
				m_nList = new node_list<char*>;
				m_nList->appendNode (copy); }
		}
#endif
	}
	delete [] copy;
}

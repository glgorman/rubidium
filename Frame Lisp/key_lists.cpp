// key_lists.cpp
#include "stdafx.h"
#include "defines.h"
#include <cstring>
#include <ctype.h>
#include "defines.h"
#include "symbol_table.h"
#include "btreetype.h"
#include "node_list.h"
#include "text_object.h"
#include "key_list.h"

#ifdef PASCAL
extern bool isNum (char aChar);
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
	s_node<char*,language> temp;
	language m_typeid = unknown;
	char *ascii;
	theInput.rewind ();
	while (theInput.m_bEnd==false) {
		theInput.getIndexWord (ascii);
//	m_typeid = temp.check (ascii);
		if (m_typeid==unknown)
			append (ascii); }
	rewind ();
}

void key_list::rewind()
{
	text_object::rewind();
}

key_list::key_list (node_list<char*> *m_pList)
{
#if 0
	s_node<char*,language> psList;
	char *ascii;
	m_nList->m_nPos = m_pList->m_nBegin;
	theWord = m_nPos;
	while (m_pList->m_nPos!=NULL) {
		if (theWord->m_typeid==unknown) {
			theWord = m_nPos;
			ascii = theWord->m_pData;
			append (ascii); }
		if (m_nPos!=NULL)
			m_nPos=theWord->m_pNext;
	}
	rewind ();
#endif
}

key_list::key_list (char *m_pText)
{
#if 0
	language theType;
#endif
	s_node<char*,language> temp;
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
			if (theType==unknown) {
				if (m_nList==NULL)
				m_nList = new node_list<char*>;
				m_nList->appendNode (copy); }
		}
#endif
	}
	delete [] copy;
}

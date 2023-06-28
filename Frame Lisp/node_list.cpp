
#include "stdafx.h"
#include "symbol_table.h"
#include "btreetype.h"
#include "language.h"
#include "node_list.h"
#include "text_object.h"
#include "sets.h"
#include "extras.h"

#define noPlayTag true

node<char*>::node<char*> ()
{
	refCount = 1;
	m_pNext = NULL;
	m_pData = NULL;
}

node<char*>::node<char*> (char* str)
{
	refCount = 1;
	m_pData = str;
	m_pNext = NULL;
}

node<char*>::~node<char*> ()
{
// todo - notify the symbol table
// manager that we no longer need
// this char
}

node_list<char*>::~node_list<char*> ()
{
	node<char*> *ptr, *next;
	ptr = m_nBegin;
	while (ptr!=NULL)
	{
		next = &ptr->next();
		delete ptr;
		ptr = next;
	}
}

//	given a list of bTreeType<nodeType> objects
//	contruct a usual node_list<nodeType> with the
//	same ascii pointers;

template <class nodeType,typename part>
node_list<nodeType,part>::node_list<nodeType,part> (bTreeType<nodeType> **source)
{
	int n = 0;
	nodeType ascii;
	refCount = 1;
	m_nBegin = NULL;
	m_nEnd = NULL;
	while (source [n]!=NULL) {
		ascii = source [n]->m_pData;
		appendNode (ascii);
		n++; }
}

template <class nodeType,typename part>
void node_list<nodeType,part>::transfer_from (node_list<nodeType,part> *text)
{
	node_list<nodeType> *aWord;
	try {
	if (&text==NULL)
		throw text;
	aWord = text->m_nBegin;
	if (aWord!=NULL) {
		text->m_nBegin = aWord->m_pNext;
		if (text->m_nBegin==NULL)
			text->m_nEnd=NULL;
		aWord->m_pNext = NULL;
		ASSERT_FALSE();
		// fixe me!
//		attachWord (aWord);
	}
		}
	catch (node_list<nodeType>) {
	}
}

//	transfer a node from the end of this
//	phrase to the beginning of another phrase

template <class nodeType,typename part>
void node_list<nodeType,part>::transferTo (node_list<nodeType,part> *text)
{
	node_list<nodeType> *aWord,*m_nPos;
	m_nPos=m_nBegin;
	while (m_nPos!=NULL) {
		aWord=m_nPos;
		if (aWord->m_pNext==NULL)
			m_nPos=NULL;
		else {
			m_nEnd=m_nPos;
			m_nPos=m_nPos->m_pNext; }
	}
	aWord->m_pNext = text->m_nBegin;
	text->m_nBegin=aWord;
}

//	Returns a pointer to a permanant char []
//	pointed to by the s_node<nodeType,grammar::gtype> pointed to by
//	the m_nPos - then increments the play
//	tag -- which must be provied by the
//	caller!!

template <class nodeType,typename part>
nodeType node_list<nodeType,part>::put_next (s_node<nodeType,part> *(&m_nPos))
{
	nodeType theWord = NULL;
	if (m_nPos!=NULL) {
		theWord = m_nPos->m_pData;
		m_nPos = m_nPos->m_pNext; }
	return theWord;
}

//	when passed a reference to an (s_node<nodeType,grammar::gtype> *) 
//	this will repair the pointer to the	lastNode of this list 
//	if necessary and put the adress of that node into the 
//	argument provided

template <class nodeType,typename part>
void node_list<nodeType,part>::put_last (s_node<nodeType,part> *(&theWord))
{
	if (m_nBegin==NULL)
		theWord = NULL;
	else if (m_nEnd!=NULL)
		theWord = m_nEnd;
	else {
		theWord = m_nBegin;
		while (theWord->m_pNext!=NULL) {
			if (theWord!=theWord->m_pNext)
				theWord = theWord->m_pNext;
			else
				theWord->m_pNext=NULL; } }
}

//	append an existing s_node<nodeType,grammar::gtype> to a node_list<nodeType>
//	WARNING! The argument must by a	dynanicly allocated
//	s_node<nodeType,grammar::gtype>, or corrpution will be an aboulute certainty.

template <class nodeType,typename part>
void node_list<nodeType,part>::attach (s_node<nodeType,part> *theWord)
{
	s_node<nodeType,enum gpart> *theNode;
	try {
	if (theWord!=NULL)
		theWord->m_pNext = NULL;
	else
		throw (theWord);
	putLastNode (theNode);
	if (theNode==NULL) {
		m_nBegin = theWord;
		m_nEnd = theWord; }	
	else {
		m_nEnd->m_pNext = theWord;
		m_nEnd = theWord; } }
	catch (s_node<nodeType,grammar::gtype>*)
	{
//	do nothing
	}
}

//	Search a node_list<nodeType> for a prarticular
//	word and return the s_node<nodeType,grammar::gtype> pointer.

template <class nodeType,typename part>
s_node<nodeType,part> *node_list<nodeType,part>::search (nodeType keyword)
{
	s_node<nodeType,part> *result = 0;
	s_node<nodeType,part> *theWord;
	nodeType inText;

	theWord = m_nBegin;
	while (theWord!=0) {	
		inText = theWord->m_pData;
		if (strcmp(keyword,inText)==0) {
			result = theWord;
			break; }
		theWord = theWord->m_pNext; }
	return result;
}

template <class nodeType,typename part>
s_node<nodeType,part> *node_list<nodeType,part>::remove_word (s_node<nodeType,part> *killer)
{
	s_node<nodeType,part> *removed;
	removed = killer->m_pNext;
	killer->m_pNext = removed->m_pNext;
	if (m_nEnd==removed)
		m_nEnd = killer;
	return removed;
}

#if 0
node_list<char*>::node_list<char*> ()
{
	refCount = 1;
	m_nBegin = NULL;
	m_nEnd = NULL;
	m_nPos = NULL;
}
#endif


#if 0
node_list<char*>::node_list<char*> (char *data, bool)
{
	refCount = 1;
	if (data==NULL)
	{
		m_nBegin = NULL;
		m_nEnd = NULL;
		m_nPos = NULL;
	}
	else {
		m_nBegin = new node<char*>;
		m_nEnd = m_nBegin;
		m_nBegin->data() = data;
		m_nBegin->m_pNext = NULL;
		m_nPos = m_nBegin;
	}
}
#endif

#if 0
template <class nodeType,typename part>
node_list<nodeType,part>::~node_list<nodeType,part> ()
{
	nosw_liar<nodeType> *m_nPos = m_nBegin;
	while (m_nPos!=NULL) {
		s_node<nodeType,grammar::gtype> *chomp = m_nPos;
		m_nPos = m_nPos->m_pNext;
		delete chomp; }
}
#endif


#if 0
node<char*> &node_list<char *>::operator >> (char *(&ptr))
{
	ASSERT(false);
// fixme - handle the case where the ptr is NULL
// or we are at the end of the list
	ptr = m_nPos->m_pData;
	m_nPos = m_nPos->m_pNext;
	return (*m_nPos);
}
#endif


//	Straightforward routine to convert a node_list of
//	type <char*> back to a null terminated string.
#if 0
template <class nodeType,typename part>
node<nodeType> &node_list<nodeType,part>::operator>> (char *(&arg))
{
	int created, needed;
	s_node<Type,part> *theWord = m_nBegin;
	nodeType buffer = NULL;
	needed = 0;
	while (theWord!=0) {
		int sizeOfThisPiece = strlen (theWord->m_pData)+1;
		needed = needed+sizeOfThisPiece;
		theWord = theWord->m_pNext;
	}
	buffer = new char [needed+1];
	strcpy (buffer,"");
	theWord = m_nBegin;
	while (theWord!=0) {
		if (theWord->check (theWord->m_pData)!=delimiter)
			strcat (buffer," ");
		strcat (buffer,theWord->m_pData);
		theWord = theWord->m_pNext;
	}
	created = strlen (buffer);
	arg = buffer;
	return (*this);
}
#endif

//	transfer a word from the beginning of
//	another phrase to the end of this
//	phrase


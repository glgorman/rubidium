
#include "stdafx.h"
#include "defines.h"
#include <cstring>
#include <ctype.h>
#include "defines.h"
#include "symbol_table.h"
#include "pstring.h"
#include "btreetype.h"
#include "language.h"
#include "sets.h"
#include "node_list.h"
#include "text_object.h"
#include "key_list.h"

#if 0
bool isNum (char theChar)
{
	if (theChar>='0'&&theChar<='9')
		return true;
	else
		return false;
}
#endif

text_object::text_object ()
{	
	m_code_page = 437;
	m_bEnd = true;
	m_nPos = NULL;
	m_nList.m_nBegin = NULL;
	m_nList.m_nEnd = NULL;
	m_nList.m_nPos = NULL;
	m_sList.m_pData = NULL;
	m_sList.m_pNext = NULL;
}

text_object::text_object (node_list<node_str> *copyFrom)
{
	m_nList = *copyFrom;
	rewind ();
}

//	construct a text_object by sharing a node_list<char*> with the source text
//	object using reference counting	to keep track of the number of text_objects
//	sharing the node_list<char*>.  This is the routine that is called when
//	passing a text object as a parameter to another function.	

text_object::text_object (const text_object &copy)
{
//	wordCount = copy.wordCount;
	m_nList.m_nBegin = copy.m_nList.m_nBegin;
	m_nList.m_nEnd = copy.m_nList.m_nEnd;
	m_nList.m_nPos = copy.m_nList.m_nPos;
	m_nPos = copy.m_nPos;
//	copy.m_sList.refCount++;
//	if (m_nPos==NULL)
	m_bEnd=true;
}

//	Construct a new text_object by
//	tokenizing an input char string

text_object::text_object (char *str)
{
	tokenize(str);
}

void text_object::operator = (node_str str)
{
	node<node_str> copy = str;
	tokenize(copy.m_pData.ascii());
}

node<node_str> *text_object::begin()
{
	node<node_str> *n;
	n = m_nList.m_nBegin;
	return n;
}

node<node_str> *text_object::next()
{
	node<node_str> *n;
	n = m_nList.m_nPos;
	return n;
}

node<node_str> *text_object::end()
{
	node<node_str> *n;
	n = m_nList.m_nEnd;
	return n;
}

//	detach from any node_list<char*> that this
//	is sharing with another text_object
//	and then convert a string to this
//	text_object;

void text_object::tokenize (char *arg)
{
	critical.Lock ();
	node_list<node_str> *n;
	n = new node_list<node_str> (arg);
	detatch ();
	if (arg!=NULL) {
		m_nList.m_nBegin = n->m_nBegin;
		m_nList.m_nPos = n->m_nBegin;
		m_nList.m_nEnd = n->m_nBegin;
		m_nPos = m_nList.m_nBegin;
		if (m_nList.m_nPos!=NULL)
			m_bEnd = false;
		else
			m_bEnd = true;
	}
	else
	{
		m_nList.m_nBegin = NULL;
		m_nList.m_nEnd = NULL;
		m_nList.m_nPos = NULL;
		m_nPos = NULL;
		m_bEnd = true;
	}
	n->m_nBegin = NULL;
	n->m_nEnd = NULL;
	n->m_nPos = NULL;
	delete n;
	
	critical.Unlock ();
}

text_object::text_object (bTreeType<char*> **where)
{
	if (where!=NULL) {
		m_nList.m_nPos = m_nList.m_nBegin;
		m_bEnd = false; }
	else {
//		wordCount = 0;
		m_nList.m_nPos = NULL;
		m_bEnd = true; }
}

text_object::~text_object ()
{
	detatch ();
}

void text_object::detatch ()
{
//	int refCheck;
//	refCheck = m_nList.refCount;
//	if (refCheck==1)
//		delete m_nList;
//	else
	m_nList.refCount--;
	m_nList.m_nBegin = NULL;
	m_nList.m_nPos = NULL;
	m_bEnd = true;
}

text_object &text_object::rewind ()
{
	m_bEnd = true;
	m_nList.m_nPos = m_nList.m_nBegin;
	m_nPos = m_nList.m_nPos;
	if (m_nPos!=NULL)
		m_bEnd = false;
	return *this;
}

//	save the m_nPos; rewind the node_list<char*>
//	and then count the sNodes in the list
//	then restore the m_nPos ...

int text_object::count_words ()
{
	int count = 0;
	char *dummy;
	critical.Lock ();
	node_list<node_str> refTag;
	refTag.m_nPos = m_nList.m_nPos;
	rewind ();
	while (m_bEnd==false) {
		get (dummy);
		count++;
	}
	m_nList.m_nPos = refTag.m_nPos;
	critical.Unlock ();
	return count;
}


//	returns a duplicate node_list<char*>
//	of the node_list<char*> associated with
//	this text_object, does not modify
//	the refCount - since the original
//	node_list<char*> is still attacted to this
//	text_object

node_list<node_str> *text_object::duplicate ()
{
	critical.Lock ();
	node<node_str> *nodeptr1;
	node_list<node_str> *copyList = new node_list<node_str> ();
	if (m_nList.m_nBegin==NULL)
		return copyList;
	nodeptr1 = m_nList.m_nBegin;
	while (nodeptr1!=NULL)
	{
		copyList->append_node (&nodeptr1->data());
		nodeptr1 = &nodeptr1->next();
	}
	critical.Unlock ();
	return copyList;
}

//	constructs a psuedo-copy of a text
//	object by detatching from any list
//	that we are sharing with another
//	text_object and then attaching to
//	the node_list<char*> belonging to another
//	text_object.

text_object &text_object::operator = (text_object &arg)
{
	critical.Lock (); 
	detatch ();
	m_nList.m_nBegin = arg.m_nList.m_nBegin;
	m_nList.m_nEnd = arg.m_nList.m_nEnd;
	m_nList.m_nPos = arg.m_nList.m_nPos;
	arg.m_nList.refCount++;
	critical.Unlock ();
	return *this;
}

void text_object::operator >> (char *(&arg))
{
	critical.Lock ();
	char *result = NULL;
//	(node_list<node_str>::
	m_nList >> result;
	if (m_nList.m_nPos==NULL)
		m_bEnd = true;
	arg = result;
	critical.Unlock ();
}

void text_object::putTempWord (char *(&theWord))
{
	node<node_str> temp;
	temp.next() = *m_nList.m_nPos;
	char *str = (temp).data().ascii();
	if (m_nList.m_nPos==NULL) {
		m_bEnd = true;
		theWord[0]= '\0';
	}
	else if (theWord!=NULL) {
		strcpy (theWord,str);
		m_nList.m_nPos = &temp.next();
	}
	if (m_nList.m_nPos==NULL)
		m_bEnd = true;
}

//	make a duplicate of any node_list<char*>
//	that we are sharing, and then
//	attach to the duplicate and then
//	attach the s_node<char*, grammar::gtype>;

void text_object::append (s_node<char*,grammar::gtype> *theWord)
{
	critical.Lock ();
//	if (m_nList==NULL)
//		m_nList = new node_list<char*>;
//	else if (m_nList.refCount>1) 
	m_nList.refCount--;
	m_nList = *duplicate ();
//	m_nList.attachWord (theWord);
	if (m_nList.m_nPos==NULL)
		m_bEnd = true;
	else
		m_bEnd = false;
	critical.Unlock ();
}

//	appends a node to a linked list.  assumes
//	that if the token is already in the tree that
//	we should leave well enough alone with the
//	pointers!  Else if the token is not in the
//	tree -- we probably want to allocate it since
//	usually that means that we are copying from a
//	temporary buffer somewhere!

void text_object::append (char *str)
{
	if (str==NULL)
		return;

	node<node_str> *nodeptr1, *nodeptr2;
	nodeptr1 = new node<node_str>();
	nodeptr1->data() = strdup(str);
	if (m_nList.m_nBegin==NULL) {
		m_nList.m_nBegin = nodeptr1;
		m_nList.m_nPos = nodeptr1;
	}
	else if (m_nList.m_nEnd!=NULL) {	
		nodeptr2 = m_nList.m_nEnd;
		nodeptr2->m_pNext = nodeptr1;
	}
	m_nList.m_nEnd = nodeptr1;
}

//	attach to a duplicate of this node_list<char*>
//	if necessary and then duplicate the node_list<char*>
//	tbelonging to another text_object, then
//	append the duplicate to this text_object

void text_object::append (text_object &source)
{
	critical.Lock ();
	if (m_nList.refCount>1) {
		m_nList.refCount--;
		m_nList = *duplicate ();
	}
	node_list<node_str> *addOn = source.duplicate ();
#if 0
	m_nList.appendList (addOn);
#endif

	m_nList.refCount = 1;
	if (m_nList.m_nPos==NULL)
		m_nList.m_nPos = m_nList.m_nBegin;
	m_bEnd = false;
	m_nList.m_nEnd->next() = *addOn->m_nBegin;
	m_nList.m_nEnd = addOn->m_nEnd;
	addOn->m_nBegin = NULL;
	addOn->m_nEnd = NULL;
	delete addOn;
	critical.Unlock ();
}

text_object &text_object::transfer_from (text_object &m_pText)
{
	critical.Lock ();
	node<node_str> *mPtr,*nPtr;
	node_list<node_str> &nSource = m_pText.m_nList;
	if (nSource.m_nBegin==NULL)
		return *this;

	mPtr = m_nList.m_nEnd;
	nPtr = nSource.m_nBegin;
	m_nList.m_nEnd = nPtr;
	if (m_nList.m_nBegin==NULL)
	{
		m_nList.m_nBegin=nPtr;
		m_nList.m_nPos=nPtr;
	}
	m_pText.m_nList.m_nBegin = nPtr->m_pNext;
	if (m_pText.m_nList.m_nPos==nPtr)
		m_pText.m_nList.m_nPos=nPtr->m_pNext;
	if (m_pText.m_nList.m_nBegin==nPtr)
		m_pText.m_nList.m_nBegin=nPtr->m_pNext;
	if (m_pText.m_nList.m_nEnd==nPtr)
		m_pText.m_nList.m_nBegin=nPtr->m_pNext;
	
	m_nList.m_nEnd = nPtr;
	nPtr->m_pNext = NULL;
	if (m_pText.m_nList.m_nPos==NULL)
		m_pText.m_nList.m_nPos=m_pText.m_nList.m_nBegin;

	critical.Unlock ();
	return *this;
}

//	shuttles one s_node<char*,grammar::gtype> from one text_object
//	to another.  It can't get much simpler.

void text_object::transfer_to (text_object &m_pText)
{
	m_pText.transfer_from (*this);
}

text_object &text_object::truncate ()
{
	node<node_str> *stop;
	char *dummy;
	node_list<node_str> garbage;
	if (m_nList.refCount>1) {
		m_nList.refCount--;
		m_nList = *duplicate ();
		m_nList.refCount=1;
	}
	stop = m_nList.m_nPos;
	garbage.m_nBegin = stop;
	rewind ();
	while (m_nList.m_nPos!=stop)
		get (dummy);
	m_nList.m_nPos->m_pNext=NULL;
	return *this;
}

void text_object::peek (char *(&result))
{
	node<node_str> *theWord;
	result = NULL;
	if (m_nList.m_nPos==NULL)
		m_bEnd=true;

	else if (m_nList.m_nPos!=NULL) {
		theWord = m_nList.m_nPos;
		result = theWord->data().ascii();
	}
}

//	attempting to read past the end of
//	the line will cause a null pointer
//	to char to be returned!!

void text_object::get (char *(&result))
{
	critical.Lock ();
	result = NULL;
	if (m_nList.m_nPos!=NULL) {
		result = m_nList.m_nPos->data().ascii();
		m_nList.m_nPos = &m_nList.m_nPos->next();
		m_nPos = m_nList.m_nPos;
		if (m_nList.m_nPos==NULL)
			m_bEnd = true; }
	else
		m_bEnd=true;
	critical.Unlock ();
}

char *text_object::get_typed (grammar::gtype &theType)
{
	critical.Lock ();
	char *result = NULL;
	if (m_nList.m_nPos!=NULL) {
//		theType = m_nList.m_nPos->m_typeid;
		result = m_nList.m_nPos->data().ascii();
		m_nList.m_nPos = &m_nList.m_nPos->next();
		if (m_nList.m_nPos==NULL)
			m_bEnd = true; }
	else
		m_bEnd=true;
	critical.Unlock ();
	return result;
}

node<node_str> *text_object::find_penultimate (node_str&found)
{
//	s_node<char*, grammar::gtype> *prevWord;
	node<node_str> *result;
	node<node_str> *prevNode;
	node<node_str> *thisNode;

	char *dummy;
	critical.Lock ();
	try
	{
		if (m_nList.m_nBegin==NULL)
			throw (m_nList);
		rewind ();
		thisNode = m_nList.m_nBegin;
		prevNode = thisNode;
		while (m_nList.m_nPos!=NULL) {
			prevNode = thisNode;
			thisNode = m_nList.m_nPos;
			get (dummy);
		}
		m_nList.m_nPos = prevNode;
		found = m_nList.m_nPos->data();
	}
		catch (node_list<node_str>*) {
			result = NULL;
		}
	critical.Unlock ();
	return m_nList.m_nPos;
}

//	search a text_object for a keyWord, starting
//	from the current position.

bool text_object::find_keyword (key_list &key_words)
{
	critical.Lock ();
	bool foundOne=false;
	int n = 1;
	char *ascii, *theWord;
	text_object result;
	node<node_str> *marker;

//	set a marker at the current position
//	so that we can leave the m_nPos unchanged
//	if a token is not found

	if (m_nList.m_nPos==NULL)
		marker = m_nList.m_nBegin;
	else
		marker = m_nList.m_nPos;
	key_words.rewind ();
	if (key_words.m_bEnd==true)
		return false;
	while (foundOne==false) {
		if (m_bEnd==true)
			break;
		get (ascii);
		key_words.rewind ();
		while (key_words.m_bEnd==false) {	
			key_words.get (theWord);
			if (strcmp (ascii,theWord)==0) {
				foundOne = true;
				break; }				
		}	
	}
	if (foundOne==false)
		m_nList.m_nPos = marker;
	critical.Unlock ();
	return foundOne;
}

//static_cast<s_node<char*,gtype>&>()

text_object text_object::get_sentnce ()
{
//	initialization code

	critical.Lock ();
	text_object result;
	s_node<node_str,grammar::gtype> *sourceTail, *resultTail, *theRipped;
	sourceTail = NULL;
	resultTail = NULL;
	theRipped = NULL;

	s_node<node_str,grammar::gtype> *s_list = (s_node<node_str,grammar::gtype>*)&result.m_sList;
	bool foundOne=false;

	try {

//	First find a delimiter indicating that
//	the next token is part of a new sentence

		find_type (grammar::delimiter);
		sourceTail = s_list;

//	Have reached the endOfText, so bail out.
//	because retreval of the next sentence fails
//	without a terminating delimiter.

		if (sourceTail==NULL)
			throw sourceTail;

//	Now get the first token in the next sentence
//	i.e. the token following the delimiter.
//	If there is no following token, then that must
//	mean that we have reached the end of text.

		theRipped = sourceTail->m_pNext;
		if (theRipped==NULL)
			throw theRipped;

//	Start with the first token after the delimiter
//	and then find the delimiter that indicates the
//	end of the sentence to be extracted.

		s_list->m_pData = theRipped->m_pData;
		s_list->m_typeid = theRipped->m_typeid;
		s_list->m_pNext = theRipped->m_pNext;

		result.rewind ();
	//	resultTail = result.find_type ((gtype)delimiter);

//	Relink the source text to the sentence following
//	the last token of the extracted sentence.
//	FIXME - possibly broken in this templated version:

		if (resultTail!=NULL)
		{	
			s_node<node_str,grammar::gtype> *tail_ptr;
			tail_ptr = resultTail->m_pNext;
			sourceTail->m_pNext = (s_node<node_str,grammar::gtype>*)m_sList.m_pNext;
			resultTail->m_pNext=NULL;								
		}
		else {
			sourceTail->m_pNext = NULL;
		}
	}
//	Failed to extract a sentence, so clean up
//	and exit.
	catch (s_node<node_str,grammar::gtype>*)
	{
		result.rewind ();
		m_bEnd = true;
	}
	critical.Unlock ();
	return result;
}

//	advance the m_nPos to the next
//	occurance of the desired type

//template<class Type, typename enum part>
//s_node<char*, enum gtype> find_type (enum gtype whatType);
void text_object::find_type (grammar::gtype theType)
{
	grammar::gtype whatType;
	node<node_str> *ptr;
	s_node<node_str,grammar::gtype> s;
	while  (m_nList.m_nPos!=NULL) {
		ASSERT(false);
// fixme - we are searching the nList - but
// want to return a matching sItem???
		whatType = s.m_typeid;
		if (whatType==theType) {
			s.m_pData = m_nList.m_nPos->data();
			break;
		}
		// be very afraid
	//	ptr = m_nList.next();
	//	m_nList.m_nPos = ptr;
	}
	if (m_nList.m_nPos==NULL)
		m_bEnd = true;
	else
		m_bEnd = false;
}

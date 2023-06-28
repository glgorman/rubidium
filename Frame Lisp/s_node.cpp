#include "stdafx.h"
#include "btreetype.h"
#include "language.h"
#include "node_list.h"

//template <char *str,typename part>
#if 0
s_node<char*>::s_node<char*> (char *data)
{
	refCount = 1;
	m_pNext = NULL;
	m_pData = data;
}
#endif

//template <char *str,typename part>
s_node<char*>::s_node<char*> ()
{
	refCount = 1;
	m_pNext = NULL;
	m_pData = NULL;
}
#if 0
s_node<char*>::~s_node<char*> ()
{
// todo - notify the symbol table
// manager that we no longer need
// this char
}
#endif

#if 0
template <class nodeType, class enumType>
s_node<nodeType,enumType>::~s_node (void)
{
#ifdef debug
	delete m_tagid;
#endif
};
#endif

//	Creates a generic s_node object for use by functions that want to
//	fill in values a temporary object
#if 0
template <class nodeType, class enumType>
s_node<nodeType,enumType>::s_node ()
{
#ifdef debug
	m_tagid = new char [16];
	strcpy (m_tagid,"s_node");
#endif
	m_pData = NULL;
	m_typeid = unknown1;
};
#endif

//	the char* implementation of this createsa linked list node
//  that contains a dictionary pointer -- i.e. a pointer to a
//	shared char* stored in a binary tree.  Checks to see if the
//	token in in the tree before calling ... 
#if 0
template <class nodeType, typename part>
s_node<nodeType,part>::s_node (nodeType arg)
{
#ifdef debug
	m_tagid = new char [16];
	strcpy (m_tagid,"s_node");
#endif
	char *buffer;
	key_word temp;
	bTreeType <nodeType> *theTwig;
	try {
		theTwig = m_root->findNode (arg);
		if (compare(theTwig->m_pData,arg)!=0) {
			buffer = new char [strlen(arg)+1];
			strcpy (buffer,arg); }
		else
			buffer = arg; 
		}
	catch (...) { 
		if (theTwig->m_pData==NULL) {
			buffer = new char [strlen(arg)+1];
			strcpy (buffer,arg); } }
	theTwig = theTwig->getNode (buffer);
	m_pData = theTwig->m_pData;
	m_typeid = temp.identify (arg);
};
#endif

//	Looks weird and redundant here, but the more
//	obvious way was allowing the assignment of the
//	enum type to corrupt the leaf pointer!

#if 0
template <class nodeType, class enumType>
enumType s_node<nodeType,enumType>::check (const nodeType &arg)
{
	key_word temp;
	enumType theKind = temp.identify (arg);
	return theKind;
};
#endif


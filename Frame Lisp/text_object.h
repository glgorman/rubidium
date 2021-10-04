
#include "stdafx.h"
#include "afxmt.h"

class key_list;

class text_object
{
	CCriticalSection critical;

public:
	UINT	m_code_page;
	node_list<char*> m_nPos;
	node_list<char*> m_nList;
	s_node<char*, language> m_sList;
	bool m_bEnd;

#if 0
	node_list<char *> *m_nList;
#endif

public:
	text_object ();
	text_object (char *m_pText);
	text_object (text_object &copy);
	text_object (node_list<char *> *copyFrom);
	text_object (bTreeType<char*> **source);
	~text_object ();

	text_object &operator = (text_object &arg);
	void operator = (char *arg);
	void operator >> (char *(&));

	void append (char *aWord);
	void append (s_node<char*, enum language> *theWord);
	void append (text_object &source);

	void peek (char *(&));
	void getIndexWord (char *(&));
	char *getIndexWord (language &theType);
	
	node<char*> *findPenultimate (char *(&));
	s_node<char*, enum language> findType (language whatType);

	void markLocation ();
	void rewind ();
	void rewind (s_node<char*, enum language> *location);

	text_object getSentence ();
	bool findKeyWord (key_list &keyWords);
	void countWords (int &count);

	void transferFrom (text_object &m_pText);
	void transferTo (text_object &m_pText);	
	void truncate ();

private:
	node_list<char *> *duplicate ();
	void detatch ();
	void putTempWord (char *theWord);
	s_node<char*, enum language> *removeWord (s_node<char*, enum language> *killer);
	s_node<char*, enum language> *search (char *keyword);
};

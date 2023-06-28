
#include "stdafx.h"
#include "afxmt.h"

class key_list;

class text_object
{
protected:
	bool m_bEnd;
	CCriticalSection critical;
	UINT	m_code_page;
	
public:
	node<node_str> *m_nPos;
	node_list<node_str> m_nList;

	text_object ();
	text_object (char *m_pText);
	text_object (node_list<node_str> *copyFrom);
	text_object (bTreeType<char*> **source);
	text_object (const text_object &copy);
	~text_object ();
	int count_words ();

	bool end_of_text () { return m_bEnd; };
	bool find_keyword (key_list &keyWords);

	s_node<node_str,grammar::gtype> m_sList;
	node<node_str> *begin();
	node<node_str> *next();
	node<node_str> *end();
	node<node_str> *find_penultimate (node_str&);

	void append (char *);
	void append (node_str);
	void append (s_node<char*,grammar::gtype> *theWord);
	void append (text_object &source);
	void find_type (grammar::gtype whatType);
	char *get_typed (grammar::gtype &theType);
	void get (char *(&));
	void peek (char *(&));
	void operator >> (char *(&));
	void operator = (node_str);
	text_object &rewind ();	
	text_object &truncate ();
	text_object &operator = (text_object &arg);
	text_object &transfer_from (text_object &m_pText);
	text_object get_sentnce ();
	
protected:
#if 0
	void get_index_word (char *(&));
#endif	
//	s_node<Type,enum part> find_type (enum part theType);	
	void markLocation ();
	void rewind (s_node<node_str,grammar::gtype> *location);
	void transfer_to (text_object &m_pText);	
	
private:
	node_list<node_str> *duplicate ();
	void tokenize (char *);
	void detatch ();
	void putTempWord (char *(&));
	void putTempWord (node_str);
	s_node<node_str,grammar::gtype> *remove_word (s_node<node_str,grammar::gtype> *killer);
	s_node<node_str,grammar::gtype> *search (node_str);
};

class key_list: public text_object
{
public:
	key_list ();
	key_list (char *m_pText);
	key_list (node_list<node_str> *m_nList);
	key_list (text_object &m_nList);
	void rewind();
};


// node_list.h
// types declared in this file.
// node is a raw node of any "Type" which acts as a container
// for the data type, which is course of type "Type"
// and for which also povides a pointer to the next node.
// s_node is a "symbolic node" which also contains a data
// value and a next pointer, but which also can contain a type
// identifier, in the form of an int or any other specified
// ordinal (or other) value).

template <class nodeType> class node;
template <class nodeType, typename part=int> class s_node;
template <class Type, typename part=bool> class node_list;

typedef enum source_type
{
	unknown2,
	whitespace,
	delimit,
	command,
	reserved_lisp,
	reserved_fortran,
	reserved_basic,
	reserved_c,
	reserved_pascal,
	reserved_perl,
	reserved_python,
	reserved_spin,
};

typedef enum parse
{
	unknown3,
	comment,
	numeric,
	string,
	function,
	procedure,
	expression,
	block,
};

class tstring
{
protected:
	char str[1];
	void *operator new (size_t,void*);	
	tstring () { };

public:
	static int total;
	static tstring *allocate(size_t);
	operator char*() { return &str[0]; }
	tstring &operator = (char*);
	char &operator [] (int i);
};

class node_str
{
friend class text_object;
friend class mStream;
protected:
	char *m_str;
	int	m_size;

public:
	node_str () { m_str = NULL; m_size = 0; }
	node_str (char *str)
	{
		m_size = strlen (str)+1;
//		m_str = new char [m_size+1];
		m_str= (char*) tstring::allocate (m_size);
		strcpy_s (m_str,m_size,str);
	}
	~node_str ()
	{
//		delete m_str;
	}
	char *ascii() { return m_str; }
	node_str &operator = (char *str)
	{
		if (m_str!=NULL)
			delete m_str;
		if (str!=NULL) {
			m_size = strlen (str)+1;
//			m_str = new char [m_size];
			m_str =(char*) tstring::allocate (m_size);
			strcpy_s (m_str,m_size,str);
		}
		else
			m_size=0;
		return (*this);
	}
	int size () { return m_size; }
	bool operator == (node_str &str)
	{
		int test;
		bool result;
		if (m_str==NULL)
			return false;
		if (str.m_str==NULL)
			return false;
		test = strcmp(m_str,str.m_str);
		if (test==0)
			result=true;
		else
			result=false;
		return result;
	}
};

template <class nodeType>
class node
{
friend class node_list<nodeType>;
friend class text_object;

protected:
	nodeType m_pData;
	node *m_pNext;
	int	refCount;

public:
	node<nodeType> () {
		refCount = 1;
		m_pNext = NULL;
		m_pData = NULL; }

	node<nodeType> (nodeType n) {
		refCount = 1;
		m_pNext = NULL;
		m_pData = n; }

	~node<nodeType> () {};
	void terminate() { m_pNext=NULL; }
	nodeType &data() { return *(&m_pData); };
	node<nodeType> &next() { return *(m_pNext); };
	// allow assignment of the value of an s_node,
	// while ignoring not the type attribute
	node<nodeType> &operator = (s_node<nodeType,grammar::gtype>);
	bool operator == (node<nodeType>&);
	bool operator != (s_node<nodeType,grammar::gtype>&);
};

template <class nodeType, typename part>
class s_node
{
protected:
	int	refCount;

public:
	part m_typeid;
	s_node *m_pNext;
	nodeType m_pData;
	s_node ()
	{
		m_pData = NULL;
		m_pNext = NULL;
		m_typeid = grammar::unknown;
		refCount = 1;
	};
	~s_node () {};
	s_node (char *str,grammar::gtype t = grammar::unknown1)
	{
		m_pData = strdup(str);
		m_pNext = NULL;
		m_typeid = t;
		refCount = 1;
	};
	s_node (node_str &data,grammar::gtype t = grammar::unknown1)
	{
		m_pData = data.ascii();
		m_pNext = NULL;
		refCount = 1;
		m_typeid = grammar::unknown1;
	}
#if 0
	s_node (nodeType &data)
	{
		refCount = 1;
		m_pNext = NULL;
		m_pData = data;
	}
#endif
	// { ASSERT(false); }
	// convert a node<nodeType> to an s_node;
	// s_node (nodeType,part) { ASSERT(false); }
	s_node (node<nodeType> &NN)
	{
		data() = NN.data();
	}
	part check (char *(&arg));
	nodeType &data() { return *(&m_pData); };
	node<nodeType> &next() { return *(m_pNext); };

	// functions that operate on the sList
	void attach (s_node<nodeType,part> *theWord);
	void put_last (s_node<nodeType,part> *(&theWord));
	void put_next (s_node<nodeType,part> *(&theWord));
	
	s_node<nodeType,grammar::gtype> &operator = (node_list<nodeType>);
	s_node<nodeType,part> *remove_word (s_node<nodeType,part> *);
	s_node<nodeType,part> *search (nodeType);
};

template <class Type, typename part>
class node_list
{
friend class key_list;
friend class text_object;

protected:
	void convert_ascii (char* str);

public:
	volatile int refCount;
	node<Type> *m_nPos;
	node<Type> *m_nBegin;
	node<Type> *m_nEnd;

public:
	node_list (char*input) { convert_ascii (input); }
	node_list (bTreeType<Type> **source);
	node_list (node_str nstr,bool) { ASSERT(false); }
//	node_list (node_str nstr) { ASSERT(false); }
	node_list (s_node<Type,grammar::gtype>,bool) { ASSERT(false); }
	node_list ()
	{
		refCount=1;
		m_nBegin=NULL;
		m_nEnd=NULL;
		m_nPos=NULL;
	};
//	node_list (Type m_pText,bool);
	~node_list () { /*FIXME*/ };

	unsigned int size ();
	node<Type> *begin () { return m_nBegin; };
	node<Type> *end () { return m_nEnd; };
	node<Type> *next () { return m_nPos; };
	node<Type> *pop_front ();
	node_list<Type,part> &concat(node<Type>&);
	node_list<Type,part> &concat_s (s_node<node_str,grammar::gtype>*);
	void push_back (node_list &);

	node<Type> &operator >> (char*(&));
//	node<Type> &operator >> (Type&);
	
	Type &operator -> ();

	void putTempWord (char *theWord);
	void append_node (Type *);
	void append_node (char *);

	void append_list (node_list*);
	void transfer_from (node_list*);
	void transferTo (node_list*);

//////////////////////////////////////////////////////////////////////
//
//	FIXME: these are here because eventually I want to implement a
//	a "sequence" and "sequence"	chain classes, which allows
//	concatenation of lists, vectors, trees, etc, 
//	
	node_list<Type> &operator = (s_node<Type,grammar::gtype>);
	void put_last (s_node<Type,part> *(&theWord));
	void attach (s_node<Type,part> *theWord);
	s_node<Type,part> *search (Type keyword);
	s_node<Type,part> *remove_word (s_node<Type,part> *);
	
	Type put_next (s_node<Type,part> *(&theWord));	
};

template <class nodeType>
bool node<nodeType>::operator == (node<nodeType> &arg)
{
	bool result;
	node_str &data1 = data();  
	node_str &data2 = arg.data();
	if (data1==data2)
		result=true;
	else
		result=false;
	return result;
}

template <class nodeType>
bool node<nodeType>::operator != (s_node<nodeType,grammar::gtype> &arg)
{
	bool result;
	node_str &data1 = data();  
	node_str &data2 = arg.data();
	if (data1==data2)
		result=false;
	else
		result=true;
	return result;
}

template<class Type,typename part>
part s_node<Type,part>::check (char *(&arg))
{
	grammar::gtype what;
	what = grammar::whatIsThis (arg);
	return what;
};

template <class nodeType,typename part>
void node_list<nodeType,part>::append_node (char *str)
{
	node<nodeType> nodeptr;
	char *symbol;
	if (str!=NULL) if (str [0]!=0) {
		symbol = strdup(str);
		nodeptr.data() = symbol;
		concat (nodeptr);
	}
}

template <class nodeType,typename part>
node_list<nodeType,part> &node_list<nodeType,part>::concat(node<nodeType> &N0)
{
	node<nodeType> *N1;
	node<nodeType> *N2 = new node<nodeType>;
	N2->data() = N0.data();

	if (m_nBegin==NULL) {
		m_nBegin = N2;
		m_nEnd = N2;
		m_nPos = N2;
	}
	else {
		N1 = m_nEnd;
		N1->m_pNext = N2;
		m_nEnd = N2;
		N2->m_pNext = NULL;
	}
	return *this;
}

// concatenate the contents of an s_node to this node_list
// the type information is ignored.

template <class nodeType,typename part>
node_list<nodeType,part> &node_list<nodeType,part>::concat_s (s_node<node_str,grammar::gtype>*S0)
{
//	if (S0==NULL)
//		return;
	node<nodeType> *N0 = new node<nodeType>;
	N0->data() = S0->data();

	node<nodeType> *position = end();
	if (position!=NULL) {
		position->m_pNext = N0;
		m_nEnd = N0;
	}
	return *this;
}

template <class nodeType,typename part>
void node_list<nodeType,part>::append_node (nodeType *NN)
{
	if (NN==NULL)
		return;

	node<nodeType> *N1 = new node<nodeType>;
	N1->data() = *NN;

	node<nodeType> *position = end();
	if (begin()==NULL) {
		m_nBegin = N1;
		m_nEnd = N1;
		m_nPos = N1;
	}
	else if (position!=NULL) {
		position->m_pNext = N1;
		m_nEnd = N1;
	}
}

//	append a node_list<nodeType> to another node_list<nodeType>
//	object by confiscating the other
//	node_list<nodeType>

template <class nodeType,typename part>
void node_list<nodeType,part>::append_list (node_list<nodeType,part> *m_nList)
{
//	s_node<nodeType,grammar::gtype> *position;
//	putLastNode (position);
	node<nodeType> *position = end();
	if ((position!=NULL)&&(m_nList!=NULL)) {
		position->m_pNext = m_nList->m_nBegin;
		m_nEnd = position->m_pNext;
		m_nList->m_nBegin = NULL;
		m_nEnd = m_nList->m_nEnd; }
	else if (m_nList!=NULL) {
		m_nBegin = m_nList->m_nBegin;
		m_nList->m_nBegin = NULL;
		m_nEnd = m_nList->m_nEnd; }
}

template<class Type,typename part>
node<Type> &node_list<Type,part>::operator >> (char *(&ptr))
{
	size_t	sz1;
	bool add_separator = false;
	int created, needed;
	node<node_str> *nodeptr = m_nBegin;
	char *buffer = NULL;
	char *separator = " ";
	needed = 0;
	while (nodeptr!=NULL) {
		sz1 = strlen (nodeptr->data().ascii());
		if (add_separator==true)
			sz1++;
		needed = needed+sz1;
		nodeptr = &nodeptr->next();
	}
	buffer = new char [needed+1];
	strcpy (buffer,"");
	nodeptr = m_nBegin;
	while (nodeptr!=0) {
//		if (nodeptr->check (nodeptr->m_pData)!=delimiter)
		strcat_s (buffer,needed+1,nodeptr->data().ascii());		
		nodeptr = &nodeptr->next();
		if (add_separator==true)
			strcat (buffer,separator);
	}
	created = strlen (buffer);
	ptr = buffer;
	return *m_nBegin;
}

template <class nodeType,typename part>
void node_list<nodeType,part>::convert_ascii (char* str)
{
	bool ignore_whitespace = false;
	refCount = 1;
	m_nBegin = NULL;
	m_nEnd = NULL;
	int length, start, position;
	start = 0;
	position = 0;
	char* buffer = NULL;
	if (str!=NULL) {
		length = strlen (str);
		buffer = new char [length+1];
	}
	else return;
	char c = str [start];
	while (c!=0)
	{
		int n = 0;
		position = start;
		c = str[position];

//	skip leading spaces, linefeeds, tabs
//	and returns;
		if (ignore_whitespace==true)
		{
			while (chartypes::whitespace.in(c))
			{
				position++;
				c = str[position];
			}
			start = position;
		}
		else if (chartypes::whitespace.in(c))
		do {
			buffer [n++] = c;
			position++;
			c = str[position];
		}	
		while (chartypes::whitespace.in(c));

//	tokenize if alphanumeric	
		else if (chartypes::alpha.in(c))
		do {
			buffer [n++] = c;
			position++;
			c = str[position];
		}	
		while (chartypes::ident.in(c));

//	else tokenize numeric
		else if (chartypes::digits.in(c)) do {
			buffer [n++] = c;
			position++;
			c = str[position];
		}
		while (chartypes::digits.in(c));

//	else any other printing chars
		else if (str[position]!=0) do {
			buffer [n++] = c;
			position++;
			c = str[position];
		}
		while ((c!=0)&&(!chartypes::ident.in(c))
			&&(!chartypes::whitespace.in(c)));

		buffer [n]=0;
		if (*buffer!=0)
			append_node (buffer);
		start = position;
		c = str[position];
	}
	delete [] buffer;
}


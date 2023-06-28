
class clause;
class text_object;
class TelnetServer;
template <class X> class bTreeType;
template <class nodeType, class enumType> class s_node;
template <class nodeType, typename part=int> class node_list;

template <class nodeType>
class comPipe
{
	node_list<node_str> buffer;

public:
	bool ready;
	CCriticalSection critical;
	comPipe ();// { ASSERT(false); }
	~comPipe ();// { ASSERT(false); };
	comPipe<nodeType> &operator << (char *str);
	comPipe &operator << (nodeType);
	void operator >> (nodeType);
};

template <class nodeType>
class debugTest
{
public:
	void openDebugFiles ();
	void closeDebugFiles ();
	
	void mapTree (bTreeType<nodeType>*);
	void traversalTest (bTreeType<nodeType>*);
	void spewMarkovian (bTreeType<nodeType>**);
	clause *extractionTest ();
};

template <class nodeType>
class s_node_pipe: private CCriticalSection
{
protected:
	node_list<s_node<node_str,grammar::gtype>> theFifo;
	s_node<nodeType,grammar::gtype> *extract ();
	int wordCount;

public:
	bool ready;
	s_node_pipe ()
	{	// the fifo should be auto initialized by its
		// own constructor.  Nothing else to see here
		// these arent the droids you are looking for ...
		wordCount=0;
		ready = false;
	}	
	s_node_pipe &operator << (s_node<nodeType,grammar::gtype>);
	s_node_pipe &operator << (text_object &);
	s_node_pipe &operator << (char* str)
	{
		node_str temp = node_str(str);
		s_node<node_str,grammar::gtype> *result;
		result =  new s_node<node_str,grammar::gtype>(temp);
		theFifo.concat_s (result);
		return *this;
	}
	s_node_pipe &operator >> (s_node<nodeType,grammar::gtype> &aNode);
};

template <class nodeType>
comPipe<nodeType> &comPipe<nodeType>::operator << (char *NN)
{
	node<nodeType> *input;
	input = new node<nodeType> (NN);

	try {
		if (input->data().ascii()==NULL)
			throw input;
		critical.Lock ();
		buffer.concat (*input);
		ready = true;
		critical.Unlock ();
	}
	catch (...) {
		ready = false;
		critical.Unlock ();
	}
	delete input;
	return *this;
}

namespace console
{
	extern TelnetServer Remote;
	extern comPipe<node_str> chatOut;
	s_node_pipe<node_str> chatIn;
	s_node_pipe<char*> consoleInPipe;
	s_node_pipe<node_str> contestInPipe;
	s_node_pipe<node_str> processPipe;

	void put_word (char *ascii,int wordCount);
	void put_text (text_object &theText);
	UINT get_line (LPVOID param);
};

template <class nodeType>
void comPipe<nodeType>::operator >> (nodeType output)
{
	while (ready==false)
		Sleep (50);
	critical.Lock ();
	output = buffer,pop_front();
	critical.Unlock ();
}

template <class nodeType>
comPipe<nodeType> &comPipe<nodeType>::operator << (nodeType NN)
{
	node<nodeType> *lastWord, *theNode;
	theNode = new node<nodeType> (NN);
	theNode->nextNode = NULL;
	try {
	if (aNode.m_pData==NULL)
		throw aNode;
	Lock ();
	if (theFifo.firstWord==NULL)
		theFifo.firstWord = theNode;
	else {
		lastWord = theFifo.lastWord;
		lastWord->nextNode = theNode; }
	theFifo.lastWord = theNode; 
	ready = true;
	Unlock ();
	}
	catch (s_node<nodeType,gtype>) {
		delete theNode;
	}
	return *this;
}

//	wait until there is an s_node available and then
//	put a copy of that s_node into the values of the
//	s_node provided by the caller, deleting the s_node
//	extracted from the stream.

template <class nodeType>
s_node_pipe<nodeType> &s_node_pipe<nodeType>::operator >> (s_node<nodeType,grammar::gtype> &output)
{
	s_node<nodeType,grammar::gtype> *S0;
	s_node<nodeType,grammar::gtype> temp;
	S0 = NULL;
	if (theFifo.begin()==NULL)
	{
		theFifo.m_nEnd=NULL;
		output.m_pData = NULL;
		output.m_pNext = NULL;
		ready = false;
	}
	while (ready==false)
		Sleep (50);

	Lock ();
	if (theFifo.begin()!=NULL)
		theFifo.begin();

	output=temp;
	Unlock ();
	return *this;
}

template <class nodeType>
s_node_pipe<nodeType> &s_node_pipe<nodeType>::operator << (text_object &text)
{
	nodeType ascii;
	Lock ();
	text.rewind ();
	while (text.endOfLine==false) {
		text.get (ascii);
		*this << ascii; }
	Unlock ();
	return *this;
}

template <class nodeType>
s_node_pipe<nodeType> &s_node_pipe<nodeType>::operator << (s_node<nodeType,grammar::gtype> aNode)
{
	s_node<nodeType,grammar::gtype> *lastWord, *theNode;
	theNode = new s_node<nodeType,grammar::gtype> (aNode);
	theNode->nextNode = NULL;
	try {
	if (aNode.m_pData==NULL)
		throw aNode;
	Lock ();
	if (theFifo.firstWord==NULL)
		theFifo.firstWord = theNode;
	else {
		lastWord = theFifo.lastWord;
		lastWord->nextNode = theNode; }
	theFifo.lastWord = theNode; 
	ready = true;
	Unlock ();
	}
	catch (s_node<nodeType,gtype>) {
		delete theNode;
	}
	return *this;
}


//	private call to extract a single s_node from the
//	the beginning of a FIFO.

template <class nodeType>
s_node<nodeType,grammar::gtype> *s_node_pipe<nodeType>::extract ()
{
	s_node<nodeType,grammar::gtype> *theNode,*nextNode;
	theNode = theFifo.firstWord;
	nextNode = theNode->nextNode;
	theFifo.firstWord = nextNode;
	theNode->nextNode = NULL;
	return theNode;
}




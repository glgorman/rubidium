
#include "stdafx.h"
#include "afxmt.h"
#include <iostream>
#include "defines.h"
#include "symbol_table.h"
#include "btreetype.h"
#include "language.h"
#include "node_list.h"
#include "text_object.h"
#include "scripts.h"
#include "frames.h"
#include "extras.h"

using namespace std;

bTreeType<char*> *m_root;
extern frame theFrame;
frame *lframe::m_pFrame = NULL;
#define HAS_CONSOLE		(YES)

#if 0
void tstring::create_sandbox()
{
	void *ptr = NULL;
	int size, new_size;
	size = frame::mem_pool.size();
	new_size = size+1;
	frame::mem_pool.resize(new_size);
	ptr = VirtualAlloc (NULL,1048576,MEM_READ_WRITE);
	frame_::mem_pool[size] = ptr;
}
#endif

tstring *tstring::allocate (size_t sz)
{
	//	round up to next 8 byte boundary
	int str_size = ((sz+7)>>3)<<3;
	char *ptr1, *ptr2;
	tstring *ptr3 = NULL;
	ptr1 = (char*) frame::mem_pool;
	ptr2 = ptr1+total;
	total = total+str_size;
	ptr3 = (tstring*) new (ptr2) tstring;
	return ptr3;
}

void *tstring::operator new (size_t sz,void *ptr)
{
	void *ptr1;
	if (ptr==NULL)
		ptr1 = malloc(sz);
	else
		ptr1 = ptr;
	return ptr;
}

token::token ()
{
	ascii = NULL;
	m_count = 0;
}

token::token (char *str, int count)
{
	int str_size = strlen(str)+1;
	do {	
		ascii  = (char*) tstring::allocate(str_size);		
		if (ascii==NULL)
			ASSERT(false);
	}
	while (ascii==NULL);
	strcpy_s (ascii,str_size,str);
	m_count = count;
}

void lframe::set_global ()
{
	m_pFrame = &theFrame;
}

void lframe::load_binary_file (char *fname)
{
	CString msg;
	msg.Format(_T("The Unicorn is in the Garden!%s"),fname);
	AfxMessageBox(msg,IDOK);
}

#define threadRipText

//	Simple way to create a b-tree. Take the words in order of
//	most-frequent to least frequent	usage and sort them by
//	frequency of use and then alphabetic position into a tree.
//	Spools pointers from a sorted array of to the get node
//	function.  If getNode returns a different pointer -- then
//	it means that the node already exists in the tree, so the
//	pointer in the list that we are converting is redirected
//	to the duplicate object and the orininal is destroyed.
#if 0
int compare (char *a, char *b)
{
	return strcmp (a,b);
}
#endif

template<class nodeType>
bTreeType<nodeType>::bTreeType  (script *theScript)
{
	size_t size;
	char *str = NULL;
	theScript->m_index.owner = false;
	m_pData = NULL;	
	bTreeType<nodeType> *theNode;
	int wordNum, needed = theScript->m_index.size();	
	root = markov = branch1 = branch2 = NULL;
	size = theScript->m_index._symbols.size();

	if (size==0)
		return;
//	loading symbols our the common index string pool
//	that was created with VirutalAlloc, so no need
//	to call new or malloc here:
	str = theScript->m_index._symbols [0].ascii;
	if (str!=NULL) {
		m_pData = str;
		str = NULL;
//		DEBUG_STR << "adding node \"" << aNode << "\"\n";
	}

	for (wordNum=1;wordNum<size;wordNum++)
	{
		str = theScript->m_index._symbols[wordNum].ascii;
#ifdef trace_additions
		DEBUG_STR << "adding node \"" << aNode << "\"\n";
#endif
		theNode = get_node (str);

#if 0
		if (str!=theNode->m_pData)
		{
			ASSERT(false);
			theScript->m_index._symbols[wordNum].ascii = theNode->m_pData; 
		}
#endif
	}
//	Sanity test on tree to see if every token was added!
	for (wordNum=1;wordNum<size;wordNum++) {
		str = theScript->m_index._symbols[wordNum].ascii;
		theNode = find_node (str);
		if (compare(str,theNode->m_pData)!=0)
		{
#ifdef HAS_CONSOLE
//			DEBUG_STR << "Could not find: " << str << "\n";
#endif
		}
	}
}

//	-------------------------------------------------------
//
//	Methods for creation and major operations with frames
//
//	--------------------------------------------------------

frame::frame ()
{
	::m_root = NULL;
	m_root = NULL;
	mem_pool = NULL;
	content.clear ();
	available = 0;
	count = 0;
	button = false;
}

frame::~frame ()
{
	int j = 0;
	delete m_root;
	m_root = NULL;
//	if (context!=NULL)	
#if 0
	if (content.size()>0) {std::
		iter = content.begin ();
		end = content.end ();
		for (;iter<end;iter++)
			delete (*iter);
	}
	delete scripts;
	delete common;
#endif
//	delete [] context;
}

void frame::load_file_images (bool trace)
{
	char theFile[64];
	std::vector<token>::iterator iter, end;
	char suffix [] = ".txt";
	UINT fail;
	int how_many = scripts.m_index.size();
#ifdef HAS_CONSOLE
	DEBUG_STR << "Loading " << how_many << " file image(s)\n";
#endif

	if (how_many==0)
		return;

	iter = scripts.m_index._symbols.begin();
	end = scripts.m_index._symbols.end ();
	int j=1;
	for (iter++;iter<end;iter++,j++)
	{
		strcpy (theFile, scripts.m_index [j].ascii);
		script *s = content[j];
#ifdef HAS_CONSOLE
		DEBUG_STR << "Preparing to load: " << s->get_file_name() << "\n";
#endif
		fail = load_file_image (j,theFile,suffix);
	}
}

// todo - make this a static member of frame?

UINT load_file_image (LPVOID param);

UINT frame::load_file_image (int j, char *fileName, char *suffix)
{
	bool endOfFile = false;
	int wordCount = 0;
	script *theScript = content [j];
	bool fileOpen = theScript->open_file ();
	if (fileOpen==true)
	{
		LPVOID param = (void*)(theScript);
#ifdef threadImageLoads
		AfxBeginThread (::loadFileImage,param,1);
#else
		::load_file_image (param);
#endif		
	}
	else {
		wordCount = -1;
//		waitForConsole
#ifdef HAS_CONSOLE
		DEBUG_STR << "Failed to open: " << fileName << "\n";
#endif
//		releaseConsole
	}
	return wordCount;
}

UINT load_file_image (LPVOID param)
{
	int wordCount;
	script *theScript = (script*)(param);
//	char *fileName = theScript->m_file_name;
	while (theScript->merged==false)
	{
#ifdef HAS_CONSOLE
		DEBUG_STR << "File Not Merged: " << theScript->get_file_name() << "\n";
#endif
		Sleep (1000);
	}
//	waitForConsole;
//	releaseConsole;
	theScript->load_file_image (NULL);
	wordCount = theScript->theImage.count_words ();
	theScript->close_output_file ();
//	waitForConsole
#ifdef HAS_CONSOLE
	DEBUG_STR << "Read " << (int) wordCount;
	DEBUG_STR << " words from " << theScript->get_file_name() << "\n\n";
#endif
//	releaseConsole
	return wordCount;
}

void frame::build_event_list ()
{
	int howMany;
	lyric_element ly;
	bool result;

	result = scripts.open_file ();
	if (result==true) {
//		scripts.construct_symbol_table (true);
		scripts.load_file_image (NULL);
		scripts.close_input_file ();
//		scripts->killDelimiters ();
		howMany = scripts.m_index.size();
//		scripts->viewIndex (howMany);
//		scripts.save_index ();
		text_object *image = &scripts.theImage;
		
		m_events.m_text.clear();
		m_events.m_timestamp.clear();
		
		int i=0;
		bool busy = true;
		UINT ts;
		while (busy)
		{
			ts = i*100;
			ly.timestamp  = ts;
			ly.phrase = "Hello World";

			m_events.m_text.push_back (ly.phrase);
			m_events.m_timestamp.push_back (ts);
			i++;
			
			if (i>100)
				break;
		}
	}
}

text_object &frame::operator [] (int contextNum)
{
	script *the_script;
	text_object *theImage;
	if ((contextNum<count)&&(contextNum>=0)) {
		the_script = content [contextNum];
		theImage = &the_script->theImage;
	}
	else theImage = NULL;
	return *theImage;
}

void frame::buildIndices (int theOrder)
{
	bool prompt = false;
	build_symbol_tables (false);
	masterIndex ("common");
}

// this method uses a hack to add the items
// to the index, by tricking the indexing
// system into placing the symbmols into the
// table in reverse order.  FIX ME!
// add some kind of parameter which specifies
// operartor precedence, or some other ordering
// scheme.

symbol_table *frame::cons (char **tokens)
{
	symbol_table *s = new symbol_table;
	int i,j,k;
	i = j = k = 0;
	s->m_allocate = false;
	s->_symbols.reserve (40);
	while (tokens[i]!=NULL)
		i++;

	// i now comtains the number of non-null
	// pointers pointed to by char **tokens
	k=(i+1);
	// sneaky we to insert the tokens into
	// the symbol table via a hack - later on
	// this might get modified to take into
	// account some form or operator precedence.
	// be very afraid.
	while (tokens[j]!=NULL) {
		s->index_symbol(tokens[j],k-j);
		j++;
	}
	return s;
}

bTreeType<char*> *frame::make_tree ()
{
	bTreeType<char*> *p_tree;
	common.m_index.owner = false;
	p_tree = new (NULL) bTreeType<char*> (&common);
	return p_tree;
}

bool frame::load_make_file (char *str_file, bool split)
{
	int howMany;
	bool result;
	scripts.set_file_name (str_file);
	scripts.set_file_suffix (".txt");
	result = scripts.open_file ();
	if (result==true) {
		scripts.construct_symbol_table (split);
		scripts.close_input_file ();
//		scripts->killDelimiters ();
		howMany = scripts.m_index.size();
//		scripts->viewIndex (howMany);
//		scripts.save_index ();
	}
	return result;
}

bool frame::open_binary_file (char *str_file)
{
	bool valid;
	int result;
//	ASSERT ((ifstream*)m_fileName!=NULL);
#ifdef HAS_CONSOLE
	DEBUG_STR << "Opening: " << str_file << "\n";
#endif
	m_iFile = new std::fstream;
	ASSERT(m_iFile!=NULL);
	m_iFile->clear ();
	m_iFile->open (str_file,ios::in|ios::binary);
	result = m_iFile->is_open ();
	if (result!=0)
		valid = true;
	else {
		valid = false;
#ifdef HAS_CONSOLE
		DEBUG_STR << "Error opening file: " << str_file << "\n";
#endif
	}
//	releaseConsole
	bool eof = false;
	int pos = m_iFile->tellg ();
	return valid;	
}

void frame::close_input_file ()
{
//	waitForConsole
#ifdef HAS_CONSOLE
	DEBUG_STR << "Closing: " << "FILE" << "\n";
#endif
//	releaseConsole
	m_iFile->close ();
}

bool frame::load_binary_file (char *str_file)
{
	bool status;
	status = open_binary_file(str_file);
	if (status==true)
	{
//		m_ifile.read()
	}
	else
		return false;
	close_input_file();
	return true;
}

UINT indexFile (LPVOID param)
{
	bool fileCheck;
	script *aContext = (script*)(param);
	char *f = aContext->get_file_name();
	fileCheck = aContext->open_file ();
	if (fileCheck==true) {
		aContext->construct_symbol_table (false);
		aContext->close_input_file ();
		aContext->indexed = true;
		aContext->save_index (f);
	}
	return 0;
}

//	Since each script possesses a token index
//	as well as a linked list of token objects, it
//	is nice to create an internal script for use
//	during operations.

void frame::build_symbol_tables (bool trace)
{
	int j=0;
	char *str_file;
	vector<script*>::iterator iter, end;
	int howMany = scripts.m_index.size();
	content.resize (howMany);
	iter = content.begin ();
	end = content.end ();
	for (;iter<end;iter++)
	{
		str_file = scripts.m_index [j].ascii;
		(*iter) = new script ();
		(*iter)->set_file_name (str_file);
		(*iter)->set_file_suffix (".txt");

		LPVOID param = (LPVOID)((*iter));

#ifdef THREAD_INDEX_LOADS
		AfxBeginThread (::indexFile,param,1);
#else
		if (strlen(str_file)>0)
			::indexFile (param);
#endif
		j++;
	}
}

script *frame::makeScript (char *thefile)
{
	script *aContext;
	aContext = new script (thefile);
	count++;
	aContext->open_file ();
	return aContext;
}

void frame::mergeFile (int j)
{
	char *buffer;
	script *theScript = content [j];
	int wordCount = theScript->m_index.size();
	char *theFileName = theScript->get_file_name();
	while (theScript->indexed==false)
		Sleep (100);

	critical.Lock ();
//	waitForConsole
#if 1
	DEBUG_STR << "Merging Index File: " << theFileName << "\n";
#endif
//	releaseConsole;
	theScript->m_index.owner = false;
	for (int k=0;k<wordCount;k++)
	{ 
		buffer = theScript->m_index[k].ascii;
		int times = theScript->m_index[k].m_count;
		common.m_index.index_symbol (buffer,times);
#if 0
//	dont need to to this if we are using VirutalAlloc!
		delete buffer;
#endif
		theScript->m_index[k].ascii = NULL;
	}
	theScript->merged=true;
	critical.Unlock ();
}

UINT mergeIndex (LPVOID param)
{
	int fileNum = (int)(param);
	theFrame.mergeFile (fileNum);
	return 0;
}

void frame::masterIndex (char *mName)
{
	script *theScript;
	long j,total;
	total = scripts.m_index.size();
	LPVOID param;
	/* The indexes are retained in memory, so make a master index */
	for (j=1;j<total;j++) {
		theScript = content [j];
		if (theScript->valid==false)
			continue;
		param = (LPVOID)(j);
#ifdef threadMasterIndex
		AfxBeginThread (mergeIndex,param,1);
#else
		mergeIndex (param);
		theScript->purgeIndex ();
#endif
	}
	common.save_index (mName);
}


UINT saveImageFile (LPVOID param)
{
	script *theScript = (script*)(param);
	char *theFile = theScript->get_file_name();
	while (theScript->imageLoaded==false)
		Sleep (100);
	theScript->saveImageFile (theFile);
	theScript->theImage.rewind ();
	return 0;
}

void frame::write_file_images (bool trace)
{
	int j;
	script *theScript;
	LPVOID param;
	int howmany = scripts.m_index.size();
	for (j=0;j<howmany;j++)
	{
		theScript = content [j];
		if (theScript->valid==false)
			continue;
		param = (LPVOID)(theScript);
#ifdef threadImageWrites
		AfxBeginThread (saveImageFile,param,1);
#else
		saveImageFile (param);
#endif
	}
#ifdef HAS_CONSOLE
	DEBUG_STR << "\n";
#endif
}

void frame::writeKeyFiles (bool trace)
{
	char theFile[256];
	vector<script*>::iterator iter,end;
	iter = content.begin ();
	end = content.end ();
	int j=0;
	for (;iter<end;iter++,j++)
	{
		strcpy (theFile,scripts.m_index[j].ascii);
		(*iter)->saveKeyFile (theFile);
		(*iter)->theImage.rewind ();

#ifdef verbose
		DEBUG_STR << m_pText;
#endif
	}
#ifdef threaded
#ifdef HAS_CONSOLE
	DEBUG_STR << "\n";
#endif
#endif
}

//	a text object called "m_echo" is included since there is
//	no other provided method for chomping off the words in a
//	text object!  It's also a great way to help debug -- since
//	the echo image can be dumped to a file to verify that the
//	excerpts were all gone through correctly!

void frame::buildMarkov (text_object &theImage, bTreeType<char*> *theTree)
{
	char *buffer;
	grammar::gtype m_typeid;
	critical.Lock ();
	mStream theExcerpt (order,theImage);
	text_object m_echo;
	theImage.rewind ();
	while (theImage.end_of_text()!=true) {
		theExcerpt.indexWordList (theTree);
#ifdef keysOnly
		do
			buffer = theImage.get (m_typeid);
		while	((m_typeid!=unknown1)&&(theImage.m_bEnd!=true));
#else
		buffer = theImage.get_typed (m_typeid);
#endif
		if (buffer!=NULL) {
			theExcerpt.append (buffer);
			m_echo.transfer_from (theExcerpt); }
	}
	critical.Unlock ();
}

//	Iterate through all scripts and add to the
//	markov tree the nodes for each script

void frame::buildMarkovians (bTreeType<char*> *theTree)
{	
	vector<script*>::iterator iter,end;
	iter = content.begin ();
	end = content.end ();

	text_object theImage;
	if (theTree!=NULL)
	for (;iter<end;iter++)
	{
//		waitForConsole
#ifdef HAS_CONSOLE
		DEBUG_STR << "Markov test for: "<< (*iter)->get_file_name() << "\n";
#endif
//		releaseConsole;
		theImage = (*iter)->theImage;
		buildMarkov (theImage, theTree);
		theImage.rewind ();
	}
	else
	{
#ifdef HAS_CONSOLE
	DEBUG_STR << "Null bTree: Unable to construct Markovains!\n";
	DEBUG_STR << "\n";
#endif
	}
}

//	Search every fileImage for a sentence
//	containing a keyWord; then append the
//	following seintence to the list.

class extractorTag
{
public:
	int count;
	extractorTag ();
	script *Script;
	key_list *keyWords;
	text_object *result;
};

extractorTag::extractorTag ()
{
	Script = NULL;
	result = NULL;
}

UINT frame::ripText (LPVOID param)
{
	bool foundOne;
	text_object found;
//	s_node<char*,language> *marker;
	extractorTag *the = (extractorTag*)(param);
	script *theScript = the->Script;
	text_object theImage = theScript->theImage;
	foundOne = theImage.find_keyword ((*the->keyWords));
	if (foundOne==false) {
#if 0
		marker = theImage.m_nList;
		theImage.rewind ();
		foundOne = theImage.findKeyWord ((*the->keyWords));
		if (foundOne==false)
			theImage.m_nList.m_nBegin = marker;
#endif
	}
	if (foundOne==true) {
		found = theImage.get_sentnce ();
		the->result->append (found);
	}
	delete the;
	return foundOne;
}

text_object frame::ripText (node_list<node_str> &keyWords)
{
	char *str;
	vector<script*>::iterator iter,end;
	iter = content.begin();
	end = content.end ();
	int n;
	text_object *result = new text_object;
	for (n=0;iter<end;iter++,n++)
	{	
		if ((*iter)->valid==false)
			continue;
		if ((*iter)->imageLoaded==true) {
			extractorTag *Tag = new extractorTag;
			keyWords >> str;
			Tag->keyWords->append (str);
			
			Tag->result = result;
			Tag->count = n;	
//			Tag->Script = &(*iter);
#ifdef threadRipText
			AfxBeginThread (ripText,(LPVOID)(Tag),1);
#else
			::ripText ((LPVOID)(Tag));
#endif
		}
	}
	Sleep (1000);
	return *result;
}


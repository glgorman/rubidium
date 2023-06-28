
#include "stdafx.h"
#include "defines.h"
#include "symbol_table.h"
#include "btreetype.h"
#include "language.h"
#include "node_list.h"
#include "text_object.h"
#include "scripts.h"

using namespace std;

#pragma warning (disable: 4996)

#define MAXSTRING 64
#define DEBUG_STR cout 
#define HAS_CONSOLE		(YES)
#define INDEX_SUFFIX ".idx"
#define HTML_SUFFIX ".htm"
#define IMAGE_FILE_SUFFIX ".xyz"
#define KEY_FILE_SUFFIX ".key"

script::script (char *fileName)
{
	int sz;
	if (fileName!=NULL) {
		sz = strlen (fileName)+1;
		m_file_name = new char [sz];
		strcpy_s (m_file_name,sz,fileName); }
	strcpy_s(m_suffix,5,".txt");
	indexed = false;
	imageLoaded = false;
	merged = false;
	valid = false;
}

script::script ()
{
	m_file_name = NULL;
	m_the_file = NULL;

	indexed = false;
	imageLoaded = false;
	merged = false;
	valid = false;
}

script::~script ()
{
	if (m_file_name!=NULL)
		delete [] m_file_name;
}

//  modified 7-5-99 so that script files will
//  be opened with read access only.

char *script::get_file_name ()
{
	return m_file_name;
}

void script::set_file_suffix (char *str)
{
	strcpy_s (m_suffix,8,str);
}

void script::set_file_name (char *str)
{
	int sz = strlen(str)+1;
	if (m_file_name!=NULL)
		delete m_file_name;
	m_file_name = new char [sz+1];
	strcpy_s (m_file_name,sz,str);
}

bool script::open_file ()
{
	int result, pos;
	char *file_name = NULL;
	ASSERT (m_file_name!=NULL);
	m_the_file = new fstream;
	m_the_file->clear ();
	int size;
	size = strlen(m_file_name)+5;
	file_name = new char (size);
	strcpy_s (file_name,size,m_file_name);
	strcat_s (file_name,size,m_suffix);

#ifdef HAS_CONSOLE
	DEBUG_STR << "Opening: " << file_name << "\n";
#endif

	m_the_file->open (file_name,ios::in);
	result = m_the_file->is_open ();
	char c = m_the_file->peek ();
	if (result!=0)
	{
		valid = true;
		eof = false;
		pos = m_the_file->tellg ();
		DEBUG_STR << "File open position = " << pos << "\n";
	}
	else {
		valid = false;
#ifdef HAS_CONSOLE
		DEBUG_STR << "Error opening file: " << file_name << "\n";
//		delete [] file_name;
#endif
	}
//	releaseConsole
	m_the_file->seekg (0);

	return valid;	
}

void script::close_input_file ()
{
//	waitForConsole
#ifdef HAS_CONSOLE
	DEBUG_STR << "Closing: " << m_file_name << "\n";
#endif
//	releaseConsole
	m_the_file->close ();
	delete m_the_file;
	m_the_file = NULL;
}

void script::close_output_file ()
{
	m_the_file->close ();
	delete m_the_file;
	m_the_file = NULL;
}

//	gets a token from the file and pokes it into a char
//	buffer provided by the caller.

int script::get_word (char *buffer)
{
	bool end = false;
	unsigned char c1 = 0;
	unsigned char c2 = 0;
	int pos = m_the_file->tellg();
	
	if (pos==-1)
	{
		eof = true;
		memset(buffer,0,MAXSTRING);
		return 0;
	}
	int N = 0;
	
	if (m_the_file->is_open()==false)
		ASSERT(false);

	try {
	while (end==false)
	{
		m_the_file->get ((char&)c1);
		if (m_the_file->eof ()==true) {
			eof = true;
			break;
		}
		if (symbol_table::isalphanum(c1)==true)
			end=false;

		// norw that 3.14159 contains a non
		// terminating '.', which messes with
		// the rules.
		else if (symbol_table::isdelimiter(c1)==true)
			end = true;

		else if ((c2=='\\')&&(split_paths==true)) {
			end = true;
			break;
		}
		else if ((c1=='\'')||(c1=='\\'))
			end = false;
		
		else if ((c1=='.')&&
		(m_the_file->eof ()==false))
		{
			c2 = m_the_file->peek();
			if (c2==' ')
				end=true;
		}
		if ((N>0)&&(end==true)) {
			m_the_file->putback (c1);
				break; }

		buffer[N] = c1;
		N++;
		if (N>MAXSTRING-2)
			break;
	}
	}
	catch(...){
		ASSERT(false);
	}
	buffer[N] = 0;
#if 0
	cout << buffer;
#endif
	return N;
}

int script::getNumber (char *buffer)
{
	bool endOfWord = false;
	int count = 0;
	char theChar;
	while (endOfWord!=true) {
		m_the_file->get (theChar);
		if (m_the_file->eof ()!=0) {
			eof = true;
			break; }
		if (theChar>'9' || theChar<'0')
			endOfWord = true;
		if ((count>0)&&(endOfWord==true)) {
			m_the_file->putback (theChar);
			break; }		
		if ((count==0)||(endOfWord!=true)) {
			buffer[count] = theChar;
			count++; } }
	buffer[count] = 0;
	int result = 0;
	for (int digit=0;digit<count;digit++) {
		result = result*10;
		result = result+ int(buffer[digit])-int('0');}
	return result;
}

void script::putword (int indexNum)
{
//	std::fstream **m_the_file = (std::fstream*)&m_ofile;
	int howmany = m_index [indexNum].m_count;
	char *ascii = m_index [indexNum].ascii;
	if (ascii[0]=='\n')
		(*m_the_file) << "'\\n'";
	else if (ascii[0]=='\t')
		(*m_the_file) << "'\\t'";
	else
	(*m_the_file) << m_index [indexNum].ascii;
	(*m_the_file) << " << " << howmany << ";\n";
}

bool script::checkWord (char *aWord)
{
	char *thisEntry;
	unsigned int number = 0;
	bool result = false;
	while (number<m_index.size()) {
		thisEntry = m_index[number].ascii;
		if (strcmp (aWord,thisEntry)==0) {
			result = true;
			break; }
		number++; }
	return result;
}

//	since we want to eventually share pointers
//	this method will return a pointer to the
//	ascii symbol_table entry for a token

char *script::findEntry (char *aWord)
{
	unsigned int number = 0;
	char *result = NULL;
	char *currentAscii;
	token currentEntry;
	while (number<m_index.size()) {
		currentEntry = m_index [number];
		currentAscii = currentEntry.ascii;
		if (strcmp (aWord,currentAscii)==0) {
			result = currentEntry.ascii;
			break; }
		number++; }
#ifdef HAS_CONSOLE
	if (result==NULL)
	{
		DEBUG_STR << "Error - token not found in symbol_table: ";
		DEBUG_STR << "\"" << aWord << "\"\n";

	}
#endif
	return result;
}

void script::construct_symbol_table (bool split)
{
	int length;
	int wordCount = 0;
	split_paths=!split;
	char buffer [MAXSTRING];
	critical.Lock ();
	while (eof==false)
	{
		length = get_word (buffer);
		if (eof==true)
			break;
		if ((length>0)&&(strcmp(buffer," ")!=0))
		{
			m_index.index_symbol (buffer,1);
			wordCount++;
		} 
	}
	// sort list of unique words
	int sz1 = m_index._unique.size();
	if (sz1)
		m_index.sort (m_index._unique);

	// dump list of unique words
	cout << "Found " << wordCount << " words, " << sz1 << " unique words.\n";
#if 0
	for (int i=0;i<sz1;i++)
		cout << m_index._unique[i].ascii << "\n";
#endif

	// sort list of regular words
	int sz2 = m_index._symbols.size();
	if (sz2>1)
		m_index.sort (m_index._symbols);

	// dump list of regular words
	cout << "Found " << wordCount << " words, " << sz2 << " regular words.\n";
#if 0
	for (int i=0;i<sz2;i++)
		cout << m_index._symbols[i].ascii << "\n";
#endif

	for (int i=0;i<sz1;i++)
	{
		token temp;
		temp.ascii = m_index._unique[i].ascii;
		temp.m_count = m_index._unique[i].m_count;
		m_index.concat_index (m_index._symbols,temp);
	}
	sz2 = m_index._symbols.size();
	cout << "Transferred " << sz1 << " unique words, to " << sz2 << " regular words.\n";

#if 0
	for (int i=0;i<sz2;i++)
		cout << m_index._symbols[i].ascii << "\n";
#endif

	critical.Unlock ();
}

void script::viewIndex (int howmany)
{
//	waitForConsole
	if (howmany>m_index.size())
		howmany=m_index.size();
	if (howmany==0)
		howmany=m_index.size();

	for (unsigned int j=0;j<howmany;j++) {
		char *copy = m_index[j].ascii;
		int howmany = m_index[j].m_count; 
		{
#ifdef HAS_CONSOLE
			if (strcmp(copy,"\n")==0) {

				DEBUG_STR << "'\\n'";
			}
			else if (strcmp(copy,"\t")==0)
			{
				DEBUG_STR << "'\\t'";
			}
			else
			{
				DEBUG_STR << copy;
			}
			DEBUG_STR << " <" <<howmany << ">\n"; 
		
#endif
		}
	}
//	releaseConsole
}

void script::purgeIndex ()
{
	m_index.purge ();
}

//	Load an index from a .idx file

void script::reload_index ()
{
	token *theWord;
	char buffer [MAXSTRING];
	set_file_suffix (".idx");
	open_file ();	
	do {
		get_word (buffer);
		m_index.index_symbol (buffer,0);
		do 
			get_word (buffer);
		while (strcmp(buffer,"<")!=0);

		int howmany = getNumber (buffer);
		int indexnum = m_index.size();
		theWord = &m_index[indexnum-1];
		theWord->m_count = howmany;
		do
			get_word (buffer);
		while (strcmp(buffer,"\n")!=0);
	}
	while (eof==false);
	close_input_file ();
}

//	Analyse the token usage in symbol_table

UINT script::load_file_image (LPVOID param)
{
	critical.Lock ();
	int count = 0;
	char theWord [MAXSTRING];

	do get_word (theWord);
		while ((strcmp (theWord," ")==0)||(strlen(theWord)==0));
		
	do {
		if (theWord[0]==-1) 
			break;
		if ((strcmp (theWord," ")!=0)||(strlen(theWord)!=0)) {
			theImage.append (theWord);
			count ++; }
		get_word (theWord);}
	while (eof!=true);
	imageLoaded = true;
	theImage.rewind ();
	critical.Unlock ();
	return count;
}

inline void script::findWordInFile (char *toFind)
{
	char buffer [MAXSTRING];
	do {
		if (eof==true)
			break;
		get_word(buffer); }
	while (strcmp(buffer,toFind)==0);
}

void script::save_index (char *str)
{
	critical.Lock ();
	char writeName[MAXSTRING];
	if (m_file_name==NULL)
		m_file_name=str;
//	std::fstream **m_the_file = (fstream*)(&m_ofile);
	strcpy_s (writeName,MAXSTRING,indexFolder);
	strcat_s (writeName,MAXSTRING,m_file_name);
	strcat_s (writeName,MAXSTRING,INDEX_SUFFIX);
//	strcat(writeName,indexSuffix);
	m_the_file = new fstream;
	m_the_file->open (writeName,ios::out);
//	iostate state = m_the_file->rdstate ();
	for (unsigned int j=0;j<m_index.size();j++)
		putword (j);	
//	waitForConsole
#ifdef HAS_CONSOLE
	DEBUG_STR << "Saved index:" << writeName << "\n"; 
#endif
	(*m_the_file) << "Wrote " << m_index.size() << " unique labels\n";
#ifdef HAS_CONSOLE
	DEBUG_STR << "Wrote " << m_index.size() << " unique labels\n";
	DEBUG_STR << tstring::total << " bytes used in dictionary.\n";
	DEBUG_STR << "\n";
#endif
//	releaseConsole
	m_the_file->close ();
	delete m_the_file;
	m_the_file = NULL;
	critical.Unlock ();
}

void script::saveMetaHTML (char *)
{
	char writeName[MAXSTRING];
	strcpy_s(writeName,MAXSTRING,m_file_name);
	strcat_s(writeName,MAXSTRING,HTML_SUFFIX);
#if 0
	DEBUG_STR << "Trying to save a meta file:" << writeName << "\n";
#endif
#ifdef verbose
	chattyLog << "Saving a meta file:" << writeName << "\n";
#endif
	
	m_the_file->open(writeName,ios::in|ios::out);

	*m_the_file << "<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML//EN\">\n";
	*m_the_file << "\n<html>\n";
	*m_the_file << "\n<head>\n";
	*m_the_file << "\n<title>";
	*m_the_file << m_file_name;
	*m_the_file << "</title>\n";
	*m_the_file << "<meta name=\"GENERATOR\" content=\"Algernon 0.5\">\n";
	*m_the_file << "</head>\n";
	*m_the_file << "<body>\n";
	*m_the_file << "<p>";

//	of [12] kind [8] other [8] <a href="point.htm">point</a> [5] thought

	for (unsigned int j=0;j<m_index.size();j++)
	{
		int howmany = m_index[j].m_count;

		*m_the_file << "<a href=\"";
		*m_the_file << m_index [j].ascii;
		*m_the_file << ".htm\">";
		*m_the_file << "[" << howmany << ":";
			
/*		*m_the_file << ": ";		
		for (int k=0;k<m_nList [j]->times;k++)
		{
			if (k!=0)
				*m_the_file << ",";
			*m_the_file << attach[k];	
		}
*/
		*m_the_file << m_index [j].ascii;
		*m_the_file << "</a> ";
	}
	m_the_file->close ();
}

void script::saveImageFile (char *)
{
	critical.Lock ();
	grammar::gtype theType;
	char writeName [MAXSTRING];
	char *buffer;

	strcpy_s (writeName,MAXSTRING,imageFolder);
	strcat_s (writeName,MAXSTRING,m_file_name);
	strcat_s (writeName,MAXSTRING,IMAGE_FILE_SUFFIX);

//	waitForConsole
#ifdef HAS_CONSOLE
	DEBUG_STR << "Writing Image file: " << writeName << "\n";
#endif
//	releaseConsole
	m_the_file = new fstream;
	m_the_file->open (writeName,ios::out);
	
	theImage.rewind ();
	while (theImage.end_of_text()==false)
	{
		buffer = theImage.get_typed (theType);
		if (theType!=delimit)
			*m_the_file << " ";
		*m_the_file << buffer;		
	}
	theImage.rewind ();
	m_the_file->close ();
	delete m_the_file;
	m_the_file=NULL;
	critical.Unlock ();
}

void script::saveKeyFile (char*)
{
	critical.Lock ();
	char writeName [MAXSTRING];
	char *buffer;
	grammar::gtype theType;

	strcpy_s (writeName,MAXSTRING,keyFolder);
	strcat_s (writeName,MAXSTRING,m_file_name);
	strcat_s (writeName,MAXSTRING,KEY_FILE_SUFFIX);

//	waitForConsole
#ifdef HAS_CONSOLE
	DEBUG_STR << "Saving KeyStream: " << writeName << "\n";
#endif
//	releaseConsole
	m_the_file = new fstream;
	m_the_file->open (writeName,ios::out);

	theImage.rewind ();
	while (theImage.end_of_text()==false)
	{
		buffer = theImage.get_typed (theType);
		if (buffer==NULL)
			break;
		if (theType==grammar::delimiter)
			*m_the_file << buffer;
		if (theType==grammar::unknown1)
			*m_the_file << " " << buffer;
	}
	theImage.rewind ();
	m_the_file->close ();
	delete m_the_file;
	m_the_file = NULL;
	critical.Unlock ();
}


#include "stdafx.h"
#include "defines.h"
#include "symbol_table.h"
#include "btreetype.h"
#include "node_list.h"
#include "text_object.h"
#include "scripts.h"
#include "frames1.h"

using namespace std;

#pragma warning (disable: 4996)

//#define DEBUG_STR DEBUG_STR
#define INDEX_SUFFIX ".idx"
#define HTML_SUFFIX ".htm"
#define IMAGE_FILE_SUFFIX ".xyz"
#define KEY_FILE_SUFFIX ".key"

script::script (char *fileName)
{
	if (fileName!=NULL) {
		m_file_name = new char [strlen (fileName)+1];
		strcpy (m_file_name,fileName); }
	indexed = false;
	imageLoaded = false;
	merged = false;
	valid = false;
}

script::script ()
{
	m_file_name = NULL;
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

void script::set_file_name (char *str)
{
	if (m_file_name!=NULL)
		delete m_file_name;
	m_file_name = strcpy (new char [strlen(str)+1],str);
}

bool script::open_file ()
{
	int result;
	ASSERT (m_file_name!=NULL);
#ifdef HAS_CONSOLE
	DEBUG_STR << "Opening: " << m_file_name << "\n";
#endif
	m_ifile.clear ();
	m_ifile.open (m_file_name,ios::in);
	result = m_ifile.is_open ();
	if (result!=0)
		valid = true;
	else {
		valid = false;
#ifdef HAS_CONSOLE
		DEBUG_STR << "Error opening file: " << m_file_name << "\n";
#endif
	}
//	releaseConsole
//	m_ifile.seekg (0);
//	m_ifile.seekg (0);
	eof = false;
	int pos = m_ifile.tellg ();

//	delete [] textName;
	return valid;	
}

void script::close_input_file ()
{
//	waitForConsole
#ifdef HAS_CONSOLE
	DEBUG_STR << "Closing: " << m_file_name << "\n";
#endif
//	releaseConsole
	m_ifile.close ();
}

void script::close_output_file ()
{
	m_ofile.close ();
}

//	gets a token from the file and pokes it into a char
//	buffer provided by the caller.

int script::get_word (char *buffer)
{
	bool end = false;
	char c1,c2;
	int N = 0;
	buffer [0] = 0;
	while (end==false)
	{	
		m_ifile.get (c1);
		if (m_ifile.eof ()!=0) {
			eof = true;
			break;
		}
		if ((c1==' '||c1=='\n')||(c1==9)||(c1==',')
				||(c1=='?')||(c1=='!'))
				end = true;
		else if ((c1=='\'')||(c1=='\\'))
				end = false;

		else if ((c1=='.')&&(m_ifile.eof ()!=0))
		{
			c2 = m_ifile.peek();
			if (c2==' ')
				end=true;
		}
		if ((N>0)&&(end==true)) {
			m_ifile.putback (c1);
				break; }

		buffer[N] = c1;
		N++;
		if (N>254)
			break;
	}
	buffer[N] = 0;
	return N;
}

int script::getNumber (char *buffer)
{
	bool endOfWord = false;
	int count = 0;
	char theChar;
	while (endOfWord!=true) {
		m_ifile.get (theChar);
		if (m_ifile.eof ()!=0) {
			eof = true;
			break; }
		if (theChar > '9' || theChar < '0')
			endOfWord = true;
		if ((count>0)&&(endOfWord==true)) {
			m_ifile.putback (theChar);
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
	std::fstream *theFile = (std::fstream*)&m_ofile;
	int howmany = m_index._symbols [indexNum].m_count;
	char *ascii = m_index._symbols [indexNum].ascii;
	if (ascii[0]=='\n')
		(*theFile) << "'\\n'";
	else if (ascii[0]=='\t')
		(*theFile) << "'\\t'";
	else
	(*theFile) << m_index._symbols [indexNum].ascii;
	(*theFile) << " << " << howmany << ";\n";
}

bool script::checkWord (char *aWord)
{
	char *thisEntry;
	unsigned int number = 0;
	bool result = false;
	while (number<m_index.size()) {
		thisEntry = m_index._symbols[number].ascii;
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
		currentEntry = m_index._symbols [number];
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

void script::construct_symbol_table (bool trace)
{
	int length;
	int wordCount = 0;
	char buffer [256];
	critical.Lock ();
	while (eof==false){
		length = get_word (buffer);
		if (eof==true)
			break;
		if ((length>0)&&(strcmp(buffer," ")!=0)) {
			m_index.index_word (buffer,1);
			wordCount++; } 
	}
	critical.Unlock ();
}

void script::viewIndex (unsigned int howmany)
{
//	waitForConsole
	if (howmany>m_index.size())
		howmany=m_index.size();
	if (howmany==0)
		howmany=m_index.size();

	for (unsigned int j=0;j<howmany;j++) {
		char *copy = m_index._symbols [j].ascii;
		int howmany = m_index._symbols [j].m_count; 
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

void script::reloadIndex ()
{
	token *theWord;
	char buffer [256];
	open_file ();	
	do {
		get_word (buffer);
		m_index.index_word (buffer,0);
		do 
			get_word (buffer);
		while (strcmp(buffer,"<")!=0);

		int howmany = getNumber (buffer);
		int indexnum = m_index.size();
		theWord = &m_index._symbols[indexnum-1];
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
	char theWord [256];

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
	char buffer [256];
	do {
		if (eof==true)
			break;
		get_word(buffer); }
	while (strcmp(buffer,toFind)==0);
}

void script::save_index ()
{
	critical.Lock ();
	char writeName[128];
	std::fstream *theFile = (fstream*)(&m_ofile);
	strcpy (writeName,indexFolder);
	strcat (writeName,m_file_name);
	strcat(writeName,INDEX_SUFFIX);
//	strcat(writeName,indexSuffix);
	theFile->open (writeName,ios::out);
//	iostate state = theFile.rdstate ();
	for (unsigned int j=0;j<m_index.size();j++)
		putword (j);	
//	waitForConsole
#ifdef HAS_CONSOLE
	DEBUG_STR << "Saved index:" << writeName << "\n"; 
#endif
	(*theFile) << "Wrote " << m_index.size() << " unique labels\n";
#ifdef HAS_CONSOLE
	DEBUG_STR << "Wrote " << m_index.size() << " unique labels\n";
	DEBUG_STR << "\n";
#endif
//	releaseConsole
	theFile->close ();
	critical.Unlock ();
}

void script::saveMetaHTML ()
{
	char writeName[32];
	fstream theFile;
	strcpy(writeName,m_file_name);
	strcat(writeName,HTML_SUFFIX);
#if 0
	DEBUG_STR << "Trying to save a meta file:" << writeName << "\n";
#endif
#ifdef verbose
	chattyLog << "Saving a meta file:" << writeName << "\n";
#endif
	
	theFile.open(writeName,ios::in|ios::out);

	theFile << "<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML//EN\">\n";
	theFile << "\n<html>\n";
	theFile << "\n<head>\n";
	theFile << "\n<title>";
	theFile << m_file_name;
	theFile << "</title>\n";
	theFile << "<meta name=\"GENERATOR\" content=\"Algernon 0.5\">\n";
	theFile << "</head>\n";
	theFile << "<body>\n";
	theFile << "<p>";

//	of [12] kind [8] other [8] <a href="point.htm">point</a> [5] thought

	for (unsigned int j=0;j<m_index.size();j++)
	{
		int howmany = m_index._symbols [j].m_count;

		theFile << "<a href=\"";
		theFile << m_index._symbols [j].ascii;
		theFile << ".htm\">";
		theFile << "[" << howmany << ":";
			
/*		theFile << ": ";		
		for (int k=0;k<m_nList [j]->times;k++)
		{
			if (k!=0)
				theFile << ",";
			theFile << link[k];	
		}
*/
		theFile << m_index._symbols [j].ascii;
		theFile << "</a> ";
	}
	theFile.close ();
}

void script::saveImageFile ()
{
	critical.Lock ();
	language theType;
	char writeName [128];
	char *buffer;

	fstream theFile;
	strcpy (writeName,imageFolder);
	strcat (writeName,m_file_name);
	strcat (writeName,IMAGE_FILE_SUFFIX);

//	waitForConsole
#ifdef HAS_CONSOLE
	DEBUG_STR << "Writing Image file: " << writeName << "\n";
#endif
//	releaseConsole
	theFile.open (writeName,ios::out);
	
	theImage.rewind ();
	while (theImage.m_bEnd==false)
	{
		buffer = theImage.getIndexWord (theType);
		if (theType!=delimiter)
			theFile << " ";
		theFile << buffer;		
	}
	theImage.rewind ();
	theFile.close ();
	critical.Unlock ();
}

void script::saveKeyFile ()
{
	critical.Lock ();
	char writeName [128];
	char *buffer;
	language theType;

	fstream theFile;
	strcpy (writeName,keyFolder);
	strcat (writeName,m_file_name);
	strcat (writeName,KEY_FILE_SUFFIX);

//	waitForConsole
#ifdef HAS_CONSOLE
	DEBUG_STR << "Saving KeyStream: " << writeName << "\n";
#endif
//	releaseConsole
	theFile.open (writeName,ios::out);

	theImage.rewind ();
	while (theImage.m_bEnd==false)
	{
		buffer = theImage.getIndexWord (theType);
		if (buffer==NULL)
			break;
		if (theType==delimiter)
			theFile << buffer;
		if (theType==unknown)
			theFile << " " << buffer;
	}
	theImage.rewind ();
	theFile.close ();
	critical.Unlock ();
}

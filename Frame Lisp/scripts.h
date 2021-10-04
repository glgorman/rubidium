
#include "afxmt.h"
#include <iostream>
#include <fstream>
#include <cstring>

class ofstream;

class script
{
private:
	char *m_file_name;
	char m_suffix [8];
	bool m_makefile;
	std::ifstream m_ifile;
	std::ofstream m_ofile;
	CCriticalSection critical;

public:
	symbol_table	m_index;
	text_object theImage;
	
volatile
	bool eof,indexed,merged,imageLoaded,valid;
	
public:// constructor and dectructor methods
//	virtual	script &operator = (const script&);
	script (char *fileName);
	script ();
	~script ();

public:
	void set_file_name (char *);
	char *get_file_name ();
	bool open_file ();
	void close_input_file ();
	void close_output_file ();	

public:// Indexing and data structures	
	void construct_symbol_table (bool trace);
	UINT load_file_image (LPVOID param);

public:// Verification and archival 
	void viewIndex (unsigned int howmany);
	void save_index ();
	void purgeIndex ();
	void reloadIndex ();
	void saveKeyFile ();
	void saveMetaHTML ();
	void saveImageFile ();
	
private:// Various methods of varying utility
	int get_word (char *buffer);
	int getNumber (char *buffer);		
	bool checkWord (char *aWord);
	// file I/O
	void putword (int indexNum);
	void findWordInFile (char *toFind);
	char *findEntry (char *aWord);	
	void addComplexNoun (char *aPhrase);			
};
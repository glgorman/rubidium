
#include "afxmt.h"
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;
class ifstream;
class ofstream;

class script
{
private:
	CCriticalSection critical;
	fstream	*m_the_file;
	std::ifstream m_ifile;
	std::ofstream m_ofile;

	char *m_file_name;
	char m_suffix [8];
	bool m_makefile;
	bool split_paths;
	
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
	void set_file_name (char *name);
	void set_file_suffix (char *idx);
	char *get_file_name ();
	bool open_file ();
	void close_file ();
	void close_input_file ();
	void close_output_file ();	

public:// Indexing and data structures
	static UINT indexFile (LPVOID param);
	void construct_symbol_table (bool trace);
	UINT load_file_image (LPVOID param);

public:// Verification and archival 
	void viewIndex (int howmany);
	void save_index (char *);
	void purgeIndex ();
	void reload_index ();
	void reload_index (char *);
	void saveKeyFile (char *);
	void saveMetaHTML (char *);
	void saveImageFile (char *);
	
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
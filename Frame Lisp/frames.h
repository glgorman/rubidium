
class frame;

class lframe
{
private:
	static frame *m_pFrame;

public:
	static frame *get()
	{ 
		return m_pFrame;
	};
	static void set_global();
	static void load_binary_file(char *);
};

#include "stdafx.h"
#include "afxmt.h"
#include <vector>

template <class X> class bTreeType;
template <class X,typename part=void> class node_list;
extern bTreeType<char*> *m_root;
class text_object;

void openDebugFiles ();
void closeDebugFiles ();

class event_list
{
public:
	std::vector<UINT>	m_timestamp;
	std::vector<char *>	m_text;
};

class ifstream;
class ofstream;

class frame// : public CWinThread
{
	std::fstream *m_iFile;
	std::fstream *m_oFile;

	CCriticalSection critical;
public:
	int count;
	static void *mem_pool;

	frame ();
	~frame ();
	text_object &operator [] (int contextNum);

public:
//static frame *get_global();
	BOOL InitInstance ();
	symbol_table *cons (char **);
	bool open_binary_file (char *);
	bool load_binary_file (char *);
	void close_input_file ();
	bool load_project ();
	void build_event_list ();
	void build_symbol_tables (bool trace);
	bool load_make_file (char *, bool prompt);
	void load_file_images (bool trace);
	void write_file_images (bool trace);
	void writeKeyFiles (bool trace);
	void buildMarkov (text_object &theImage, bTreeType<char*>*);
	void buildMarkovians (bTreeType<char*>*);
	void buildIndices (int theOrder);
	bTreeType<char*> *make_tree ();
	text_object ripText (node_list<node_str> &keyWords);

private:
	event_list m_events;
	script common;
	script scripts;
	std::vector<script*>	content;
	bTreeType<char*> *m_root;
	char *m_str_make;

	int available;
	bool button;

private:	
	UINT load_file_image (int j, char* fileName, char *suffix);
	void mergeFile (int j);
	
//	void build_symbol_table (int indexNum);
	void masterIndex (char *mName);
	script *indexFile (char *theFile);
	script *indexLoad (char *theFile);
	script *makeScript (char *thefile);
	void putword (std::ofstream &theFile, int indexNum);
	friend UINT mergeIndex (LPVOID);
	static UINT ripText (LPVOID param);
};


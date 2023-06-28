
#include "stdafx.h"
#include "defines.h"
#include "sets.h"
#include "btreetype.h"
#include "language.h"
#include "node_list.h"
#include "text_object.h"
#include "symbol_table.h"
#include "scripts.h"

#define MAXSTRING	256
#define MAX_BUFFER 4096
#pragma warning (disable: 4996)
#define slothful2
#define sortOn

using namespace std;

namespace chartypes
{
	extern SETOFCHAR ident;
	extern SETOFCHAR digits;
	extern SETOFCHAR hex;
	extern SETOFCHAR whitespace;
	extern SETOFCHAR delimiters;
	extern SETOFCHAR alpha;
	extern SETOFCHAR operat;
	extern SETOFCHAR punct1;
	extern SETOFCHAR punct2;
}

void toUpper (char *str)
{
	if (str==NULL)
		return;

	int length = strlen (str);
	int i;
	for (i=0;i<length;i++)
	if ((str[i]>='a')&&(str[i]<='z'))
		str[i]=str[i]-('a'-'A');
}	

void toLower (char *str)
{
	if (str==NULL)
		return;

	int length = strlen (str);
	int i;
	for (i=0;i<length;i++)
	if ((str[i]>='A')&&(str[i]<='Z'))
		str[i]=str[i]+('a'-'A');
}

symbol_table::symbol_table ()
{
//	flag to indicate whether or not this list owns the
//	character pointers that it has, i.e. to determine
//	whether	or not it is allowed to delete them when the
//	destructor is called

	owner = true;
	_symbols.reserve (32);
	available = 32;
	totalKnown = 0;
}

symbol_table::symbol_table (int total)
{
	_symbols.reserve (total);
	available = total;
	for (long j=0;j<total;j++) {
		_symbols [j].ascii=NULL;
		_symbols [j].m_count=0;
	}
	available = total;
	totalKnown = 0;
}

symbol_table::~symbol_table ()
{
	int j;
	char *ascii;
	int sz = _symbols.size();

	if (owner==true)
		for (j=0;j<sz;j++)
	{
		ascii = _symbols [j].ascii;
		if (ascii!=NULL)
			delete [] ascii;
		ascii = NULL;
	}
//	delete theList;
	_symbols.clear();
}

token &symbol_table::operator [] (int entryNum)
{
	totalKnown = _symbols.size();
	if (entryNum<totalKnown)
		return _symbols [entryNum];
	else
		throw entryNum;
}

void symbol_table::swap (int item1, int item2)
{
	token temp = _symbols [item2];
	_symbols [item2] = _symbols [item1];
	_symbols [item1] = temp;
}

symbol_table *symbol_table::sort(vector<token> &_list)
{
	int sz = _list.size();
	std::qsort((void*)(&_list[0]),sz,sizeof(token),&_compare);
	return this;
}

bool symbol_table::isdelimiter (char ch)
{
	bool result;
	result = chartypes::delimiters.in(ch);
	return result;
}

bool symbol_table::iswhitespace (char ch)
{
	bool result;
	result = chartypes::whitespace.in(ch);
	return result;
}

bool symbol_table::isalphanum (char ch)
{
	bool result;
	result = chartypes::ident.in(ch);
	return result;
}

void symbol_table::purge ()
{
	int j;
	char *ascii;
	int sz = _symbols.size();

	if (owner==true)
		for (j=0;j<sz;j++)
	{
		ascii = _symbols [j].ascii;
		if (ascii!=NULL)
			delete [] ascii;
		ascii = NULL;
	}
//	delete theList;
	_symbols.clear();
	available = 0;
	totalKnown = 0;
}

void symbol_table::extend (int howMany)
{
#if 0
	word *oldvocab = theList;
	theList = new word [available+howMany];
	for (int i=0; i<available; i++)
		theList[i] = oldvocab [i];
	available=available+howMany;
	delete [] oldvocab;
#endif
	_symbols.resize (howMany);
	available = howMany;
	cout << ".";
}

//	Checks where the last word on the list to see where
//	it really belongs, and then uses insertWord to put in
//	into the correct position.  Remember that we only need
//	to call addWord for new words.  We always call insert
//	word, i.e. just to take care of the case where a word
//	needs to be moved up the list because of frequency of
//	use.

void symbol_table::index_symbol (char *buffer, int times)
{
#if 0
	cout << "\nIndexing word: \"" << buffer << "\"\n";
#endif
	int reqular_index = -1;
	int unique_index = -1;

	if (strlen(buffer)==0) 
		return;

	reqular_index = search_regular_index (buffer);
	if (reqular_index>=0)
	{
		token &temp = _symbols[reqular_index];
		temp.m_count+=times;
		sort_word (_symbols,reqular_index);
	}
#if 0
	else
	{
		cout << "Not found in regular index.\n";
	}
#endif
	if (reqular_index<0)
	{
#if 0
		cout << "Searching unique index\n";
#endif
		unique_index = search_unique_index (buffer);
		if (unique_index<0)
		{
#if 0
			cout << "Not found  adding.\n";
#endif
			// this calls the constructer which we noramlly
			// avoid doing by using reference parameters.
			token new_t (buffer,times);
			concat_index (_unique,new_t);
		}
		else
		{
			token &temp = _unique[unique_index];
#if 0
			cout << "Found at position: " << unique_index << ", " << temp.m_count << " times\n";
#endif
			temp.m_count+=times;
		}
		sort_word (_unique,unique_index);
	}
//	Run quicksort after every 1024 additions?
	size_t sz3 = _unique.size();
	if (sz3%1024==0)
		sort (_unique);
//	TODO - once we have seen a unique word a certain number
//	of times, it can be moved into the reqular table.
}

//	remove an object from an index and delete the object
//	pointed to if this index owns the object.

void symbol_table::remove_symbol (std::vector<token> &_index, int i)
{
	int j;
	token theWord, nextWord;
	theWord = _index [i];
	if (owner==true)
		delete [] theWord.ascii;
	if (i==totalKnown-1) {
		j=i;
		_index [j].ascii = NULL; }
	else {
		for (j=i;j<totalKnown-1;j++) {
			nextWord = _index [j+1];
			_index [j] = nextWord;
			theWord = nextWord;
		}
		_index [j].ascii=NULL;
		_index [j].m_count=0;
	}
	totalKnown=totalKnown--;
	_index.resize (totalKnown);
}

void symbol_table::kill_delimiters ()
{
	int sz1, sz2;
	sz1 = _symbols.size();
	for (int i=0; i<sz1; i++) {
		char *theWord = _symbols [i].ascii;
		char aChar = theWord [0];
		if (isalphanum (aChar)==false)
			remove_symbol (_symbols,i);
	}
	sz2 = _unique.size();
	for (int i=0; i<sz2; i++) {
		char *theWord = _unique [i].ascii;
		char aChar = theWord [0];
		if (isalphanum (aChar)==false)
			remove_symbol (_unique,i);
	}
}

// case insenstive string comparaion function
// borrowed from the pascal compiler port.
// should be a lot faster than copying the
// strings to a buffer,rather than copying to
// to a buffer, etc., and then converting to
// lower case.

int symbol_table::compare1 (const token &item1, const token &item2)
{
	const char *str1 = item1.ascii;
	const char *str2 = item2.ascii;
	char c1, c2;
	int i,j,k,l;
	int test = 0;
	i = strlen(str1);
	j = strlen(str2);
	if (i>j)
		l=i;
	else
		l=j;

	for (k=0;k<l;k++)
	{
		test = 0;
		c1 = tolower(str1[k]);
		c2 = tolower(str2[k]);
		if ((c1==0)&&(c2==32)
			||(c2==0)&&(c1==32))
			break;
		if (c1!=c2)
		{
			test = (c1>c2?1:-1);
			break;
		}
	}
	if (((i<j)&&(test==0))&&(str2[k]!=32))
		test=-1;

	return test;
}

#if 0
// old ineffecint case insenstive string comparason function

int symbol_table::compare1 (const token &item1, const token &item2)
{
	int result;
	char buffer1 [MAXSTRING], buffer2 [MAXSTRING];
	strcpy_s (buffer1,MAXSTRING,item1.ascii);
	strcpy_s (buffer2,MAXSTRING,item2.ascii);
	toLower (buffer1);
	toLower (buffer2);
	result = strcmp(buffer1,buffer2);
	return result;
}
#endif

// items are sorted first by frequency and then
// by alphabetic position.

int symbol_table::_compare (const void *ptr1, const void *ptr2)
{
	int result;
	const token *item1 = (token*)ptr1;
	const token *item2 = (token*)ptr2;

	char buffer1 [MAXSTRING], buffer2 [MAXSTRING];
	strcpy_s (buffer1,MAXSTRING,item1->ascii);
	strcpy_s (buffer2,MAXSTRING,item2->ascii);
	toLower (buffer1);
	toLower (buffer2);

	if (item1->m_count>item2->m_count)
		return -1;
	else if (item1->m_count<item2->m_count)
		return 1;
	
	result = strcmp(buffer1,buffer2);
	return result;
}

// used to perform a two tiered comparison, first
// checking the number of times that a symbol has
// appeared, and then checking for alpabetcal
// position, in a case insensitive manner - even
// thhough the final insersion is case sensitive.

int symbol_table::compare0 (const token &item1, const token &item2)
{
	int result;
	char buffer1 [MAXSTRING], buffer2 [MAXSTRING];
	// this method sorts according to the number
	// of times we have seen an item.  Return false
	// if the item is out of position.
	if	(item1.m_count<item2.m_count)
		return false;

	// if the item count is greater then the item
	// we are comparing against, we are in the
	// correct "section" - hopefully!
	if (item1.m_count>item2.m_count)
		return 1;
	else if (item1.m_count<item2.m_count)
		return -1;

	// if we get this far, it means that the item
	// counts are equal, so we need to move on
	// to actually comparing the strings.
	strcpy_s (buffer1,MAXSTRING,item1.ascii);
	strcpy_s (buffer2,MAXSTRING,item2.ascii);
	toLower (buffer1);
	toLower (buffer2);
	result = strcmp(buffer1,buffer2);
	return result;
}

// Plain vanilla case insensitve comparason function.
// Experimenatl.  Allows splitting of a list into
// two sections, the first part where the most
// frequent elements are stored, and then a second
// list, stores the rest of the elements.

int symbol_table::search_index (vector<token> &idx, char *buffer)
{
	int index = 0;
	int result = -1;
	int cmp;
	std::vector<token>::iterator begin,iter,end;
	token test;
	if (_symbols.size()==0)
		return -1;
	iter = begin = idx.begin();
	test.ascii = buffer;
	test.m_count = 1;
	end = idx.end();
	while (iter!=end)
	{
		cmp = compare1(test,*iter);
		if (cmp<=0)
		{
			result = index;
			break;
		}
		iter++;
		index++;
	}
}

int symbol_table::search_regular_index (char *buffer)
{
	int index = 0;
	int result = -1;
	int cmp;
	std::vector<token>::iterator begin,iter,end;
	token test;
	if (_symbols.size()==0)
		return -1;
	iter = begin = _symbols.begin();
	test.ascii = buffer;
	test.m_count = 1;
	end = _symbols.end();
	while (iter!=end)
	{
		cmp = compare1(test,*iter);
		if (cmp==0)
		{
			result = index;
			break;
		}
		iter++;
		index++;
	}
	return result;
}

//	Returns -1 if the ascii token is not found
//	and returns the index position if it is found.
//	searches the entire list.  To do - make another

int symbol_table::search_unique_index (char *buffer)
{	
	int index = 0;
	int result = -1;
	int cmp;
	std::vector<token>::iterator begin,iter,end;
	token test;
	if (_unique.size()==0)
		return -1;
	iter = begin = _unique.begin();
	test.ascii = buffer;
	test.m_count = 1;
	end = _unique.end();
	while (iter!=end)
	{
		cmp = compare1(test,*iter);
		if (cmp==0)
		{
			result = index;
			break;
		}
		iter++;
		index++;
	}
	return result;
}

// as much as I despise code bloat - this is here for now
// until it is futher optimized.  Deal with it.  I do.
// What out for "frequent words!"  They might be next!

#if 0
void symbol_table::add_regular_word (char *buffer, int times)
{
	token t;
	size_t i,j,k,sz;
	i = _symbols.size();
	j = _symbols.capacity();
	k = i+1;

//	increase the size of symbol buffer often enough
//	but not "too" often.
 	if (i==j)
		_symbols.reserve (j+32);

	sz = _symbols.size();
	if (sz!=k)
		_symbols.resize (k);
	sz = _symbols.size();
	ASSERT(sz==k);

//	allocate the bufer to receive
//	the string
	int str_size = strlen(buffer)+1;
	do
	{	
		t.ascii  = (char*) tstring::allocate(str_size);
		t.m_count = 1;
		if (t.ascii==NULL)
			ASSERT(false);
	}
	while (t.ascii==NULL);
	strcpy_s (t.ascii,str_size,buffer);
	_symbols [i] = t;
	cout << " added: \"" << t.ascii << "\" to regular list.\n"; 
}
#endif

//	Add a token to the end of an index, while increasing
//	the size of the index by one.

void symbol_table::concat_index (std::vector<token> &_idx, token &t)
{
	size_t i,j,k,sz;
	i = _idx.size();
	j = _idx.capacity();
	k = i+1;

//	increase the size of symbol buffer often enough
//	but not "too" often.
 	if (i==j)
		_idx.reserve (j+64);

	sz = _idx.size();
	if (sz!=k)
		_idx.resize (k);
	sz = _idx.size();
	ASSERT(sz==k);

	_idx [i] = t;
#if 0
	cout << " added: \"" << t.ascii << "\" to index at position: " << i << ".\n";
#endif
}

//	A slightly more effecient than the last insertion
//	method moving the last item up a list or table.
//	This one is a little smarter on how it swaps things,
//	cutting the swapping time by about 2/3 ... 

void symbol_table::sort_word (vector<token> &_idx, int pos)
{
	int i,j,l;
	int location = pos;
	int status;
	size_t sz1 = _idx.size();
	// dont sort a list with less than
	// two elements.
	if ((sz1<2)||(location<1))
		return;
	
	// make a temporary copy of the word
	// we want to sort and (maybe) poke a NULL in
	// the original list at that location.
	token &temp = _idx[location];
//	_idx[where].ascii=NULL;
	
	vector<token>::iterator begin,end,iter1,iter2;
	begin = _idx.begin();
	end = _idx.end();
	iter1 = _idx.begin();

	cout << "\nSearching for location to insert: \"" << temp.ascii << "\"\n";
	// find the location to perform the insertion
	i=0;
	while (iter1!=end)
	{
		j = compare1(temp,*iter1++);
		if (j<=0)
			break;
		i++;
	}

	// iter1 now contatns the location where we
	// need to perform the insertion

	report_context (_idx,location,i);
}

void symbol_table::report_context (vector<token> &_idx, int p, int i)
{
	size_t sz1 = _idx.size();
	int max_index = sz1-1;
	int q,r,s;

	q = i-1;
	r = i;
	s = i+1;

	cout << "Found location for insertion at position: " << i << "\n";
	if ((q>=0)&&(q<=max_index))
		cout << "Context: [" << q << "] = \"" << _idx[q].ascii << "\"\n";
	if ((r>=0)&&(r<=max_index))
		cout << "Context: [" << r << "] = \"" << _idx[r].ascii << "\"\n";
	if ((s>=0)&&(s<=max_index))
		cout << "Context: [" << s << "] = \"" << _idx[s].ascii << "\"\n";
	if (r==p)
		cout << "Already in position - returning!\n";
}

//	check to see if the word at position is correctly inserted
//	and reinsert it if it isnt, not presently working!

void symbol_table::sift (int position)
{
	int n = 0;
	token theWord, aWord;
	theWord = _symbols [position];
	aWord = _symbols [n];
	if (strcmp (theWord.ascii,aWord.ascii)==0)
	{
//	delete the char * of the word we are sifting
//	into place if it points to a duplicate string

		if (theWord.ascii!=aWord.ascii)
			delete (aWord.ascii);
		theWord.m_count = theWord.m_count+aWord.m_count;
		aWord.m_count = 0;
	}
}

//	Merges a second symbol_table List with this symbol_tableList,
//  then returns the resulting List.  Resets the owner flag
//	on the list that has been merged, deletes duplicate
//	pointers to char ... etc.

symbol_table symbol_table::merge (symbol_table another)
{
	another.owner = false;
	symbol_table theIndex;
	token theWord, aWord, bWord;
	int aPos, bPos;
	aPos = bPos = 0;
	int required = totalKnown+another.totalKnown;
//	theIndex.theList = new word [required];
	theIndex._symbols.resize(required);

	for (int count=0;count<required;count++) {
		aWord = _symbols [aPos];
		bWord = another [bPos];
		if (aWord.m_count>bWord.m_count) {
			theIndex [count] = aWord;
			aPos++;
			theIndex.sift (count); }
		else {
			theIndex._symbols [count] = bWord;
			bPos++;
			theIndex.sift (count); }
	}
	return theIndex;
}
 
/////////////////////////////////////////////////////////////////////////////////////////////////////////


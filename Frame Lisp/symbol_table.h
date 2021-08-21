// symbol_table.h
#include <vector>

class token
{	
public:
	char *ascii;
	int m_count;
	token ();
};

class symbol_table
{
public:
	std::vector<token>	_symbols;
	bool owner;
	bool m_allocate;

private:
	inline int checkWord (char *buffer, int times);
	void addword (char *buffer, int times);
	void removeWord (int i);
	static int _compare (const void*, const void*);
	inline bool compare (token item1, token item2);	
	inline void swap (int item1, int item2);
	void extend (int howMany);
	void sortWord (int where);

public:
	symbol_table *sort();
	void purge ();
	size_t size(){ return _symbols.size(); };
	symbol_table ();
	symbol_table (int total);
	~symbol_table ();

	token &operator [] (unsigned int entryNum);
	void index_word (char *buffer, int times);

	void killDelimiters ();
	symbol_table merge (symbol_table another);
	void sift (int position);
};

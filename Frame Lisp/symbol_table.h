// symbol_table.h
#include <vector>

class tstring;

class token
{
public:
	token ();
	explicit token (char*,int);
	char *ascii;
	union {
		int m_count;
		int m_index;
	};
};

class symbol_table
{
friend class script;

protected:
	int available;
	int totalKnown;

public:
	bool m_allocate;
	std::vector<token>	_symbols;
	std::vector<token>	_unique;
	bool owner;
	
public:
	symbol_table ();
	symbol_table (int total);
	~symbol_table ();

	static bool isalphanum (char ch);
	static bool iswhitespace (char ch);
	static bool isdelimiter (char ch);
	token &operator [] (int entryNum);
	size_t size(){ return _symbols.size(); };

	symbol_table *sort(std::vector<token>&);
	void purge ();
	void index_symbol (char *buffer, int times);
	void kill_delimiters ();
	symbol_table merge (symbol_table another);
	void sift (int position);

protected:
#if 0
	void add_regular_word (char *, int times);
	void add_unique_word (char *, int times);
#endif
	void report_context (std::vector<token> &_idx, int p, int i);
	static int _compare (const void*, const void*);
	inline void concat_index (std::vector<token>&,token&);
	inline int compare0 (const token &, const token &);
	inline int compare1 (const token &, const token &);
	inline void extend (int howMany);
	inline void swap (int item1, int item2);
	inline void remove_symbol (std::vector<token> &, int i);
	inline int search_index (std::vector<token> &, char *);
	inline int search_regular_index (char *);
	inline int search_unique_index (char *);
	inline void sort_word (std::vector<token> &, int);
	
};


class frame;
class debug_stream;

class debug_stream
{
public:
	node_list<char*> N;
};

class lframe
{
private:
	static frame *m_pFrame;

public:
	static debug_stream *m_debugstr;  
	static frame *get()
	{ 
		return m_pFrame;
	};
	static void set_global();
	static void load_binary_file(char *);
	static LPARAM preprocess_data (char *);
};
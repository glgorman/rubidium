
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
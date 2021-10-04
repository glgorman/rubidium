
class key_list: public text_object
{
public:
	key_list ();
	key_list (char *m_pText);
	key_list (node_list<char *> *m_nList);
	key_list (text_object &m_nList);
	void rewind();
};

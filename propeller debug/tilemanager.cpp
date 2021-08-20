#include "stdafx.h"

class process_list;
class rectangle;
class render_proc;
class worker_proc;
class comm_proc;
class render_context;
class work_context;
class comm_context;

class rectangle
{
public:
	int	m_objectid;
	rectangle *m_owner;
	rectangle *m_sibling;
	rectangle *m_child1;
	rectangle *m_child2;
	int m_height;
	int m_width;
	int m_xpos;
	int m_ypos;
	void ((*render_proc)(render_context &));
	void ((*worker_proc)(work_context &));
	void ((*comm_proc)(comm_context &));
};

class display_list
{
public:
	int N;
	rectangle *m_deque;
};

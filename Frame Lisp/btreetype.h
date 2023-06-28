
class script;
long random ();

extern void to_upper (char*);
extern void to_lower (char*);
int compare (char *item1, char *item2);

class identifier;
template <class X> class bTreeType;
typedef char *datatype;

extern bTreeType<char*> *m_root;

#pragma warning (disable: 4812)
#pragma warning (disable: 4996)

using namespace std;

template <class X>
class bNodeType
{
public:
	bNodeType<X> *root;
	bNodeType<X> *branch1;
	bNodeType<X> *branch2;
	bNodeType<X> *markov;
	
	bNodeType ();
	~bNodeType ();
	void *bNodeType<X>::operator new (size_t,void*);

	bNodeType<X> *find_node (X &arg);
	char *get_data ()
	{
		char *result = NULL;
		return result;
	}
	bNodeType<X> *add_node (X &arg);
	void put_node (bNodeType<X> *(&));
	void del_tree (bNodeType<X> *);
	void trace_root (bNodeType<X> *(&found));
	int compare (const X &arg1, const X &arg2)
	{
		return ::compare (arg1,arg2); 
	}
};

template<class X>
bNodeType<X>::bNodeType<X> (void)
{
	root = NULL;
	branch1 = NULL;
	branch2 = NULL;
	markov = NULL;
}

template <class X> 
class bTreeType: public bNodeType<X> 
{
friend class bNodeType<X>;
protected:
	X m_pData;

public:
	const X &data () { return *(&m_pData); };

public:
	bTreeType (script*);
	bTreeType (X&, int count);
	bTreeType (X&, bool allocate);
	bTreeType ();
	~bTreeType ();

	bTreeType<X> *get_node (X&);
	bTreeType<X> *find_node (X&);
	bTreeType<X> *add_node (X&);
	bTreeType<X> *add_node (bTreeType<X>*);
	int reGraft (int count);

	bTreeType<X> *findMarkovian (X&);
	int index_markov (const X&);
	
private:
	bTreeType<X> *path (X&);
	void trace_root (bTreeType *(&));
	void openTestFile ();
	void saveTree (int depth);
	void indexMarkovians1 ();
	void closeTestFile ();


public:// code that works
	bTreeType<X> *next_node (bTreeType *previous);
	void recursiveTest ();
	void traversalTest ();
	int count_nodes ();
	void printTree (int depth);
	void mapTree (int depth);
	void indexMetasToHTML ();

public:// obselete and problematic code
	bTreeType<X> *add_tree (bTreeType<X> *);
	void link_node (X &);
};

template<class X>
void *bNodeType<X>::operator new (size_t sz1,void* ptr2)
{
#if 0
	size_t sz2;
	sz2 = sizeof(bNodeType<X>);
#endif

	bNodeType<X> *ptr;
	if (ptr2==NULL)
	{
		ptr = (bNodeType<X>*) malloc (sz1);
		return ptr;
	}
	else
		return ptr2;
}
//	Simple constructor that transfers the number of times
//	that a Node has been seen, i.e. from an index object
 
template<class X>
bTreeType<X>::bTreeType<X> (X &arg, int count)
{
	m_pData = arg;
	root = NULL;
	branch1 = NULL;
	branch2 = NULL;
	markov = NULL;
}

template<class X>
bNodeType<X>::~bNodeType<X> ()
{
// trivial destructor - TODO:
// add logging or debugging code
}

template<class X>
bTreeType<X>::bTreeType<X> (void)
{
	root = NULL;
	branch1 = NULL;
	branch2 = NULL;
	markov = NULL;
}

//	#define allocateNodes 1

//	givan a X *, this constructor copies the given m_pData
//	pointer to the constructed node, but does not add the
//	node to any tree.

template<class X>
bTreeType<X>::bTreeType<X> (X &arg, bool allocate)
{
	if (allocate==true) {
		m_pData = new char [strlen(arg)+1];
		strcpy (m_pData,arg); }
	else
	 m_pData = arg;
	root = NULL;
	branch1 = NULL;
	branch2 = NULL;
	markov = NULL;
}

//	Recrusive destructor, deletes every subtree of this
//	tree.  Need to make special provisions when	multiple
//	trees begin sharing leaf pointers, i.e. markov pointer
//	for example..

template<class X>
bTreeType<X>::~bTreeType<X> (void)
{
//	if (m_pData!=NULL)
//	{
//	delete [] m_pData;
//		m_pData=NULL;
//	}
	if (branch1!=NULL) {
		delete branch1;
		branch1=NULL; }
	if (branch2!=NULL) {
		delete branch2;
		branch2=NULL; }
	if (markov!=NULL) {
		del_tree (markov);
		delete markov;
		markov=NULL; }
}

//	navigate through a tree and nullify all references
//	to any leaves, so that when the destructor is called
//	they will be left alone!  The root has to be nulled
//	out at first because otherwise the routine will crawl
//	back into the main tree and start munching leaves,
//	hence causing a nasty access violation when the
//	destructor gets called again.

template<class X>
void bNodeType<X>::del_tree (bNodeType<X> *tree)
{
	tree->root = NULL;

	bNodeType<X> *position,*last;
	bTreeType<X> *pTree;
	last = NULL;
//	theTree->m_pData = NULL;
	tree->put_node (position);
	while (position!=NULL)
{
//		next = position->m_pNext (last);
		last = position;
		pTree = static_cast<bTreeType<X>*>(position);
		position = pTree->next_node(pTree);		
	}
}

//	recursive findNode routine.  Tries to find a
//	*X *in the bTree and returns the node where 
//	it was found or the the last node checked when 
//	the findNode bottomed out.  If the node to
//	find is bogus, i.e., NULL -- then the function
//	will fail, returning a NULL value.  If the
//	seach fails because of a NULL node in the tree,
//	then the assumption <for now> is that the bogus
//	leaf should be returned.

template<class X>
bNodeType<X> *bNodeType<X>::find_node (X &arg)
{
	bNodeType<X> *node;
	int test = 0;
	X data = get_data();
	test = compare (arg,data);
	if ((test<0)&&(branch1!=NULL))
	{
		node = static_cast<bTreeType<X>*>(branch1);
		node->find_node (arg);
	}
	else if ((test>0)&&(branch2!=NULL))
	{
		node = static_cast<bTreeType<X>*>(branch2);
		node->find_node (arg);
	}
	else
		node = this;
	return node;
}


template <class X>
void bTreeType<X>::trace_root (bTreeType *(&found))
{
	bTreeType *location = this;
	found = static_cast<bTreeType<X>*>(root);
	if (found!=NULL)
	do {
		if (found->branch2!=location)
			break;
		location = found;
		found = static_cast<bTreeType<X>*>(location->root); 
	}
	while (found!=NULL);
}

template<class X>
bTreeType<X> *bTreeType<X>::find_node (X &arg)
{
	bNodeType<X> *found;
	bTreeType<X> *node1;
	int test = 0;
#if 0
	cout << "bTreeType<X>::find_node(X &arg) RECURSIVE\n";
#endif
	try {
	test = compare (arg,m_pData);
	if ((test<0)&&(branch1!=NULL))
		found = branch1;
	if ((test>0)&&(branch2!=NULL))
		found = branch2;
	else
		found = this;
	
	if ((found!=NULL)&&(found!=this))
	{
		node1 = static_cast<bTreeType<X>*>(found);
		node1 = node1->find_node(arg);
	}
	}
	catch (...)	{
		if (m_pData==NULL)
			node1=this;
		else
			node1=NULL; }
	if (found==this)
		node1 = this;

	ASSERT (node1!=NULL);
	return node1;
}

template<class X>
bNodeType<X> *bNodeType<X>::add_node (X &arg)
{
	int test;
	bTreeType<X> *path, *path1, *path2, *node;
	X data;
	node = NULL;
	path = static_cast<bTreeType<X>*>(this);
retry:
	data = (static_cast<bTreeType<X>*>(path))->data();
	test = compare (arg,data);

#if 0
	cout << "bNodeType<X>::add_node(X &arg)\n";
	cout << " ... test = " << test << "\"";
#endif
	
	// does parallelization make this actually run
	// faster, i.e., on Pentium or Later?
	path1 = static_cast<bTreeType<X>*>(path->branch1);
	path2 = static_cast<bTreeType<X>*>(path->branch2);

	if ((test<0)&&(path1!=NULL))
	{
		path = static_cast<bTreeType<X>*>(path1);	
		goto retry;
	}
	if ((test>0)&&(path2!=NULL))
	{
		path = static_cast<bTreeType<X>*>(path2);	
		goto retry;
	}
	if ((test<0)&&(path1==NULL))
	{
		path1 = new (NULL) bTreeType<X> (arg,false);
#if 0
		cout << "allocated new bTreeType<X>\n";
#endif
		path1->root = path;
		node = static_cast<bTreeType<X>*>(path1);
#if 0
		cout << "path = branch1\n";
#endif
		goto finish;
	}
	if ((test>0)&&(path2==NULL))
	{
		path2 = new (NULL) bTreeType<X> (arg,false);
#if 0
		cout << "allocated new bTreeType<X>\n";
#endif
		path2->root = path;
		node = static_cast<bTreeType<X>*>(path2);
#if 0
		cout << "path = branch1\n";
#endif
		goto finish;
	}
	if (test==0)
		node = path;
finish:
	if (node->data()==NULL)
		node->m_pData = arg;

	return node;
}

//	Generic Routine looks for a location to add a Node and
//	then adds the node to the identified branch, creating a
//	new branch for the new Node as necessary.  When called
//	from addNode (bTreeType<X>*) find node gets called twice
//	but this should have no effect or loss of effiency, why?

#if 0
template<class X>
bNodeType<X> *bNodeType<X>::add_node (X &arg)
{
	int test;
	bNodeType<X> *path;
	X data = get_data();
	test = compare (arg,data);
	if ((test<0)&&(branch1==NULL)) {
		branch1 = new (NULL) bTreeType<X> (arg,false);
		branch1->root = this;
		path = branch1; }
	else if ((test>0)&&(branch2==NULL)) {
		branch2 = new (NULL) bTreeType<X> (arg,false);
		branch2->root = this;
		path = branch2; }
	else if (test!=0) {
		path = find_node (arg);
		path->add_node (arg); }
	else 
		path = this;

	bTreeType<X> *result = static_cast<bTreeType<X>*>(path);
	return result;
}
#endif

template<class X>
bTreeType<X> *bTreeType<X>::add_node (X &arg)
{
	int test;
	bNodeType<X> *path;
	bTreeType<X> *alloc;
	test = compare (arg,m_pData);
#if 0
	cout << "bTreeType<X>::add_node (X &arg)\n";
#endif
	try {
		if ((test<0)&&(branch1==NULL))
		{
			alloc = new (NULL) bTreeType<X> (arg,false);
			path = branch1 = alloc;
			branch1->root = this;
		}
		else if ((test>0)&&(branch2==NULL))
		{
			alloc = new (NULL) bTreeType<X> (arg,false);
			path = branch2 = alloc;
			branch2->root = this;
		}
		else if (test!=0) {
			path = find_node (arg);
			path->add_node (arg); }
		else 
			path = this;
	}
	catch (X) {
		if (m_pData==NULL) {
			m_pData = arg; }
		}
	bTreeType<X> *result = static_cast<bTreeType<X>*>(path);
	return result;
}

//	Straightforward routine to add a bTreeType to a tree
//	in whatever natural loaction is	returned by findNode
//	This addNode is different from the other version, in
//	that that version must create the node from an object
//	and then add it.

template<class X>
bTreeType<X> *bTreeType<X>::add_node (bTreeType<X> *arg)
{
#if 0
	cout << "bTreeType<X>::add_node (bTreeType<X> *arg)\n";
#endif
	bTreeType *location = NULL;
	location = find_node (arg->m_pData);
	location->add_node (arg->m_pData);
	ASSERT (location!=NULL);

	return location;
}


//	searches a binary tree for a Node, adds the Node if
//	it is not found, otherwise increment the instance
//	count. <instance count no longer supported>

template<class X>
bTreeType<X> *bTreeType<X>::get_node (X &arg)
{
#if 0
	cout << "bTreeType<X>::get_node (X &arg)\n";
#endif
	X leaf = NULL;
	bTreeType *branch, *location = NULL;
	branch = find_node (arg);
	location = branch;
	ASSERT (location!=NULL);

	if ((m_pData==NULL)||(compare(arg,m_pData)!=NULL))
		location->add_node (arg);

#if 0
	cout << "added node " << arg << "\n";
#endif

	return location;
}

//	obtain a pointer indicating the path to take to get
//	to a particular object.

template<class X>
bTreeType<X> *bTreeType<X>::path (X &arg)
{
#if 0
	cout << "bTreeType<X>::path (X &arg)\n";
#endif
	bTreeType<X> *thePath = this;
	int test;
	if (arg!=NULL)
	do {
		m_pData = thePath->m_pData;
		if (m_pData==NULL)
			return NULL;
		test = compare (arg,m_pData);
		if (test<0)
			thePath = thePath->branch1;
		else if (test>0)
			thePath = thePath->branch2;
		if (thePath==NULL)
			break;
	}
	while (test!=0);
	else thePath=NULL;
	return thePath;
}

template <class X>
bTreeType<X> *bTreeType<X>::next_node (bTreeType *previous)
{
//	cout << "bTreeType<X>::next_node (bTreeType *previous)\n";
	bTreeType<X> *result = NULL;
	bNodeType<X> *path;
	if (branch2==NULL)
		trace_root (result);
	else if (previous!=branch2)
	{
		branch2->put_node (path);
		result = static_cast<bTreeType<X>*>(path);
	}
	else
		trace_root (result);

	if (result==this)		// OOPS!
		result = static_cast<bTreeType<X>*>(root);
	return result;
}

//	The rule is "always turn right" at every corner.
//	First obtain the root of this node and then follow
//	upward until a node is found where we passed on
//	through the first time, i.e. a node that has a
//	branch one that we already took!

template <class X>
void bNodeType<X>::trace_root (bNodeType *(&found))
{
	bTreeType<X> *location = this;
	found = root;
	if (found!=NULL)
	do {
		if (found->branch2!=location)
			break;
		location = found;
		found = location->root; }
	while (found!=NULL);
}

//	Non-recursive method to seek out the bottom left node
//	of a binary tree.

template <class X>
void bNodeType<X>::put_node (bNodeType<X> *(&location))
{
	location = this;
	while (location->branch1!=0)
		location = location->branch1;
}

//	this is really screwed up right now, the idea is
//	to splice in another tree in the correct spot under
//	this one.  Useful for optimizing trees.

template<class X>
bTreeType<X> *bTreeType<X>::add_tree (bTreeType<X> *aTree)
{
	int depth = 0;
	int test;
	X arg = aTree->m_pData;
	bTreeType<X> *location;
	test = ::compare (arg,m_pData);
	if (test==0)
		location = root;
	else if (test>0)
		location = branch1;
	else 
		location = branch2;	
	if (location!=NULL)
		location = location->findNode (arg);
	else {
		location = aTree;
		location->root = this;
	}
	return location;
}

//	This routine traces out a tree in lexical order, and
//	it puts in parenthesis to indicate recursion depth
//	This might be useful for reconstructing trees from
//	disk.

#define DEBUG_STR cout
template<class X>
void bTreeType<X>::printTree (int depth)
{
	DEBUG_STR << "(";
	if (branch1==NULL)
		DEBUG_STR << "NULL";
	else
		(static_cast<bTreeType<X>*>(branch1))->printTree (depth+1);
	DEBUG_STR << ", ";
	if (m_pData!=NULL)
		DEBUG_STR << "\"" << m_pData << "\", ";
	else
		DEBUG_STR << "<NULL>, ";
	if (branch2==NULL)
		DEBUG_STR << "NULL";
	else
		(static_cast<bTreeType<X>*>(branch2))->printTree (depth+1);
	DEBUG_STR << ")";
}

template<class X>
void bTreeType<X>::openTestFile ()
{		
	m_the_file->open ("bTreeTest.txt",ios::in|ios::out);
}

template<class X>
void bTreeType<X>::saveTree (int depth)
{
	if (branch1!=NULL)
		branch1->saveTree (depth+1);
//	for (int i=0;i<depth;i++)
//		theFile << "*";
	theFile << " " << m_pData << " [" << times;
	if (branch1!=NULL)
		theFile << "," << branch1->m_pData;
	if (branch2!=NULL)
		theFile << "," << branch2->m_pData;
	theFile << "]\n";
	if (branch2!=NULL)
		branch2->saveTree (depth+1);	
}

template<class X>
void bTreeType<X>::closeTestFile ()
{
	m_the_file->close ();
}

template<class X>
void bTreeType<X>::recursiveTest ()
{
	openTestFile ();
	saveTree (0);
	closeTestFile ();
}

template<class X>
int bTreeType<X>::reGraft (int count)
{
	bTreeType<X> *child1, *child2, *parent;
	child1 = branch1;
	child2 = branch2;
	parent = root;

//	Following child1 until we bottom out.

	if (child1!=NULL)
		count = child1->reGraft (count);
	
//	Arriving at the current node.  Either child1
//	is null or we have already been there and done
//	that, i.e. we have printed out the list.

	count++;
//	DEBUG_STR << count << " " << m_pData << "\n";

//	The nice thing is that we never have to go
//	down that path again!  So we are free to
//	reassign it is necessary!

	if (child2!=NULL)
		count = child2->reGraft (count);
	return count;
}

//	Routine of general utility for finding the bottom
//	left branch of a tree.

template<class X>
int bTreeType<X>::count_nodes ()
{
	int loopCount = 0;
	DEBUG_STR << "\nTraversal Test -- ";
	bTreeType<X> *lastNode = this;
	bTreeType<X> *next;
	bNodeType *position;
	put_node (position);
	while (position!=NULL) {
		m_pData = (static_cast<bTreeType<X>*>(position))->m_pData;
		if (m_pData!=0)
		{
			cout << "\"" << m_pData << "\", ";
			loopCount++;
			next = (static_cast<bTreeType<X>*>(position))->next_node (lastNode);
		}
		lastNode = static_cast<bTreeType<X>*>(position);
		position = next;
	}
	return loopCount;
}

//	used to find the root node of a markov whose
//	penultimate/pred token is the same as theWord

template<class X>
bTreeType<X> *bTreeType<X>::findMarkovian (X &theWord)
{
	bTreeType<X> *rootNode, *m_pNext = NULL;
	if (theWord!=NULL) {
		rootNode = m_root->findNode (theWord);
		m_pNext = rootNode->markov;
	}
	return m_pNext;
}


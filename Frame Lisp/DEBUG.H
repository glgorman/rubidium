

#include "stdafx.h"
#include "defines.h"

#include <iostream>
#include <fstream>

#define theConsole cout

template <class nodeType>
class debugTest
{
public:
	void openDebugFiles ();
	void closeDebugFiles ();
	
	void mapTree (bTreeType<nodeType>*);
	void traversalTest (bTreeType<nodeType>*);
	void spewMarkovian (bTreeType<nodeType>**);
};

//	Visits the tree in base traversal order and
//	prints a *to show the depth of each point.

template <class X> void bTreeType<X>::mapTree (int depth)
{
//	reGraft (root);
	if (branch1!=NULL)
		branch1->mapTree (depth+1);
//	for (int i=0;i<depth;i++)
//		theConsole << "*";
	if (theLeaf!=NULL) {
		theConsole << " " << theLeaf << " [" << times;
	if (branch1!=NULL)
		theConsole << "," << branch1->theLeaf;
	if (branch2!=NULL)
		theConsole << "," << branch2->theLeaf;
	theConsole << "]\n";
	if (branch2!=NULL)
		branch2->mapTree (depth+1);
	}
}


template <class X>
void debugTest<X>::spewMarkovian (bTreeType<X> **where)
{
	X spew;
	textObject theMarkovian = where;
	theMarkovian >> spew;
	theConsole << spew << "\n";
	delete [] spew;;
}

template <class X>
void debugTest<X>::openDebugFiles ()
{
	preIndex.open ("preIndex.txt",ios::out|ios::in,0);
	debugOut.open ("postIndex.txt",ios::out|ios::in,0);
}

template <class X>
void debugTest<X>::closeDebugFiles ()
{
	preIndex.close ();
	debugOut.close ();
}

template <class X>
void debugTest<X>::mapTree (bTreeType<X> *theTree)
{
//	char aChar;
	theTree->mapTree (0);
	theConsole << "Does this tree look ok to you?\n";
//	cin >> aChar;
}

template <class X>
void debugTest<X>::traversalTest (bTreeType<X> *theTree)
{
	int loopCount = 0;
	X ascii;
	theConsole << "bTree Trversal Test -- ";
	bTreeType<X> *lastNode = theTree;;
	bTreeType<X> *next,*position;
	try {
	
	if (theTree==NULL)
		throw (theTree);

	theTree->putNode1 (position);
	while (position!=NULL) {
		ascii = position->theLeaf;
		if (ascii!=0) {
	debugOut << position->theLeaf << "\n";

#ifdef spewNodes
	theConsole << "bTree node at ";
	theConsole << (long) position << "\n";
	theConsole << position->theLeaf << "\n";
	theConsole << (long) position->root << "\n";
	theConsole << (long) position->branch1 << "\n";
	theConsole << (long) position->branch2 << "\n";
	theConsole << (long) position->markovian << "\n\n";
#endif
			loopCount++; }
		next = position->nextNode (lastNode);
		lastNode = position;
		position = next;		
	}
	debugOut << "Total Labels = " << loopCount << "\n";
	theConsole <<"Total Labels = " << loopCount << "\n";
		}
	catch (bTreeType<X>*)
	{
	debugOut << "Can't Traverse NULL tree!\n";
	theConsole << "Can't Traverse NULL tree!\n";
	}
}

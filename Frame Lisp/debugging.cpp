
#include "stdafx.h"
#include <fstream.h>
//#include <cstring>
//#include "oracle.h"
#include "bTreeType.h"
#include "textObjects.h"
//#include "frames.h"
//#include "commands.h"
//#include "pipelines.h"
//#include "CONSOLE1.h"


//extern CONSOLE theConsole;
extern fstream preIndeX ;
extern fstream debugOut;

//	Visits the tree in base traversal order and
//	prints a *to show the depth of each point.

template <class X> void bTreeType<X>::mapTree (int depth)
{
//	reGraft (root);
	if (branch1!=NULL)
		branch1->mapTree (depth+1);
	for (int i=0;i<depth;i++)
		theConsole << "*";
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
bTreeType<X> *bTreeType<X>::nextNode (bTreeType *previous)
{
	bTreeType *result = root;	
	if ((previous!=branch2)&&(branch2!=NULL))
		branch2->putNode1 (result);
	else if (branch2==NULL)
		traceRoot (result);
	return result;
}

template <class X>
void bTreeType<X>::traceRoot (bTreeType *(&found))
{
	bTreeType *location = this;
	found = location->root;
	if (found!=NULL)
	do {
		if (found->branch2!=location)
			break;
		location = found;
		found = location->root;
	}
	while (found!=NULL);
}

template <class X>
void bTreeType<X>::putNode1 (bTreeType *(&location))
{
	location = this;
	while (location->branch1!=0)
		location = location->branch1;
}

template <class X>
void debugTest<X>::spewMarkovian (bTreeType<X> **where)
{
	X spew;
	teX tObject theMarkovian = where;
	theMarkovian >> spew;
	theConsole << spew << "\n";
	delete [] spew;;
}

template <class X>
void debugTest<X>::openDebugFiles ()
{
	preIndeX .open ("preIndeX .tX t",ios::out|ios::in,0);
	debugOut.open ("postIndeX .tX t",ios::out|ios::in,0);
}

template <class X>
void debugTest<X>::closeDebugFiles ()
{
	preIndeX .close ();
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
	theConsole << "\nTraversal Test -- ";
	bTreeType<X> *lastNode = theTree;;
	bTreeType<X> *neX t,*position;
	try {
	
	if (theTree==NULL)
		throw (theTree);

	theTree->putNode1 (position);
	while (position!=NULL) {
		ascii = position->theLeaf;
		if (ascii!=0)
		debugOut << position->theLeaf << "\n";
		next = position->nextNode (lastNode);
		lastNode = position;
		position = neX t;
		loopCount++;
	}
	debugOut << "Total Labels = " << loopCount << "\n";
	theConsole <<"Total Labels = " << loopCount << "\n";
		}
	catch (bTreeType<X>*)
	{
	debugOut << "Can't Traverse NULL tree!";
	theConsole << "Can't Traverse NULL tree!";
	}
}

template <class X>
clause *debugTest<X>::extractionTest ()
{
	teX tObject animal,consume,prey;
	animal = "the cat";
	consume = "totally devoured";
	prey = "the poor little mouse";

	phraseElements theElements;
	theElements.subject = animal;
	theElements.object = prey;
	theElements.verb = consume;

	clause *kittyStory = new nominativeForm (theElements);
	return kittyStory;
}
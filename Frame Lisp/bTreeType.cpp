
#include "stdafx.h"
#include "afxmt.h"
#include "defines.h"
#include <fstream>

using namespace std;

#include "symbol_table.h"
#include "bTreeType.h"

#if 0
#include "keywords.h"

extern fstream debugOut, preIndex;
bTreeType<char*> *theRoot;

class TRAP
{
	CCriticalSection critical;
public:
	void wait ();
	void resume ();
};

void TRAP::wait ()
{
	critical.Lock ();
	critical.Unlock ();
}

void TRAP::resume ()
{
	critical.Unlock ();
}

TRAP trap;
#endif

//	simple string comparison called from
//	bTreeNode routines -- throws an exception
//	if either argument is NULL;

int compare (char *item1, char *item2)
{
	char buffer1 [64], buffer2 [64];
	int result;
	try {
		if ((item1==NULL)||(item2==NULL))
			throw ((char*)NULL);
		strcpy (buffer1,item1);
		strcpy (buffer2,item2);
		to_lower (buffer1);
		to_lower (buffer2);
		result = strcmp (buffer1,buffer2);
		return result;
		}
	catch (char* item) {
//		cout << "Null error in bTreeTypes comparison function.\n";
		throw item;
	}
}








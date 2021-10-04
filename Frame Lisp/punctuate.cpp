
#include "defines.h"
#include "stdafx.h"

namespace punct
{
	void capPronouns (char *a);
	void capFirstWords (char *);
	void fixDoubleSpace (char *);
	void fixContractions (char *);
	void fixDelimiters (char *);
	void removeReturns (char *);
	void removeChar (char *, int);
	bool isContract (char theChar);
};

void punctuate (char *aText)
{
	if (aText!=NULL) {
		punct::fixDelimiters (aText);
		punct::capPronouns (aText);
		punct::capFirstWords (aText);
		punct::fixContractions (aText);
		punct::removeReturns (aText);
		punct::fixDoubleSpace (aText); }
}

void punct::removeChar (char *theString,int i)
{
	int j;
	int length = strlen (theString);
	for (j=i;j<length;j++) {
		theString [j]=theString [j+1];
	}
	theString [j] = 0;
}

void punct::capFirstWords (char *sentence)
{
	bool isPunct;
	char theChar;
	int i, length;	
	while (sentence [0]==' ')
		removeChar (sentence,0);
	length = strlen (sentence);
	i = 0;
	sentence [0] = toupper (sentence[0]);
	while (sentence [i]!='\0')
	{
		theChar = sentence [i];
		isPunct = ((theChar=='.')||(theChar=='!')||
			(theChar=='?'));
	
		if (isPunct==true)
			do {
				i++;
				theChar = sentence [i];
				theChar = toupper (theChar);
				sentence [i] = theChar;
				}
			while (theChar==' ');
		i++;
	}
}

void punct::capPronouns (char *sentence)
{
	int i=0, length;
	length = strlen (sentence);

	for (;i<length; i++) {
//	also capitalize the token I:

		sentence [i] = tolower (sentence[i]);
		if (sentence [i]=='i') {
			if (sentence[i-1]==' '&&sentence[i+1]==' ')
				sentence [i] = toupper (sentence [i]);
		}
	}
}

//	remove space in front of or behind apostrophe

bool punct::isContract (char theChar)
{
	bool result;
	if ((theChar=='\'')||(theChar=='-')||(theChar=='/'))
		result = true;
	else
		result = false;
	return result;
}

void punct::fixContractions (char *sentence)
{
	bool symbol1,symbol2;
	char char1, char2;
	int i, length = strlen (sentence);
	i = 0;
	while (sentence [i]!='\0') {
		char1 = sentence [i];
		char2 = sentence [i+1];		
		symbol1 = isContract (char1);
		symbol2 = isContract (char2);

		if ((char1==' ')&&symbol2)
			removeChar (sentence,i);
		else if (symbol1&&(char2==' '))
			removeChar (sentence,i+1);
		else
			i++;		
	}
}

void punct::fixDelimiters (char *sentence)
{
//	remove space in front of comma or period or semi-colon
//	or question mark or any bracket or parenthesis,

	char theChar,prevChar;
	int length = strlen (sentence);
	for (int i=1; i<length; i++) {
		prevChar = sentence [i-1];
		theChar = sentence [i];
		if ((prevChar==' ')&&((theChar==';')
		||(theChar==']')||(theChar==')')
		||(theChar=='?')||(theChar==':')
		||(theChar==',')||(theChar=='!')
		||(theChar==';')||(theChar=='.')))
		removeChar (sentence,i-1);
	}
}

void punct::fixDoubleSpace (char *sentence)
{
	int i = 0, length = strlen (sentence);
	while (i<length-1) {
		if ((sentence [i]==' ')&&(sentence [i+1]==' '))			
			removeChar (sentence,i);
		else
			i++;
		length = strlen (sentence); }
}

void punct::removeReturns (char *sentence)
{
	int length = strlen (sentence);
	for (int i=0; i<length; i++) {
		if (sentence [i]=='\n')
			sentence [i] = ' '; }
}


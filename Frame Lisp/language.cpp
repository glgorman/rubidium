
#include "stdafx.h"
#include "defines.h"
#include <cstring>
#include "language.h"
#include "framelisp.h"

char *colors [] =
{
	"red","orange","yellow","green","blue","violet","white"
	"black","brown","pink","magenta","teal","grey","crimson",
	"sweet"
};

char *flavors [] =
{
	"salty","ripe","bitter","sweet","sour",
	"spicy","#"
};

char *quantifiers [] =
{
	"all","few","very","any","many","some","few",
	"least","most","#"
};

char *adjectives [] =
{
	"rare","common","friendly","good","bad","ugly","sexy",
	"hot","cold","beautifal","sure",
	"sinful","fun","awful","reasonable","impossible","quaint",
	"viscious","pleasant","rare","spiritual","evil","calm",
	"windy","sunny","silent","quiet","noisy","fresh","rotten",
	"messy","nice","neat","smart","slimy","casual",
	"formal","putrid","gross","dark","bright","fabulous",
	"spendid","terrible","wonderful","intrepid","bold",
	"wild","terse","deep","shallow","obtuse","acute",
	"naked","bloody","true","false","just","capitol",
	"just","ever","very","better","good","#"
};

char *affirmatives [] =
{
	"agree","alright","right","of course","yes","#"
};

char *beVerbs [] =
{
	"am","are","be","had","have","has","is",
	"was","were","isnt","wasnt","arent","aint",
	"been","#"
};

char *comparatives [] =
{
	"about","approximately","equal","equivalent",
	"less","like","more","same","similar","#"
};

char *commonVerbs [] =
{	
	"do","did","does","has","can","have","get","gets",
	"take","takes","put","puts","find","finds",
	"took","made","bought","likes","loves","smells",
	"brought","said","went","sold","lost","taught",
	"pick","select","think","go","been",

	"change","know",
	"like","live","love","eat","ate","see","saw",
	"hear","heard","felt","smell","tried",
	"made","program","ran","walk","walks",
	"wore","drank","cut","pull","know"
	"pull","stole","gave","give","grope",
	"watch","piss","sank","swear","laugh",
	"sneak","suggest","sail","swore","lied",
	"told","bore","camp","dive","chomp",
	"digest","form","pray","create","choking",
	"destroy","stare","waste","pour",
	"stop","broke","bang","snap","spent"
	"smash","drove","screw","solve","trust",
	"abstain","remove","remain","sang",
	"kiss","marry","date","engage","spoke",
	"said","shout","yell","crush","divide",
	"add","enslave","encounter","dance",
	"knife","grab","untie","repair","drew"
	"paint","fix","bake","cook","plant",
	"remember","clos","open",
	"spent","earn","dug","fill","drill",
	"glued","tap","fir","drown","cook",
	"froze","heat","warm","forgot","fathom"
	"imply","test","inspire","sew","sav",
	"edit","record","paste","copy",
	"evaluat","kill","rob","murder",
	"rape","molest","catpure","solder",
	"refine","destroy","bury","caught",
	"guide","heal","inspire","breathe",
	"warn","wish","bleed","pour",
	"require","grade","evaluate","knew",
	"knot","fought","battle","harvest",
	"stich","serve","clean","snatch",
	"print","format","type","wrote",
	"booked","arrest","execute","pick",
	"elect","embody","watch","wait",
	"read","analyze","synthesize","fry",
	"boiled","crack","stir","savor",
	"balance","dry","wash","press",
	"consider","examine","mix","play",
	"purchase","invent","built","hammer",
	"decide","hunt","hunt","scared",
	"scare","call","answer",
	"run","die","wreck","run",
	"shot","grab","net","earn",
	"show","show","jump","hide","win",
	"skate","sure","say","may","try","tell",
	"describe","left","gives",
	"#"
};

char *idVerbs [] =
{
	"believe","care","feel","love","loved","hope",
	"crave","craves","desire","desires","get","gets",
	"lust","like","admire","admired","liked",
	"envy","envied","need","needs","needed",
	"want","wants","cried","cry","require",
	"request","prefer","hunger","thirst","#"
};

char *proVerbs [] =
{
	"always","can","cant","could","didnt","dont",
	"might","must","never","occasionaly"
	"shall","should","would","will","there",
	"wont","shouldnt","shall","#"
};

char *conditionals [] =
{
	"if","when","whenever","until","while",
	"then",
		"#"
};

char *conjunctives [] =
{
	"and","but","or","so","yet","because","#"
};

char *determiners [] =
{
	"a","an","all","any","each","every","most",
	"much","only","some","another",
	"that","the","them","these","this","those",
	"various","more","few","enough","much",
	"little",
	"one","two","three","four","five","six",
	"seven","eight","nine","ten","#"
};

char *interjectives [] =
{
	"well","hey","lets","#"
};

char *nominatives [] =
{
	"that","which","of","about","with","having",
	"#"
};

char *interrogatives [] =
{
	"aint","am","are","arent","can","cant",
	"could","did","didnt","do","dont","does",
	"how","is","must","might","shall","should",
	"shouldnt","was","were","what","when","where",
	"who","why","will","would","wont",
	"#"
};

char *negatives [] =
{
	"doesnt","dont",
	"cant","no","not","wont","didnt","#"
};

char *possessives [] =
{
	"my","his","her","hers","our",
	"their","your","its","#"
};

char *infinitives [] =
{
	"to","#"
};

char *prepositions [] =
{
	"about","among","along","also","as","at",
	"by","down","for","in","into","from","of",
	"off","like","near","on","onto","out",
	"over","beside","within","without","than",
	"to","too","toward","up","upon","which","under",
	"with","#"
};

char *pronouns [] =
{
	"anyone","anybody",
	"i","everybody","everyone","he","she","they","you",
	"me","somebody","someone","we","it","us","whom",
	"#"
};

char *simpleActions [] =
{
	"do","did","does","has","can","have","get","gets",
	"take","takes","put","puts","find","finds",
	"found","went","had","#"
};

char *delimiters [] = 
{
	".","!","?","#"
};

char *modifiers [] = 
{
	"@","$","%","^","&","#",
};

char *connectors [] = 
{
	"-","_","#",
};

char *separators [] = 
{
	".", ",", ":", ";", "!", "?","\n","\t","#"
};

bool grammar::isParticularType (char *is_in, char **theTypeList)
{
	int count = 0;
	char *possible, isThis [128];
	if (is_in==NULL)
		return false;
	strcpy_s (isThis,128,is_in);
	toLower (isThis);
	do {
		possible = theTypeList [count];
		int test = strcmp (possible,isThis);
		if (test == 0)
			return true;
		count++;
	}
	while (possible [0]!='#');
	return false;
};

bool grammar::is_delimiter (char *is_in)
{
	bool result; 
	result = isParticularType (is_in,delimiters);
	return result;
}

bool grammar::isSeparator (char *is_in)
{
	bool result; 
	result = isParticularType (is_in,separators);
	return result;
}

bool grammar::isAdjective (char *is_in)
{
	bool result; 
	result = isParticularType (is_in,affirmatives);
	if (result==false)
		result = isParticularType (is_in,quantifiers);
	if (result==false)
		result = isParticularType (is_in,adjectives);
	return result;
}

bool grammar::isAffirmative (char *is_in)
{
	bool result; 
	result = isParticularType (is_in,affirmatives);
	return result;
}

bool grammar::isBeVerb (char *is_in)
{
	bool result; 
	result = isParticularType (is_in,beVerbs);
	return result;
}

bool grammar::isCommonVerb (char *is_in)
{
	bool result; 
	result = isParticularType (is_in,commonVerbs);
	return result;
}

bool grammar::isIdVerb (char *is_in)
{
	bool result; 
	result = isParticularType (is_in,idVerbs);
	return result;
}

bool grammar::isConditional (char *is_in)
{
	bool result; 
	result = isParticularType (is_in,conditionals);
	return result;
}

bool grammar::isProVerb (char *is_in)
{
	bool result; 
	result = isParticularType (is_in,proVerbs);
	return result;
}

bool grammar::isConjunctive (char *is_in)
{
	bool result; 
	result = isParticularType (is_in,conjunctives);
	return result;
}

bool grammar::isComparative (char *is_in)
{
	bool result; 
	result = isParticularType (is_in,comparatives);
	return result;
}

bool grammar::isDeterminer (char *is_in)
{
	bool result; 
	result = isParticularType (is_in,determiners);
	return result;
}

bool grammar::isGerund (char *is_in)
{
	bool result = false;
	int length;

	length = strlen (is_in);
	
	if (length>4)
	if (is_in[length-3]=='i')
	if (is_in[length-2]=='n')
	if (is_in[length-1]=='g')
		result = true;
	else
		result = false;

	return result;
}

bool grammar::isPastVerb (char *is_in)
{
	bool result = false;
	int length;
	length = strlen (is_in);	
	if (length>4)
	if (is_in[length-2]=='e')
	if (is_in[length-1]=='d')
		result = true;
	else
		result = false;
	return result;
}

bool grammar::isAdverb (char *is_in)
{
	bool result = false;
	int length;
	length = strlen (is_in);	
	if (length>4)
	if (is_in[length-2]=='l')
	if (is_in[length-1]=='y')
		result = true;
	else
		result = false;
	return result;
}

bool grammar::isInterrogative (char *is_in)
{
	bool result; 
	result = isParticularType (is_in,interrogatives);
	return result;
}

bool grammar::isNegative (char *is_in)
{
	bool result; 
	result = isParticularType (is_in,negatives);
	return result;
}

bool grammar::isInterjective (char *is_in)
{
	bool result; 
	result = isParticularType (is_in,interjectives);
	return result;
}

bool grammar::isInfinitive (char *is_in)
{
	bool result; 
	result = isParticularType (is_in,infinitives);
	return result;
}

bool grammar::isPossessive (char *is_in)
{
	bool result; 
	result = isParticularType (is_in,possessives);
	return result;
}

bool grammar::isPronoun (char *is_in)
{
	bool result; 
	result = isParticularType (is_in,pronouns);
	return result;
}

bool grammar::isPreposition (char *is_in)
{
	bool result; 
	result = isParticularType (is_in,prepositions);
	return result;
}

bool grammar::isNominative (char *is_in)
{
	bool result; 
	result = isParticularType (is_in,nominatives);
	return result;
}

bool grammar::isNumeric (char *is_in)
{
	char theChar;
	bool result = true;
	unsigned int n, j = strlen (is_in);
	for (n=0;n<j;n++)
	{
		theChar = is_in [n];
		if ((theChar<'0')||(theChar>'9')) {
			result=false;
			break;
		}
	}
	if (j==1) {
		theChar = is_in [0];
		if ((theChar=='+')||
		(theChar=='-')||
		(theChar=='*')||
		(theChar=='=')||
		(theChar=='/'))
		result = true;
	}
	return result;
}

bool grammar::isOpcode (char *is_in)
{
	char theChar;
	bool result = false;
	unsigned  j = strlen (is_in);
	if (j==1) {
		theChar = is_in [0];
		if ((theChar=='+')||
		(theChar=='-')||
		(theChar=='*')||
		(theChar=='=')||
		(theChar=='/'))
		result = true;
	}
	return result;
}

grammar::gtype grammar::whatVerb (char *ascii)
{
	gtype result;
	char isThis [1024];

	strcpy_s (isThis,1024,ascii);
	toLower(isThis);
	if (isAdverb (isThis)==true)
		result = adverb;
	else if (isIdVerb (isThis)==true)
		result = idVerb;	
	else if (isBeVerb (isThis)==true)
		result = beVerb;
	else if (isProVerb (isThis)==true)
		result = proVerb;	
	else if (isCommonVerb (isThis)==true)
		result = commonVerb;
	else if (isPastVerb (isThis)==true)
		result = commonVerb;
	return result;
}

bool grammar::isAnyVerb (char *ascii)
{
	bool result;
	char isThis [1024];
	strcpy_s (isThis,1024,ascii);
	toLower(isThis);
	if ((isAdverb (isThis)==true)||
		(isIdVerb (isThis)==true)||
		(isBeVerb (isThis)==true)||
		(isProVerb (isThis)==true)||	
		(isCommonVerb (isThis)==true)||
		(isPastVerb (isThis)==true))
		result = true;
	else 
		result = false;
	return result;
}

grammar::gtype grammar::whatIsThis (char *ascii)
{
	char *isThis = ascii;
	gtype result;	
	if (strcmp(isThis," ")==0)
		result = space;
	else if (isDeterminer (isThis)==true)
		result = determiner;
	else if (isInfinitive (isThis)==true)
		result = infinitive;
	else if (isPronoun (isThis)==true)
		result = pronoun;
	else if (isConjunctive (isThis)==true)
		result = conjunctive;
	else if (isAnyVerb (isThis)==true)
		result=whatVerb (isThis);
	else if (isAdjective (isThis)==true)
		result = adjective;	
	else if (isConditional (isThis)==true)
		result = conditional;
	else if (isComparative (isThis)==true)
		result = comparative;
	else if (isAffirmative (isThis)==true)
		result = affirmative;
	else if (isNominative (isThis)==true)
		result = nominative;
	else if (isPossessive (isThis)==true)
		result = possessive;
	else if (isNegative (isThis)==true)
		result = negative;
	else if (isPreposition (isThis)==true)
		result = preposition;
	else if (is_delimiter (isThis)==true)
		result = delimiter;
	else if (isSeparator (isThis)==true)
		result = separator;
	else if (isGerund (isThis)==true)
		result = gerund;
	else if (isInterrogative (isThis)==true)
		result = interrogative;
	else if (isInterjective (isThis)==true)
		result = interjective;
	else
		result = unknown1;	
	return result;
}

void grammar::pToName (char *result, gtype aType)
{
	switch (aType)
	{
		case adjective:
			strcpy_s (result,1024,"adjective");
			break;
			
		case adverb:
			strcpy_s (result,1024,"adverb");
			break;

		case affirmative:
			strcpy_s (result,1024,"affirmative");
			break;

		case beVerb:
			strcpy_s (result,1024,"beVerb");
			break;

		case commonVerb:		
			strcpy_s (result,1024,"commonVerb");
			break;
		
		case idVerb:
			strcpy_s (result,1024,"idVerb");
			break;

		case comparative:
			strcpy_s (result,1024,"comparative");
			break;

		case proVerb:		
			strcpy_s (result,1024,"proVerb");
			break;

		case conditional:
			strcpy_s (result,1024,"conditional");
			break;

		case conjunctive:	
			strcpy_s (result,1024,"conjunctive");
			break;

		case determiner:		
			strcpy_s (result,1024,"determiner");
			break;

		case gerund:			
			strcpy_s (result,1024,"gerund");
			break;

		case interrogative:	
			strcpy_s (result,1024,"interrogative");
			break;

		case negative:		
			strcpy_s (result,1024,"negative");
			break;

		case possessive:		
			strcpy_s (result,1024,"possessive");
			break;

		case preposition:	
			strcpy_s (result,1024,"preposition");
			break;

		case pronoun:		
			strcpy_s (result,1024,"pronoun");
			break;

		case space:			
			strcpy_s (result,1024,"space");
			break;

		case delimiter:		
			strcpy_s (result,1024,"delimiter");
			break;

		default:
			strcpy_s(result,1024,"checkType");
	}
}

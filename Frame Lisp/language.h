
void toUpper (char* aWord);
void toLower (char* aWord);
bool isalphanum (char theChar);

namespace grammar
{
	typedef enum
	{	
	unknown1,determiner, adjective, gerund,
	comparative, conjunctive,
	nominative, possessive, infinitive,
	preposition, noun, pronoun,
	adverb, beVerb, commonVerb,
	pastVerb,idVerb, proVerb,
	delimiter, space, separator, unknown,
	affirmative, negative, conditional,
	interrogative,interjective,
	expletive, emotive, transitive,
	singularForm, pluralForm, masculine, feminine,
	pastTense, presentTense, futureTense,
	}
	gtype;

	grammar::gtype whatIsThis (char *ascii);
	grammar::gtype whatVerb (char *is_in);

	bool isAnyVerb (char *ascii);
	void pToName (char *result, gtype aType);
	bool isCommand (char *is_in);
	bool isInterrogative (char *is_in);
	bool isNumeric (char *is_in);
	bool isOpcode (char *is_in);
	bool isParticularType (char *is_in, char **theTypeList);
	bool isInterjective (char *is_in);
	bool isBeVerb (char *is_in);
	bool isCommonVerb (char *is_in);
	bool isIdVerb (char *is_in);
	bool isConditional (char *is_in);
	bool isProVerb (char *is_in);
	bool isAdjective (char *is_in);
	bool isAffirmative (char *is_in);
	bool isConjunctive (char *is_in);
	bool isComparative (char *is_in);
	bool isDeterminer (char *is_in);
	bool isInfinitive (char *is_in);
	bool isNominative (char *is_in);
	bool isGerund (char *is_in);
	bool isAdverb (char *is_in);
	bool isNegative (char *is_in);
	bool isPossessive (char *is_in);
	bool isPronoun (char *is_in);
	bool isPreposition (char *is_in);
	bool is_delimiter (char *is_in);
	bool isSeparator (char *is_in);
	bool isPastVerb (char *is_in);
};

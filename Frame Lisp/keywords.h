
#include "text_object.h"

typedef enum language;

class key_word
{
public:
	source_type identify_program (char *ascii);
	void pToName (char *result, language aType);

	bool is_command (char *is_in);
	bool is_numeric (char *is_in);
	bool is_opcode (char *is_in);
	bool is_delimiter (char *is_in);
	bool is_seperator (char *is_in);

private:
	bool is_whitespace (char *is_in);
	bool is_type (char *is_in, char **theTypeList);
	bool is_conditional (char *is_in);
	bool is_function (char *is_in);
	bool is_symbol (char *is_in);
	bool is_control (char *is_in);

private:
	bool is_pascal (char *is_in);
	bool is_basic (char *is_in);
	bool is_c (char *is_in);
	bool is_lisp (char *is_in);
};

#include "compatibility.h"
#include "DebugDisplayUnit.h"

class propeller_debug
{
protected:
	TDebugDisplayDoc	**DisplayForms;
	bool				DisplayStrFlag;
	int		DebugDisplayCount;
	render_context m_rc;

public:
	void Close();
	void FormKeyDown(short &Key);
	void FormClose();
	void ResetDisplays();
	void CloseDisplays();
	void char_in (unsigned char);
	void NewChr(char x);
	void NewLine();
};

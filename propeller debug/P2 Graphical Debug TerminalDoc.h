// P2 Graphical Debug TerminalDoc.h : interface of the CTerminalDoc class
//


#pragma once

//#include <iostream>
//#include <fstream>
class fstream;
class lframe;

#define OUTPUT_BUFFER_SIZE (1048576)

class CTerminalDoc : public CDocument
{
private:
	char m_outbuff [OUTPUT_BUFFER_SIZE];

protected: // create from serialization only
	CTerminalDoc();
	DECLARE_DYNCREATE(CTerminalDoc)

// Attributes
public:
	CFont m_font;
	CString m_fileName;
	
public:
	bool open_file ();
	void close_input_file ();
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual ~CTerminalDoc();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	void OnUpdateText ();
	afx_msg void OnDebugLoadbinary();
	afx_msg LRESULT OnRxString (WPARAM, LPARAM);
public:
	afx_msg void OnFileOpen();
};



// P2 Graphical Debug TerminalDoc.cpp : implementation of the CTerminalDoc class
//
#include "stdafx.h"
#include "afxmt.h"
#include "atlconv.h"
#include <cstring>
#include <vector>
#include "globalunit.h"
#include "P2 Graphical Debug Terminal.h"
#include "P2 Graphical Debug TerminalDoc.h"
#include "comthread.h"

//using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DEFAULT_TAB_STOPS (16)

// CTerminalDoc

IMPLEMENT_DYNCREATE(CTerminalDoc, CDocument)

BEGIN_MESSAGE_MAP(CTerminalDoc, CDocument)
	ON_COMMAND(ID_DEBUG_LOADBINARYANDRUN, &CTerminalDoc::OnDebugLoadbinary)
//	ON_COMMAND(WM_UPDATE_TEXT, &CTerminalDoc::OnUpdateText)
	ON_COMMAND(ID_FILE_OPEN, &CTerminalDoc::OnFileOpen)
END_MESSAGE_MAP()

// CTerminalDoc construction/destruction
// Apparently in _DEBUG mode, LOGFONT is 
// Unicode aware - but not in release mode?
// This might be an issue with the project
// settings or it might be a bug in Visual Studio?
// BE VERY AFRAID

CTerminalDoc::CTerminalDoc()
{
	m_outbuff [0] = '\0';
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));   
	lf.lfHeight = 16;
	lf.lfWeight = FW_DEMIBOLD;
//#ifdef _DEBUG
	wchar_t default_font[] = _T("Courier");
	wcscpy_s (lf.lfFaceName,32,default_font);
//#else
//	strcpy_s (lf.lfFaceName,32,"Courier");
//#endif
	m_font.CreateFontIndirect(&lf);
}

CTerminalDoc::~CTerminalDoc()
{
}

BOOL CTerminalDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	((CEditView*)m_viewList.GetHead())->SetFont(&m_font);
	m_outbuff [0] = '\0';
	OnUpdateText ();	
	return TRUE;
}

void CTerminalDoc::OnFileOpen()
{
	int i,j;
	CStdioFile f;
	CFileException ex;
	char buff[256];
	UINT flags;
	flags = CFile::modeRead;
	CString strLine, strText;
	if (m_strPathName.GetLength()==0)
		return;
	f.Open (m_strPathName,flags,&ex); 
	while (f.Read (buff,255))
	{
		buff [255]=0;
		j = strlen(buff);
		strLine = _T("");
		for (i=0;i<j;i++)
		{
			if (buff[i]!=0x0a)
				strLine.AppendChar(buff[i]);
			else
				strLine.Append(_T("\r\n"));
		}
		strText += strLine;
	}
	CEditView *pEdit;
	pEdit = (CEditView*)m_viewList.GetHead();
	pEdit->SetTabStops (DEFAULT_TAB_STOPS);
	pEdit->SetWindowText(strText);
}

void TDebugDisplayDoc::save_to_file(CString, char *)
{
#if 0
	ASSERT(false);
#endif
}

void CTerminalDoc::OnUpdateText()
{
	CEditView *pEdit;
	pEdit = (CEditView*)m_viewList.GetHead();
	CString strText(m_outbuff);
//	pEdit->SetWindowText(strText);
	pEdit->SetFont (&m_font);
}

void CTerminalDoc::Serialize(CArchive& ar)
{
	bool b_read;
#if 0
	INT_PTR v_count = m_viewList.GetCount();
	((CEditView*)m_viewList.GetHead())->SetFont(&m_font);
#endif
	if (ar.IsStoring())
	{
		AfxMessageBox (_T("Saving is not yet supported."));
		b_read = false;
	}
	else
	{	
		m_strPathName = ar.m_strFileName;
		b_read = true;
	}
	if (b_read==false)
		return;
}


// CTerminalDoc diagnostics

#ifdef _DEBUG
void CTerminalDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTerminalDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

void CTerminalDoc::OnDebugLoadbinary()
{
	CString strFile;
	CDebugTerminalApp *pApp;
	pApp = (CDebugTerminalApp*)AfxGetApp();
	CComThread* pComThread = (CComThread*) pApp->m_pComThread;
	LRESULT status;
	CFileDialog dlg(TRUE);
	status = dlg.DoModal();
	if (status!=IDOK)
		return;
	strFile = dlg.GetPathName();
	// proxy call which then posts the thread
	// message to the actual com thread ...
	CComThread::LoadHardwareAndRun(pComThread,strFile);
}

#if 0
char *get_suffix (char *fname)
{
	std::vector<int>	_dir;
	char *dir, *last, *path, *suffix, *file;
	path = strdup (fname);
	dir = strtok (path,"\\");
	while (dir!=NULL)
	{
		dir = strtok(NULL,"\\");
		if (dir!=NULL) {
			_dir.push_back ((int)(dir-path));
			last=dir;
		}
	}
	if (strlen(last)>0) {
		file = strtok (last,".");
		suffix = strtok (NULL,".");
	}
	int res = suffix-path;
	delete path;
	return &(fname[res]);
}
#endif


// frames.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "afxmt.h"
#include <iostream>
#include "defines.h"
#include "symbol_table.h"
#include "btreetype.h"
#include "node_list.h"
#include "text_object.h"
#include "scripts.h"
#include "frames.h"
#include "frames1.h"
#include "extras.h"

using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void lframe::load_binary_file (char *fname)
{
	CString msg;
	msg.Format(_T("The Unicorn is in the Garden!%s"),fname);
	AfxMessageBox(msg,IDOK);
}

LPARAM lframe::preprocess_data (char *w)
{
	bool cmd;
	char *continuation, *old;
	node<char*> *packet1, *packet2;
	packet1 = new node<char*>(strdup((char*)w));
	m_debugstr->N.concat(packet1);
	m_debugstr->N.m_nPos=m_debugstr->N.m_nEnd;
	size_t i, j;
	j = strlen(packet1->m_pData);
	int fragments = 0;
// find new line characters and split the strings at those
// points
	char ch;
	int symbols=0;
	cmd = false;
	for(i=0;i<j;i++)
	{
		ch = packet1->m_pData[i];
//		if (ch==',')
//			symbols++;
		if (ch==13)
		{
			fragments++;
			continuation = &(packet1->m_pData[i]);
			packet2 = new node<char*>(strdup((char*)continuation));
			m_debugstr->N.concat(packet2);
			packet1->m_pData[i]=0;
			old = packet1->m_pData;
			packet1->m_pData=strdup(old);
			delete [] old;
			packet1 = packet2;
			packet2 = NULL;
			symbols = 0;
		}
	}
  	LPARAM l2 = (LPARAM)m_debugstr;
	return l2;
}


	
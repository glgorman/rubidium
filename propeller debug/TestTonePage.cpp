// TestTonePage.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TestTonePage.h"


// CTestTonePage dialog

IMPLEMENT_DYNAMIC(CTestTonePage, CPropertyPage)

CTestTonePage::CTestTonePage()
	: CPropertyPage(CTestTonePage::IDD)
{

}

CTestTonePage::~CTestTonePage()
{
}

void CTestTonePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTestTonePage, CPropertyPage)
END_MESSAGE_MAP()


// CTestTonePage message handlers

// OscilloscopeSettings.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "OscilloscopeSettings.h"


// COscilloscopeSettings dialog

IMPLEMENT_DYNAMIC(COscilloscopeSettings, CPropertyPage)

COscilloscopeSettings::COscilloscopeSettings()
	: CPropertyPage(COscilloscopeSettings::IDD)
{

}

COscilloscopeSettings::~COscilloscopeSettings()
{
}

void COscilloscopeSettings::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COscilloscopeSettings, CPropertyPage)
END_MESSAGE_MAP()


// COscilloscopeSettings message handlers

// AudioOutputPage.cpp : implementation file
//
#include "stdafx.h"
#include <Mmsystem.h>
#include "resource.h"
#include "AudioOutputPage.h"

// CAudioOutputPage dialog

IMPLEMENT_DYNAMIC(CAudioOutputPage, CPropertyPage)

CAudioOutputPage::CAudioOutputPage()
	: CPropertyPage(CAudioOutputPage::IDD)
{

}

CAudioOutputPage::~CAudioOutputPage()
{
}

void CAudioOutputPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, m_list1);
}


BEGIN_MESSAGE_MAP(CAudioOutputPage, CPropertyPage)
END_MESSAGE_MAP()


// CAudioOutputPage message handlers
BOOL CAudioOutputPage::OnInitDialog(void)
{
	WAVEOUTCAPS		woc;
	UINT_PTR uDeviceID;
	MMRESULT mmRes;
	CString outDeviceName;
//	CDialog::OnInitDialog();
	CPropertyPage::OnInitDialog();
	UINT iNumWaveDevices = waveOutGetNumDevs();
	if (iNumWaveDevices>0) 
	for (uDeviceID=0;uDeviceID<iNumWaveDevices;uDeviceID++)
	{
		mmRes = waveOutGetDevCaps(uDeviceID,&woc,sizeof(WAVEOUTCAPS));
		outDeviceName = woc.szPname;
		m_list1.InsertString ((int)uDeviceID,outDeviceName);
		TRACE ("Found audio out device: %s\n",outDeviceName);
	}
	else {
		m_list1.InsertString (0,_T("NO AUDIO DEVICES AVAILABLE!"));
	}
	uDeviceID = 0;
	m_list1.SetCurSel (0);
	return TRUE;
}

void CAudioOutputPage::OnOK() 
{
	m_iOutputDevice = m_list1.GetCurSel ();
	CPropertyPage::OnOK();
}

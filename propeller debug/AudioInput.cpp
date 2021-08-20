// AudioInput.cpp : implementation file

#include "stdafx.h"
#include <Mmsystem.h>
#include "resource.h"
#include "AudioInput.h"

// CAudioInput dialog

IMPLEMENT_DYNAMIC(CAudioInput, CPropertyPage)

CAudioInput::CAudioInput()
	: CPropertyPage(CAudioInput::IDD)
{

}

CAudioInput::~CAudioInput()
{
}

void CAudioInput::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, m_list1);
}

BEGIN_MESSAGE_MAP(CAudioInput, CPropertyPage)
END_MESSAGE_MAP()

BOOL CAudioInput::OnInitDialog(void)
{
	WAVEINCAPS		wic;
	UINT_PTR uDeviceID;
	MMRESULT mmRes;
	CString inDeviceName;
	CPropertyPage::OnInitDialog();
	UINT iNumWaveDevices = waveInGetNumDevs();
	if (iNumWaveDevices>0) 
	for (uDeviceID=0;uDeviceID<iNumWaveDevices;uDeviceID++)
	{
		mmRes = waveInGetDevCaps(uDeviceID,&wic,sizeof(WAVEINCAPS));
		inDeviceName = wic.szPname;
		m_list1.InsertString ((int)uDeviceID,inDeviceName);
		TRACE ("Found audio in device: %s\n",inDeviceName);
	}
	else {
		m_list1.InsertString (0,_T("NO AUDIO DEVICES AVAILABLE!"));
	}
	uDeviceID = 0;
	m_list1.SetCurSel (0);
	return TRUE;
}

void CAudioInput::OnOK() 
{
	m_iInputDevice = m_list1.GetCurSel ();
	CPropertyPage::OnOK();
}

// CAudioInput message handlers

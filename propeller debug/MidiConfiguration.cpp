// MidiConfiguration.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "windows.h"
#include "mmsystem.h"
#include "MidiConfiguration.h"

// CMidiConfiguration dialog

IMPLEMENT_DYNAMIC(CMidiConfiguration, CPropertyPage)

CMidiConfiguration::CMidiConfiguration()
	: CPropertyPage(CMidiConfiguration::IDD)
{
	m_pPlayer = new CMidiPlayer;
}

CMidiConfiguration::~CMidiConfiguration()
{
}

void CMidiConfiguration::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list1);
	DDX_Control(pDX, IDC_LIST2, m_list2);
}


BEGIN_MESSAGE_MAP(CMidiConfiguration, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK3, &CMidiConfiguration::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK2, &CMidiConfiguration::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK1, &CMidiConfiguration::OnBnClickedCheck1)
END_MESSAGE_MAP()

BOOL CMidiConfiguration::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UINT uDeviceID;  
	CString szName;
	UINT iNumInDevices = m_pPlayer->m_iNumInDevices;
	UINT iNumOutDevices = m_pPlayer->m_iNumOutDevices;
	if (iNumInDevices>0) {
	for (uDeviceID=0;uDeviceID<iNumInDevices;uDeviceID++)
		{
		szName = m_pPlayer->m_lpMidiInCaps[uDeviceID].szPname;
		m_list1.InsertString (uDeviceID,szName);
		}
	}
	else {
		m_list1.InsertString (0,_T("NO PC-MIDI INPUT DEVICES FOUND"));
		iNumInDevices = 1;
	}
	m_list1.InsertString (iNumInDevices,_T("Parallax P2 EVAL REV B."));
	m_list1.SetCurSel (0);
	
	if (m_pPlayer->m_iNumOutDevices>0) {
	for (uDeviceID=0;uDeviceID<iNumOutDevices;uDeviceID++)
		{
		szName = m_pPlayer->m_lpMidiOutCaps[uDeviceID].szPname;
		m_list2.InsertString (uDeviceID,szName);
		}
	}
	else {
		m_list2.InsertString (0,_T("NO PC-MIDI OUTPUT DEVICES FOUND"));
		iNumOutDevices = 1;
	}
	m_list2.InsertString (iNumOutDevices,_T("Parallax P2 EVAL REV B."));
	m_list2.SetCurSel (0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// CMidiConfiguration message handlers

CMidiPlayer::CMidiPlayer ()
{
	UINT uDeviceID;
	MMRESULT mmresult;

//	m_pThread = AfxBeginThread (RUNTIME_CLASS(CMidiThread),NULL,NULL);
//	m_pThread->PostThreadMessage (WM_MIDI_SET_PLAYER,(WPARAM)config_info,(LPARAM)this);

	m_iNumOutDevices = midiOutGetNumDevs ();
	m_iNumInDevices = midiInGetNumDevs ();
	m_lpMidiOutCaps = new MIDIOUTCAPS [m_iNumOutDevices];
	m_lpMidiInCaps = new MIDIINCAPS [m_iNumInDevices];
	for (uDeviceID=0;uDeviceID<m_iNumInDevices;uDeviceID++)
	{
		mmresult = midiInGetDevCaps(uDeviceID,&m_lpMidiInCaps[uDeviceID],sizeof(MIDIINCAPS));
		ASSERT (mmresult==MMSYSERR_NOERROR);
	}
#if 0
	OnMidiInOpen (0);
#endif
	for (uDeviceID=0;uDeviceID<m_iNumOutDevices;uDeviceID++)
	{
		mmresult = midiOutGetDevCaps(uDeviceID,&m_lpMidiOutCaps[uDeviceID],sizeof(MIDIOUTCAPS));
		ASSERT (mmresult==MMSYSERR_NOERROR);
	}
#if 0
	OnMidiOutOpen (m_iNumOutDevices-1);
#endif
//	OnChangePgm (0,4);	
//	m_pThread->PostThreadMessage (WM_MIDI_PLAY_CHORD,60,0);
}

CMidiPlayer::~CMidiPlayer ()
{
	CMidiPlayer::midiInClose ();
	CMidiPlayer::midiOutClose ();
	delete m_lpMidiInCaps;
	delete m_lpMidiOutCaps;
}

MMRESULT CMidiPlayer::midiInClose ()
{
	DWORD result;
	result = ::midiInReset (m_hmi);
	result = ::midiInClose (m_hmi);
	m_hmi = NULL;
	return result;
}

MMRESULT CMidiPlayer::midiOutClose ()
{
	DWORD result;
	result = ::midiOutClose (m_hmo);
	m_hmo = NULL;
	return result;
}

UINT CMidiPlayer::midiOutGetNumDevs()
{
	m_iNumOutDevices = ::midiOutGetNumDevs();
	return m_iNumOutDevices;
};

void CMidiConfiguration::OnBnClickedCheck3()
{
	// TODO: Add your control notification handler code here
}

void CMidiConfiguration::OnBnClickedCheck2()
{
	// TODO: Add your control notification handler code here
}

void CMidiConfiguration::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here
}

#pragma once

#include "windows.h"
#include "mmsystem.h"
// CMidiConfiguration dialog

class CMidiPlayer
{
protected:
	MMTIME	m_time_code;

public:
	int m_iInputDevice, m_iOutputDevice;
	UINT m_iNumInDevices, m_iNumOutDevices;
	LPMIDIOUTCAPS m_lpMidiOutCaps;
	LPMIDIINCAPS  m_lpMidiInCaps;
	HMIDIIN  m_hmi;
	HMIDIOUT m_hmo;

protected:
	static void CALLBACK MidiInProc(HMIDIIN hmi,UINT wMsg,DWORD dwInstance,DWORD dwParam1,DWORD dwParam2);
	static void CALLBACK MidiOutProc(HMIDIOUT hmo,UINT wMsg,DWORD dwInstance,DWORD dwParam1,DWORD dwParam2);
//	static void newMidiMessage(ofxMidiEventArgs& eventArgs, CMidiPlayer *pInstance);
//	static MMRESULT midiStreamOpen (LPHMIDISTRM,LPUINT,DWORD,DWORD,DWORD,DWORD fdwOpen);
//	static DWORD midiOutMessage (HMIDIOUT,UINT,DWORD,DWORD);	
	UINT midiOutGetNumDevs(VOID);

public:
	void CancelMIDIThread ();
//	CMidiPlayer (CMainConfig*);
	CMidiPlayer ();
//	CWinThread  *m_pThread;
	virtual ~CMidiPlayer ();
//	void load_midi_file (char *szFileName);
//	void OnMidiInOpen (LPARAM l, WPARAM w=0);
//	void OnMidiOutOpen (LPARAM l, WPARAM w=0);
	MMRESULT midiOutClose ();
	MMRESULT midiInClose ();
//	void QuickPlay (HMIDIOUT handle);
//	void play_note (LPARAM l, WPARAM w, int v);
//	void OnPlayNote (LPARAM l, WPARAM w);
//	void OnStopNote (LPARAM l, WPARAM w);
//	void OnChangePgm (LPARAM l, WPARAM w);
//	void OnDJControl (LPARAM l, WPARAM w);
	MMTIME get_time_code ();
};

class CMidiConfiguration : public CPropertyPage
{
	DECLARE_DYNAMIC(CMidiConfiguration)

public:
	CMidiPlayer *m_pPlayer;
	CMidiConfiguration();
	virtual ~CMidiConfiguration();

// Dialog Data
	enum { IDD = IDD_MIDI_CONFIG };
	CComboBox	m_list1;
	CComboBox	m_list2;
	CComboBox	m_list3;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog ();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheck3();
public:
	afx_msg void OnBnClickedCheck2();
public:
	afx_msg void OnBnClickedCheck1();
};

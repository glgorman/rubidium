// PortSettingsPage.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "messages.h"
#include "mainfrm.h"
#include "propserial.h"
#include "ComThread.h"
#include "PortSettingsPage.h"

extern int port_speeds [];

int port_speeds [] =
{
	110, 300, 1200, 2400, 4800, 9600, 19200, 28400, 
	38400, 57600, 64000, 115200, 128000, 230400, 460800,
	921600, 2000000, 3000000, 12000000, -1,
};

// CPortSettingsPage dialog

IMPLEMENT_DYNAMIC(CPortSettingsPage, CPropertyPage)

BEGIN_MESSAGE_MAP(CPortSettingsPage, CPropertyPage)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

CPortSettingsPage::CPortSettingsPage()
	: CPropertyPage(CPortSettingsPage::IDD)
{
	m_pThread = 0;
}

CPortSettingsPage::~CPortSettingsPage()
{
	
}

BOOL CPortSettingsPage::OnInitDialog ()
{
	CDialog::OnInitDialog();
	UINT i, uDeviceID;  
	CString szName;
	UINT iNumPorts = 10;
	szName.Format(_T("AUTO DETECT"));
	m_list1.InsertString (0,szName);

	if (iNumPorts>0)
	for (uDeviceID=1;uDeviceID<iNumPorts;uDeviceID++)
	{
		szName.Format(_T("COM%d:"),uDeviceID);
		m_list1.InsertString (uDeviceID,szName);
	}
	else {
		m_list1.InsertString (0,_T("NO COM PORT DEVICES FOUND"));
		iNumPorts = 1;
	}
	for (i=0;i<19;i++)
	{
		szName.Format(_T("%8d bits/sec"),port_speeds[i]);
		m_list2.InsertString (i,szName);
	}
	m_list3.InsertString (0,_T("Parallax Basic Stamp (1/2)"));
	m_list3.InsertString (0,_T("Parallax P1"));
	m_list3.InsertString (0,_T("Parallax P2 EVAL REV (B/C)"));
	m_list3.InsertString (0,_T("Atmega 2650 MEGA"));
	m_list3.InsertString (0,_T("Other Terminal Mode Device"));
	m_list1.SetCurSel (3);
	m_list2.SetCurSel (13);
	m_list3.SetCurSel (2);
	return TRUE;  // return TRUE unless you set the focus to a control
	
}

void CPortSettingsPage::OnClose()
{
	CWnd::OnClose();
}

void CPortSettingsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list1);
	DDX_Control(pDX, IDC_LIST2, m_list2);
	DDX_Control(pDX, IDC_LIST3, m_list3);
}

LRESULT CPortSettingsPage::OnWizardNext()
{
	int item_index = m_list2.GetCurSel();
	m_baud_rate = port_speeds [item_index];
	m_port_num = m_list1.GetCurSel();
	if (m_pThread!=NULL)
		m_pThread->m_baud_rate = m_baud_rate;
	return 0;
}

void CPortSettingsPage::OnOK ()
{
	CPropertyPage::OnOK ();
}

void CPortSettingsPage::OnEnChangeEdit2()
{
	
}

void CPortSettingsPage::OnEnChange1023()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CPortSettingsPage::OnEnChangeEdit3()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

#pragma once

class CComThread;

class CPortSettingsPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CPortSettingsPage)

public:
	CPortSettingsPage();
	virtual ~CPortSettingsPage();
	virtual BOOL OnInitDialog ();
	enum { IDD = IDD_PORT_SETTINGS };
	int m_port_num;
	int m_baud_rate;
	CComboBox	m_list1;
	CComboBox	m_list2;
	CComboBox	m_list3;
	CComThread *m_pThread;

protected:
	virtual LRESULT OnWizardNext();
	virtual void OnOK ();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnClose();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChange1023();
	afx_msg void OnEnChangeEdit3();
};

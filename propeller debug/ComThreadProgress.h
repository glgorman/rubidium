#pragma once

#include "resource.h"
#include "afxwin.h"

class CComThread;

class CComThreadProgress : public CPropertyPage
{
	DECLARE_DYNAMIC(CComThreadProgress)

public:
	CComThreadProgress();
	virtual ~CComThreadProgress();
	BOOL OnInitDialog();
	enum { IDD = IDD_SERIAL_PROGRESS };
	CProgressCtrl	m_progressBar;
	void SetProgressInfo (WPARAM);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual LRESULT OnWizardNext();
	DECLARE_MESSAGE_MAP()

public:
	CComThread *m_pThread;
	int m_baud_rate;
	int m_port_num;

	afx_msg void OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult);
	
	CEdit		CProgressEdit;
	CComboBox m_port_control;
	CComboBox m_baud_control;
	CComboBox m_device_control;
	CString m_progress_text;

public:
	afx_msg void OnEnChangeEdit1();
};

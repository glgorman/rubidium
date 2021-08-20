#pragma once

#include "resource.h"

class CComThread;

class CUploadProgress : public CPropertyPage
{
	CPropertySheet *m_parentWnd;
	DECLARE_DYNAMIC(CUploadProgress)

public:
	CUploadProgress();
	virtual ~CUploadProgress();
	BOOL OnInitDialog();
	enum { IDD = IDD_SERIAL_PROGRESS };
	CProgressCtrl	m_progressBar;
	

protected:
	afx_msg LRESULT OnSetProgressInfo (WPARAM, LPARAM);
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual LRESULT OnWizardNext();
	virtual BOOL OnWizardFinish();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult);
	CComThread *m_pThread;
	CEdit		CProgressEdit;
	CString m_progress_text;
	afx_msg void OnEnChangeEdit1();
};

// ComTreadProgress.cpp : implementation file
//

#include "stdafx.h"
#include "ComThreadProgress.h"

// CComThreadProgress dialog

IMPLEMENT_DYNAMIC(CComThreadProgress, CPropertyPage)

BEGIN_MESSAGE_MAP(CComThreadProgress, CPropertyPage)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS1, &CComThreadProgress::OnNMCustomdrawProgress1)
	ON_EN_CHANGE(IDC_EDIT1, &CComThreadProgress::OnEnChangeEdit1)
END_MESSAGE_MAP()

CComThreadProgress::CComThreadProgress()
	: CPropertyPage(CComThreadProgress::IDD)
{

}

CComThreadProgress::~CComThreadProgress()
{
}

BOOL CComThreadProgress::OnInitDialog() 
{
//	GetWindowThreadProcessId (GetSafeHwnd(), &m_dwThisProcessId);
	CPropertyPage::OnInitDialog();
	CProgressEdit.SetReadOnly(TRUE);
	CProgressEdit.SetWindowText(m_progress_text.GetBuffer());
	m_progressBar.SetRange(0,100);
	m_progressBar.SetStep(1);
	m_progressBar.SetPos(50);
	return TRUE;            
}

void CComThreadProgress::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_progressBar);
	DDX_Control(pDX, IDC_EDIT1, CProgressEdit);
}


// CComThreadProgress message handlers

void CComThreadProgress::OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CComThreadProgress::SetProgressInfo (WPARAM)
{
	int progress;
	progress = 75;
	m_progressBar.SetPos(progress);
}

LRESULT CComThreadProgress::OnWizardNext ()
{
	return 0;
}

void CComThreadProgress::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

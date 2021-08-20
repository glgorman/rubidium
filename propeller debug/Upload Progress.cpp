// SerialProgress.cpp : implementation file
//

#include "stdafx.h"
#include "messages.h"
#include "mainfrm.h"
#include "propserial.h"
#include "ComThread.h"
#include "Upload Progress.h"

// CUploadProgress dialog

IMPLEMENT_DYNAMIC(CUploadProgress, CPropertyPage)

BEGIN_MESSAGE_MAP(CUploadProgress, CPropertyPage)
	ON_MESSAGE(WM_SET_UPLOAD_PROGRESS,OnSetProgressInfo)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS1, &CUploadProgress::OnNMCustomdrawProgress1)
	ON_EN_CHANGE(IDC_EDIT1, &CUploadProgress::OnEnChangeEdit1)
END_MESSAGE_MAP()

CUploadProgress::CUploadProgress()
	: CPropertyPage(CUploadProgress::IDD)
{

}

CUploadProgress::~CUploadProgress()
{
}

BOOL CUploadProgress::OnInitDialog() 
{
//	GetWindowThreadProcessId (GetSafeHwnd(), &m_dwThisProcessId);
	m_parentWnd = (CPropertySheet *)GetParent();
	CPropertyPage::OnInitDialog();
	CProgressEdit.SetReadOnly(TRUE);
	CProgressEdit.SetWindowText(m_progress_text.GetBuffer());
	m_progressBar.SetRange(0,1000);
	m_progressBar.SetStep(1);
	m_progressBar.SetPos(1);
	return TRUE;            
}

void CUploadProgress::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_progressBar);
	DDX_Control(pDX, IDC_EDIT1, CProgressEdit);
}

LRESULT CUploadProgress::OnSetProgressInfo (WPARAM w, LPARAM l)
{
	int progress;
	if (l==0)
		return -1;

	progress = 1000 - (1000*(l-w))/l;
	if (progress>900)
		progress=1000;
	m_progressBar.SetPos(progress);
	if (w==l){
		m_parentWnd->EndDialog(int(w));
	}
	return 0;
}

LRESULT CUploadProgress::OnWizardNext()
{
	LRESULT res;
	CUploadProgress::OnSetProgressInfo(1,100);
	m_parentWnd->SetFinishText(_T("Upload"));
	res = CPropertyPage::OnWizardNext();
	return res;
}

BOOL CUploadProgress::OnWizardFinish()
{
	CPropertySheet *pS;
	CMainFrame* m_pMain;
	pS = (CPropertySheet*) GetParent();
	m_pMain = (CMainFrame*)pS->GetOwner();
//	if (res==ID_WIZFINISH)
	m_pThread->PostThreadMessage(WM_LOAD_HARDWARE,0,0);
	m_parentWnd->SetWizardButtons(PSWIZB_DISABLEDFINISH);
	return FALSE;
}

// CComThreadProgress message handlers

void CUploadProgress::OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CUploadProgress::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

// CUploadProgress message handlers

#pragma once


// CAudioOutputPage dialog

class CAudioOutputPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CAudioOutputPage)
	CComboBox	m_list1;

public:
	CAudioOutputPage();
	virtual ~CAudioOutputPage();

// Dialog Data
	enum { IDD = IDD_SELECTAUDIOOUTPUT };
	int m_iOutputDevice;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

public:
	BOOL OnInitDialog(void);
	virtual void OnOK ();
};

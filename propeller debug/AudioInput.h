#pragma once


// CAudioInput dialog

class CAudioInput : public CPropertyPage
{
	DECLARE_DYNAMIC(CAudioInput)
	CComboBox	m_list1;

public:
	CAudioInput();
	virtual ~CAudioInput();

// Dialog Data
	enum { IDD = IDD_SELECTAUDIOINPUT };
	int m_iInputDevice;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

public:
	BOOL OnInitDialog(void);
	virtual void OnOK ();
};

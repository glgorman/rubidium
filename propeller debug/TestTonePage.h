#pragma once


// CTestTonePage dialog

class CTestTonePage : public CPropertyPage
{
	DECLARE_DYNAMIC(CTestTonePage)

public:
	CTestTonePage();
	virtual ~CTestTonePage();

// Dialog Data
	enum { IDD = IDD_TEST_TONE_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};

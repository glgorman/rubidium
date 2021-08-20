#pragma once


// COscilloscopeSettings dialog

class COscilloscopeSettings : public CPropertyPage
{
	DECLARE_DYNAMIC(COscilloscopeSettings)

public:
	COscilloscopeSettings();
	virtual ~COscilloscopeSettings();

// Dialog Data
	enum { IDD = IDD_FILTER_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};

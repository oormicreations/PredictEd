#pragma once
#include "afxcolorbutton.h"
#include "SysHelper.h"

// CPredictEdSettingsDlg dialog

class CPredictEdSettingsDlg : public CDialog
{
	DECLARE_DYNAMIC(CPredictEdSettingsDlg)

public:
	CPredictEdSettingsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPredictEdSettingsDlg();

	UINT m_LTMSz, m_STMSz, m_MaxLimit, m_Margins;
	COLORREF m_BkColor, m_TxtColor;
	LOGFONT m_DefFont;
	CMFCColorButton m_ColorButton;
	CString m_DictionaryFile, m_ContextFile;
	CSysHelper m_SysHelper;

	void SetDefaultFont();
	void Reset();
	void SetParams();
	void SetFontName();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SETTINGS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEditSetLimit();
	afx_msg void OnEnChangeEditSetMargins();
	afx_msg void OnBnClickedButtonSetDeffont();
	afx_msg void OnBnClickedButtonSetReset();
	virtual void OnOK();
	afx_msg void OnBnClickedButtonSetDefdic();
	afx_msg void OnBnClickedButtonSetDefcon();
};

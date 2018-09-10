#pragma once
#include "CryptHelper.h"
#include "SysHelper.h"


// CBase64Dlg dialog

class CBase64Dlg : public CDialog
{
	DECLARE_DYNAMIC(CBase64Dlg)

public:
	CBase64Dlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBase64Dlg();

	CSysHelper m_SysHelper;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BASE64 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonFilesel();
	afx_msg void OnBnClickedButtonTobase64();
	afx_msg void OnBnClickedButtonFromb64();
};

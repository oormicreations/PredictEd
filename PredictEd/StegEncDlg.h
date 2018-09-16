#pragma once
#include "CryptHelper.h"
#include "SysHelper.h"
#include "afxcmn.h"


// CStegEncDlg dialog

class CStegEncDlg : public CDialog
{
	DECLARE_DYNAMIC(CStegEncDlg)

public:
	CStegEncDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStegEncDlg();

	CSysHelper m_SysHelper;
	CString m_ImageFile, m_DataFile, m_DataFilePath;
	CString m_Password;
	CCryptHelper m_CryptHelper;
	CProgressCtrl m_PassStrengthBar;
	BOOL m_UsePass;

	CString GetPass();



// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_STEG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonEmb();
	afx_msg void OnBnClickedButtonTar();
	afx_msg void OnBnClickedButtonGenpass();
	afx_msg void OnBnClickedButtonStegdec();
	afx_msg void OnBnClickedButtonStegenc();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEnChangeEditPass();
	afx_msg void OnBnClickedCheckEnc();
};

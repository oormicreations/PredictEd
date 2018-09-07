#pragma once
#include "CryptHelper.h"
#include "afxcmn.h"
#include "afxwin.h"

// CAESEncryptDlg dialog

class CAESEncryptDlg : public CDialog
{
	DECLARE_DYNAMIC(CAESEncryptDlg)

public:
	CAESEncryptDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAESEncryptDlg();

	CCryptHelper m_CryptHelper;
	CString m_SourceFileName, m_BackupFileName;
	CProgressCtrl m_PassStrengthProg;
	BOOL m_DeleteOriginal;


	void DisplayPasswordStrength(CString pass);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ENCPASS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEditPass1();
	afx_msg void OnBnClickedButtonGenpass();
	CButton m_DelCheckBox;
};

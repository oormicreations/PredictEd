#pragma once
#include "CryptHelper.h"
#include "SysHelper.h"
#include "afxwin.h"


// CHashDlg dialog

class CHashDlg : public CDialog
{
	DECLARE_DYNAMIC(CHashDlg)

public:
	CHashDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHashDlg();

	CSysHelper m_SysHelper;
	CString m_Content;
	CFont * m_EdFont;
	CString m_HashStr;


	void SetHashDisplayFont();
	void DisplayFormattedHash();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_HASH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonHashtext();
	afx_msg void OnBnClickedButtonHashfile();
	afx_msg void OnBnClickedButtonCopyhash();
	afx_msg void OnBnClickedButtonCompare();
	afx_msg void OnBnClickedButtonSavehash();
	CEdit m_HashEd;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	CEdit m_Cols;
	CEdit m_Rows;
};

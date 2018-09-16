#pragma once
#include "CryptHelper.h"
#include "SysHelper.h"


// CStegDecDlg dialog

class CStegDecDlg : public CDialog
{
	DECLARE_DYNAMIC(CStegDecDlg)

public:
	CStegDecDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStegDecDlg();

	CSysHelper m_SysHelper;
	CString m_ImageFile, m_DataFile, m_DataFilePath;
	CString m_Password;
	CCryptHelper m_CryptHelper;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_STEGDEC };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonImgfile();
	afx_msg void OnBnClickedOk();
};

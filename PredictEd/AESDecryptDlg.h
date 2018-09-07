#pragma once
#include "CryptHelper.h"


// CAESDecryptDlg dialog

class CAESDecryptDlg : public CDialog
{
	DECLARE_DYNAMIC(CAESDecryptDlg)

public:
	CAESDecryptDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAESDecryptDlg();

	CCryptHelper m_CryptHelper;

	CString m_EncryptedFilename;
	CString m_DecryptedFilename;
	int m_DecryptedFileType;
	BOOL m_DecryptResult;


// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DEC };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

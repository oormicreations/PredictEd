#pragma once
#include "SysHelper.h"
#include "WordList.h"
#include "afxcmn.h"

#define MAX_INPUT_FILES 20

// CTrain dialog

class CTrain : public CDialog
{
	DECLARE_DYNAMIC(CTrain)

public:
	CTrain(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTrain();

	CSysHelper m_SysHelper;
	CString m_FileList[MAX_INPUT_FILES];
	int m_FileCount, m_Count;
	CString m_FileName, m_FileHeader;
	BOOL m_Result, m_Abort;
	CWordList LTM;
	CProgressCtrl m_Progress;

	CString FilterString(CString str);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TRAIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonStart();
	afx_msg LRESULT OnTrainThreadNotify(WPARAM, LPARAM);

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedCancel();
};

#pragma once
#include "afxwin.h"
#include "KeyWordMap.h"


// CPreWordsDlg dialog

class CPreWordsDlg : public CDialog
{
	DECLARE_DYNAMIC(CPreWordsDlg)

public:
	CPreWordsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPreWordsDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PREWORDS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_Word1;
	CEdit m_Word2;
	CEdit m_Word3;
	CEdit m_Word4;
	CEdit m_Word5;
	CEdit m_Ph1;
	CEdit m_Ph2;
	CEdit m_Ph3;
	int m_PhPos;

	void SetWords(CKeyWordMap map, CString * phrases);
	void ShiftWords();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

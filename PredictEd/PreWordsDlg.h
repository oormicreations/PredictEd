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

	void SetWords(CKeyWordMap map);
	void ShiftWords();
};

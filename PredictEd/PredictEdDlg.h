
// PredictEdDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "PredictEdCtrl.h"
#include "SysHelper.h"


// CPredictEdDlg dialog
class CPredictEdDlg : public CDialogEx
{
// Construction
public:
	CPredictEdDlg(CWnd* pParent = NULL);	// standard constructor
// Dialog Data

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PREDICTED_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
//	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CPredictEdCtrl m_Ed;
	CMenu m_Menu;
	BOOL m_IsShellOpen, m_Saved;
	CString m_FileName;
	CSysHelper m_SysHelper;




	void InitEd();
	void InsertText(CString text, COLORREF color = RGB(0, 0, 0), bool bold = false, bool italic = false);








	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnFileExit();
	afx_msg void OnFileOpen32771();
	afx_msg void OnFileSave32772();
	afx_msg void OnHelpAboutpredicted();
};

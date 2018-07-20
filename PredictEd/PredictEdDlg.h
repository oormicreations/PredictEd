
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
	BOOL m_IsShellOpen/*, m_Saved*/, m_SaveCanceled;
	CString m_FileName;
	CSysHelper m_SysHelper;
	UINT m_StartHour, m_StartMinute;
	CTime m_StartTime;




	void InitEd();
	void InsertText(CString text, COLORREF color = RGB(0, 0, 0), bool bold = false, bool italic = false);
	void ShowMessage(CString msg);








	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnFileExit();
	afx_msg void OnFileOpen32771();
	afx_msg void OnFileSave32772();
	afx_msg void OnHelpAboutpredicted();
	afx_msg void OnOptionsErasememories();
	afx_msg void OnOptionsTrain();
	afx_msg void OnFileSavepredictions();
	afx_msg void OnOptionsMergestminltm();
	afx_msg void OnFileSavefileas();
	afx_msg void OnFileCopy();
	afx_msg void OnFilePaste();
	afx_msg void OnEditFindandreplace();
	afx_msg void OnEditClearformatting();
	afx_msg void OnOptionsFontandsize();
	afx_msg void OnFileNewfile();
	afx_msg void OnBnClickedButtonNew();
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonSaveas();
	afx_msg void OnBnClickedButtonCopy();
	afx_msg void OnBnClickedButtonPaste();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonFonts();
	afx_msg void OnBnClickedButtonTrain2();
	afx_msg void OnOptionsSettings();
};

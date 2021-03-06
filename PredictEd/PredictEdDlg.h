
// PredictEdDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "PredictEdCtrl.h"
#include "SysHelper.h"
#include "PredictEdSettingsDlg.h"
#include "Train.h"
#include "NetHelper.h"
#include "AESEncryptDlg.h"
#include "AESDecryptDlg.h"
#include "HashDlg.h"
#include "StegEncDlg.h"
#include "StegDecDlg.h"
#include "SpellCheckDlg.h"
#include "PredictEdContext.h"

#define PREDICTED_CLOCK_INTERVEL 5 //sec
#define PREDICTED_MAX_TIPS 100
#define PREDICTED_TIP_INTERVEL PREDICTED_CLOCK_INTERVEL * 12 //sec

// Register FindReplace window message.
static UINT WM_FINDREPLACE = ::RegisterWindowMessage(FINDMSGSTRING);

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
	CString m_DictionaryFile, m_ContextFile;
	CSysHelper m_SysHelper;
	UINT m_Timer;
	CTime m_StartTime;
	LONG m_MaxLimit;
	UINT m_Margin, m_DefaultFontSz;
	CFindReplaceDialog * m_pFRDlg;
	UINT m_PredictEdVersionMaj, m_PredictEdVersionMin;
	CNetHelper m_NetHelper;
	CToolTipCtrl* m_ToolTip;
	CPredictEdContext m_Context;
	CString m_CurFileTitle;
	CString m_Tips[PREDICTED_MAX_TIPS];
	int m_TipDuration, m_TipNum;
	BOOL m_Abort;
	CString m_ShellOpenFileName;

	typedef struct tagPREDICTEDSET
	{
		UINT m_LTMSz, m_STMSz, m_MaxLimit, m_Margins;
		COLORREF m_BkColor, m_TxtColor;
		LOGFONT m_DefFont;
		TCHAR m_DictionaryFileTch[MAX_PATH+1];
		TCHAR m_ContextFileTch[MAX_PATH+1];
	} PREDICTEDSET;

	void InitEd();
	void InsertText(CString text, COLORREF color = RGB(0, 0, 0), bool bold = false, bool italic = false);
	void ShowMessage();
	void ShowDuration();
	void ShowTips();

	void SetDefaultStyle();
	void InitFindReplaceDlg();
	void SetButtons();
	void BackupMemories();
	void SetToolTips();
	void InitContext();
	void InitTips();


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
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LONG OnFindReplace(WPARAM wParam, LPARAM lParam);
	afx_msg void OnHelpOnlinehelp();
	afx_msg void OnHelpCheckforupdates();
	afx_msg void OnHelpGetmorefreeapps();
	afx_msg void OnEncryptionOpenencryptedfile();
	afx_msg void OnEncryptionSaveasencryptedfile();
	afx_msg void OnEncryptionEncrypt();
	afx_msg void OnEncryptionSha512hash();
	afx_msg void OnEncryptionConvertto();
	afx_msg void OnBase64Encode();
	afx_msg void OnBase64Decode();
	afx_msg void OnEncryptionSteg();
	afx_msg void OnStegEncode();
	afx_msg void OnStegDecode();
	afx_msg void OnEditSpellingcheck();
	afx_msg LONG OnSelectMisspell(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnSelectSpellSuggestion(WPARAM wParam, LPARAM lParam);
	afx_msg void OnContextsLoadcontext();
	afx_msg void OnContextsSavecontext();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT OnUpdateWordCount(WPARAM, LPARAM);

	afx_msg void OnDropFiles(HDROP hDropInfo);
};

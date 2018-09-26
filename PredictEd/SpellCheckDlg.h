#pragma once
#include "SysHelper.h"

#define WM_SELMISSPELLED WM_APP + 128
#define WM_SETSPELLSUGGESTION WM_APP + 129

#define MAX_SPELL_SUGGESTIONS 100

// CSpellCheckDlg dialog

class CSpellCheckDlg : public CDialog
{
	DECLARE_DYNAMIC(CSpellCheckDlg)

public:
	CSpellCheckDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSpellCheckDlg();

	CString m_Content, m_Word, m_LastAdded, m_DicFile;
	int m_Position;
	CString m_Suggestions[MAX_SPELL_SUGGESTIONS];
	CSysHelper m_SysHelper;
	CStringArray m_DicWords, m_AddedDicWords;
	float m_SuggestionsProbs[MAX_SPELL_SUGGESTIONS];
	int m_DispSuggestion;
	BOOL m_IsDicChanged;

	BOOL SpellCheck();
	BOOL LoadDictionary();
	void GetSpellingSuggestions();
	void ClearSuggestions();
	void SortSuggestions();
	BOOL UpdateDictionary();
	void ShowSuggestions();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SPELL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonNxtmisspell();
	afx_msg void OnBnClickedButtonSpell1();
	afx_msg void OnBnClickedButtonSpell2();
	afx_msg void OnBnClickedButtonSpell3();
	afx_msg void OnBnClickedButtonSpell4();
	afx_msg void OnBnClickedButtonAddtodic();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonSpellprev();
	afx_msg void OnBnClickedButtonSpellnxt();
	afx_msg void OnBnClickedButtonLoaddic();
	virtual BOOL OnInitDialog();
};

#pragma once
#include "SysHelper.h"

#define WM_SELMISSPELLED WM_APP + 128
#define WM_SETSPELLSUGGESTION WM_APP + 129

#define MAX_SPELL_SUGGESTIONS 40

// CSpellCheckDlg dialog

class CSpellCheckDlg : public CDialog
{
	DECLARE_DYNAMIC(CSpellCheckDlg)

public:
	CSpellCheckDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSpellCheckDlg();

	CString m_Content, m_Word;
	int m_Position, m_LastPosition;
	CString m_Suggestions[MAX_SPELL_SUGGESTIONS], m_SelSuggestion;
	CSysHelper m_SysHelper;
	CStringArray m_DicWords;
	float m_SuggestionsProbs[MAX_SPELL_SUGGESTIONS];

	BOOL SpellCheck();
	BOOL LoadDictionary();
	void GetSpellingSuggestions();
	void ClearSuggestions();
	void SortSuggestions();

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
};

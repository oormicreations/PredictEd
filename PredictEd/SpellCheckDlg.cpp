// SpellCheckDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PredictEd.h"
#include "SpellCheckDlg.h"
#include "afxdialogex.h"


// CSpellCheckDlg dialog

IMPLEMENT_DYNAMIC(CSpellCheckDlg, CDialog)

CSpellCheckDlg::CSpellCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_SPELL, pParent)
{
	m_Position = 0;
	m_DispSuggestion = 0;
}

CSpellCheckDlg::~CSpellCheckDlg()
{
}

void CSpellCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSpellCheckDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_NXTMISSPELL, &CSpellCheckDlg::OnBnClickedButtonNxtmisspell)
	ON_BN_CLICKED(IDC_BUTTON_SPELL1, &CSpellCheckDlg::OnBnClickedButtonSpell1)
	ON_BN_CLICKED(IDC_BUTTON_SPELL2, &CSpellCheckDlg::OnBnClickedButtonSpell2)
	ON_BN_CLICKED(IDC_BUTTON_SPELL3, &CSpellCheckDlg::OnBnClickedButtonSpell3)
	ON_BN_CLICKED(IDC_BUTTON_SPELL4, &CSpellCheckDlg::OnBnClickedButtonSpell4)
	ON_BN_CLICKED(IDC_BUTTON_ADDTODIC, &CSpellCheckDlg::OnBnClickedButtonAddtodic)
	ON_BN_CLICKED(IDCANCEL, &CSpellCheckDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_SPELLPREV, &CSpellCheckDlg::OnBnClickedButtonSpellprev)
	ON_BN_CLICKED(IDC_BUTTON_SPELLNXT, &CSpellCheckDlg::OnBnClickedButtonSpellnxt)
END_MESSAGE_MAP()


// CSpellCheckDlg message handlers

BOOL CSpellCheckDlg::LoadDictionary()
{
	if(m_DicFile.IsEmpty()) m_DicFile = m_SysHelper.GetPredictEdFileName(PREDICTED_DIC_FILE);
	if(m_DicFile.IsEmpty()) return FALSE;

	CString content = m_SysHelper.ReadStringFromFile(m_DicFile);

	CString line;
	int pos = 0;
	while (pos >= 0)
	{
		line = content.Tokenize(_T("\r\n"), pos);
		m_DicWords.Add(line);
	}

	return TRUE;
}

BOOL CSpellCheckDlg::SpellCheck()
{
	for (int n = 0; n < m_DicWords.GetCount(); n++)
	{
		if (m_Word == m_DicWords.GetAt(n)) return TRUE;
	}

	return FALSE;
}

void CSpellCheckDlg::ClearSuggestions()
{
	for (int n = 0; n < MAX_SPELL_SUGGESTIONS; n++)
	{
		m_Suggestions[n].Empty();
		m_SuggestionsProbs[n] = 0.0f;
	}
}

void CSpellCheckDlg::SortSuggestions()
{
	float tprob = 0.0f;
	CString tmp;

	for (int i = 0; i < MAX_SPELL_SUGGESTIONS; i++)
	{
		for (int j = i + 1; j < MAX_SPELL_SUGGESTIONS; j++)
		{
			if (m_SuggestionsProbs[i] < m_SuggestionsProbs[j])
			{
				tprob = m_SuggestionsProbs[i];
				m_SuggestionsProbs[i] = m_SuggestionsProbs[j];
				m_SuggestionsProbs[j] = tprob;

				tmp = m_Suggestions[i];
				m_Suggestions[i] = m_Suggestions[j];
				m_Suggestions[j] = tmp;
			}
		}
	}

}

void CSpellCheckDlg::GetSpellingSuggestions()
{
	int scount = 0;
	float pthreshold = 0.45f; //min probability to qualify as a spelling suggestion

	ClearSuggestions();

	for (int n = 0; n < m_DicWords.GetCount(); n++)
	{
		CString spell = m_DicWords.GetAt(n);
		CString tmp;

		float weight[] = { 1.0f, 0.8f, 0.7f, 0.1f };
		float matches[] = { 0,0,0,0 };
		float norm = 1.15f; //to offset weights and keep max probability at 1.0

		int lenw = m_Word.GetLength();
		int lens = spell.GetLength();

		if ((lenw < 1) || (lens < 1))continue;

		//1gram matches
		for (int i = 0; i < lenw; i++)
		{
			int pos = spell.Find(m_Word.GetAt(i));
			if (pos >= 0) matches[0]++;
		}


		//2gram matches
		if (lenw > 1)
		{
			for (int i = 0; i < lenw - 1; i++)
			{
				tmp.Empty();
				tmp.AppendChar(m_Word.GetAt(i));
				tmp.AppendChar(m_Word.GetAt(i + 1));
				int pos = spell.Find(tmp);
				if (pos >= 0) matches[1]++;
			}
		}

		//3gram matches
		if (lenw > 2)
		{
			for (int i = 0; i < lenw - 2; i++)
			{
				tmp.Empty();
				tmp.AppendChar(m_Word.GetAt(i));
				tmp.AppendChar(m_Word.GetAt(i + 1));
				tmp.AppendChar(m_Word.GetAt(i + 2));
				int pos = spell.Find(tmp);
				if (pos >= 0) matches[2]++;
			}
		}


		//first last matches
		bool firstmatch = (m_Word.GetAt(0) == spell.GetAt(0));
		bool lastmatch = (m_Word.GetAt(lenw - 1) == spell.GetAt(lens - 1));

		matches[3] = ((float)firstmatch + (float)lastmatch);

		float probability = weight[0] * matches[0] / (float)lenw;
		if (lenw > 1)
		{
			probability = probability + (weight[1] * matches[1] / (float)(lenw - 1));
		}
		if (lenw > 2)
		{
			probability = probability + (weight[2] * matches[2] / (float)(lenw - 2));
		}

		probability = probability + weight[3] * matches[3] / 2.0f;

		probability = norm * probability / 3.0f;

		if (probability > pthreshold)
		{
			m_Suggestions[scount] = spell;
			m_SuggestionsProbs[scount] = probability;
			scount++;
			if (scount >= MAX_SPELL_SUGGESTIONS) break;
		}
	}

	SortSuggestions();
}


void CSpellCheckDlg::OnBnClickedButtonNxtmisspell()
{
	bool proceed = true;
	while (proceed)
	{
		if (m_Position >= 0)
		{
			m_Word = m_Content.Tokenize(_T(" \r\n.,;:?()[]~`!@#$%^&*-+=|\\{}\"\'<>/1234567890"), m_Position);
			if (m_Position < 0)
			{
				SetDlgItemText(IDC_BUTTON_NXTMISSPELL, _T("Finished Spell Check"));
				return;
			}

			m_Word.MakeLower();

			if (!SpellCheck())
			{
				GetSpellingSuggestions();
				GetParent()->PostMessage(WM_SELMISSPELLED, (WPARAM)&m_Word, 0);
				
				SetDlgItemText(IDC_BUTTON_NXTMISSPELL, m_Word);
				m_DispSuggestion = 0;
				ShowSuggestions();

				proceed = false;
			}
		}
		else
		{
			SetDlgItemText(IDC_BUTTON_NXTMISSPELL, _T("Finished Spell Check"));
			proceed = false;
		}
	}
}

void CSpellCheckDlg::ShowSuggestions()
{
	SetDlgItemText(IDC_BUTTON_SPELL1, m_Suggestions[m_DispSuggestion + 0]);
	SetDlgItemText(IDC_BUTTON_SPELL2, m_Suggestions[m_DispSuggestion + 1]);
	SetDlgItemText(IDC_BUTTON_SPELL3, m_Suggestions[m_DispSuggestion + 2]);
	SetDlgItemText(IDC_BUTTON_SPELL4, m_Suggestions[m_DispSuggestion + 3]);

}

void CSpellCheckDlg::OnBnClickedButtonSpell1()
{
	GetParent()->PostMessage(WM_SETSPELLSUGGESTION, (WPARAM)&m_Suggestions[m_DispSuggestion + 0], 0);
}


void CSpellCheckDlg::OnBnClickedButtonSpell2()
{
	GetParent()->PostMessage(WM_SETSPELLSUGGESTION, (WPARAM)&m_Suggestions[m_DispSuggestion + 1], 0);
}


void CSpellCheckDlg::OnBnClickedButtonSpell3()
{
	GetParent()->PostMessage(WM_SETSPELLSUGGESTION, (WPARAM)&m_Suggestions[m_DispSuggestion + 2], 0);
}


void CSpellCheckDlg::OnBnClickedButtonSpell4()
{
	GetParent()->PostMessage(WM_SETSPELLSUGGESTION, (WPARAM)&m_Suggestions[m_DispSuggestion + 3], 0);
}


void CSpellCheckDlg::OnBnClickedButtonAddtodic()
{
	if (m_LastAdded == m_Word)
	{
		AfxMessageBox(_T("This word was already added."), MB_ICONINFORMATION);
		return;
	}

	m_AddedDicWords.Add(m_Word);
	m_DicWords.Add(m_Word);
	SetDlgItemText(IDC_BUTTON_NXTMISSPELL, _T("New word added"));

	m_LastAdded = m_Word;

}


void CSpellCheckDlg::OnBnClickedCancel()
{
	if (!UpdateDictionary())
	{
		AfxMessageBox(_T("Error updating the Dictionary file."), MB_ICONERROR);
	}

	CDialog::OnCancel();
}

BOOL CSpellCheckDlg::UpdateDictionary()
{
	CString dicfilename = m_SysHelper.GetPredictEdFileName(PREDICTED_DIC_FILE);
	if (dicfilename.IsEmpty()) return FALSE;

	CStdioFile dfile;
	if (!dfile.Open(dicfilename, CStdioFile::modeWrite)) return FALSE;

	dfile.SeekToEnd();

	CString line;
	for (int n = 0; n < m_AddedDicWords.GetCount(); n++)
	{
		line = m_AddedDicWords.GetAt(n);
		dfile.WriteString(_T("\n"));
		dfile.WriteString(line);
	}

	dfile.Close();
	return TRUE;
}

void CSpellCheckDlg::OnBnClickedButtonSpellprev()
{
	m_DispSuggestion -= 4;
	if (m_DispSuggestion < 0)m_DispSuggestion = 0;
	ShowSuggestions();
}


void CSpellCheckDlg::OnBnClickedButtonSpellnxt()
{
	m_DispSuggestion += 4;
	if (m_DispSuggestion > MAX_SPELL_SUGGESTIONS) m_DispSuggestion = 0;
	ShowSuggestions();
}

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
	m_LastPosition = 0;
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
END_MESSAGE_MAP()


// CSpellCheckDlg message handlers

BOOL CSpellCheckDlg::LoadDictionary()
{
	CString dicfilename = m_SysHelper.GetPredictEdFileName(PREDICTED_DIC_FILE);
	if (dicfilename.IsEmpty()) return FALSE;

	CStdioFile dfile;
	if(!dfile.Open(dicfilename, CStdioFile::modeRead)) return FALSE;

	CString line;
	
	
	while (dfile.ReadString(line))
	{
		m_DicWords.Add(line);
	}

	dfile.Close();
	return TRUE;
}

BOOL CSpellCheckDlg::SpellCheck()
{
	for (int n = 0; n < m_DicWords.GetCount(); n++)
	{
		if (m_Word == m_DicWords.GetAt(n)) return TRUE;
	}

	//CString word = _T("temporery");
	//CString spell = _T("temporary"); //in dic
	//CString tmp;

	//float weight[] = { 1.0f, 0.8f, 0.7f, 0.1f };
	//float matches[] = { 0,0,0,0 };
	//float norm = 1.15f; //to offset weights and keep max probability at 1.0

	//int lenw = word.GetLength();
	//int lens = spell.GetLength();

	////1gram matches
	////int matches1 = 0;
	//for (int i = 0; i < lenw; i++)
	//{
	//	int pos = spell.Find(word.GetAt(i));
	//	if (pos >= 0) matches[0]++;
	//}


	////2gram matches
	////int matches2 = 0;
	//if (lenw > 1)
	//{
	//	for (int i = 0; i < lenw - 1; i++)
	//	{
	//		tmp.Empty();
	//		tmp.AppendChar(word.GetAt(i));
	//		tmp.AppendChar(word.GetAt(i + 1));
	//		int pos = spell.Find(tmp);
	//		if (pos >= 0) matches[1]++;
	//	}
	//}

	////3gram matches
	////int matches3 = 0;
	//if (lenw > 2)
	//{
	//	for (int i = 0; i < lenw - 2; i++)
	//	{
	//		tmp.Empty();
	//		tmp.AppendChar(word.GetAt(i));
	//		tmp.AppendChar(word.GetAt(i + 1));
	//		tmp.AppendChar(word.GetAt(i + 2));
	//		int pos = spell.Find(tmp);
	//		if (pos >= 0) matches[2]++;
	//	}
	//}


	////first last matches
	//bool firstmatch = (word.GetAt(0) == spell.GetAt(0));
	//bool lastmatch = (word.GetAt(lenw - 1) == spell.GetAt(lens - 1));

	//matches[3] = ((float)firstmatch + (float)lastmatch);

	//float probability = weight[0] * matches[0] / (float)lenw;
	//if (lenw > 1)
	//{
	//	probability = probability + (weight[1] * matches[1] / (float)(lenw - 1));
	//}
	//if (lenw > 2)
	//{
	//	probability = probability + (weight[2] * matches[2] / (float)(lenw - 2));
	//}

	//probability = probability + weight[3] * matches[3] / 2.0f;

	//probability = norm * probability / 3.0f;

	////if(probability)

	return FALSE;


}

void CSpellCheckDlg::ClearSuggestions()
{
	for (int n = 0; n < MAX_SPELL_SUGGESTIONS; n++)
	{
		m_Suggestions[n].Empty();
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
	float pthreshold = 0.35f; //min probability to qualify as a spelling suggestion

	ClearSuggestions();

	for (int n = 0; n < m_DicWords.GetCount(); n++)
	{
		//CString word = _T("temporery");
		CString spell = m_DicWords.GetAt(n);
		CString tmp;

		float weight[] = { 1.0f, 0.8f, 0.7f, 0.1f };
		float matches[] = { 0,0,0,0 };
		float norm = 1.15f; //to offset weights and keep max probability at 1.0

		int lenw = m_Word.GetLength();
		int lens = spell.GetLength();

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
			m_LastPosition = m_Position;
			m_Word = m_Content.Tokenize(_T(" \r\n.,;:?()[]~`!@#$%^&*-+=|\\{}\"\'<>/1234567890"), m_Position);
			m_Word.MakeLower();

			if (!SpellCheck())
			{
				GetSpellingSuggestions();
				GetParent()->PostMessage(WM_SELMISSPELLED, (WPARAM)&m_Word, 0);
				SetDlgItemText(IDC_BUTTON_NXTMISSPELL, m_Word);
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


void CSpellCheckDlg::OnBnClickedButtonSpell1()
{
	m_Suggestions[0] = _T("testsqqqqqqqqqqqqqqqqqqq");

	GetParent()->PostMessage(WM_SETSPELLSUGGESTION, (WPARAM)&m_Suggestions[0], 0);

}

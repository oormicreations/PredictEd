// PredictEdCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "PredictEd.h"
#include "PredictEdCtrl.h"


// CPredictEdCtrl

IMPLEMENT_DYNAMIC(CPredictEdCtrl, CRichEditCtrl)

CPredictEdCtrl::CPredictEdCtrl()
{
	SetFmtChars('*', '/', '_');
	m_TabCount = 0;
	m_PreCaretStartPos = 0;
	m_PreCaretEndPos = 0;
	m_IsWordCommitted = FALSE;
	m_LastPreLength = 0;
	m_CaretStartPos = 0;
	m_CaretEndPos = 0;

	m_AcCaretStartPos = 0;
	m_AcCaretEndPos = 0;
	m_LastAcLength = 0;

	m_pDialog = NULL;
	m_Saved = TRUE;

	m_AutoBackupFileName = m_SysHelper.GetPredictEdFileName(PREDICTED_AUTOBK_FILE);

	m_LTMFileName = m_SysHelper.GetPredictEdFileName(PREDICTED_LTM_FILE);
	if (!m_LTMFileName.IsEmpty())
	{
		if(!m_SysHelper.CreateFileAndInit(m_LTMFileName, LTM_HEADER)) m_LTMFileName = _T("");
	}
	if (!m_LTMFileName.IsEmpty())
	{
		if (!m_LTM.LoadMap(m_LTMFileName)) AfxMessageBox(_T("Error: LTM could not be loaded!"));
	}

	m_STMFileName = m_SysHelper.GetPredictEdFileName(PREDICTED_STM_FILE);
	if (!m_STMFileName.IsEmpty())
	{
		if (!m_SysHelper.CreateFileAndInit(m_STMFileName, STM_HEADER)) m_STMFileName = _T("");
	}
	if (!m_STMFileName.IsEmpty())
	{
		if (!m_STM.LoadMap(m_STMFileName)) AfxMessageBox(_T("Error: STM could not be loaded!"));
	}


}

CPredictEdCtrl::~CPredictEdCtrl()
{
}


BEGIN_MESSAGE_MAP(CPredictEdCtrl, CRichEditCtrl)
	ON_WM_CHAR()
	ON_WM_LBUTTONUP()
	ON_WM_KEYUP()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CPredictEdCtrl message handlers


void CPredictEdCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nRepCnt != 999)
	{
		Process((TCHAR)nChar);
		return;
	}

	CRichEditCtrl::OnChar(nChar, nRepCnt, nFlags);
	UpdateQueue();
}

void CPredictEdCtrl::UpdateQueue()
{
	CString str;
	GetWindowText(str);
	str.Replace(_T("\n"), _T("")); //GetSel counts NL as one char, so remove \n
	m_CharQueue.Clear();

	long nStartChar, nEndChar;
	GetSel(nStartChar, nEndChar);
	int from = nStartChar - MAX_QUEUE_CHARS;
	if (from < 0)from = 0;
	str = str.Mid(from, nStartChar-from);
	int len = str.GetLength();
	m_CharQueue.InsertString(str);
	//m_CharQueue.Dump();

	//m_FwdCharQueue.Clear();
	//m_FwdCharQueue.ReverseInsertString(str.Mid(nEndChar, MAX_QUEUE_CHARS));
	//m_FwdCharQueue.Dump();

	m_CaretStartPos = nStartChar;
	m_CaretEndPos = nEndChar;

	m_CaretCoords = GetCaretPos();
	m_Saved = FALSE;
}

void CPredictEdCtrl::Process(TCHAR c)
{
	CString str(c);
	str = SentenceCase(c);

	if(Format(c)) str = _T("");

	Train(c);
	Predict(c);
	AutoComplete(c);
	AutoSave(c);

	for (int i = 0; i < str.GetLength(); i++)
	{
		PostMessage(WM_CHAR, str.GetAt(i), 999); //999 is sent as repcount
	}

}

int CPredictEdCtrl::FindOpenFmtTag(TCHAR fc)
{
	//check for opening tag
	CString str;
	GetWindowText(str);
	str.Replace(_T("\r\n"), _T("\r")); //NL count fix

	//find exactly two instances of formatter char in the str
	BOOL found = FALSE;
	int from = 0;
	int pos = -1;
	int len = str.GetLength();

	while (!found)
	{
		int pos1 = str.Find(fc, from);
		if (pos1 >= 0)
		{
			TCHAR t0, t1, t2;
			t0 = t1 = t2 = 'x';
			if ((pos1 - 1) > 0) t0 = str.GetAt(pos1 - 1);
			if ((pos1 + 1) < len) t1 = str.GetAt(pos1 + 1);
			if ((pos1 + 2) < len) t2 = str.GetAt(pos1 + 2);

			if ((t0 != fc) && (t2 != fc) && (t1 == fc))
			{
				found = TRUE;
				pos = pos1;
			}
			else
			{
				from = pos1 + 1;
				if (from >= len) break;
			}
		}
		else break;

	}

	return pos;
}

BOOL CPredictEdCtrl::Format(TCHAR c)
{
	TCHAR fc;

	BOOL isbold = FALSE;
	BOOL isitalic = FALSE;
	BOOL isunderline = FALSE;

	if (c == m_BoldChar) { fc = m_BoldChar; isbold = TRUE; }
	if (c == m_ItalicChar) { fc = m_ItalicChar; isitalic = TRUE; }
	if (c == m_UnderlineChar) { fc = m_UnderlineChar; isunderline = TRUE; }
	if (!(isbold || isitalic || isunderline)) return FALSE;

	if (c == fc)
	{
		//check if its a formatter
		TCHAR c1 = m_CharQueue.GetLast(0);
		TCHAR c2 = m_CharQueue.GetLast(1);
		if ((c1 == fc) && (c2 == fc)) return FALSE;

		if (c1 == fc)
		{
			//save current cursor
			long nStartChar, nEndChar;
			GetSel(nStartChar, nEndChar);

			int pos = FindOpenFmtTag(fc);

			if (pos >= 0)
			{
				//check current style
				SetSel(pos+2, nEndChar-1);
				BOOL curbold = SelectionIsBold();
				BOOL curitalic = SelectionIsItalic();
				BOOL curunderline = SelectionIsUnderlined();

				//select to the current cursor from opening pos and set style
				SetSel(pos, nEndChar);
				SetCharStyle(isbold | curbold, isitalic | curitalic, isunderline | curunderline);

				//delete opening and closing tags (DEL key won't work so replace with nothing)
				SetSel(pos, pos + 2);
				ReplaceSel(_T(""));

				long len = GetTextLength();
				SetSel(nEndChar - 2 - 1, nEndChar - 2);
				ReplaceSel(_T(""));

				SetCharStyle(FALSE, FALSE, FALSE);//toggle back

				return TRUE;
			}
		}

	}

	return FALSE;
}

CString CPredictEdCtrl::SentenceCase(TCHAR c)
{
	CString str(c);
	TCHAR c1, c2;
	c1 = m_CharQueue.GetLast(0);
	c2 = m_CharQueue.GetLast(1);

	if (c != L' ')
	{
		if (c1 == L'.')
		{
			str.MakeUpper();
			//insert newline at para break, 
			//sending second '\r' won't work so use replacesel without selection
			if (c == L'\r')
			{
				ReplaceSel(_T("\r\n"));
				SetCharStyle(FALSE, FALSE, FALSE);
			}
			else
			{
				if (c1 != L' ') str = _T(" ") + str;
			}
		}

		if (c1 == L' ')
		{
			if (c2 == L'.') str.MakeUpper();
		}

		if (c1 == L'#')
		{
			if (c2 == L'#') str.MakeUpper();
		}

		if ((c1 == L'\n') || (c1 == L'\r'))
		{
			str.MakeUpper();
		}
	}

	return str;
}


void CPredictEdCtrl::SetCharStyle(BOOL bold, BOOL italic, BOOL underline)
{
	CHARFORMAT2 cf = {};
	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE;
	cf.dwEffects = (bold ? CFE_BOLD : 0) | (italic ? CFE_ITALIC : 0) | (underline ? CFE_UNDERLINE : 0);

	SetSelectionCharFormat(cf);

}



void CPredictEdCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	UpdateQueue();
	if(m_pDialog) m_pDialog->ShowWindow(SW_HIDE);

	CRichEditCtrl::OnLButtonUp(nFlags, point);
}


void CPredictEdCtrl::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	UpdateQueue();
	if(nChar == VK_ESCAPE) if (m_pDialog) m_pDialog->ShowWindow(SW_HIDE);

	CRichEditCtrl::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CPredictEdCtrl::SetFmtChars(TCHAR b, TCHAR i, TCHAR u)
{
	m_BoldChar = b;
	m_ItalicChar = i;
	m_UnderlineChar = u;
}


BOOL CPredictEdCtrl::SelectionIsBold()
{
	CHARFORMAT cf = GetCharFormat();

	if (cf.dwEffects & CFM_BOLD) return TRUE;
	else return FALSE;
}

BOOL CPredictEdCtrl::SelectionIsItalic()
{
	CHARFORMAT cf = GetCharFormat();

	if (cf.dwEffects & CFM_ITALIC) return TRUE;
	else return FALSE;
}

BOOL CPredictEdCtrl::SelectionIsUnderlined()
{
	CHARFORMAT cf = GetCharFormat();

	if (cf.dwEffects & CFM_UNDERLINE) return TRUE;
	else return FALSE;
}

CHARFORMAT CPredictEdCtrl::GetCharFormat(DWORD dwMask)
{
	CHARFORMAT cf;
	cf.cbSize = sizeof(CHARFORMAT);

	cf.dwMask = dwMask;

	GetSelectionCharFormat(cf);

	return cf;
}

CString CPredictEdCtrl::FilterString(CString str)
{
	str.MakeLower();

	//TCHAR filtered[] = 		{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	//						'~', '`', '!', '@', '#', '$', '%', '^', '&', '*',
	//						'(', ')', '-', '_', '+', '=', '|', '\\', '{', '[', 
	//						'}', ']', ':', ';', '\"', '\'', '<', ',', '>', '.', 
	//						'?', '/', '\r', '\n'}; //44
	int pos = str.FindOneOf(_T("0123456789~!@#$%^&*()-_+=|\\{[}]:;\"\'<,>.?/\r\n"));
	if (pos >= 0)
	{
		str.Truncate(pos);
		return str;
	}

	//for (int i = 0; i < 44; i++)
	//{
	//	str.Replace(filtered[i], '=');
	//}

	//str.Replace(_T("="), _T(""));
	return str;
}

void CPredictEdCtrl::Train(TCHAR c)
{
	if ((c == ' ') || (c == '\r'))
	{
		TCHAR c1 = m_CharQueue.GetLast(0);
		if ((c1 == ' ') || (c1 == '\r')) return; //already trained

		m_CharQueue.GetWords();

		if (!m_LTM.HasKeyWord(m_CharQueue.m_Words[0]))
		{
			m_STM.AddKeyWord(m_CharQueue.m_Words[0]);
		}

		m_LTM.CreateRelation(m_CharQueue.m_Words[1], m_CharQueue.m_Words[0]);
		m_STM.CreateRelation(m_CharQueue.m_Words[1], m_CharQueue.m_Words[0]);

		CString dispstr;
		dispstr.Format(_T("KW Count LTM:%d | STM:%d | Queue: "), m_LTM.m_LastKeyWordIndex, m_STM.m_LastKeyWordIndex);
		for (int i = 0; i < MAX_WORDS; i++)
		{
			dispstr = dispstr + _T(" ") + m_CharQueue.m_Words[i];
		}

		UpdateStatusMessage(dispstr);
	}
}

void CPredictEdCtrl::UpdateStatusMessage(CString msg)
{
	CWnd * wnd = AfxGetApp()->GetMainWnd()->GetDlgItem(IDC_EDIT_WORDS);
	wnd->SetWindowText(msg);
}

void CPredictEdCtrl::Predict(TCHAR c)
{
	CString prediction;

	if ((c == ' ') || (c == '\r'))
	{
		//try stm first then ltm
		m_PredictionMap = m_STM.GetPredictions(m_CharQueue.m_Words[0]);
		if(m_PredictionMap.m_KeyWord.IsEmpty()) m_PredictionMap = m_LTM.GetPredictions(m_CharQueue.m_Words[0]);

		m_TabCount = 0;

		GetSel(m_PreCaretStartPos, m_PreCaretEndPos);
		m_IsWordCommitted = TRUE;

		prediction = _T("");

		ShowPredictions(c);

	}

	if (c == '\t')
	{
		if (m_IsWordCommitted)
		{
			SetSel(m_PreCaretStartPos + 1, m_PreCaretEndPos + 1);
			m_PreCaretEndPos = m_PreCaretStartPos;
		}
		else
		{
			//set cursor to end to avoid selection of whole text(default behaviour of CRicheditctrl)
			SetSel(m_CaretStartPos, m_CaretEndPos);
			return;
		}

		if (m_TabCount >= MAX_PREDICTION_COUNT) m_TabCount = 0;

		prediction = m_PredictionMap.m_Predictions[m_TabCount];
		m_TabCount++;

		if (prediction == _T("#"))
		{
			m_TabCount = 0;
			prediction = m_PredictionMap.m_Predictions[m_TabCount];
			m_TabCount++;
		}

		if (prediction == _T("#")) prediction = _T("");

		m_LastPreLength = prediction.GetLength();
		m_PreCaretEndPos = m_PreCaretEndPos + m_LastPreLength;


		for (int i = 0; i < prediction.GetLength(); i++)
		{
			PostMessage(WM_CHAR, prediction.GetAt(i), 999); //999 is sent as repcount
		}

		m_pDialog->ShiftWords();
	}

	if (!((c == '\t') || (c == ' ') || (c == '\r'))) m_IsWordCommitted = FALSE;

}

void CPredictEdCtrl::AutoComplete(TCHAR c)
{
	if (c == '\t')
	{
		if (m_IsWordCommitted) return;

		if (m_TabCount >= MAX_PREDICTION_COUNT) m_TabCount = 0;

		if (m_PredictionMap.m_Predictions[m_TabCount] != _T("#"))
		{
			SetSel(m_AcCaretStartPos + 1, m_AcCaretEndPos + 1);
			m_AcCaretEndPos = m_AcCaretStartPos;

			CString autocompleteword = m_PredictionMap.m_Predictions[m_TabCount].Right(m_PredictionMap.m_Predictions[m_TabCount].GetLength() - m_PartialWord.GetLength());

			if (autocompleteword.IsEmpty()) ReplaceSel(_T(""));

			m_LastAcLength = autocompleteword.GetLength();
			m_AcCaretEndPos = m_AcCaretEndPos + m_LastAcLength;

			for (int i = 0; i < autocompleteword.GetLength(); i++)
			{
				PostMessage(WM_CHAR, autocompleteword.GetAt(i), 999); //999 is sent as repcount
			}
		}

		m_TabCount++;
		m_pDialog->ShiftWords();
	}
	else
	{
		if ((c == ' ') || (c == '\r'))
		{
			m_PartialWord = _T("");
			return;
		}

		TCHAR c1 = m_CharQueue.GetLast(0);
		if ((c1 != ' ') || (c1 != '\r'))
		{
			m_PartialWord = m_CharQueue.FormString();
			int pos = m_PartialWord.ReverseFind(' ');
			if (pos < 0) pos = m_PartialWord.ReverseFind('#');

			m_PartialWord = m_PartialWord.Right(MAX_QUEUE_CHARS - pos - 1);
			m_PartialWord.AppendChar(c);
			m_PartialWord.MakeLower();
		}

		//get suggestions from LTM and fill up the blanks from STM, if any
		int count = 0;
		m_PredictionMap.InitMap();
		count = m_LTM.GetKeyWordStartingWith(m_PartialWord, m_PredictionMap.m_Predictions, count);
		count = m_STM.GetKeyWordStartingWith(m_PartialWord, m_PredictionMap.m_Predictions, count);

		m_TabCount = 0;
		GetSel(m_AcCaretStartPos, m_AcCaretEndPos);
		ShowPredictions(c);

	}
	
}

void CPredictEdCtrl::ShowPredictions(TCHAR c)
{

	if (m_pDialog == NULL)
	{
		m_pDialog = new CPreWordsDlg;
		if (!m_pDialog->Create(IDD_DIALOG_PREWORDS))
		{
			delete m_pDialog;
			m_pDialog = NULL;
		}
	}

	if (m_pDialog)
	{
		if (c != '\r') m_pDialog->ShowWindow(SW_SHOW);
		else m_pDialog->ShowWindow(SW_HIDE);

		CRect rect, dlgrect;
		GetClientRect(rect);

		m_pDialog->GetWindowRect(dlgrect);
		int sz = dlgrect.Width();
		int padding = 32;

		if (m_CaretCoords.x > (rect.right - sz)) m_CaretCoords.x = rect.right - sz;
		//if (m_CaretCoords.y > (rect.bottom - sz - padding)) m_CaretCoords.y -= sz + padding;

		m_pDialog->MoveWindow(m_CaretCoords.x, m_CaretCoords.y + padding, sz, sz);
		m_pDialog->SetWords(m_PredictionMap);

	}

	Invalidate();
	SetFocus();
	if (m_pDialog)m_pDialog->Invalidate();

}

CString CPredictEdCtrl::GetRTF()
{
	EDITSTREAM es;
	es.dwError = 0;
	es.pfnCallback = CBStreamOut;	

	CString sRTF = _T("");
	es.dwCookie = (DWORD)&sRTF;
	StreamOut(SF_RTF, es);

	return sRTF;

}

void CPredictEdCtrl::SetRTF(CString sRTF)
{
	EDITSTREAM es;
	es.dwError = 0;
	es.pfnCallback = CBStreamIn;
	es.dwCookie = (DWORD)&sRTF;
	StreamIn(SF_RTF, es);

}

/*
Callback function to stream an RTF string into the rich edit control.
*/
DWORD CALLBACK CPredictEdCtrl::CBStreamIn(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	CString *pstr = (CString *)dwCookie;

	int len = pstr->GetLength();
	if (cb > len) cb = len;

	for (int i = 0; i<cb; i++)
	{
		*(pbBuff + i) = pstr->GetAt(i);
	}

	*pcb = cb;
	*pstr = pstr->Mid(cb);

	return 0;
}

/*
Callback function to stream the RTF string out of the rich edit control.
*/
DWORD CALLBACK CPredictEdCtrl::CBStreamOut(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	// Address of our string var is in psEntry
	CString *psEntry = (CString*)dwCookie;


	CString tmpEntry = _T("");
	tmpEntry = (CString)pbBuff;

	*psEntry += tmpEntry.Left(cb);

	return 0;
}

void CPredictEdCtrl::AutoSave(TCHAR c)
{
	if ((c == '\n')|| (c == '\r'))
	{
		TCHAR c1 = m_CharQueue.GetLast(0);
		if ((c1 == '\n') || (c1 == '\r')) return; //already saved

		CString msg;

		if (!m_AutoBackupFileName.IsEmpty())
		{
			BOOL res = m_SysHelper.SaveString(m_AutoBackupFileName, GetRTF());
			if (res) msg = _T("Auto Saved...");
			else msg = _T("Error: Auto Save of text failed.");
		}

		if (!msg.IsEmpty()) UpdateStatusMessage(msg);
	}
}

void CPredictEdCtrl::Erase()
{
	INT_PTR res = AfxMessageBox(_T("Do you really wish to erase all knowledge in LTM?"), MB_YESNO);
	if (res == IDYES)
	{
		if (!m_LTMFileName.IsEmpty())
		{
			if (DeleteFile(m_LTMFileName))
			{
				if (!m_SysHelper.CreateFileAndInit(m_LTMFileName, LTM_HEADER)) m_LTMFileName = _T("");
				else UpdateStatusMessage(_T("PredictEd Memories were erased."));
			}
			else UpdateStatusMessage(_T("Error: Erasure failed!"));
		}

		m_LTM.InitList();
	}

}

void CPredictEdCtrl::TrainFromFiles()
{
	CString filelist[20];
	filelist[0] = _T("C:\\Users\\Sanjeev\\Documents\\Oormi Creations\\PredictEd\\train00.txt");
	//filelist[1] = _T("C:\\Users\\Sanjeev\\Documents\\Oormi Creations\\PredictEd\\train01.txt");

	for (int i = 0; i < 20; i++)
	{
		if (!filelist[i].IsEmpty())
		{
			UpdateStatusMessage(_T("Training ...") + filelist[i]);

			if (m_LTM.m_LastKeyWordIndex >= MAX_LIST_COUNT)
			{
				AfxMessageBox(_T("List is full, only weights and relations will be updated."));
			}

			CString content = m_SysHelper.ReadStringFromFile(filelist[i]);
			if (!content.IsEmpty())
			{
				UINT len = content.GetLength();
				CString word, lastword;

				for (UINT p = 0; p < len; p++)
				{

					TCHAR c = content.GetAt(p);
					if (c == '/')
					{
						int t=0;
					}
					if ((c != ' ') && (c != '\r') && (c != '\n') && (c != '\t') && (c != '/') && (c != '.') && (c != ',')) //word delimiters
					{
						word.AppendChar(c);
					}
					else
					{
						word = FilterString(word);
						m_LTM.AddKeyWord(word);
						m_LTM.CreateRelation(lastword, word);
						lastword = word;
						word = _T("");
					}
				}
			}
		}
	}

	m_LTM.SortList();

	if (m_LTM.SaveMap(m_LTMFileName, LTM_HEADER)) UpdateStatusMessage(_T("Training complete..."));
	else UpdateStatusMessage(_T("Error: Knowledge file could not be saved!"));

	//refresh
	m_LTM.LoadMap(m_LTMFileName);
}



void CPredictEdCtrl::OnDestroy()
{
	CRichEditCtrl::OnDestroy();

	SavePredictions();

	if (m_pDialog)
	{
		m_pDialog->DestroyWindow();
		delete m_pDialog;
		m_pDialog = NULL;
	}
}

void CPredictEdCtrl::SavePredictions()
{
	CString msg;

	m_LTM.SortList();
	m_STM.SortList();

	if (!m_LTMFileName.IsEmpty())
	{
		BOOL res = m_LTM.SaveMap(m_LTMFileName, LTM_HEADER);
		if (res) msg += (_T("Saved LTM..."));
		else msg += (_T("Error: Save LTM failed!"));
	}

	if (!m_STMFileName.IsEmpty())
	{
		BOOL res = m_STM.SaveMap(m_STMFileName, STM_HEADER);
		if (res) msg += (_T("Saved STM..."));
		else msg += (_T("Error: Save STM failed!"));
	}

	UpdateStatusMessage(msg);

}

void CPredictEdCtrl::Merge()
{
	m_LTM.SortList();
	m_STM.SortList();

	CWordList tmp;
	int pos = 0;

	//merge
	for (int i = 0; (i < MAX_LIST_COUNT) && (pos < MAX_LIST_COUNT); i++)
	{
		for (int j = pos; j < MAX_LIST_COUNT; j++)
		{
			if (m_LTM.m_WordList[j].m_Frequency >= m_STM.m_WordList[i].m_Frequency)
			{
				tmp.m_WordList[j] = m_LTM.m_WordList[j];
			}
			else
			{
				tmp.m_WordList[j] = m_STM.m_WordList[i];
				pos = j+1;
				break;
			}
		}
	}

	m_STM.InitList(); //clear stm
	m_STM.SaveMap(m_STMFileName, STM_HEADER);
	tmp.SaveMap(m_LTMFileName, LTM_HEADER); //overwrite with merged list
	m_LTM.LoadMap(m_LTMFileName); //refresh

	UpdateStatusMessage(_T("Merge complete. STM cleared."));
}


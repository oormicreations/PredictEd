#include "stdafx.h"
#include "WordList.h"


CWordList::CWordList()
{
	InitList();
}


CWordList::~CWordList()
{
}

void CWordList::InitList()
{
	m_LastKeyWordIndex = 0;
	m_LastEntrySaved = 0;

	for (int i = 0; i < MAX_LIST_COUNT; i++)
	{
		m_WordList[i].InitMap();
	}

}

BOOL CWordList::HasKeyWord(CString keyword)
{
	if (keyword.IsEmpty()) return FALSE;

	//search and if found update frequency
	for (int i = 0; i < MAX_LIST_COUNT; i++)
	{
		if (m_WordList[i].m_KeyWord.IsEmpty()) return FALSE; //rest of the list must be blank

		if (m_WordList[i].m_KeyWord == keyword)
		{
			m_WordList[i].m_Frequency++;
			return TRUE;
		}
	}
	return FALSE;
}

void CWordList::AddKeyWord(CString keyword)
{
	if (keyword.IsEmpty()) return;
	if (HasKeyWord(keyword)) return;

	//detect overflow and add it at the end
	if (m_LastKeyWordIndex < MAX_LIST_COUNT)
	{
		m_WordList[m_LastKeyWordIndex].m_KeyWord = keyword;
		m_WordList[m_LastKeyWordIndex].m_Frequency = 1;
		m_LastKeyWordIndex++;
	}

}

void CWordList::CreateRelation(CString before, CString after)
{
	if (before.IsEmpty()) return;
	if (after.IsEmpty()) return;

	for (int i = 0; i < MAX_LIST_COUNT; i++)
	{
		if (m_WordList[i].m_KeyWord.IsEmpty()) return;

		if (m_WordList[i].m_KeyWord == before)
		{
			m_WordList[i].AddPrediction(after);
		}
	}

}

CKeyWordMap CWordList::GetPredictions(CString keyword)
{
	CKeyWordMap tmap;

	for (int i = 0; i < MAX_LIST_COUNT; i++)
	{
		if (m_WordList[i].m_KeyWord == keyword)
		{
			return m_WordList[i];
		}
	}

	return tmap; //blank
}

CString CWordList::GetPredictionAt(CString keyword, int nprediction)
{
	CString prediction;

	for (int i = 0; i < MAX_LIST_COUNT; i++)
	{
		if (m_WordList[i].m_KeyWord == keyword)
		{
			prediction = m_WordList[i].m_Predictions[nprediction];
			return prediction;
		}
	}

	return prediction;

}

BOOL CWordList::SaveMap(CString filename, CString header)
{
	if (filename.IsEmpty()) return FALSE;

	CString content = header;
	int i;
	for (i = 0; i < MAX_LIST_COUNT; i++)
	{
		if (m_WordList[i].m_KeyWord.IsEmpty()) break;
		content = content + m_WordList[i].GetPredictionCsvStr() + _T("\r\n");
	}
	m_LastEntrySaved = i;

	return 	m_SysHelper.SaveString(filename, content);

}

BOOL CWordList::LoadMap(CString filename)
{
	if (filename.IsEmpty()) return FALSE;

	FILE *fStream;
	errno_t e = _tfopen_s(&fStream, filename, _T("rt,ccs=UTF-8"));
	if (e != 0)
	{
		return FALSE;
	}
	CStdioFile file(fStream);  // open the file from this stream

	CString line, token, data[MAX_MAP_LEN];

	BOOL res1 = file.ReadString(line);
	if (!res1) return FALSE;

	if (line.Find(_T("PredictEd Knowledge Map,Version,1")) < 0)
	{
		return FALSE;
	}

	int row = 0;

	while (true)
	{
		BOOL res = file.ReadString(line);
		if (!res)break;

		if (!line.IsEmpty())
		{
			int col = 0;
			int start = 0;

			while (start >= 0)
			{
				token = line.Tokenize(_T(","), start);
				data[col] = token;
				col++;
				if (col >= MAX_MAP_LEN)break;
			}

			m_WordList[row].Set(data);
		}

		row++;
		if (row >= MAX_LIST_COUNT)break; 
	}

	m_LastKeyWordIndex = row;

	file.Close();

	return TRUE;
}

void CWordList::SortList()
{
	CKeyWordMap tmap;

	for (int i = 0; i < MAX_LIST_COUNT; i++)
	{
		for (int j = i+1; j < MAX_LIST_COUNT; j++)
		{
			if (m_WordList[i].m_Frequency < m_WordList[j].m_Frequency)
			{
				tmap = m_WordList[i];
				m_WordList[i] = m_WordList[j];
				m_WordList[j] = tmap;
			}
		}
	}

}


int CWordList::GetKeyWordStartingWith(CString partstr, CString * match, int start)
{
	for (int i = 0; (i < MAX_LIST_COUNT) && (start < MAX_SUGGESTION_COUNT); i++)
	{
		if (m_WordList[i].m_KeyWord.Find(partstr) == 0)
		{
			match[start] = m_WordList[i].m_KeyWord;
			start++;
		}
	}

	return start;
}

void CWordList::GetPhrases(CString preword, CString * phrases)
{
	CString str;

	phrases[0] = _T("#");
	phrases[1] = _T("#");
	phrases[2] = _T("#");

	if (preword.IsEmpty()) return;
	if (preword == _T("#")) return;
	str = GetPredictions(preword).m_Predictions[0];
	if (str == _T("#")) return;
	phrases[0] = preword +_T(" ") + str;
	str = GetPredictions(str).m_Predictions[0];
	if (str == _T("#")) return;
	phrases[1] = phrases[0] + _T(" ") + str;
	str = GetPredictions(str).m_Predictions[0];
	if (str == _T("#")) return;
	phrases[2] = phrases[1] + _T(" ") + str;
}
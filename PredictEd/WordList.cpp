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

void CWordList::AddKeyWord(CString keyword)
{
	if (m_LastKeyWordIndex >= MAX_LIST_COUNT)
	{
		AfxMessageBox(_T("Error: Keyword list is full!"));
		return;
	}

	for (int i = 0; i < MAX_LIST_COUNT; i++)
	{
		if (m_WordList[i].m_KeyWord == keyword)
		{
			m_WordList[i].m_Frequency++;
			return;
		}
	}

	m_WordList[m_LastKeyWordIndex].m_KeyWord = keyword;
	m_WordList[m_LastKeyWordIndex].m_Frequency = 1;
	m_LastKeyWordIndex++;
}

void CWordList::CreateRelation(CString before, CString after)
{
	for (int i = 0; i < MAX_LIST_COUNT; i++)
	{
		if (m_WordList[i].m_KeyWord.IsEmpty()) return;

		if (m_WordList[i].m_KeyWord == before)
		{
			m_WordList[i].AddPrediction(after);
		}
	}

}

CString CWordList::GetPredictions(CString keyword)
{
	//can be computed in addkeyword

	CString predictions;

	for (int i = 0; i < MAX_LIST_COUNT; i++)
	{
		if (m_WordList[i].m_KeyWord == keyword)
		{
			predictions = m_WordList[i].GetPredictionStr();
			return predictions;
		}
	}

	return predictions;
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

BOOL CWordList::SaveMap(CString filename)
{
	if (filename.IsEmpty()) return FALSE;

	CFile file;
	BOOL res1 = file.Open(filename, CFile::modeWrite | CStdioFile::modeNoTruncate);
	if (!res1)
	{
		//AfxMessageBox(_T("Error : Failed to save the file"));
		return FALSE;
	}

	CString content;
	int i;
	for (i = m_LastEntrySaved; i < MAX_LIST_COUNT; i++)
	{
		if (m_WordList[i].m_KeyWord.IsEmpty()) break;
		content = content + m_WordList[i].GetPredictionCsvStr() + _T("\r\n");
	}
	m_LastEntrySaved = i;

	UINT len = (UINT)content.GetLength();
	char * outputString = (char*)malloc(len + 1);
	ZeroMemory(outputString, len + 1);

	len = WideCharToMultiByte(CP_UTF8, NULL, content, -1, outputString, len, NULL, NULL);

	file.SeekToEnd();
	file.Write(outputString, ::strlen(outputString));
	file.Close();
	free(outputString);

	return TRUE;

}


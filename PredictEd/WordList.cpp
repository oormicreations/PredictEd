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
		if(m_WordList[i].m_KeyWord == keyword) return;
	}

	m_WordList[m_LastKeyWordIndex].m_KeyWord = keyword;
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
#include "stdafx.h"
#include "KeyWordMap.h"


CKeyWordMap::CKeyWordMap()
{
}


CKeyWordMap::~CKeyWordMap()
{
}

void CKeyWordMap::InitMap()
{
	m_KeyWord = _T("");
	m_Frequency = 0;

	for (int i = 0; i < MAX_PREDICTION_COUNT; i++)
	{
		m_Predictions[i] = _T("");
		m_Weights[i] = 0;
	}

}

void CKeyWordMap::AddPrediction(CString prediction)
{
	BOOL exists = FALSE;

	for (int i = 0; i < MAX_PREDICTION_COUNT; i++)
	{
		if (m_Predictions[i] == prediction)
		{
			//already exists, so update weight
			m_Weights[i]++; 
			exists = TRUE;
			break;
		}
	}

	if (!exists)
	{
		for (int i = 0; i < MAX_PREDICTION_COUNT; i++)
		{
			if (m_Predictions[i].IsEmpty())
			{
				//add and update weight
				m_Predictions[i] = prediction;
				m_Weights[i]++;
				break;
			}
		}
	}

	SortPrediction();

}

CString CKeyWordMap::GetPredictionStr()
{
	CString prestr, tmp;

	prestr.Format(_T("KW=%s[F=%d] | "), m_KeyWord, m_Frequency);

	for (int i = 0; i < MAX_PREDICTION_COUNT; i++)
	{
		tmp.Format(_T(" | %s[%d]"), m_Predictions[i], m_Weights[i]);
		prestr = prestr /*+ _T(" | ")*/ + tmp;
	}
	return prestr;
}

void CKeyWordMap::SortPrediction()
{
	CString prestr, tmp;
	UINT w;

	for (int i = 0; i < MAX_PREDICTION_COUNT-1; i++)
	{

		if (m_Weights[i + 1] > m_Weights[i])
		{
			w = m_Weights[i];
			m_Weights[i] = m_Weights[i+1];
			m_Weights[i + 1] = w;

			tmp = m_Predictions[i];
			m_Predictions[i] = m_Predictions[i + 1];
			m_Predictions[i + 1] = tmp;
		}

		
	}
}

CString CKeyWordMap::GetPredictionCsvStr()
{
	CString prestr, tmp;
	tmp.Format(_T(",%d"), m_Frequency);

	prestr = m_KeyWord + tmp;

	for (int i = 0; i < MAX_PREDICTION_COUNT; i++)
	{
		prestr = prestr + _T(",") + m_Predictions[i];
	}

	for (int i = 0; i < MAX_PREDICTION_COUNT; i++)
	{
		tmp.Format(_T(",%d"), m_Weights[i]);
		prestr = prestr + tmp;
	}

	return prestr;
}

void CKeyWordMap::Set(CString * data)
{
	m_KeyWord = data[0];
	m_Frequency = StrToInt(data[1]);

	for (int i = 0; i < MAX_PREDICTION_COUNT; i++)
	{
		m_Predictions[i] = data[i+2];
		m_Weights[i] = StrToInt(data[i + 2 + MAX_PREDICTION_COUNT]);
	}

}


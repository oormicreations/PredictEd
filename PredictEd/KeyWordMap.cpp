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
	m_Frequency = 0;

	for (int i = 0; i < MAX_PREDICTION_COUNT; i++)
	{
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
			return;
		}
	}

	for (int i = 0; i < MAX_PREDICTION_COUNT; i++)
	{
		if (m_Predictions[i].IsEmpty())
		{
			//add and update weight
			m_Predictions[i] = prediction;
			m_Weights[i]++; 
			return;
		}
	}


}

CString CKeyWordMap::GetPredictionStr()
{
	CString prestr, tmp;

	for (int i = 0; i < MAX_PREDICTION_COUNT; i++)
	{
		tmp.Format(_T("%s[%d]"), m_Predictions[i], m_Weights[i]);
		prestr = prestr + _T(" | ") + tmp;
	}
	return prestr;
}
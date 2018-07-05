#pragma once
#define MAX_PREDICTION_COUNT 5
#define MAX_PRECEEDING_COUNT 3


class CKeyWordMap
{
public:
	CKeyWordMap();
	~CKeyWordMap();

	CString m_KeyWord;
	CString m_Predictions[MAX_PREDICTION_COUNT];
	CString m_PreceedingWords[MAX_PREDICTION_COUNT];
	UINT m_Weights[MAX_PREDICTION_COUNT];
	UINT m_Frequency; //to be used for word completion suggestions

	void InitMap();
	void AddPrediction(CString prediction);
	CString GetPredictionStr();
	void SortPrediction();
};


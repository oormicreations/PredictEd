#pragma once
#define MAX_PREDICTION_COUNT 5
#define MAX_PRECEEDING_COUNT 3
#define MAX_MAP_LEN MAX_PREDICTION_COUNT*2 + 2


class CKeyWordMap
{
public:
	CKeyWordMap();
	~CKeyWordMap();

	CString m_KeyWord;
	UINT m_Frequency; //to be used for word completion suggestions and sorting the list
	CString m_Predictions[MAX_PREDICTION_COUNT];
	//CString m_PreceedingWords[MAX_PREDICTION_COUNT];
	UINT m_Weights[MAX_PREDICTION_COUNT];

	void InitMap();
	void AddPrediction(CString prediction);
	CString GetPredictionStr();
	void SortPrediction();
	CString GetPredictionCsvStr();
	void Set(CString * data);
};


#pragma once
#include "KeyWordMap.h"

#define MAX_LIST_COUNT 100

class CWordList
{
public:
	CWordList();
	~CWordList();

	CKeyWordMap m_WordList[MAX_LIST_COUNT];
	UINT m_LastKeyWordIndex;
	UINT m_LastEntrySaved;


	void InitList();
	BOOL HasKeyWord(CString keyword);
	void AddKeyWord(CString keyword);
	void CreateRelation(CString before, CString after);
	CKeyWordMap GetPredictions(CString keyword);
	CString GetPredictionAt(CString keyword, int nprediction);
	BOOL SaveMap(CString filename, CString header);
	BOOL LoadMap(CString filename);
	void SortList();

};


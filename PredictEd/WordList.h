#pragma once
#include "KeyWordMap.h"
#include "SysHelper.h"


#ifdef _DEBUG
#define MAX_LIST_COUNT 100
#else
#define MAX_LIST_COUNT 5000
#endif

#define MAX_SUGGESTION_COUNT 5
#define MAX_PHRASE_COUNT 3

class CWordList
{
public:
	CWordList();
	~CWordList();

	CKeyWordMap m_WordList[MAX_LIST_COUNT];
	UINT m_LastKeyWordIndex;
	UINT m_LastEntrySaved;
	CSysHelper m_SysHelper;

	void InitList();
	BOOL HasKeyWord(CString keyword);
	void AddKeyWord(CString keyword);
	void CreateRelation(CString before, CString after);
	CKeyWordMap GetPredictions(CString keyword);
	CString GetPredictionAt(CString keyword, int nprediction);
	BOOL SaveMap(CString filename, CString header);
	BOOL LoadMap(CString filename);
	void SortList();
	int GetKeyWordStartingWith(CString partstr, CString * match, int start);
	void GetPhrases(CString preword, CString *phrases);
};


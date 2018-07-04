#pragma once
#include "CharQueue.h"
#include "SysHelper.h"
#include "WordList.h"

// CPredictEdCtrl

class CPredictEdCtrl : public CRichEditCtrl
{
	DECLARE_DYNAMIC(CPredictEdCtrl)

public:
	CPredictEdCtrl();
	virtual ~CPredictEdCtrl();

	//CSysHelper m_SysHelper;
	//CString m_InWords[MAX_WORDS];

	CCharQueue m_CharQueue, m_FwdCharQueue;
	int m_FormatterPos, m_TabCount;
	TCHAR m_BoldChar, m_ItalicChar, m_UnderlineChar;
	CWordList m_KnowledgeMap;

	void Train(TCHAR c);
	CString Predict(TCHAR c);
	void Process(TCHAR c);
	CString SentenceCase(TCHAR c);
	void UpdateQueue();
	BOOL Format(TCHAR c);
	void SetCharStyle(BOOL bold, BOOL italic, BOOL underline);
	int FindOpenFmtTag(TCHAR fc);
	void SetFmtChars(TCHAR b, TCHAR i, TCHAR u);
	CHARFORMAT GetCharFormat(DWORD dwMask = CFM_COLOR | CFM_FACE | CFM_SIZE | CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE);
	BOOL SelectionIsUnderlined();
	BOOL SelectionIsBold();
	BOOL SelectionIsItalic();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};



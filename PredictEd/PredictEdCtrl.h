#pragma once
#include "CharQueue.h"
#include "SysHelper.h"

#define FMT_BOLD 1
#define FMT_ITALICS 2
#define FMT_UNDERLINE 3
#define MAX_WORDS 5

// CPredictEdCtrl

class CPredictEdCtrl : public CRichEditCtrl
{
	DECLARE_DYNAMIC(CPredictEdCtrl)

public:
	CPredictEdCtrl();
	virtual ~CPredictEdCtrl();

	CCharQueue m_CharQueue, m_FwdCharQueue;
	//CSysHelper m_SysHelper;
	int m_FormatterPos;
	TCHAR m_BoldChar, m_ItalicChar, m_UnderlineChar;
	CString m_InWords[MAX_WORDS];

	void Train(TCHAR c);
	CString Predict();
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



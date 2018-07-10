#pragma once
#include "CharQueue.h"
#include "SysHelper.h"
#include "WordList.h"
#include "PreWordsDlg.h"

// CPredictEdCtrl

class CPredictEdCtrl : public CRichEditCtrl
{
	DECLARE_DYNAMIC(CPredictEdCtrl)

public:
	CPredictEdCtrl();
	virtual ~CPredictEdCtrl();

	CSysHelper m_SysHelper;
	CCharQueue m_CharQueue, m_FwdCharQueue;
	int m_FormatterPos, m_TabCount;
	TCHAR m_BoldChar, m_ItalicChar, m_UnderlineChar;
	CWordList m_KnowledgeMap;
	long m_PreCaretStartPos, m_PreCaretEndPos;
	long m_CaretStartPos, m_CaretEndPos;
	BOOL m_IsWordCommitted;
	UINT m_LastPreLength;
	CString m_AutoBackupFileName;
	CString m_KnowledgeMapFileName;
	CPoint m_CaretCoords;
	CPreWordsDlg *m_pDialog;

	void UpdateQueue();
	void Train(TCHAR c);
	void Predict(TCHAR c);
	void Process(TCHAR c);
	void Save(TCHAR c);

	CString SentenceCase(TCHAR c);

	BOOL Format(TCHAR c);
	void SetCharStyle(BOOL bold, BOOL italic, BOOL underline);
	int FindOpenFmtTag(TCHAR fc);
	void SetFmtChars(TCHAR b, TCHAR i, TCHAR u);
	CHARFORMAT GetCharFormat(DWORD dwMask = CFM_COLOR | CFM_FACE | CFM_SIZE | CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE);
	BOOL SelectionIsUnderlined();
	BOOL SelectionIsBold();
	BOOL SelectionIsItalic();

	void SetRTF(CString sRTF);
	CString GetRTF();

	void UpdateStatusMessage(CString msg);
	void Erase();
	void TrainFromFiles();
	CString FilterString(CString str);

private:
	static DWORD CALLBACK CBStreamIn(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
	static DWORD CALLBACK CBStreamOut(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG* pcb);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDestroy();
};



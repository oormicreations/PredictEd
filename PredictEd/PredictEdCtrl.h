#pragma once
#include "CharQueue.h"
#include "SysHelper.h"


// CPredictEdCtrl

class CPredictEdCtrl : public CRichEditCtrl
{
	DECLARE_DYNAMIC(CPredictEdCtrl)

public:
	CPredictEdCtrl();
	virtual ~CPredictEdCtrl();

	CCharQueue m_CharQueue;
	//CSysHelper m_SysHelper;

	void Train();
	CString Predict();
	void Process(TCHAR c);
	CString SentenceCase(TCHAR c);
	void UpdateQueue();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};



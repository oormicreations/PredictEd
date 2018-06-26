// PredictEdCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "PredictEd.h"
#include "PredictEdCtrl.h"


// CPredictEdCtrl

IMPLEMENT_DYNAMIC(CPredictEdCtrl, CRichEditCtrl)

CPredictEdCtrl::CPredictEdCtrl()
{
}

CPredictEdCtrl::~CPredictEdCtrl()
{
}


BEGIN_MESSAGE_MAP(CPredictEdCtrl, CRichEditCtrl)
	ON_WM_CHAR()
END_MESSAGE_MAP()



// CPredictEdCtrl message handlers


void CPredictEdCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_CharQueue.Insert((TCHAR)nChar);

	if (nChar == 22) //ctrl-v
	{
		CString pasted = m_SysHelper.GetClipboardText();
		m_CharQueue.InsertString(pasted);
	}

	if (nChar == 32) //space
	{
		Train();
		Predict();
	}

	if (nChar == 46) //dot
	{
		//PostMessage(WM_CHAR, L' ', 0);
		//return;
	}

	m_CharQueue.Dump();
	CRichEditCtrl::OnChar(nChar, nRepCnt, nFlags);
}

void CPredictEdCtrl::Train()
{
}

CString CPredictEdCtrl::Predict()
{
	CString preStr = _T("");

	return preStr;
}
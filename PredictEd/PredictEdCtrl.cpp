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
	//DEL key press is not detected, so can't process that

	if (nRepCnt != 999)
	{
		Process((TCHAR)nChar);
		return;
	}

	CRichEditCtrl::OnChar(nChar, nRepCnt, nFlags);
	UpdateQueue();
	m_CharQueue.Dump();
}

void CPredictEdCtrl::UpdateQueue()
{
	//GetLine won't get the newline char so getwindowtext is used

	//int nLineLength, nLineLengthBuf, nLineIndex, nLineCount = GetLineCount();
	//CString strText, str;

	//for (int i = nLineCount-1; i >=0 ; i--)
	//{
	//	nLineIndex = LineIndex(i);
	//	nLineLength = LineLength(nLineIndex);
	//	nLineLengthBuf = nLineLength;
	//	if (nLineLength < 2)nLineLengthBuf = 2;
	//	GetLine(i, strText.GetBufferSetLength(nLineLengthBuf + 1), nLineLengthBuf);

	//	strText.SetAt(nLineLength, _T('\0')); // null terminate
	//	strText.ReleaseBuffer(nLineLength + 1);

	//	CString t(strText.GetBuffer()); //length correction
	//	str = t + str;
	//	//int len1 = t.GetLength();
	//	int len = str.GetLength();

	//	m_CharQueue.Clear();
	//	m_CharQueue.InsertString(str);
	//	TRACE(_T("line %d: '%s'\r\n"), i, strText);
	//	if (len > MAX_QUEUE_CHARS)
	//	{
	//		break;
	//	}

	//}

	CString str;
	GetWindowText(str);
	m_CharQueue.Clear();
	m_CharQueue.InsertString(str.Right(MAX_QUEUE_CHARS));
}

void CPredictEdCtrl::Process(TCHAR c)
{
	CString str = SentenceCase(c);

	for (int i = 0; i < str.GetLength(); i++)
	{
		PostMessage(WM_CHAR, str.GetAt(i), 999); //999 is sent as repcount
	}
}


CString CPredictEdCtrl::SentenceCase(TCHAR c)
{
	CString str(c);
	TCHAR c1, c2;
	c1 = m_CharQueue.GetLast(0);
	c2 = m_CharQueue.GetLast(1);

	if (c != L' ')
	{
		if (c1 == L'.')
		{
			str.MakeUpper();
			//insert newline at para break, 
			//sending second '\r' won't work so use replacesel without selection
			if (c == L'\r') ReplaceSel(_T("\r\n")); 
			else
			{
				if (c1 != L' ') str = _T(" ") + str;
			}
		}

		if (c1 == L' ')
		{
			if (c2 == L'.') str.MakeUpper();
		}

		if (c1 == L'#')
		{
			if (c2 == L'#') str.MakeUpper();
		}

		if (c1 == L'\n')
		{
			str.MakeUpper();
		}

	}

	return str;
}


void CPredictEdCtrl::Train()
{
}

CString CPredictEdCtrl::Predict()
{
	CString preStr = _T("");

	return preStr;
}

//if (nChar == 32) //space
//{
//	Train();
//	Predict();
//}

//if (nChar == 46) //dot
//{
//	//PostMessage(WM_CHAR, L' ', 0);
//	//return;
//}



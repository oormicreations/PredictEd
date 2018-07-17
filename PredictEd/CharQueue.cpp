#include "stdafx.h"
#include "CharQueue.h"


CCharQueue::CCharQueue()
{
	Clear();
}


CCharQueue::~CCharQueue()
{
}


void CCharQueue::Clear()
{
	for (int i = 0; i < MAX_QUEUE_CHARS; i++)
	{
		m_Queue[i] = L'#';
	}
}

void CCharQueue::Dump()
{
	TRACE(FormString() + _T("\r\n"));
}

void CCharQueue::Insert(TCHAR c)
{
	for (int i = 1; i < MAX_QUEUE_CHARS; i++)
	{
		m_Queue[i - 1] = m_Queue[i];
	}

	m_Queue[MAX_QUEUE_CHARS - 1] = c;

	//for (int i = 0; i < MAX_QUEUE_CHARS; i++)
	//{
	//	m_Queue[i] = c;
	//}

}

void CCharQueue::ReverseInsert(TCHAR c)
{
	for (int i = MAX_QUEUE_CHARS-1; i > 0; i--)
	{
		m_Queue[i] = m_Queue[i-1];
	}

	m_Queue[0] = c;
}

CString CCharQueue::GetPartialWord(TCHAR c)
{
	CString str;
	for (int i = MAX_QUEUE_CHARS - 1; i > 0; i--)
	{
		if ((m_Queue[i] == ' ') || (m_Queue[i] == '#') || (m_Queue[i] == '\r') || (m_Queue[i] == '.') || (m_Queue[i] == ',')) break;
		else str.AppendChar(m_Queue[i]);
	}

	str.MakeReverse();
	str.AppendChar(c);
	str.MakeLower();

	return str;
}

CString CCharQueue::FormString()
{
	CString str;

	str.SetString(m_Queue, MAX_QUEUE_CHARS);

	return str;
}

void CCharQueue::InsertString(CString str)
{
	int len = str.GetLength();
	for (int i = 0; i < len; i++)
	{
		Insert(str.GetAt(i));
	}

}

void CCharQueue::ReverseInsertString(CString str)
{
	int len = str.GetLength();
	for (int i = 0; i < len; i++)
	{
		m_Queue[i] = (str.GetAt(i));
	}

}


TCHAR CCharQueue::GetLast(int pos)
{
	return m_Queue[MAX_QUEUE_CHARS - 1 - pos];
}

//CString CCharQueue::GetWord(int wordpos)
//{
//	CString str;
//	int pos = 0;
//	int count = 0;
//
//	if(GetLast(0)==' ') return str;
//
//	for (int i = MAX_QUEUE_CHARS - 1; i >= 0; i--)
//	{
//		TCHAR c = m_Queue[i];
//
//		if (c == '#')
//		{
//			if (i > 0)
//			{
//				if (m_Queue[i - 1] == '#')
//				{
//					if (m_Queue[i + 1] != '#')
//					{
//						count++;
//						pos = i;
//					}
//				}
//			}
//		}
//
//		if ((c == ' ') || (c == '\r'))
//		{
//			count++;
//			pos = i;
//		}
//		if (count == (wordpos + 1))
//		{
//			break;
//		}
//	}
//
//	if (count != (wordpos + 1)) return str;
//
//	for (int i = pos + 1; i < MAX_QUEUE_CHARS; i++)
//	{
//		TCHAR c = m_Queue[i];
//		if (c == '#')continue;
//		if (c == ' ')break;
//		str.AppendChar(c);
//	}
//
//	return FilterWord(str);
//
//}
//
//CString CCharQueue::FilterWord(CString str)
//{
//	if (str.IsEmpty()) return str;
//
//	str.MakeLower();
//
//	TCHAR filtered[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
//		'~', '`', '!', '@', '#', '$', '%', '^', '&', '*',
//		'(', ')', '-', '_', '+', '=', '|', '\\', '{', '[', '}', ']',
//		':', ';', '\"', '\'', '<', ',', '>', '.', '?', '/' }; //42
//
//	for (int i = 0; i < 42; i++)
//	{
//		int pos = str.Find(filtered[i]);
//		if (pos >= 0)
//		{
//			str.Delete(pos, 1);
//		}
//	}
//
//	return str;
//}

void CCharQueue::GetWords()
{
	CString str;
	int pos = 0;
	int count = 0;

	str = CString(m_Queue, MAX_QUEUE_CHARS);
	str.MakeLower();

	TCHAR filtered[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
		'~', '`', '!', '@', '#', '$', '%', '^', '&', '*',
		'(', ')', '-', '_', '+', '=', '|', '\\', '{', '[', '}', ']',
		':', ';', '\"', '\'', '<', ',', '>', '.', '?', '/' }; //42

	for (int i = 0; i < 42; i++)
	{
		str.Replace(filtered[i], '=');
	}

	str.Replace(_T("="), _T(""));
	str.Replace(_T("\r"), _T(" "));

	int spcount = 1;
	while (spcount)
	{
		spcount = str.Replace(_T("  "), _T(" "));
	}

	str.Trim();

	spcount = 0;
	while (spcount < 5)
	{
		int pos = str.ReverseFind(' ');
		if (pos < 0) break;

		m_Words[spcount] = str.Right(str.GetLength() - pos - 1);
		str = str.Left(pos);
		spcount++;
	}

	if (spcount < 5) m_Words[spcount] = str;

}
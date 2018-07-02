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
}

void CCharQueue::ReverseInsert(TCHAR c)
{
	for (int i = MAX_QUEUE_CHARS-1; i > 0; i--)
	{
		m_Queue[i] = m_Queue[i-1];
	}

	m_Queue[0] = c;
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

CString CCharQueue::GetWord(int wordpos)
{
	CString str;
	int pos = 0;
	int count = 0;

	for (int i = MAX_QUEUE_CHARS - 1; i >= 0; i--)
	{
		TCHAR c = m_Queue[i];
		if ((c == ' ') || (c == '#'))
		{
			count++;
			pos = i;
		}
		if (count == (wordpos + 1))
		{
			break;
		}
	}

	if (count != (wordpos + 1)) return str;

	for (int i = pos + 1; i < MAX_QUEUE_CHARS; i++)
	{
		TCHAR c = m_Queue[i];
		if (c == '#')continue;
		if (c == ' ')break;
		str.AppendChar(c);
	}

	return str;

}
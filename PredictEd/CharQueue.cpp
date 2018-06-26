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
		m_Queue[i] = L' ';
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

CString CCharQueue::FormString()
{
	CString str;

	str.SetString(m_Queue, MAX_QUEUE_CHARS);

	return str;
}

void CCharQueue::InsertString(CString str)
{
	for (int i = 0; i < str.GetLength(); i++)
	{
		Insert(str.GetAt(i));
	}

}
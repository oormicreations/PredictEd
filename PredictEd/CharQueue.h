#pragma once
#define MAX_QUEUE_CHARS 100


class CCharQueue
{
public:
	CCharQueue();
	~CCharQueue();

	TCHAR m_Queue[MAX_QUEUE_CHARS];

	void Clear();
	void Dump();
	void Insert(TCHAR c);
	CString FormString();
	void InsertString(CString str);
};


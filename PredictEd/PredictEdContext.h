#pragma once
#include "SysHelper.h"



class CPredictEdContext
{
public:
	CPredictEdContext();
	~CPredictEdContext();

	CSysHelper m_SysHelper;

	CString m_sContextFile;
	CString m_sContextName;
	CString m_sContextDir;
	CString m_sVersion;
	CString m_sDictionary;
	CString m_sLTMFile;
	CString m_sSTMFile;

	BOOL m_bAutoLoad;
	BOOL m_bCopy;

	void SetVersion(int iVmaj, int iVmin);
	BOOL LoadContext();
	BOOL CreateContext();
	CString GetValue(CString sEntry, CString sKey);
	BOOL CreateDefaultContext();
	BOOL GetNewContextName();
	BOOL UpdateContext();

};

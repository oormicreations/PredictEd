#pragma once
class CSysHelper
{
public:
	CSysHelper();
	~CSysHelper();

	CString m_FileName;

	CString GetClipboardText();
	BOOL SetClipboardText(CString text);
	CString GetFileContent();
	BOOL SetFileContent(CString content);
};


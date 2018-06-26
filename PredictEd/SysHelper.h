#pragma once
class CSysHelper
{
public:
	CSysHelper();
	~CSysHelper();

	CString GetClipboardText();
	BOOL SetClipboardText(CString text);
};


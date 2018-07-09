#pragma once
#define PREDICTED_USER_FOLDER 1

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
	BOOL SaveString(CString filename, CString content);
	CString GetAutoBackupFileName();
	CString GetKnowledgeMapFileName();
	CString GetUserDocumentPath(UINT type);
	BOOL CreateFileAndInit(CString filename, CString content);
	CString ReadStringFromFile(CString filename);
};


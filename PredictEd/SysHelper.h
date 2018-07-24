#pragma once
#define PREDICTED_USER_FOLDER 1
#define PREDICTED_AUTOBK_FILE 1
#define PREDICTED_LTM_FILE 2
#define PREDICTED_STM_FILE 3
#define TWIPS_PER_PT 20

class CSysHelper
{
public:
	CSysHelper();
	~CSysHelper();

	CString m_FileName, m_FileTitle;

	CString GetClipboardText();
	BOOL SetClipboardText(CString text);
	CString GetFileContent();
	BOOL SetFileContent(CString content);
	BOOL SaveString(CString filename, CString content);
	//CString GetAutoBackupFileName();
	CString GetPredictEdFileName(UINT type);
	CString GetUserDocumentPath(UINT type);
	BOOL CreateFileAndInit(CString filename, CString content);
	CString ReadStringFromFile(CString filename);
	bool IsFontInstalled(LPCTSTR lpszFont);
};


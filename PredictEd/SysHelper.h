#pragma once
#define PREDICTED_USER_FOLDER 1
#define PREDICTED_AUTOBK_FILE 1
#define PREDICTED_LTM_FILE 2
#define PREDICTED_STM_FILE 3
#define PREDICTED_DEC_FILE 4
#define PREDICTED_DIC_FILE 5
#define PREDICTED_CON_FILE 6
#define PREDICTED_DIC_FOLDER 7
#define PREDICTED_CON_FOLDER 8
#define PREDICTED_DOC_FOLDER 9

#define TWIPS_PER_PT 20

#define LTM_HEADER 1//_T("PredictEd Knowledge Map,Version,11,LTM\r\n")
#define STM_HEADER 2//_T("PredictEd Knowledge Map,Version,11,STM\r\n")

#define PREDICTED_CONTEXT_DEFAULT_CONTEXT	_T("Contexts\\Default.context")

#define PREDICTED_CONTEXT_DIR_NAME	_T("Contexts")
#define PREDICTED_DOC_DIR_NAME		_T("Documents")
#define PREDICTED_DIC_DIR_NAME		_T("Dictionary")

#define PREDICTED_CONTEXT_MAX_ENTRIES	4

#define PREDICTED_CONTEXT_KEY1 _T("PredictEd Context File Version,")
#define PREDICTED_CONTEXT_KEY2 _T("Context Name,")
#define PREDICTED_CONTEXT_KEY3 _T("Knowledge Dir,")
#define PREDICTED_CONTEXT_KEY4 _T("Dictionary File,")

#define PREDICTED_AUTOBK_FILE_NAME	_T("PredictEd_AutoBackup.rtf")
#define PREDICTED_LTM_FILE_NAME		_T("PredictEd_LTM.txt")
#define PREDICTED_STM_FILE_NAME		_T("PredictEd_STM.txt")
#define PREDICTED_DEC_FILE_NAME		_T("PredictEd_tmp.dec")
#define PREDICTED_DIC_FILE_NAME		_T("Dictionary\\Default\\DefaultDict.txt")
#define PREDICTED_CON_FILE_NAME		_T("Contexts\\Default.context")



class CSysHelper
{
public:
	CSysHelper();
	~CSysHelper();

	CString m_FileName, m_FileTitle, m_FileExt, m_FileNameNoPath, m_FilePath;
	static CString m_LTMHeader, m_STMHeader; //remains same in all instances of this class

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
	void SelectMultipleFiles(CString * files, int maxfiles);
	BOOL GetSaveFileNameType();
	BOOL SaveAsTextFile(CString content);
	BOOL GetFileNameToOpen(CString filetype, CString caption);
	CString GetFileNameFromPath(CString filepath);
	BOOL SysGetFileNameToSave(BOOL isopen, CString caption, CString initialdir, CString ext1, CString ext2 = _T(""), CString ext3 = _T(""));
	void EmptyFileData();
	bool SHCopy(LPCTSTR from, LPCTSTR to);
	bool SHDelete(LPCTSTR from);
	void SetHeaders(int vmaj, int vmin);
	CString GetHeader(int type);
};


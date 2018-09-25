#include "stdafx.h"
#include "SysHelper.h"

CString CSysHelper::m_LTMHeader;
CString CSysHelper::m_STMHeader;

CSysHelper::CSysHelper()
{
}


CSysHelper::~CSysHelper()
{
}


CString CSysHelper::GetClipboardText()
{
	// Try opening the clipboard
	if (!OpenClipboard(0))
	{
		//AfxMessageBox(_T("<Failed to access the clipboard>"));
		//MS Word or Excel will cause problems by locking up the clipbroad many times. 
		//So return last clip in case of failure.
		return _T("");
	}

	// Get handle of clipboard object for unicode text
	HANDLE hData = GetClipboardData(CF_UNICODETEXT);
	if (hData == nullptr)
	{
		//AfxMessageBox(_T("<No data in the clipboard>"));
		CloseClipboard();
		return _T("");
	}

	// Lock the handle to get the actual text pointer
	TCHAR * pszText = static_cast<TCHAR*>(GlobalLock(hData));
	if (pszText == nullptr)
	{
		//AfxMessageBox(_T("<The clipboard has no text>"));
		// Release the lock
		GlobalUnlock(hData);

		// Release the clipboard
		CloseClipboard();

		return _T("");
	}

	// Save text in a string class instance
	CString text(pszText);

	// Release the lock
	GlobalUnlock(hData);

	// Release the clipboard
	CloseClipboard();

	return text;
}


BOOL CSysHelper::SetClipboardText(CString text)
{
	// Try opening the clipboard
	if (!OpenClipboard(0))
	{
		return FALSE;
	}

	LPTSTR  lptstrCopy;
	HGLOBAL hglbCopy;
	int iSize = (text.GetLength() + 1) * sizeof(TCHAR);

	hglbCopy = GlobalAlloc(GMEM_MOVEABLE, iSize);
	if (hglbCopy == NULL)
	{
		CloseClipboard();
		return FALSE;
	}

	// Lock the handle and copy the text to the buffer. 
	lptstrCopy = (LPTSTR)GlobalLock(hglbCopy);
	ZeroMemory(lptstrCopy, iSize); // adds null character also
	memcpy(lptstrCopy, text.GetBuffer(), text.GetLength() * sizeof(TCHAR));

	//lptstrCopy[iSize] = /*(TCHAR)*/0;    // null character 
	GlobalUnlock(hglbCopy);

	EmptyClipboard();
	HANDLE hData = SetClipboardData(CF_UNICODETEXT, hglbCopy);
	if (hData == nullptr)
	{
		CloseClipboard();
		return FALSE;
	}

	// Release the clipboard
	CloseClipboard();

	return TRUE;
}

CString CSysHelper::ReadStringFromFile(CString filename)
{
	CString content;
	CFile file;

	if (file.Open(filename, CFile::modeRead))
	{
		int len = file.GetLength();
		char * buf = new char[len + 1];

		file.Read(buf, len);
		file.Close();

		buf[len] = 0;

		int cc = 0;
		// get length (cc) of the new widechar excluding the \0 terminator first
		if ((cc = MultiByteToWideChar(CP_UTF8, 0, buf, -1, NULL, 0) - 1) > 0)
		{
			// convert
			wchar_t *buf1 = content.GetBuffer(cc);
			if (buf1) MultiByteToWideChar(CP_UTF8, 0, buf, -1, buf1, cc);
			content.ReleaseBuffer();
		}

		delete[] buf;
	}

	return content;
}

CString CSysHelper::GetFileContent()
{
	CFileDialog DataFileOpenDialog(true, _T("rtf"), _T(""), OFN_FILEMUSTEXIST, _T("Rich text Files (*.rtf)|*.rtf|Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"));
	DataFileOpenDialog.m_ofn.lpstrTitle = _T("Load an RTF or Text File ...");
	//DataFileOpenDialog.m_ofn.lpstrInitialDir = GetUserDocumentPath(PREDICTED_FOLDER);

	INT_PTR res = DataFileOpenDialog.DoModal();
	if (res == IDCANCEL) return _T("");
	m_FileName = DataFileOpenDialog.GetPathName();
	if (m_FileName.IsEmpty()) return _T("");
	m_FileExt = DataFileOpenDialog.GetFileExt();

	m_FileTitle = DataFileOpenDialog.GetFileTitle();
	if (m_FileTitle.GetLength() > 20)
	{
		m_FileTitle.Truncate(20);
		m_FileTitle = m_FileTitle + _T("...");
	}

	return ReadStringFromFile(m_FileName);
}

BOOL CSysHelper::GetSaveFileNameType()
{
	CFileDialog DataFileOpenDialog(false, _T("rtf"), _T(""), OFN_OVERWRITEPROMPT, _T("Rich text Files (*.rtf)|*.rtf|Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"));
	DataFileOpenDialog.m_ofn.lpstrTitle = _T("Save an RTF or Text File ...");
	//DataFileOpenDialog.m_ofn.lpstrInitialDir = GetUserDocumentPath(PREDICTED_FOLDER);

	INT_PTR res = DataFileOpenDialog.DoModal();
	if (res == IDCANCEL) return FALSE;
	m_FileName = DataFileOpenDialog.GetPathName();
	if (m_FileName.IsEmpty()) return FALSE;
	m_FileExt = DataFileOpenDialog.GetFileExt();

	m_FileTitle = DataFileOpenDialog.GetFileTitle();
	if (m_FileTitle.GetLength() > 10)
	{
		m_FileTitle.Truncate(10);
		m_FileTitle = m_FileTitle + _T("...");
	}

	return TRUE;
}

BOOL CSysHelper::SetFileContent(CString content)
{
	if (m_FileName.IsEmpty())
	{
		GetSaveFileNameType();
	}

	if(!SaveString(m_FileName, content)) m_FileName = _T("");
	
	return TRUE;

}

BOOL CSysHelper::SaveString(CString filename, CString content)
{
	if (filename.IsEmpty()) return FALSE;

	CFile file;
	BOOL res1 = file.Open(filename, CFile::modeWrite|CFile::modeCreate);
	if (!res1)
	{
		AfxMessageBox(_T("Error : Failed to save the file"));
		return FALSE;
	}

	UINT len = (UINT)content.GetLength() * sizeof(TCHAR);
	char * outputString = (char*)malloc(len+1);
	ZeroMemory(outputString, len+1);

	len = WideCharToMultiByte(CP_UTF8, NULL, content, -1, outputString, len, NULL, NULL);

	file.Write(outputString, ::strlen(outputString));
	file.Close();
	free(outputString);

	return TRUE;

}

//CString CSysHelper::GetAutoBackupFileName()
//{
//	CString path = GetUserDocumentPath(PREDICTED_USER_FOLDER);
//	if (!path.IsEmpty())
//	{
//		path = path + _T("\\PredicEd_AutoBackup.rtf");
//		return path;
//	}
//	return _T("");
//}

CString CSysHelper::GetPredictEdFileName(UINT type)
{
	CString path = GetUserDocumentPath(PREDICTED_USER_FOLDER);

	if (!path.IsEmpty())
	{
		CString fname;
		if (type == PREDICTED_AUTOBK_FILE)	fname = PREDICTED_AUTOBK_FILE_NAME;
		if (type == PREDICTED_LTM_FILE)		fname = PREDICTED_LTM_FILE_NAME;
		if (type == PREDICTED_STM_FILE)		fname = PREDICTED_STM_FILE_NAME;
		if (type == PREDICTED_DEC_FILE)		fname = PREDICTED_DEC_FILE_NAME;
		if (type == PREDICTED_DIC_FILE)		fname = PREDICTED_DIC_FILE_NAME;
		if (type == PREDICTED_CON_FILE)		fname = PREDICTED_CON_FILE_NAME;

		if (fname.IsEmpty()) return _T("");

		path = path + _T("\\") + fname;
		return path;
	}

	return _T("");
}

CString CSysHelper::GetUserDocumentPath(UINT type)
{
	TCHAR my_documents[MAX_PATH];
	HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);

	if (result != S_OK)
	{
		return _T("");
	}
	else
	{
		CString str;
		str.SetString(my_documents);

		if (type == PREDICTED_USER_FOLDER)
		{
			str = str + _T("\\Oormi Creations\\PredictEd dev");
		}

		if (GetFileAttributes(str) == INVALID_FILE_ATTRIBUTES)
		{
			//path doesn't exist, attempt creation
			if (SHCreateDirectoryEx(NULL, str, NULL) == ERROR_SUCCESS)
			{
				return str;
			}
			else
			{
				return _T("");
			}
		}

		//path is valid
		return str;
	}

}

BOOL CSysHelper::CreateFileAndInit(CString filename, CString content)
{
	if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(filename) && GetLastError() == ERROR_FILE_NOT_FOUND)
	{
		SaveString(filename, content);
		return TRUE;
	}
	else return TRUE;

	return FALSE;
}

static int CALLBACK EnumFontFamExProc(ENUMLOGFONTEX* /*lpelfe*/, NEWTEXTMETRICEX* /*lpntme*/, int /*FontType*/, LPARAM lParam)
{
	LPARAM* l = (LPARAM*)lParam;
	*l = TRUE;
	return TRUE;
}

bool CSysHelper::IsFontInstalled(LPCTSTR lpszFont)
{
	// Get the screen DC
	CDC dc;
	if (!dc.CreateCompatibleDC(NULL))
	{
		return false;
	}
	LOGFONT lf = { 0 };
	// Any character set will do
	lf.lfCharSet = DEFAULT_CHARSET;
	// Set the facename to check for
	_tcscpy_s(lf.lfFaceName, lpszFont);
	LPARAM lParam = 0;
	// Enumerate fonts
	::EnumFontFamiliesEx(dc.GetSafeHdc(), &lf, (FONTENUMPROC)EnumFontFamExProc, (LPARAM)&lParam, 0);
	return lParam ? true : false;
}

void CSysHelper::SelectMultipleFiles(CString * files, int maxfiles)
{
	CFileDialog DataFileOpenDialog(true, _T("rtf"), _T(""), OFN_ALLOWMULTISELECT, _T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"));
	DataFileOpenDialog.m_ofn.lpstrTitle = _T("Select Files ...");
	//DataFileOpenDialog.m_ofn.lpstrInitialDir = GetUserDocumentPath(PREDICTED_FOLDER);

	int nLnBuff = maxfiles*256;
	TCHAR* pBuffFileSelect = new TCHAR[nLnBuff];

	memset(pBuffFileSelect, 0, nLnBuff * sizeof(TCHAR));

	DataFileOpenDialog.m_ofn.lpstrFile = pBuffFileSelect;
	DataFileOpenDialog.m_ofn.nMaxFile = nLnBuff;

	INT_PTR res = DataFileOpenDialog.DoModal();
	if (res == IDOK)
	{

		int f = 0;
		POSITION pos(DataFileOpenDialog.GetStartPosition());
		while (pos)
		{
			CString filename = DataFileOpenDialog.GetNextPathName(pos);

			files[f] = filename;
			f++;
			if (f >= maxfiles) break;
		}
	}

	delete[] pBuffFileSelect;
}


BOOL CSysHelper::SaveAsTextFile(CString content)
{
	CFileDialog DataFileOpenDialog(false, _T("txt"), _T(""), OFN_OVERWRITEPROMPT, _T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"));
	DataFileOpenDialog.m_ofn.lpstrTitle = _T("Save as Text File ...");
	//DataFileOpenDialog.m_ofn.lpstrInitialDir = GetUserDocumentPath(PREDICTED_USER_FOLDER);

	INT_PTR res = DataFileOpenDialog.DoModal();
	if (res == IDCANCEL) return FALSE;
	m_FileName = DataFileOpenDialog.GetPathName();
	if (m_FileName.IsEmpty()) return FALSE;

	SaveString(m_FileName, content);

	return TRUE;
}

BOOL CSysHelper::GetFileNameToOpen(CString filetype, CString caption)
{
	EmptyFileData();

	CFileDialog DataFileOpenDialog(true, _T(""), _T(""), OFN_FILEMUSTEXIST, filetype);
	DataFileOpenDialog.m_ofn.lpstrTitle = caption;

	INT_PTR res = DataFileOpenDialog.DoModal();
	if (res != IDCANCEL)
	{
		m_FileName = DataFileOpenDialog.GetPathName();
		if (m_FileName.IsEmpty()) return FALSE; 
		m_FileTitle = DataFileOpenDialog.GetFileTitle();
		m_FileExt = DataFileOpenDialog.GetFileExt();
		m_FileNameNoPath = DataFileOpenDialog.GetFileName();
		m_FilePath = DataFileOpenDialog.GetFolderPath();
		return TRUE;
	}

	return FALSE;
}

void CSysHelper::EmptyFileData()
{
	m_FileName.Empty();
	m_FileTitle.Empty();
	m_FileExt.Empty();
	m_FileNameNoPath.Empty();
	m_FilePath.Empty();

}

BOOL CSysHelper::SysGetFileNameToSave(BOOL isopen, CString caption, CString initialdir, CString ext1, CString ext2, CString ext3 )
{
	EmptyFileData();

	CString filetype;

	if (!ext1.IsEmpty())filetype = filetype + ext1 + _T(" files (*.") + ext1 + _T(")|*.") + ext1 + _T("|");
	if (!ext2.IsEmpty())filetype = filetype + ext2 + _T(" files (*.") + ext2 + _T(")|*.") + ext2 + _T("|");
	if (!ext3.IsEmpty())filetype = filetype + ext3 + _T(" files (*.") + ext3 + _T(")|*.") + ext3 + _T("|");
	filetype = filetype + _T("|");

	CFileDialog DataFileOpenDialog(isopen, _T(""), _T(""), OFN_OVERWRITEPROMPT, filetype);
	DataFileOpenDialog.m_ofn.lpstrTitle = caption;
	DataFileOpenDialog.m_ofn.lpstrInitialDir = initialdir;

	INT_PTR res = DataFileOpenDialog.DoModal();
	if (res != IDCANCEL)
	{
		m_FileName = DataFileOpenDialog.GetPathName();
		if (m_FileName.IsEmpty()) return FALSE;
		m_FileTitle = DataFileOpenDialog.GetFileTitle();
		m_FileExt = DataFileOpenDialog.GetFileExt();
		m_FileNameNoPath = DataFileOpenDialog.GetFileName();
		m_FilePath = DataFileOpenDialog.GetFolderPath();
		return TRUE;
	}

	return FALSE;
}



CString CSysHelper::GetFileNameFromPath(CString filepath)
{
	return filepath.Right(filepath.GetLength() - filepath.ReverseFind('\\') - 1);
}


bool CSysHelper::SHCopy(LPCTSTR from, LPCTSTR to)
{
	SHFILEOPSTRUCT fileOp = { 0 };
	fileOp.wFunc = FO_COPY;
	TCHAR newFrom[MAX_PATH];
	_tcscpy_s(newFrom, from);
	newFrom[_tcsclen(from) + 1] = NULL;
	fileOp.pFrom = newFrom;
	TCHAR newTo[MAX_PATH];
	_tcscpy_s(newTo, to);
	newTo[_tcsclen(to) + 1] = NULL;
	fileOp.pTo = newTo;
	fileOp.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_NOCONFIRMMKDIR;

	int result = SHFileOperation(&fileOp);

	return result == 0;
}

bool CSysHelper::SHDelete(LPCTSTR from)
{
	SHFILEOPSTRUCT fileOp = { 0 };
	fileOp.wFunc = FO_DELETE;
	TCHAR newFrom[MAX_PATH];
	_tcscpy_s(newFrom, from);
	newFrom[_tcsclen(from) + 1] = NULL;
	fileOp.pFrom = newFrom;
	fileOp.fFlags = FOF_FILESONLY | FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_NORECURSION;

	int result = SHFileOperation(&fileOp);

	return result == 0;
}

CString CSysHelper::GetHeader(int type)
{
	if (type == LTM_HEADER)return CSysHelper::m_LTMHeader;
	if (type == STM_HEADER)return CSysHelper::m_STMHeader;
	return _T("");
}

void CSysHelper::SetHeaders(int vmaj, int vmin)
{
	CSysHelper::m_LTMHeader.Format(_T("PredictEd Knowledge Map,Version,%d,LTM\r\n"), vmaj * 10 + vmin);
	CSysHelper::m_STMHeader = m_LTMHeader;
	CSysHelper::m_STMHeader.Replace(_T("LTM"), _T("STM"));
}
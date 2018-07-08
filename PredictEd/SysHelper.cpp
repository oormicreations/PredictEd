#include "stdafx.h"
#include "SysHelper.h"


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

CString CSysHelper::GetFileContent()
{
	CFileDialog DataFileOpenDialog(true, _T("rtf"), _T(""), OFN_HIDEREADONLY, _T("Rich text Files (*.rtf)|*.rtf|All Files (*.*)|*.*||"));
	DataFileOpenDialog.m_ofn.lpstrTitle = _T("Load an RTF File ...");
	//DataFileOpenDialog.m_ofn.lpstrInitialDir = GetUserDocumentPath(PREDICTED_FOLDER);
	INT_PTR res = DataFileOpenDialog.DoModal();
	if (res == IDCANCEL) return _T("");
	m_FileName = DataFileOpenDialog.GetPathName();
	if (m_FileName.IsEmpty()) return _T("");
	//if (DataFileOpenDialog.GetFileExt() != _T("rtf")) return;


	CFile file;
	BOOL res1 = file.Open(m_FileName, CFile::modeRead);
	if (!res1)
	{
		AfxMessageBox(_T("Error : Failed to open the file"));
		m_FileName = _T("");
		return _T("");
	}

	UINT len = (UINT)file.GetLength();
	CHAR *buf = new CHAR[len + 1];
	ZeroMemory(buf, len + 1);

	file.Read(buf, len);
	file.Close();
	CString content(buf);
	return content;
}

BOOL CSysHelper::SetFileContent(CString content)
{
	CFileDialog DataFileOpenDialog(false, _T("rtf"), _T(""), OFN_HIDEREADONLY, _T("Rich text Files (*.rtf)|*.rtf|All Files (*.*)|*.*||"));
	DataFileOpenDialog.m_ofn.lpstrTitle = _T("Save an RTF File ...");
	//DataFileOpenDialog.m_ofn.lpstrInitialDir = GetUserDocumentPath(PREDICTED_FOLDER);

	INT_PTR res = DataFileOpenDialog.DoModal();
	if (res == IDCANCEL) return FALSE;
	m_FileName = DataFileOpenDialog.GetPathName();
	if (m_FileName.IsEmpty()) return FALSE;
	//if (DataFileOpenDialog.GetFileExt() != _T("rtf")) return;

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

	UINT len = (UINT)content.GetLength();
	char * outputString = (char*)malloc(len+1);
	ZeroMemory(outputString, len+1);

	len = WideCharToMultiByte(CP_UTF8, NULL, content, -1, outputString, len, NULL, NULL);

	file.Write(outputString, ::strlen(outputString));
	file.Close();
	free(outputString);

	return TRUE;

}

CString CSysHelper::GetAutoBackupFileName()
{
	CString path = GetUserDocumentPath(PREDICTED_USER_FOLDER);
	if (!path.IsEmpty())
	{
		path = path + _T("\\PredicEd_AutoBackup.rtf");
		return path;
	}
	return _T("");
}

CString CSysHelper::GetKnowledgeMapFileName()
{
	CString path = GetUserDocumentPath(PREDICTED_USER_FOLDER);
	if (!path.IsEmpty())
	{
		path = path + _T("\\PredicEd_KnowledgeMap.txt");
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
			str = str + _T("\\Oormi Creations\\PredictEd");
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
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


	CFile file;
	BOOL res1 = file.Open(m_FileName, CFile::modeWrite|CFile::modeCreate);
	if (!res1)
	{
		AfxMessageBox(_T("Error : Failed to save the file"));
		m_FileName = _T("");
		return FALSE;
	}

	UINT len = (UINT)content.GetLength();
	char * outputString = (char*)malloc(len+1);
	ZeroMemory(outputString, len+1);

	len = WideCharToMultiByte(CP_UTF8, NULL, content, -1, outputString, len, NULL, NULL);

	file.Write(outputString, ::strlen(outputString));
	file.Close();

	return TRUE;
}

#include "stdafx.h"
#include "PredictEdContext.h"



CPredictEdContext::CPredictEdContext()
{
	m_bAutoLoad = FALSE;
	m_bCopy = FALSE;
}


CPredictEdContext::~CPredictEdContext()
{
}

void CPredictEdContext::SetVersion(int iVmaj, int iVmin)
{
	m_sVersion.Format(_T("%d"), iVmaj * 10 + iVmin);
}

BOOL CPredictEdContext::LoadContext()
{
	m_sContextRootDir = m_SysHelper.GetUserDocumentPath(PREDICTED_USER_FOLDER) + _T("\\") + PREDICTED_CONTEXT_DIR_NAME;

	if (!m_bAutoLoad)
	{
		if (!m_SysHelper.SysGetFileNameToSave(TRUE, _T("Specify a filename for the context..."), m_sContextRootDir, _T("context")))
		{
			return FALSE;
		}

		m_sContextFile = m_SysHelper.m_FileName;
	}

	if (m_sContextFile.IsEmpty())
	{
		AfxMessageBox(_T("Error: Context file name is not specified."), MB_ICONERROR);
		return FALSE;
	}

	CString sContextInfo = m_SysHelper.ReadStringFromFile(m_sContextFile);
	CString sLine[PREDICTED_CONTEXT_MAX_ENTRIES];
	int iPos = 0;
	int iEntry = 0;

	//parse
	while ((iPos >= 0)&&(iEntry<PREDICTED_CONTEXT_MAX_ENTRIES))
	{
		sLine[iEntry] = sContextInfo.Tokenize(_T("\r\n"), iPos);
		iEntry++;
	}

	//data check
	if(sLine[0].IsEmpty())
	{
		AfxMessageBox(_T("Error: This context file is invalid."), MB_ICONERROR);
		return FALSE;
	}

	//ver check
	CString sValue = GetValue(sLine[0], PREDICTED_CONTEXT_KEY1);
	if (!((sValue == _T("11")) || (sValue == _T("1"))))
	{
		AfxMessageBox(_T("Error: This context file belongs to a different version."), MB_ICONERROR);
		return FALSE;
	}
	
	//name
	sValue = GetValue(sLine[1], PREDICTED_CONTEXT_KEY2);
	if (sValue.IsEmpty())
	{
		AfxMessageBox(_T("Error: This context has no name, assuming a temp name."), MB_ICONERROR);
		m_sContextName = _T("UntitledContext");
	}
	else m_sContextName = sValue;

	//knowledge data files
	sValue = GetValue(sLine[2], PREDICTED_CONTEXT_KEY3);
	if (sValue.IsEmpty())
	{
		AfxMessageBox(_T("Error: Knowledge file path is missing."), MB_ICONERROR);
		return FALSE;
	}

	//if (sValue == _T("Default"))
	//{
		m_sLTMFile = m_sContextRootDir + _T("\\") + sValue + _T("\\") + PREDICTED_LTM_FILE_NAME;
		m_sSTMFile = m_sContextRootDir + _T("\\") + sValue + _T("\\") + PREDICTED_STM_FILE_NAME;
	//}
	//else
	//{
	//	m_sLTMFile = sValue + _T("\\") + PREDICTED_LTM_FILE_NAME;
	//	m_sSTMFile = sValue + _T("\\") + PREDICTED_STM_FILE_NAME;
	//}

	//dictionary file
	sValue = GetValue(sLine[3], PREDICTED_CONTEXT_KEY4);
	if (sValue.IsEmpty())
	{
		AfxMessageBox(_T("Error: Dictionary file path is missing."), MB_ICONERROR);
		return FALSE;
	}

	//if (sValue == _T("DefaultDict.txt"))
	//{
		m_sDictionary = m_SysHelper.GetUserDocumentPath(PREDICTED_USER_FOLDER) + _T("\\") + PREDICTED_DIC_DIR_NAME + _T("\\")+ sValue;
	//}
	//else m_sDictionary = sValue;

	m_sContextDir = m_sContextRootDir + _T("\\") + m_sContextName;

	return TRUE;
}

CString CPredictEdContext::GetValue(CString sEntry, CString sKey)
{
	sEntry.Replace(sKey, _T(""));
	return sEntry;
}

BOOL CPredictEdContext::GetNewContextName()
{
	int iRes = AfxMessageBox(_T("Do you wish to copy the existing knowledge over to the new context?\r\n\r\nClick Yes to confirm.\r\nClick No to start with blank knowledge. "), MB_ICONQUESTION | MB_YESNOCANCEL);
	if (iRes == IDCANCEL) return FALSE;
	m_bCopy = (iRes == IDYES);

	m_sContextRootDir = m_SysHelper.GetUserDocumentPath(PREDICTED_USER_FOLDER) + _T("\\") + PREDICTED_CONTEXT_DIR_NAME;

	if (!m_SysHelper.SysGetFileNameToSave(FALSE, _T("Specify a filename for the context..."), m_sContextRootDir, _T("context")))
	{
		return FALSE;
	}

	m_sContextDir = m_sContextRootDir + _T("\\") + m_SysHelper.m_FileTitle;
	m_sContextFile = m_SysHelper.m_FileName;
	m_sContextName = m_SysHelper.m_FileTitle;

	return TRUE;
}

BOOL CPredictEdContext::CreateContext()
{
	CString sContextInfo
		= PREDICTED_CONTEXT_KEY1 + m_sVersion		+ _T("\r\n")
		+ PREDICTED_CONTEXT_KEY2 + m_sContextName	+ _T("\r\n")
		+ PREDICTED_CONTEXT_KEY3 + m_sContextName + _T("\r\n")
		+ PREDICTED_CONTEXT_KEY4 + m_SysHelper.GetFileNameFromPath(m_sDictionary)
		;

	if (!m_SysHelper.SaveString(m_sContextFile, sContextInfo))
	{
		AfxMessageBox(_T("Error: Failed to save context file."), MB_ICONERROR);
		return FALSE;
	}

	if (!PathFileExists(m_sContextDir))
	{
		if (!CreateDirectory(m_sContextDir, NULL))
		{
			AfxMessageBox(_T("Error: Failed to create context folder.\r\nEnsure that the name is unique and you have write permissions."), MB_ICONERROR);
			return FALSE;
		}
	}

	CString sNewLTMFile = m_sContextDir + _T("\\") + PREDICTED_LTM_FILE_NAME;
	CString sNewSTMFile = m_sContextDir + _T("\\") + PREDICTED_STM_FILE_NAME;
	
	if (m_bCopy)
	{
		if (!m_sLTMFile.IsEmpty())
		{
			if (!CopyFile(m_sLTMFile, sNewLTMFile, FALSE))
			{
				AfxMessageBox(_T("Error: Failed to copy knowledge files."), MB_ICONERROR);
				return FALSE;
			}
		}
		if (!m_sSTMFile.IsEmpty())
		{
			if (!CopyFile(m_sSTMFile, sNewSTMFile, FALSE))
			{
				AfxMessageBox(_T("Error: Failed to copy knowledge files."), MB_ICONERROR);
				return FALSE;
			}
		}
	}
	else
	{
		CString sHeader = m_SysHelper.GetHeader(LTM_HEADER);

		if (!m_SysHelper.SaveString(sNewLTMFile, sHeader))
		{
			AfxMessageBox(_T("Error: Failed to create knowledge files."), MB_ICONERROR);
			return FALSE;
		}

		sHeader= m_SysHelper.GetHeader(STM_HEADER);

		if (!m_SysHelper.SaveString(sNewSTMFile, sHeader))
		{
			AfxMessageBox(_T("Error: Failed to create knowledge files."), MB_ICONERROR);
			return FALSE;
		}

	}

	AfxMessageBox(_T("Context created.\r\nIf you wish to switch to the new context, load it from the Menu>Load Context.\r\nElse you can keep working in present context."), MB_ICONINFORMATION);

	return TRUE;
}

BOOL CPredictEdContext::CreateDefaultContext()
{
	//look for context folder
	//GetUserDocumentPath will also create user folder if not present
	CString sUserDir = m_SysHelper.GetUserDocumentPath(PREDICTED_USER_FOLDER);
	if (sUserDir.IsEmpty()) return FALSE;

	m_sContextRootDir = sUserDir + _T("\\") + PREDICTED_CONTEXT_DIR_NAME;

	if (!PathFileExists(m_sContextRootDir))
	{
		if (!CreateDirectory(m_sContextRootDir, NULL)) return FALSE;
	}
	
	//create new context
	m_bCopy = FALSE;
	m_sContextFile = m_sContextRootDir + _T("\\Default.context");
	m_sDictionary = m_SysHelper.GetPredictEdFileName(PREDICTED_DIC_FILE);
	m_sContextName = _T("Default");
	m_sContextDir = m_sContextRootDir + _T("\\") + m_sContextName;

	if (!CreateContext()) return FALSE;

	return TRUE;
}

BOOL CPredictEdContext::UpdateContext()
{
	CString sContextInfo
		= PREDICTED_CONTEXT_KEY1 + m_sVersion + _T("\r\n")
		+ PREDICTED_CONTEXT_KEY2 + m_sContextName + _T("\r\n")
		+ PREDICTED_CONTEXT_KEY3 + m_sContextName + _T("\r\n")
		+ PREDICTED_CONTEXT_KEY4 + m_SysHelper.GetFileNameFromPath(m_sDictionary)
		;

	if (!m_SysHelper.SaveString(m_sContextFile, sContextInfo))
	{
		AfxMessageBox(_T("Error: Failed to update the context file."), MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}


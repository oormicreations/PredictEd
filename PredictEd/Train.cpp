// Train.cpp : implementation file
//

#include "stdafx.h"
#include "PredictEd.h"
#include "Train.h"
#include "afxdialogex.h"


// CTrain dialog

IMPLEMENT_DYNAMIC(CTrain, CDialog)

CTrain::CTrain(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_TRAIN, pParent)
{

}

CTrain::~CTrain()
{
}

void CTrain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_TRAIN, m_Progress);
}


BEGIN_MESSAGE_MAP(CTrain, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CTrain::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_START, &CTrain::OnBnClickedButtonStart)
END_MESSAGE_MAP()


// CTrain message handlers


void CTrain::OnBnClickedButtonAdd()
{
	m_SysHelper.SelectMultipleFiles(m_FileList, MAX_INPUT_FILES);
	
	m_FileCount = 0;
	for (int i = 0; i < MAX_INPUT_FILES; i++)
	{
		if (!m_FileList[i].IsEmpty())
		{
			m_FileCount++;
		}
	}
	CString str;
	str.Format(_T("%d files added."), m_FileCount);
	SetDlgItemText(IDC_EDIT_PROG, str);

	m_Progress.SetRange(0, 100);
	m_Progress.SetPos(0);
}


void CTrain::OnBnClickedButtonStart()
{
	CString str;
	UINT count = 0;
	CWordList LTM;
	LTM.InitList();

	for (int i = 0; i < m_FileCount; i++)
	{
		if (!m_FileList[i].IsEmpty())
		{
			str.Format(_T("Processing file %d of %d ..."), i, m_FileCount);
			SetDlgItemText(IDC_EDIT_PROG, str);

			if (LTM.m_LastKeyWordIndex >= MAX_LIST_COUNT)
			{
				//AfxMessageBox(_T("List is full, only weights and relations will be updated."));
			}

			CString content = m_SysHelper.ReadStringFromFile(m_FileList[i]);
			if (!content.IsEmpty())
			{
				UINT len = content.GetLength();
				CString word, lastword;

				for (UINT p = 0; p < len; p++)
				{

					TCHAR c = content.GetAt(p);
					//if (c == '/')
					//{
					//	int t=0;
					//}
					if ((c != ' ') && (c != '\r') && (c != '\n') && (c != '\t') && (c != '/') && (c != '.') && (c != ',')) //word delimiters
					{
						word.AppendChar(c);
					}
					else
					{
						word = FilterString(word);
						LTM.AddKeyWord(word);
						LTM.CreateRelation(lastword, word);
						lastword = word;
						word = _T("");
						count++;
						//TRACE("%d\n", count);
					}
				}

				str.Format(_T("%d Words processed, %d Entries added"), count, LTM.m_LastKeyWordIndex);
				SetDlgItemText(IDC_EDIT_RES, str);

			}
		}

		m_Progress.SetPos((i * 100) / m_FileCount);
	}

	LTM.SortList();

	//if (LTM.SaveMap(LTMFileName, LTM_HEADER)) UpdateStatusMessage(_T("Training complete..."));
	//else UpdateStatusMessage(_T("Error: Knowledge file could not be saved!"));

	m_Progress.SetPos(100);
	SetDlgItemText(IDC_EDIT_PROG, _T("Training finished..."));

}


CString CTrain::FilterString(CString str)
{
	if (str.Find(_T("bad")) >= 0)
	{
		int t = 0;
	}
	str.MakeLower();

	int pos = str.FindOneOf(_T("0123456789~!@#$%^&*()-_+=|\\{[}]:;\"\'<,>.?/\r\n–“”’"));
	if (pos >= 0)
	{
		str.Truncate(pos);
		return str;
	}

	return str;
}
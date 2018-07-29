// Train.cpp : implementation file
//

#include "stdafx.h"
#include "PredictEd.h"
#include "Train.h"
#include "afxdialogex.h"

#define TRAIN_THREAD_NOTIFY (WM_APP + 1)

//CString content;

struct ThreadParam
{
	CTrain *ptrain;
	HWND mDlg;
};


// CTrain dialog

IMPLEMENT_DYNAMIC(CTrain, CDialog)

CTrain::CTrain(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_TRAIN, pParent)
{
	m_FileCount = m_Count = 0;
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
	ON_MESSAGE(TRAIN_THREAD_NOTIFY, OnTrainThreadNotify)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDCANCEL, &CTrain::OnBnClickedCancel)
END_MESSAGE_MAP()


// CTrain message handlers

LRESULT CTrain::OnTrainThreadNotify(WPARAM wp, LPARAM lp)
{
	CString str;

	if ((int)lp == 0)
	{
		str.Format(_T("Processing file %d of %d ..."), (int)wp+1, m_FileCount);
		SetDlgItemText(IDC_EDIT_PROG, str);
	}

	if ((int)lp == 1)
	{
		SetDlgItemText(IDC_EDIT_ERR, _T("List is full, only weights and relations will be updated."));
	}

	if ((int)lp == 2)
	{
		str.Format(_T("%d Words processed, %d Entries added"), m_Count, LTM.m_LastKeyWordIndex);
		SetDlgItemText(IDC_EDIT_RES, str);
	}

	if ((int)lp == 3)
	{
		m_Progress.SetPos((((int)wp+1) * 100) / m_FileCount);
	}

	if ((int)lp == 4)
	{
		m_Progress.SetPos(100);
		SetDlgItemText(IDC_EDIT_PROG, _T("Training finished..."));
		m_Result = TRUE;
	}

	if ((int)lp == 5)
	{
		m_Progress.SetPos(0);
		SetDlgItemText(IDC_EDIT_ERR, _T("Error: Knowledge file could not be saved!"));
	}

	return 0;
}

UINT TrainDataProc(LPVOID param)
{
	ThreadParam* p = static_cast<ThreadParam*> (param);
	CTrain *ptrain = (CTrain*)p->ptrain;

	for (int i = 0; i < ptrain->m_FileCount; i++)
	{
		if (!ptrain->m_FileList[i].IsEmpty())
		{
			::SendMessage(p->mDlg, TRAIN_THREAD_NOTIFY, i, 0);


			if (ptrain->LTM.m_LastKeyWordIndex >= MAX_LIST_COUNT)
			{
				::SendMessage(p->mDlg, TRAIN_THREAD_NOTIFY, 0, 1);
			}

			CString content = ptrain->m_SysHelper.ReadStringFromFile(ptrain->m_FileList[i]);


			if (!content.IsEmpty())
			{
				UINT len = content.GetLength();
				CString word, lastword;

				for (UINT np = 0; np < len; np++)
				{
					if (ptrain->m_Abort)
					{
						delete p;
						return 0;
					}

					TCHAR c = content.GetAt(np);
					if ((c != ' ') && (c != '\r') && (c != '\n') && (c != '\t') && (c != '/') && (c != '.') && (c != ',')) //word delimiters
					{
						word.AppendChar(c);
					}
					else
					{
						word = ptrain->FilterString(word);
						ptrain->LTM.AddKeyWord(word);
						ptrain->LTM.CreateRelation(lastword, word);
						lastword = word;
						word = _T("");
						ptrain->m_Count++;
						//TRACE("%d\n", count);
					}
					::SendMessage(p->mDlg, TRAIN_THREAD_NOTIFY, 0, 2);
				}
			}
		}

		::SendMessage(p->mDlg, TRAIN_THREAD_NOTIFY, i, 3);
	}

	ptrain->LTM.SortList();

	if (ptrain->LTM.SaveMap(ptrain->m_FileName, ptrain->m_FileHeader))
	{

		::SendMessage(p->mDlg, TRAIN_THREAD_NOTIFY, 0, 4);

	}
	else
	{
		::SendMessage(p->mDlg, TRAIN_THREAD_NOTIFY, 0, 5);
	}

		delete p;
		return 0;

	
}

void CTrain::OnBnClickedButtonStart()
{
	CString str;
	m_Count = 0;
	m_Result = FALSE;
	m_Abort = FALSE;
	
	LTM.InitList();

	ThreadParam* param = new ThreadParam;
	param->mDlg = m_hWnd; 
	param->ptrain = this;

	CWinThread* hTh1 = AfxBeginThread(TrainDataProc, param);

}


void CTrain::OnBnClickedButtonAdd()
{
	CString tfilelist[MAX_INPUT_FILES];
	m_SysHelper.SelectMultipleFiles(tfilelist, MAX_INPUT_FILES);

	int c = 0;
	for (int i = 0; i < MAX_INPUT_FILES; i++)
	{
		if (m_FileList[i].IsEmpty())
		{
			if (!tfilelist[c].IsEmpty())
			{
				m_FileList[i] = tfilelist[c];
				m_FileCount++;
				c++;
			}
		}
	}

	CString str;
	str.Format(_T("%d files added."), m_FileCount);
	SetDlgItemText(IDC_EDIT_PROG, str);

	m_Progress.SetRange(0, 100);
	m_Progress.SetPos(0);
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

HBRUSH CTrain::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID() == IDC_EDIT_PROG)
	{
		pDC->SetTextColor(RGB(0, 150, 0));
	}

	if (pWnd->GetDlgCtrlID() == IDC_EDIT_ERR)
	{
		pDC->SetTextColor(RGB(150, 0, 0));
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


void CTrain::OnDestroy()
{
	CDialog::OnDestroy();
}


void CTrain::OnBnClickedCancel()
{
	m_Abort = TRUE;
	CDialog::OnCancel();
}

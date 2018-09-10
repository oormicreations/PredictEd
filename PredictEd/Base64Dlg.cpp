// Base64Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "PredictEd.h"
#include "Base64Dlg.h"
#include "afxdialogex.h"


// CBase64Dlg dialog

IMPLEMENT_DYNAMIC(CBase64Dlg, CDialog)

CBase64Dlg::CBase64Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_BASE64, pParent)
{

}

CBase64Dlg::~CBase64Dlg()
{
}

void CBase64Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBase64Dlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_FILESEL, &CBase64Dlg::OnBnClickedButtonFilesel)
	ON_BN_CLICKED(IDC_BUTTON_TOBASE64, &CBase64Dlg::OnBnClickedButtonTobase64)
	ON_BN_CLICKED(IDC_BUTTON_FROMB64, &CBase64Dlg::OnBnClickedButtonFromb64)
END_MESSAGE_MAP()


// CBase64Dlg message handlers


void CBase64Dlg::OnBnClickedButtonFilesel()
{

	if (m_SysHelper.GetFileNameToOpen(_T("All Files (*.*)|*.*||"), _T("Select a file to hash...")))
	{
		CFile file;
		if (file.Open(m_SysHelper.m_FileName, CFile::modeRead | CFile::typeBinary))
		{
			ULONGLONG fsz = file.GetLength();
			if (fsz < 1)
			{
				AfxMessageBox(_T("File has no data!"), MB_ICONERROR);
				file.Close();
				return;
			}

			PBYTE data = (PBYTE)HeapAlloc(GetProcessHeap(), 0, (SIZE_T)fsz);
			if (NULL == data)
			{
				AfxMessageBox(_T("Memory allocation failed for reading the file"), MB_ICONERROR);
				file.Close();
				return;
			}

			file.Read(data, (UINT)fsz);

			CCryptHelper crypthelper;
			if (!crypthelper.Create_SHA512_Hash_Bin(data, (ULONG)fsz))
			{
				AfxMessageBox(_T("Hash failed!"), MB_ICONERROR);
			}
			else
			{
				//m_HashStr = crypthelper.m_sSHA512;
				//DisplayFormattedHash();
				//SetDlgItemText(IDC_BUTTON_HASHFILE, m_SysHelper.m_FileNameNoPath);
			}

			if (data) HeapFree(GetProcessHeap(), 0, data);
			file.Close();

		}
	}
}


void CBase64Dlg::OnBnClickedButtonTobase64()
{
}


void CBase64Dlg::OnBnClickedButtonFromb64()
{
}

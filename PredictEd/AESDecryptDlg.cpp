// AESDecryptDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PredictEd.h"
#include "AESDecryptDlg.h"
#include "afxdialogex.h"


// CAESDecryptDlg dialog

IMPLEMENT_DYNAMIC(CAESDecryptDlg, CDialog)

CAESDecryptDlg::CAESDecryptDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_DEC, pParent)
{
	m_DecryptedFileType = 0;
}

CAESDecryptDlg::~CAESDecryptDlg()
{
}

void CAESDecryptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAESDecryptDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CAESDecryptDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAESDecryptDlg message handlers


void CAESDecryptDlg::OnBnClickedOk()
{
	if (m_EncryptedFilename.IsEmpty())
	{
		AfxMessageBox(_T("File name is required!"), MB_ICONERROR);
		return;
	}

	CString pass;
	GetDlgItemText(IDC_EDIT_DECPASS, pass);

	if (pass.IsEmpty())
	{
		AfxMessageBox(_T("Password is required!"), MB_ICONERROR);
		return;
	}

	m_DecryptResult = 0;
	m_DecryptResult = m_CryptHelper.CryptFile(false, m_EncryptedFilename, m_DecryptedFilename, pass);

	if (m_DecryptResult)
	{
		CFile file;
		if (file.Open(m_DecryptedFilename, CFile::modeRead))
		{
			CHAR *buf = new CHAR[13];
			ZeroMemory(buf, 13);

			file.Read(buf, 12);
			file.Close();

			CString content(buf);
			delete[] buf;

			if (content == _T("{\\rtf1\\ansi\\"))
			{
				m_DecryptedFileType = 1;
			}
		}
	}
	else AfxMessageBox(_T("Failed to decrypt the file:\r\n\r\n") + m_CryptHelper.GetLastError(), MB_ICONERROR);

	CDialog::OnOK();
}

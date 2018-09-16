// StegDecDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PredictEd.h"
#include "StegDecDlg.h"
#include "afxdialogex.h"


// CStegDecDlg dialog

IMPLEMENT_DYNAMIC(CStegDecDlg, CDialog)

CStegDecDlg::CStegDecDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_STEGDEC, pParent)
{

}

CStegDecDlg::~CStegDecDlg()
{
}

void CStegDecDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CStegDecDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_IMGFILE, &CStegDecDlg::OnBnClickedButtonImgfile)
	ON_BN_CLICKED(IDOK, &CStegDecDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CStegDecDlg message handlers


void CStegDecDlg::OnBnClickedButtonImgfile()
{
	if (m_SysHelper.GetFileNameToOpen(_T("PNG Files (*.png)|*.png||"), _T("Select the encoded image file...")))
	{
		m_ImageFile = m_SysHelper.m_FileName;
		m_DataFilePath = m_SysHelper.m_FilePath;
		SetDlgItemText(IDC_BUTTON_IMGFILE, _T("Image: ") + m_SysHelper.m_FileNameNoPath);
	}
}


void CStegDecDlg::OnBnClickedOk()
{
	CString pass;
	GetDlgItemText(IDC_EDIT_DECPASS, pass);

	if (m_ImageFile.IsEmpty()) return;

	if (m_CryptHelper.PredictEdStegDecode(m_ImageFile, m_DataFilePath, pass))
	{
		AfxMessageBox(_T("Decoded data successfully saved as:\r\n\r\n") + m_CryptHelper.m_OutputFile, MB_ICONINFORMATION);
	}

	CDialog::OnOK();
}

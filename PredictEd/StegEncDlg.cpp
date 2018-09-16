// StegEncDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PredictEd.h"
#include "StegEncDlg.h"
#include "afxdialogex.h"


// CStegEncDlg dialog

IMPLEMENT_DYNAMIC(CStegEncDlg, CDialog)

CStegEncDlg::CStegEncDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_STEG, pParent)
{

}

CStegEncDlg::~CStegEncDlg()
{
}

void CStegEncDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_PASSSTRENGTH, m_PassStrengthBar);
}


BEGIN_MESSAGE_MAP(CStegEncDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_EMB, &CStegEncDlg::OnBnClickedButtonEmb)
	ON_BN_CLICKED(IDC_BUTTON_TAR, &CStegEncDlg::OnBnClickedButtonTar)
	ON_BN_CLICKED(IDC_BUTTON_GENPASS, &CStegEncDlg::OnBnClickedButtonGenpass)
	ON_BN_CLICKED(IDC_BUTTON_STEGDEC, &CStegEncDlg::OnBnClickedButtonStegdec)
	ON_BN_CLICKED(IDC_BUTTON_STEGENC, &CStegEncDlg::OnBnClickedButtonStegenc)
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_EDIT_PASS, &CStegEncDlg::OnEnChangeEditPass)
	ON_BN_CLICKED(IDC_CHECK_ENC, &CStegEncDlg::OnBnClickedCheckEnc)
END_MESSAGE_MAP()


// CStegEncDlg message handlers


void CStegEncDlg::OnBnClickedButtonEmb()
{
	if (m_SysHelper.GetFileNameToOpen(_T("All Files (*.*)|*.*||"), _T("Select the data file to encode...")))
	{
		m_DataFile = m_SysHelper.m_FileNameNoPath;
		m_DataFilePath = m_SysHelper.m_FilePath;
		SetDlgItemText(IDC_BUTTON_EMB, _T("Data: ") + m_DataFile);
	}
}


void CStegEncDlg::OnBnClickedButtonTar()
{
	if (m_SysHelper.GetFileNameToOpen(_T("JPG Files (*.jpg)|*.jpg|JPEG Files (*.jpeg)|*.jpeg||"), _T("Select the image file...")))
	{
		m_ImageFile = m_SysHelper.m_FileName;
		SetDlgItemText(IDC_BUTTON_TAR, _T("Image: ") + m_SysHelper.m_FileNameNoPath);
	}
}


void CStegEncDlg::OnBnClickedButtonGenpass()
{
	SetDlgItemText(IDC_EDIT_GPASS, m_CryptHelper.PasswordGen());
}


void CStegEncDlg::OnBnClickedButtonStegdec()
{
}


void CStegEncDlg::OnBnClickedButtonStegenc()
{
	if (m_ImageFile.IsEmpty()) return;
	if (m_DataFile. IsEmpty()) return;

	if (m_CryptHelper.PredictEdStegEncode(m_ImageFile, m_DataFile, m_DataFilePath, GetPass()))
	{
		SetDlgItemText(IDC_EDIT_MSG, _T("Encoded image successfully saved as:\r\n") + m_CryptHelper.m_OutputFile);
	}
}


BOOL CStegEncDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	m_PassStrengthBar.SetRange(0, 100);
	//GetDlgItem(IDC_BUTTON_TAR)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


HBRUSH CStegEncDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID() == IDC_EDIT_MSG)
	{
		pDC->SetTextColor(RGB(150, 0, 0));
	}
	if (pWnd->GetDlgCtrlID() == IDC_EDIT_GPASS)
	{
		pDC->SetTextColor(RGB(0, 150, 0));
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


void CStegEncDlg::OnEnChangeEditPass()
{
	m_PassStrengthBar.SetPos(m_CryptHelper.GetPasswordStrength(GetPass()));
}


void CStegEncDlg::OnBnClickedCheckEnc()
{
	m_UsePass = IsDlgButtonChecked(IDC_CHECK_ENC);
	if(m_UsePass) 	SetDlgItemText(IDC_EDIT_MSG, _T("Store this password securely. It can't be recovered if you forget it."));
	else 	SetDlgItemText(IDC_EDIT_MSG, _T(""));
	GetDlgItem(IDC_EDIT_PASS)->EnableWindow(m_UsePass);
}

CString CStegEncDlg::GetPass()
{
	CString pass;
	GetDlgItemText(IDC_EDIT_PASS, pass);
	if (m_UsePass) return pass;
	else return _T("");
}
// HashDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PredictEd.h"
#include "HashDlg.h"
#include "afxdialogex.h"


// CHashDlg dialog

IMPLEMENT_DYNAMIC(CHashDlg, CDialog)

CHashDlg::CHashDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_HASH, pParent)
{
	m_EdFont = NULL;
}

CHashDlg::~CHashDlg()
{
	if (m_EdFont != NULL) delete m_EdFont;
}

void CHashDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_HASH, m_HashEd);
	DDX_Control(pDX, IDC_EDIT_COLS, m_Cols);
	DDX_Control(pDX, IDC_EDIT_ROWS, m_Rows);
}


BEGIN_MESSAGE_MAP(CHashDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_HASHTEXT, &CHashDlg::OnBnClickedButtonHashtext)
	ON_BN_CLICKED(IDC_BUTTON_HASHFILE, &CHashDlg::OnBnClickedButtonHashfile)
	ON_BN_CLICKED(IDC_BUTTON_COPYHASH, &CHashDlg::OnBnClickedButtonCopyhash)
	ON_BN_CLICKED(IDC_BUTTON_COMPARE, &CHashDlg::OnBnClickedButtonCompare)
	ON_BN_CLICKED(IDC_BUTTON_SAVEHASH, &CHashDlg::OnBnClickedButtonSavehash)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CHashDlg message handlers

BOOL CHashDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	SetHashDisplayFont();

	m_Rows.SetWindowText(_T("01\r\n02\r\n03\r\n04\r\n05\r\n06\r\n07\r\n08\r\n"));
	m_Cols.SetWindowText(_T("01 02 03 04 05 06 07 08"));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CHashDlg::SetHashDisplayFont()
{
	CString fontname = _T("Courier New");
	int fontsz = 16;
	if (!m_SysHelper.IsFontInstalled(fontname))
	{
		fontname = _T("Lucida Console");
		fontsz = 14;
	}

	m_EdFont = new CFont;
	m_EdFont->CreateFontW(fontsz, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, fontname);

	m_HashEd.SetFont(m_EdFont);
	m_Rows.SetFont(m_EdFont);
	m_Cols.SetFont(m_EdFont);
}

void CHashDlg::DisplayFormattedHash()
{
	CString fhash;
	fhash = m_HashStr;

	int len = m_HashStr.GetLength();

	//add spaces and lines
	len = (len / 2) - 1;
	int sp = 2;
	int nl = 1;

	for (int i = 0; i < len; i++) 
	{
		if (nl == 8)
		{
			fhash.Insert(i + sp, _T("+"));
			nl = 0;
		}
		else fhash.Insert(i + sp, _T(" "));

		nl++;
		sp = sp + 2;
	}

	fhash.Replace(_T("+"), _T("\r\n"));
	m_HashEd.SetWindowText(fhash);
}

void CHashDlg::OnBnClickedButtonHashtext()
{
	if (m_Content.IsEmpty()) return;

	CCryptHelper crypthelper;

	if (crypthelper.Create_SHA512_Hash(m_Content))
	{
		m_HashStr = crypthelper.m_sSHA512;
		DisplayFormattedHash();
	}
	else m_HashEd.SetWindowText(_T("Error: Could not compute the hash.\r\n\r\n") + crypthelper.GetLastError());

}


void CHashDlg::OnBnClickedButtonHashfile()
{
	if (m_SysHelper.GetFileNameToOpen(_T("All Files (*.*)|*.*||")))
	{

	}
}


void CHashDlg::OnBnClickedButtonCopyhash()
{
	m_SysHelper.SetClipboardText(m_HashStr);
}


void CHashDlg::OnBnClickedButtonCompare()
{
	if (m_SysHelper.GetClipboardText() == m_HashStr) AfxMessageBox(_T("Hash Matched!"), MB_ICONEXCLAMATION);
	else AfxMessageBox(_T("Hash did not match."), MB_ICONERROR);
}


void CHashDlg::OnBnClickedButtonSavehash()
{
	m_SysHelper.SaveAsTextFile(m_HashStr);
}


HBRUSH CHashDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	int id = pWnd->GetDlgCtrlID();

	if (id == IDC_EDIT_HASH)
	{
		pDC->SetTextColor(RGB(170, 0, 0));
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


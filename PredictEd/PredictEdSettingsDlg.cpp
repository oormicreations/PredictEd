// PredictEdSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PredictEd.h"
#include "PredictEdSettingsDlg.h"
#include "afxdialogex.h"


// CPredictEdSettingsDlg dialog

IMPLEMENT_DYNAMIC(CPredictEdSettingsDlg, CDialog)

CPredictEdSettingsDlg::CPredictEdSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_SETTINGS, pParent)
{

}

CPredictEdSettingsDlg::~CPredictEdSettingsDlg()
{
}

void CPredictEdSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON_BK, m_ColorButton);
}


BEGIN_MESSAGE_MAP(CPredictEdSettingsDlg, CDialog)
	ON_EN_CHANGE(IDC_EDIT_SET_LIMIT, &CPredictEdSettingsDlg::OnEnChangeEditSetLimit)
	ON_EN_CHANGE(IDC_EDIT_SET_MARGINS, &CPredictEdSettingsDlg::OnEnChangeEditSetMargins)
	ON_BN_CLICKED(IDC_BUTTON_SET_DEFFONT, &CPredictEdSettingsDlg::OnBnClickedButtonSetDeffont)
	ON_BN_CLICKED(IDC_BUTTON_SET_RESET, &CPredictEdSettingsDlg::OnBnClickedButtonSetReset)
END_MESSAGE_MAP()


// CPredictEdSettingsDlg message handlers


BOOL CPredictEdSettingsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetParams();

	return TRUE;  
}


void CPredictEdSettingsDlg::OnEnChangeEditSetLimit()
{
	int n = GetDlgItemInt(IDC_EDIT_SET_LIMIT);
	if (n <= 32) n = m_MaxLimit;
	if (n > 100000) n = 100000;
	m_MaxLimit = n;
}


void CPredictEdSettingsDlg::OnEnChangeEditSetMargins()
{
	int n = GetDlgItemInt(IDC_EDIT_SET_MARGINS);
	if (n < 0) n = m_Margins;
	if (n > 100) n = 100;
	m_Margins = n;
}


void CPredictEdSettingsDlg::OnBnClickedButtonSetDeffont()
{
	CFontDialog dlg(&m_DefFont);

	dlg.m_cf.Flags |= CF_EFFECTS;
	dlg.m_cf.rgbColors = m_TxtColor;

	if (dlg.DoModal() == IDOK)
	{
		m_TxtColor = dlg.m_cf.rgbColors;
		SetFontName();
	}
}

void CPredictEdSettingsDlg::SetFontName()
{
	CString str;
	CClientDC dc(this); // expects a CWnd that has already been initialized

	long ht = -MulDiv(m_DefFont.lfHeight, 72, dc.GetDeviceCaps(LOGPIXELSY)); //em to pt

	str.Format(_T("Default Font: %s, %d"), m_DefFont.lfFaceName, ht);
	SetDlgItemText(IDC_BUTTON_SET_DEFFONT, str);

}

void CPredictEdSettingsDlg::SetDefaultFont() //predicted default, not user default
{
	CString fontname = _T("Georgia");
	LONG ht = 12; //pt

	memset(&m_DefFont, 0, sizeof(LOGFONT));

	CClientDC dc(this); // expects a CWnd that has already been initialized

	m_DefFont.lfHeight = -MulDiv(ht, dc.GetDeviceCaps(LOGPIXELSY), 72); //pt to em
	_tcscpy_s(m_DefFont.lfFaceName, LF_FACESIZE, fontname);

	m_TxtColor = RGB(0, 0, 0);

	SetFontName();

}

void CPredictEdSettingsDlg::Reset()
{
	m_LTMSz = 5000;
	m_STMSz = 5000;
	m_MaxLimit = 2000;
	m_Margins = 30;
	m_BkColor = RGB(255, 253, 245);
	m_DefFont = {};
}

void CPredictEdSettingsDlg::OnBnClickedButtonSetReset()
{
	Reset();
	SetDefaultFont();
	SetParams();

}

void CPredictEdSettingsDlg::SetParams()
{
	CString str;
	str.Format(_T("%d"), m_LTMSz);
	SetDlgItemText(IDC_EDIT_SET_LTMSZ, str);
	str.Format(_T("%d"), m_STMSz);
	SetDlgItemText(IDC_EDIT_SET_STMSZ, str);
	str.Format(_T("%d"), m_MaxLimit);
	SetDlgItemText(IDC_EDIT_SET_LIMIT, str);
	str.Format(_T("%d"), m_Margins);
	SetDlgItemText(IDC_EDIT_SET_MARGINS, str);

	m_ColorButton.SetColor(m_BkColor);

	if (m_DefFont.lfHeight == 0) SetDefaultFont();

	SetFontName();
}

void CPredictEdSettingsDlg::OnOK()
{
	m_BkColor = m_ColorButton.GetColor();

	CDialog::OnOK();
}

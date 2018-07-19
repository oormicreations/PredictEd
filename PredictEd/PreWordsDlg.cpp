// PreWordsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PredictEd.h"
#include "PreWordsDlg.h"
#include "afxdialogex.h"


// CPreWordsDlg dialog

IMPLEMENT_DYNAMIC(CPreWordsDlg, CDialog)

CPreWordsDlg::CPreWordsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_PREWORDS, pParent)
{

}

CPreWordsDlg::~CPreWordsDlg()
{
}

void CPreWordsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_W1, m_Word1);
	DDX_Control(pDX, IDC_EDIT_W2, m_Word2);
	DDX_Control(pDX, IDC_EDIT_W3, m_Word3);
	DDX_Control(pDX, IDC_EDIT_W4, m_Word4);
	DDX_Control(pDX, IDC_EDIT_W5, m_Word5);
}


BEGIN_MESSAGE_MAP(CPreWordsDlg, CDialog)
END_MESSAGE_MAP()


// CPreWordsDlg message handlers

void CPreWordsDlg::SetWords(CKeyWordMap map)
{
	m_Word1.SetWindowText(map.m_Predictions[0]);
	m_Word2.SetWindowText(map.m_Predictions[1]);
	m_Word3.SetWindowText(map.m_Predictions[2]);
	m_Word4.SetWindowText(map.m_Predictions[3]);
	m_Word5.SetWindowText(map.m_Predictions[4]);
}

void CPreWordsDlg::ShiftWords()
{
	CString str, tmp;
	m_Word1.GetWindowText(str);

	m_Word2.GetWindowText(tmp);
	m_Word1.SetWindowText(tmp);

	m_Word3.GetWindowText(tmp);
	m_Word2.SetWindowText(tmp);

	m_Word4.GetWindowText(tmp);
	m_Word3.SetWindowText(tmp);

	m_Word5.GetWindowText(tmp);
	m_Word4.SetWindowText(tmp);

	m_Word5.SetWindowText(str);

}

BOOL CPreWordsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	//SetWindowPos(&this->wndTop, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);//sit on top of app

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

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
	DeleteObject(m_PhraseBkBrush);
}

void CPreWordsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_W1, m_Word1);
	DDX_Control(pDX, IDC_EDIT_W2, m_Word2);
	DDX_Control(pDX, IDC_EDIT_W3, m_Word3);
	DDX_Control(pDX, IDC_EDIT_W4, m_Word4);
	DDX_Control(pDX, IDC_EDIT_W5, m_Word5);
	DDX_Control(pDX, IDC_EDIT_P1, m_Ph1);
	DDX_Control(pDX, IDC_EDIT_P2, m_Ph2);
	DDX_Control(pDX, IDC_EDIT_P3, m_Ph3);
}


BEGIN_MESSAGE_MAP(CPreWordsDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CPreWordsDlg message handlers

BOOL CPreWordsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	//SetWindowPos(&this->wndTop, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);//sit on top of app
	m_PhPos = 5;
	m_PhraseBkBrush = CreateSolidBrush(RGB(220, 220, 220));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CPreWordsDlg::SetWords(CKeyWordMap map, CString * phrases)
{
	m_Word1.SetWindowText(map.m_Predictions[0]);
	m_Word2.SetWindowText(map.m_Predictions[1]);
	m_Word3.SetWindowText(map.m_Predictions[2]);
	m_Word4.SetWindowText(map.m_Predictions[3]);
	m_Word5.SetWindowText(map.m_Predictions[4]);

	m_Ph1.SetWindowText(phrases[0]);
	m_Ph2.SetWindowText(phrases[1]);
	m_Ph3.SetWindowText(phrases[2]);

	m_PhPos = 5;
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

	m_Ph1.GetWindowText(tmp);
	m_Word5.SetWindowText(tmp);

	m_Ph2.GetWindowText(tmp);
	m_Ph1.SetWindowText(tmp);

	m_Ph3.GetWindowText(tmp);
	m_Ph2.SetWindowText(tmp);
	
	m_Ph3.SetWindowText(str);

	m_PhPos--;
	if (m_PhPos < 0)m_PhPos = 7;
}

void CPreWordsDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialog::OnPaint() for painting messages
	CRect rect;
	GetClientRect(&rect);
	CBrush br;
	br.CreateSolidBrush(RGB(255, 255, 255));
	dc.FillRect(rect, &br);
	br.DeleteObject();

	//rect.DeflateRect(5, 5);
	CBrush br2;
	br2.CreateSolidBrush(RGB(200, 200, 200));
	CBrush* old = dc.SelectObject(&br2);
	dc.RoundRect(rect, CPoint(10, 10));
	dc.SelectObject(old);
	br2.DeleteObject();

}


HBRUSH CPreWordsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	UINT ec[] = { IDC_EDIT_W1, IDC_EDIT_W2, IDC_EDIT_W3, IDC_EDIT_W4, IDC_EDIT_W5, IDC_EDIT_P1, IDC_EDIT_P2, IDC_EDIT_P3};

	int n1 = m_PhPos;
	int n2 = n1 + 1;
	int n3 = n2 + 1;
	if (n2 > 7) { n2 = 0; n3 = 1; }
	if (n3 > 7) n3 = 0;

	if ((pWnd->GetDlgCtrlID() == ec[n1])|| (pWnd->GetDlgCtrlID() == ec[n2])|| (pWnd->GetDlgCtrlID() == ec[n3]))
	{
		return (HBRUSH)m_PhraseBkBrush;
	}

	// TODO:  Return a different brush if the default is not desired
	else return hbr;
}

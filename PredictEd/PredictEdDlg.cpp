
// PredictEdDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PredictEd.h"
#include "PredictEdDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPredictEdDlg dialog



CPredictEdDlg::CPredictEdDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PREDICTED_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPredictEdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT21, m_Ed);
}

BEGIN_MESSAGE_MAP(CPredictEdDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CPredictEdDlg message handlers

BOOL CPredictEdDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	InitEd();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPredictEdDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPredictEdDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPredictEdDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPredictEdDlg::InitEd()
{
	CHARFORMAT2 cf = {};
	CString fontname = L"Georgia";
	cf.dwMask = CFM_FACE | CFM_SIZE;
	for (int n = 0; n < fontname.GetLength(); n++) cf.szFaceName[n] = fontname.GetAt(n);
	cf.yHeight = 22 * 10;
	m_Ed.SetDefaultCharFormat(cf);

	m_Ed.SetAutoURLDetect(TRUE);

}


void CPredictEdDlg::InsertText(CString text, COLORREF color, bool bold, bool italic)
{
	CHARFORMAT2 cf = {};
	int txtLen = m_Ed.GetTextLength();

	cf.cbSize = sizeof(cf);
	//	cf.dwMask = (bold ? CFM_BOLD : 0) | (italic ? CFM_ITALIC : 0) | CFM_COLOR;
	cf.dwMask = CFM_BOLD | CFM_ITALIC | CFM_COLOR;
	//	cf.dwEffects = (bold ? CFE_BOLD : 0) | (italic ? CFE_ITALIC : 0) | ~CFE_AUTOCOLOR;
	cf.dwEffects = (bold ? CFE_BOLD : 0) | (italic ? CFE_ITALIC : 0);
	cf.crTextColor = color;

	m_Ed.SetSel(txtLen, -1); // Set the cursor to the end of the text area and deselect everything.
	m_Ed.ReplaceSel((txtLen < 1 ? L"" : L"\r\n") + text); // Inserts when nothing is selected.

														   // Apply formating to the just inserted text.
	m_Ed.SetSel(txtLen, m_Ed.GetTextLength());
	m_Ed.SetSelectionCharFormat(cf);
	m_Ed.SendMessage(WM_VSCROLL, SB_BOTTOM);
}


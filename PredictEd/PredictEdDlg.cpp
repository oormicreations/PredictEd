
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
//	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CPredictEdDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDCANCEL, &CPredictEdDlg::OnBnClickedCancel)
	ON_WM_SYSCOMMAND()
	ON_COMMAND(ID_FILE_EXIT, &CPredictEdDlg::OnFileExit)
	ON_COMMAND(ID_FILE_OPEN32771, &CPredictEdDlg::OnFileOpen32771)
	ON_COMMAND(ID_FILE_SAVE32772, &CPredictEdDlg::OnFileSave32772)
	ON_COMMAND(ID_HELP_ABOUTPREDICTED, &CPredictEdDlg::OnHelpAboutpredicted)
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
	m_Menu.LoadMenu(IDR_MENU_TOP);
	SetMenu(&m_Menu);

	m_IsShellOpen = FALSE;
	m_Saved = TRUE;

	InitEd();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

//void CPredictEdDlg::OnSysCommand(UINT nID, LPARAM lParam)
//{
//	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
//	{
//		CAboutDlg dlgAbout;
//		dlgAbout.DoModal();
//	}
//	else
//	{
//		CDialogEx::OnSysCommand(nID, lParam);
//	}
//}

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



void CPredictEdDlg::OnBnClickedButton1()
{
	//int nLineLength, nLineLengthBuf, nLineIndex, nLineCount = m_Ed.GetLineCount();
	//CString strText;

	//// Dump every line of text of the rich edit control.
	//for (int i = 0; i < nLineCount; i++)
	//{
	//	nLineIndex = m_Ed.LineIndex(i);
	//	nLineLength = m_Ed.LineLength(nLineIndex);
	//	nLineLengthBuf = nLineLength;
	//	if (nLineLength < 2)nLineLengthBuf = 2;
	//	m_Ed.GetLine(i, strText.GetBufferSetLength(nLineLengthBuf + 1), nLineLengthBuf);
	//	strText.SetAt(nLineLength, _T('\0')); // null terminate
	//	strText.ReleaseBuffer(nLineLength + 1);

	//	TRACE(_T("line %d: '%s'\r\n"), i, strText);
	//}
	//long c1, c2;
	//m_Ed.GetSel(c1, c2);
	//m_Ed.SetSel(0, 8);
	//m_Ed.ReplaceSel(_T("xxxx"));
	//CHARRANGE cr;
	//cr.cpMax = 10;
	//cr.cpMin = 0;
	//SendMessageW(EM_SETSEL, 0, (LPARAM)&cr);

	long len = m_Ed.GetTextLength();
}


void CPredictEdDlg::OnBnClickedCancel()
{
	// disable close by ESC
	//CDialogEx::OnCancel();
}


void CPredictEdDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;  

		dlgAbout.DoModal();
	}
	else if ((nID & 0xFFF0) == SC_CLOSE)
	{
		EndDialog(IDOK);   
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}


void CPredictEdDlg::OnFileExit()
{
	EndDialog(IDOK);
}


void CPredictEdDlg::OnFileOpen32771()
{
	CString content;

	if (!m_IsShellOpen)
	{
		if (!m_Saved)
		{
			int ex = AfxMessageBox(_T("Do you wish to save your work before loading another file??"), MB_YESNOCANCEL | MB_ICONQUESTION);
			if (ex == IDCANCEL)	return;
			if (ex == IDYES)	OnFileSave32772();
		}

		content = m_SysHelper.GetFileContent();
	}

	m_Ed.SetRTF(content);
}


void CPredictEdDlg::OnFileSave32772()
{
	CString content;
	content = m_Ed.GetRTF();
	m_SysHelper.SetFileContent(content);
}


void CPredictEdDlg::OnHelpAboutpredicted()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

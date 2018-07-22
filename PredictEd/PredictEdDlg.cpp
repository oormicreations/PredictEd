
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
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CPredictEdDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDCANCEL, &CPredictEdDlg::OnBnClickedCancel)
	ON_WM_SYSCOMMAND()
	ON_COMMAND(ID_FILE_EXIT, &CPredictEdDlg::OnFileExit)
	ON_COMMAND(ID_FILE_OPEN32771, &CPredictEdDlg::OnFileOpen32771)
	ON_COMMAND(ID_FILE_SAVE32772, &CPredictEdDlg::OnFileSave32772)
	ON_COMMAND(ID_HELP_ABOUTPREDICTED, &CPredictEdDlg::OnHelpAboutpredicted)
	ON_COMMAND(ID_OPTIONS_ERASEMEMORIES, &CPredictEdDlg::OnOptionsErasememories)
	ON_COMMAND(ID_OPTIONS_TRAIN, &CPredictEdDlg::OnOptionsTrain)
	ON_COMMAND(ID_FILE_SAVEPREDICTIONS, &CPredictEdDlg::OnFileSavepredictions)
	ON_COMMAND(ID_OPTIONS_MERGESTMINLTM, &CPredictEdDlg::OnOptionsMergestminltm)
	ON_COMMAND(ID_FILE_SAVEFILEAS, &CPredictEdDlg::OnFileSavefileas)
	ON_COMMAND(ID_FILE_COPY, &CPredictEdDlg::OnFileCopy)
	ON_COMMAND(ID_FILE_PASTE, &CPredictEdDlg::OnFilePaste)
	ON_COMMAND(ID_EDIT_FINDANDREPLACE, &CPredictEdDlg::OnEditFindandreplace)
	ON_COMMAND(ID_OPTIONS_CLEARFORMATTING, &CPredictEdDlg::OnEditClearformatting)
	ON_COMMAND(ID_OPTIONS_FONTANDSIZE, &CPredictEdDlg::OnOptionsFontandsize)
	ON_COMMAND(ID_FILE_NEWFILE, &CPredictEdDlg::OnFileNewfile)
	ON_BN_CLICKED(IDC_BUTTON_NEW, &CPredictEdDlg::OnBnClickedButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CPredictEdDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CPredictEdDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_SAVEAS, &CPredictEdDlg::OnBnClickedButtonSaveas)
	ON_BN_CLICKED(IDC_BUTTON_COPY, &CPredictEdDlg::OnBnClickedButtonCopy)
	ON_BN_CLICKED(IDC_BUTTON_PASTE, &CPredictEdDlg::OnBnClickedButtonPaste)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CPredictEdDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_FONTS, &CPredictEdDlg::OnBnClickedButtonFonts)
	ON_BN_CLICKED(IDC_BUTTON_TRAIN2, &CPredictEdDlg::OnBnClickedButtonTrain2)
	ON_COMMAND(ID_OPTIONS_SETTINGS, &CPredictEdDlg::OnOptionsSettings)
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
	//m_Saved = TRUE;
	m_SaveCanceled = FALSE;

	m_MaxLimit = 2000000; //(bytes)get from settings

	InitEd();

	m_StartTime = CTime::GetCurrentTime();
	m_SysHelper.m_FileTitle = _T("Untitled");
	ShowMessage();

	m_Timer = SetTimer(WM_USER + 100, 5000, NULL);

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
	SetDefaultStyle(); //todo : get from settings
	m_Ed.SetAutoURLDetect(TRUE);
	UINT limit = m_Ed.GetLimitText();
	m_Ed.LimitText(m_MaxLimit);
	UINT limit2 = m_Ed.GetLimitText();

	m_Margin = 30; //todo : get from settings
	CRect rect;
	m_Ed.GetClientRect(rect);
	rect.left = m_Margin;
	rect.right = rect.right - m_Margin;
	m_Ed.SetRect(rect);
}

void CPredictEdDlg::SetDefaultStyle()
{
	CClientDC dc(this);
	CHARFORMAT2 cf = {};
	CString fontname = L"Georgia";
	cf.dwMask = CFM_FACE | CFM_SIZE;
	for (int n = 0; n < fontname.GetLength(); n++) cf.szFaceName[n] = fontname.GetAt(n);
	cf.yHeight = -MulDiv(11*10*2, dc.GetDeviceCaps(LOGPIXELSY), 72); //x 2 for twips
	m_Ed.SetDefaultCharFormat(cf);
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

	CPreWordsDlg *m_pDialog = new CPreWordsDlg;
	if (!m_pDialog->Create(IDD_DIALOG_PREWORDS))
	{
		delete m_pDialog;
		m_pDialog = NULL;
	}
	m_pDialog->ShowWindow(SW_SHOW);

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
		OnFileExit();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}


void CPredictEdDlg::OnFileExit()
{
	if (!m_Ed.m_Saved)
	{
		int ex = AfxMessageBox(_T("Do you wish to save your work before closing??"), MB_YESNOCANCEL | MB_ICONQUESTION);
		if (ex == IDCANCEL)	return;
		if (ex == IDYES) OnFileSave32772();
		if (m_SaveCanceled)
		{
			m_SaveCanceled = FALSE;
			return;
		}
	}

	KillTimer(m_Timer);
	EndDialog(IDOK);
}


void CPredictEdDlg::OnFileOpen32771()
{
	CString content;

	if (!m_IsShellOpen)
	{
		if (!m_Ed.m_Saved)
		{
			int ex = AfxMessageBox(_T("Do you wish to save your work before loading another file??"), MB_YESNOCANCEL | MB_ICONQUESTION);
			if (ex == IDCANCEL)	return;
			if (ex == IDYES) OnFileSave32772();
		}

		content = m_SysHelper.GetFileContent();
	}

	m_Ed.SetRTF(content);

	m_StartTime = CTime::GetCurrentTime();
	ShowMessage();

}


void CPredictEdDlg::OnFileSave32772()
{
	CString content;
	content = m_Ed.GetRTF();
	m_SaveCanceled = m_Ed.m_Saved =  (m_SysHelper.SetFileContent(content));

}


void CPredictEdDlg::OnHelpAboutpredicted()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


void CPredictEdDlg::OnOptionsErasememories()
{
	m_Ed.Erase();
}


void CPredictEdDlg::OnOptionsTrain()
{
	m_Ed.TrainFromFiles();
}


void CPredictEdDlg::OnFileSavepredictions()
{
	m_Ed.SavePredictions();
}


void CPredictEdDlg::OnOptionsMergestminltm()
{
	m_Ed.Merge();
}


void CPredictEdDlg::OnFileSavefileas()
{
	CString tmp = m_SysHelper.m_FileName;
	m_SysHelper.m_FileName = _T("");
	OnFileSave32772();
	if (!m_Ed.m_Saved) m_SysHelper.m_FileName = tmp;
}


void CPredictEdDlg::OnFileCopy()
{
	m_SysHelper.SetClipboardText(m_Ed.GetSelText());
}


void CPredictEdDlg::OnFilePaste()
{
	m_Ed.ReplaceSel(m_SysHelper.GetClipboardText(), TRUE);
}


void CPredictEdDlg::OnEditFindandreplace()
{
	// TODO: Add your command handler code here
}


void CPredictEdDlg::OnEditClearformatting()
{
	CHARFORMAT2 cf = {};
	m_Ed.GetDefaultCharFormat(cf);
	m_Ed.SetSelectionCharFormat(cf);
}


void CPredictEdDlg::OnOptionsFontandsize()
{
	CClientDC dc(this); // expects a CWnd that has already been initialized
	
	CHARFORMAT2 cf = {};
	m_Ed.GetSelectionCharFormat(cf);

	CString fontname = cf.szFaceName;
	LONG ht = cf.yHeight/(2*10); // 2 x for converting from twips
	ht = MulDiv(ht, 72, dc.GetDeviceCaps(LOGPIXELSY));

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));

	lf.lfHeight = -MulDiv(ht, dc.GetDeviceCaps(LOGPIXELSY), 72);
	_tcscpy_s(lf.lfFaceName, LF_FACESIZE, fontname);

	// Show the font dialog 
	CFontDialog dlg(&lf);

	dlg.m_cf.Flags |= CF_EFFECTS;
	dlg.m_cf.rgbColors = cf.crTextColor;

	if (dlg.DoModal() == IDOK)
	{
		CHARFORMAT2 newcf = {};

		_tcscpy_s(newcf.szFaceName, dlg.GetFaceName());
		newcf.dwMask |= CFM_FACE;
		newcf.yHeight = (dlg.GetSize() / 10) * 20;	// convert into twips
		newcf.dwMask |= CFM_SIZE;
		newcf.crTextColor = dlg.GetColor();
		newcf.dwMask |= CFM_COLOR;
		newcf.dwMask |= CFM_BOLD;
		if (dlg.IsBold()) 
		{
			newcf.dwEffects |= CFE_BOLD;
		}
		newcf.dwMask |= CFM_ITALIC;
		if (dlg.IsItalic()) 
		{
			newcf.dwEffects |= CFE_ITALIC;
		}
		newcf.dwMask |= CFM_UNDERLINE;
		if (dlg.IsUnderline()) 
		{
			newcf.dwEffects |= CFE_UNDERLINE;
		}
		newcf.dwMask |= CFM_STRIKEOUT;
		if (dlg.IsStrikeOut()) 
		{
			newcf.dwEffects |= CFE_STRIKEOUT;
		}

		m_Ed.SetSelectionCharFormat(newcf);
	}
}


void CPredictEdDlg::OnFileNewfile()
{
	if (!m_Ed.m_Saved)
	{
		int ex = AfxMessageBox(_T("Do you wish to save your work??"), MB_YESNOCANCEL | MB_ICONQUESTION);
		if (ex == IDCANCEL)	return;
		if (ex == IDYES) OnFileSave32772();
	}

	m_Ed.Reset();
	m_StartTime = CTime::GetCurrentTime();
	m_SysHelper.m_FileTitle = _T("Untitled");
	m_SysHelper.m_FileName = _T("");
	ShowMessage();
}


void CPredictEdDlg::OnBnClickedButtonNew()
{
	OnFileNewfile();
}


void CPredictEdDlg::OnBnClickedButtonOpen()
{
	OnFileOpen32771();
}


void CPredictEdDlg::OnBnClickedButtonSave()
{
	OnFileSave32772();
}


void CPredictEdDlg::OnBnClickedButtonSaveas()
{
	OnFileSavefileas();
}


void CPredictEdDlg::OnBnClickedButtonCopy()
{
	OnFileCopy();
}


void CPredictEdDlg::OnBnClickedButtonPaste()
{
	OnFilePaste();
}


void CPredictEdDlg::OnBnClickedButtonClear()
{
	OnEditClearformatting();
}


void CPredictEdDlg::OnBnClickedButtonFonts()
{
	OnOptionsFontandsize();
}


void CPredictEdDlg::OnBnClickedButtonTrain2()
{
	OnOptionsTrain();
}


void CPredictEdDlg::OnOptionsSettings()
{
	// TODO: Add your command handler code here
}


void CPredictEdDlg::ShowMessage()
{
	CString str, duration;
	CTime t = CTime::GetCurrentTime();
	CTimeSpan diff = t - m_StartTime;
	duration = diff.Format(_T("%H:%M:%S"));

	str.Format(_T("%s | Words: %d | Lines: %d | Duration: %s"), m_SysHelper.m_FileTitle, m_Ed.GetWordCount(), m_Ed.GetLineCount(), duration);
	GetDlgItem(IDC_EDIT_PRE)->SetWindowTextW(str);
}


void CPredictEdDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == m_Timer)
	{
		ShowMessage();
	}

	CDialogEx::OnTimer(nIDEvent);
}

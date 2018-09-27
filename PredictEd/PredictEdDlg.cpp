
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
	ON_REGISTERED_MESSAGE(WM_FINDREPLACE, &CPredictEdDlg::OnFindReplace)
	ON_COMMAND(ID_HELP_ONLINEHELP, &CPredictEdDlg::OnHelpOnlinehelp)
	ON_COMMAND(ID_HELP_CHECKFORUPDATES, &CPredictEdDlg::OnHelpCheckforupdates)
	ON_COMMAND(ID_HELP_GETMOREFREEAPPS, &CPredictEdDlg::OnHelpGetmorefreeapps)
	ON_COMMAND(ID_ENCRYPTION_OPENENCRYPTEDFILE, &CPredictEdDlg::OnEncryptionOpenencryptedfile)
	ON_COMMAND(ID_ENCRYPTION_SAVEASENCRYPTEDFILE, &CPredictEdDlg::OnEncryptionSaveasencryptedfile)
	ON_COMMAND(ID_ENCRYPTION_ENCRYPT, &CPredictEdDlg::OnEncryptionEncrypt)
	ON_COMMAND(ID_ENCRYPTION_SHA512HASH, &CPredictEdDlg::OnEncryptionSha512hash)
	ON_COMMAND(ID_ENCRYPTION_CONVERTTO, &CPredictEdDlg::OnEncryptionConvertto)
	ON_COMMAND(ID_BASE64_ENCODE, &CPredictEdDlg::OnBase64Encode)
	ON_COMMAND(ID_BASE64_DECODE, &CPredictEdDlg::OnBase64Decode)
	ON_COMMAND(ID_ENCRYPTION_STEG, &CPredictEdDlg::OnEncryptionSteg)
	ON_COMMAND(ID_STEG_ENCODE, &CPredictEdDlg::OnStegEncode)
	ON_COMMAND(ID_STEG_DECODE, &CPredictEdDlg::OnStegDecode)
	ON_COMMAND(ID_EDIT_SPELLINGCHECK, &CPredictEdDlg::OnEditSpellingcheck)
	ON_MESSAGE(WM_SELMISSPELLED, &CPredictEdDlg::OnSelectMisspell)
	ON_MESSAGE(WM_SETSPELLSUGGESTION, &CPredictEdDlg::OnSelectSpellSuggestion)
	ON_COMMAND(ID_CONTEXTS_LOADCONTEXT, &CPredictEdDlg::OnContextsLoadcontext)
	ON_COMMAND(ID_CONTEXTS_SAVECONTEXT, &CPredictEdDlg::OnContextsSavecontext)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_PREDICTED_UPDATEWORDCOUNT, OnUpdateWordCount)
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
	m_PredictEdVersionMaj = 1;
	m_PredictEdVersionMin = 1;

	m_SysHelper.SetHeaders(m_PredictEdVersionMaj, m_PredictEdVersionMin);

	m_Menu.LoadMenu(IDR_MENU_TOP);
	SetMenu(&m_Menu);
	SetButtons();

	m_Abort = FALSE;
	m_IsShellOpen = FALSE;
	m_SaveCanceled = FALSE;

	m_MaxLimit = 2000000; //(bytes)
	m_Margin = 30;
	m_DefaultFontSz = 12;

	m_TipDuration = 0;
	m_TipNum = 0;

	InitEd();
	InitContext();
	InitTips();

	m_StartTime = CTime::GetCurrentTime();
	m_SysHelper.m_FileTitle = _T("Untitled");


	m_Timer = SetTimer(WM_USER + 100, PREDICTED_CLOCK_INTERVEL * 1000, NULL);
	m_pFRDlg = NULL;

	m_NetHelper.ReportUsage(_T("PredictEd"), m_PredictEdVersionMaj*10 + m_PredictEdVersionMin);

	SetToolTips();
	ShowMessage();
	ShowDuration();
	ShowTips();


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPredictEdDlg::SetButtons()
{
	UINT id[] = { IDC_BUTTON_NEW, IDC_BUTTON_OPEN, IDC_BUTTON_SAVE, IDC_BUTTON_SAVEAS, IDC_BUTTON_COPY,
		IDC_BUTTON_PASTE, IDC_BUTTON_FONTS, IDC_BUTTON_CLEAR, IDC_BUTTON_TRAIN };
	UINT bm[] = { IDB_BITMAP_NEW, IDB_BITMAP_OPEN, IDB_BITMAP_SAVE, IDB_BITMAP_SAVEAS, IDB_BITMAP_COPY,
		IDB_BITMAP_PASTE, IDB_BITMAP_FONTS, IDB_BITMAP_CLEAR, IDB_BITMAP_INS };

	for (int i = 0; i < 9; i++)
	{
		CBitmap bmp;
		bmp.LoadBitmap(bm[i]);

		CButton* pButton = (CButton*)GetDlgItem(id[i]);
		pButton->ModifyStyle(0, BS_BITMAP);
		pButton->SetBitmap(bmp);
	}


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
		//CPaintDC dc(this); // device context for painting
		//CRect rect;
		//GetClientRect(&rect);
		//dc.FillSolidRect(rect, RGB(100, 100, 100));
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
	UINT n;
	PREDICTEDSET *pps = NULL;
	CString profile;
	profile.Format(_T("%d%d"), m_PredictEdVersionMaj, m_PredictEdVersionMin);

	if (AfxGetApp()->GetProfileBinary(_T("PredictEd"), _T("Settings") + profile, (LPBYTE*)&pps, &n))
	{
		if (n == sizeof(PREDICTEDSET))
		{
			m_Ed.SetBackgroundColor(0, pps->m_BkColor);
			//m_LTMSz = pps->m_LTMSz;
			//m_STMSz = pps->m_STMSz;
			m_MaxLimit = pps->m_MaxLimit * 1000; //its in kb in settings
			m_Margin = pps->m_Margins;

			CClientDC dc(this);
			CHARFORMAT2 cf = {};
			CString fontname(pps->m_DefFont.lfFaceName);

			cf.dwMask = CFM_FACE | CFM_SIZE | CFM_COLOR | CFM_BOLD;
			for (int n = 0; n < fontname.GetLength(); n++) cf.szFaceName[n] = pps->m_DefFont.lfFaceName[n];

			long ht = -MulDiv(pps->m_DefFont.lfHeight, 72, dc.GetDeviceCaps(LOGPIXELSY)); //em to pt
			cf.yHeight = ht * TWIPS_PER_PT; // pt to twips 

			cf.crTextColor = pps->m_TxtColor;
			cf.wWeight = (WORD)pps->m_DefFont.lfWeight;

			m_Ed.SetDefaultCharFormat(cf);

			m_ContextFile.SetString(pps->m_ContextFileTch);
			m_DictionaryFile.SetString(pps->m_DictionaryFileTch);


		}
		else
		{
			SetDefaultStyle();
		}

		if(pps) delete[] pps;
	}
	else
	{
		SetDefaultStyle();
	}

	m_Ed.LimitText(m_MaxLimit);
	m_Ed.SetAutoURLDetect(TRUE);
	m_Ed.SetOptions(ECOOP_OR, ECO_NOHIDESEL);

	CRect rect;
	m_Ed.GetClientRect(rect);
	rect.DeflateRect(m_Margin, 0);
	m_Ed.SetRect(rect);
}

void CPredictEdDlg::SetDefaultStyle()
{
	CClientDC dc(this);
	CHARFORMAT2 cf = {};
	CString fontname = L"Georgia";
	if (!m_SysHelper.IsFontInstalled(fontname))
	{
		fontname = L"";
	}
	cf.dwMask = CFM_FACE | CFM_SIZE;
	for (int n = 0; n < fontname.GetLength(); n++) cf.szFaceName[n] = fontname.GetAt(n);
	cf.yHeight = m_DefaultFontSz * TWIPS_PER_PT; // pt to twips 

	m_Ed.SetDefaultCharFormat(cf);

	m_Ed.SetBackgroundColor(0, RGB(255, 253, 245));

}

void CPredictEdDlg::InitContext()
{
	m_Context.SetVersion(m_PredictEdVersionMaj, m_PredictEdVersionMin);
	if (m_ContextFile.IsEmpty())
	{
		m_ContextFile = m_SysHelper.GetUserDocumentPath(PREDICTED_USER_FOLDER) + _T("\\") + PREDICTED_CONTEXT_DEFAULT_CONTEXT;
	}
	m_Context.m_sContextFile = m_ContextFile;

	m_Context.m_bAutoLoad = TRUE;
	OnContextsLoadcontext();
	m_Context.m_bAutoLoad = FALSE;

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

	if (m_ToolTip) delete m_ToolTip;

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

	if (m_SysHelper.m_FileExt == _T("rtf"))
	{
		m_Ed.SetRTF(content);
	}
	else
	{
		m_Ed.SetWindowText(content);
	}

	m_StartTime = CTime::GetCurrentTime();
	m_CurFileTitle = m_SysHelper.m_FileTitle;

	ShowMessage();

	m_Ed.SetFocus();
	m_Ed.SetScrollPos(SB_VERT, 0);
	m_Ed.SetCaretPos(CPoint(0, 0));
	m_Ed.SetSel(0, 0);
}


void CPredictEdDlg::OnFileSave32772()
{
	if (m_SysHelper.m_FileName.IsEmpty())
	{
		BOOL res = m_SysHelper.GetSaveFileNameType();
		m_SaveCanceled = !res;
		if (!res) return;
	}

	CString content;
	if (m_SysHelper.m_FileExt == _T("rtf"))
	{
		content = m_Ed.GetRTF();
	}
	else m_Ed.GetWindowText(content);

	m_Ed.m_Saved = (m_SysHelper.SetFileContent(content));

	m_CurFileTitle = m_SysHelper.m_FileTitle;
	ShowMessage();
}


void CPredictEdDlg::OnHelpAboutpredicted()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


void CPredictEdDlg::OnOptionsErasememories()
{
	BackupMemories();
	m_Ed.Erase();
}

void CPredictEdDlg::BackupMemories()
{
	CString backupname = m_Ed.m_LTMFileName;
	backupname.Replace(_T(".txt"), _T("_Backup.txt"));
	m_Ed.m_LTM.SaveMap(backupname, m_SysHelper.GetHeader(LTM_HEADER));

	backupname = m_Ed.m_STMFileName;
	backupname.Replace(_T(".txt"), _T("_Backup.txt"));
	m_Ed.m_STM.SaveMap(backupname, m_SysHelper.GetHeader(STM_HEADER));

}

void CPredictEdDlg::OnOptionsTrain()
{
	BackupMemories();

	CTrain traindlg;
	traindlg.m_FileName = m_Ed.m_LTMFileName;
	traindlg.m_FileHeader = m_SysHelper.GetHeader(LTM_HEADER);
	traindlg.DoModal();
	if (traindlg.m_Result)
	{
		//refresh
		m_Ed.m_LTM.LoadMap(m_Ed.m_LTMFileName);
	}
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
	CString str = m_Ed.GetSelText();
	if (str.IsEmpty()) m_Ed.SetSel(0, -1);
	m_Ed.Copy();
}


void CPredictEdDlg::OnFilePaste()
{
	m_Ed.Paste();
	//m_Ed.SetRTF(m_SysHelper.GetClipboardText());
	m_Ed.m_Saved = FALSE;
}


void CPredictEdDlg::OnEditFindandreplace()
{
	InitFindReplaceDlg();
}

void CPredictEdDlg::InitFindReplaceDlg()
{
	if (NULL == m_pFRDlg)
	{
		m_pFRDlg = new CFindReplaceDialog();  // Must be created on the heap

		m_pFRDlg->Create(FALSE, m_Ed.GetSelText(), _T(""), FR_DOWN, this);

		m_pFRDlg->m_fr.lStructSize = sizeof(FINDREPLACE);
		m_pFRDlg->m_fr.hwndOwner = this->m_hWnd;
	}
}

LONG CPredictEdDlg::OnFindReplace(WPARAM wParam, LPARAM lParam)
{
	if (m_pFRDlg != NULL)
	{
		if (m_pFRDlg->IsTerminating())
		{
			m_pFRDlg = NULL;
		}
		else
		{
			CString sfind = m_pFRDlg->GetFindString();
			CString srepl = m_pFRDlg->GetReplaceString();
			BOOL mcase = m_pFRDlg->MatchCase();
			BOOL mword = m_pFRDlg->MatchWholeWord();
			BOOL repall = m_pFRDlg->ReplaceAll();
			BOOL repcur = m_pFRDlg->ReplaceCurrent();
			BOOL next = m_pFRDlg->FindNext();

			long nStartChar, nEndChar, n;
			m_Ed.GetSel(nStartChar, nEndChar);

			FINDTEXTEX ft;
			ft.chrg.cpMin = nEndChar;
			ft.chrg.cpMax = -1;
			ft.lpstrText = sfind;

			if (next)
			{
				n = m_Ed.FindText((FR_MATCHCASE*mcase) | (FR_WHOLEWORD*mword) | FR_DOWN, &ft);
				if (n != -1)
				{
					m_Ed.SetSel(ft.chrgText);
				}
				else m_Ed.SetSel(0, 0); //find from top again
			}

			if (repcur)
			{
				if(!m_Ed.GetSelText().IsEmpty()) m_Ed.ReplaceSel(srepl);
			}

			if (repall)
			{
				long n = 0;
				UINT rcount = 0;

				while (n >= 0)
				{
					ft.chrg.cpMin = 0;

					n = m_Ed.FindText((FR_MATCHCASE*mcase) | (FR_WHOLEWORD*mword) | FR_DOWN, &ft);
					if (n != -1)
					{
						m_Ed.SetSel(ft.chrgText);
						if (!m_Ed.GetSelText().IsEmpty())
						{
							m_Ed.ReplaceSel(srepl);
							rcount++;
						}
					}

				}

				CString res;
				res.Format(_T("%d occurances were replaced."), rcount);
				m_Ed.UpdateStatusMessage(res);
			}
		}
	}
	return 0;
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
	LONG ht = cf.yHeight/ TWIPS_PER_PT;// twips to pt

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));

	lf.lfHeight = -MulDiv(ht, dc.GetDeviceCaps(LOGPIXELSY), 72); //pt to em
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
		newcf.yHeight = (dlg.GetSize() / 10) * TWIPS_PER_PT;	// GetSize returns size in pt * 10. convert into twips
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
	m_CurFileTitle = m_SysHelper.m_FileTitle;
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
	CPredictEdSettingsDlg setdlg;
	UINT n;
	PREDICTEDSET ps, *pps;
	CString profile;
	profile.Format(_T("%d%d"), m_PredictEdVersionMaj, m_PredictEdVersionMin);

	if (AfxGetApp()->GetProfileBinary(_T("PredictEd"), _T("Settings") + profile, (LPBYTE*)&pps, &n))
	{
		if (n == sizeof(PREDICTEDSET))
		{
			setdlg.m_BkColor		= pps->m_BkColor;
			setdlg.m_LTMSz			= pps->m_LTMSz;
			setdlg.m_STMSz			= pps->m_STMSz;
			setdlg.m_MaxLimit		= pps->m_MaxLimit;
			setdlg.m_Margins		= pps->m_Margins;
			setdlg.m_TxtColor		= pps->m_TxtColor;
			setdlg.m_DefFont		= pps->m_DefFont;

			setdlg.m_DictionaryFile.SetString(pps->m_DictionaryFileTch);
			setdlg.m_ContextFile.SetString(pps->m_ContextFileTch);

		}
		else setdlg.Reset();
		delete[] pps;
	}
	else setdlg.Reset();

	if (setdlg.DoModal() == IDOK)
	{
		ps.m_BkColor		= setdlg.m_BkColor;
		ps.m_LTMSz			= setdlg.m_LTMSz;
		ps.m_STMSz			= setdlg.m_STMSz;
		ps.m_MaxLimit		= setdlg.m_MaxLimit;
		ps.m_Margins		= setdlg.m_Margins;
		ps.m_TxtColor		= setdlg.m_TxtColor;
		ps.m_DefFont		= setdlg.m_DefFont;

		for (int i = 0; i < MAX_PATH+1; i++)
		{
			ps.m_DictionaryFileTch[i] = 0;
			ps.m_ContextFileTch[i] = 0;
		}
		for (int i = 0; i < setdlg.m_DictionaryFile.GetLength(); i++)
		{
			ps.m_DictionaryFileTch[i] = setdlg.m_DictionaryFile.GetAt(i);
		}
		for (int i = 0; i < setdlg.m_ContextFile.GetLength(); i++)
		{
			ps.m_ContextFileTch[i] = setdlg.m_ContextFile.GetAt(i);
		}



		AfxGetApp()->WriteProfileBinary(_T("PredictEd"), _T("Settings") + profile, (LPBYTE)&ps, sizeof(ps));

		InitEd();
	}
}


void CPredictEdDlg::ShowMessage()
{
	CString str;
	str.Format(_T("Context: %s | File: %s | Words: %d | Lines: %d"), m_Context.m_sContextName, m_SysHelper.m_FileTitle, m_Ed.GetWordCount(), m_Ed.GetLineCount());
	GetDlgItem(IDC_EDIT_PRE)->SetWindowText(str);
}

void CPredictEdDlg::ShowDuration()
{
	CString str, duration;

	CTime t = CTime::GetCurrentTime();
	CTimeSpan diff = t - m_StartTime;
	duration = diff.Format(_T("%H:%M:%S"));

	str.Format(_T("Duration: %s"), duration);
	GetDlgItem(IDC_EDIT_DUR)->SetWindowText(str);
}

void CPredictEdDlg::ShowTips()
{
	if (m_Tips[m_TipNum].IsEmpty()) m_TipNum = 0;
	GetDlgItem(IDC_EDIT_TIPS)->SetWindowText(m_Tips[m_TipNum]);
}

void CPredictEdDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == m_Timer)
	{
		ShowDuration();

		m_TipDuration += PREDICTED_CLOCK_INTERVEL;

		if (m_TipDuration >= PREDICTED_TIP_INTERVEL)
		{
			ShowTips();
			m_TipNum++;
			if (m_TipNum >= PREDICTED_MAX_TIPS)
			{
				m_TipNum = 0;
			}
			m_TipDuration = 0;
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CPredictEdDlg::OnHelpOnlinehelp()
{
	ShellExecute(NULL, _T("open"), _T("https://oormi.in/software/predicted/help.html"), NULL, NULL, SW_SHOWNORMAL);
}


void CPredictEdDlg::OnHelpCheckforupdates()
{
	//SetDlgItemText(IDC_EDIT_WORDS, _T("Checking for updates..."));

	m_NetHelper.Checkforupdates(m_PredictEdVersionMaj, m_PredictEdVersionMin, _T("https://oormi.in/software/predicted/updatepredicted.txt"),
		_T("https://github.com/oormicreations/PredictEd/releases"), _T("PredictEd App"));
}


void CPredictEdDlg::OnHelpGetmorefreeapps()
{
	ShellExecute(NULL, _T("open"), _T("https://github.com/oormicreations?tab=repositories"), NULL, NULL, SW_SHOWNORMAL);
}


void CPredictEdDlg::OnEncryptionOpenencryptedfile()
{
	if (!m_Ed.m_Saved)
	{
		int ex = AfxMessageBox(_T("Do you wish to save your work before loading another file??"), MB_YESNOCANCEL | MB_ICONQUESTION);
		if (ex == IDCANCEL)	return;
		if (ex == IDYES) OnFileSave32772();
	}

	CAESDecryptDlg ddlg;

	CFileDialog DataFileOpenDialog(true, _T(""), _T(""), OFN_FILEMUSTEXIST, _T("All Files (*.*)|*.*||"));
	DataFileOpenDialog.m_ofn.lpstrTitle = _T("Open an Encrypted File ...");

	INT_PTR res = DataFileOpenDialog.DoModal();
	if (res != IDCANCEL)
	{
		ddlg.m_EncryptedFilename = DataFileOpenDialog.GetPathName();
		ddlg.m_DecryptedFilename = m_SysHelper.GetPredictEdFileName(PREDICTED_DEC_FILE);

		if (!ddlg.m_EncryptedFilename.IsEmpty())
		{
			if (ddlg.DoModal())
			{
				if (!ddlg.m_DecryptedFilename.IsEmpty() && ddlg.m_DecryptResult)
				{
					CString content = m_SysHelper.ReadStringFromFile(ddlg.m_DecryptedFilename);

					if (ddlg.m_DecryptedFileType == 1)
					{
						m_Ed.SetRTF(content);
					}
					else
					{
						m_Ed.SetWindowText(content);
					}

					m_StartTime = CTime::GetCurrentTime();
					m_SysHelper.m_FileName.Empty();
					m_SysHelper.m_FileTitle.Empty();
					m_SysHelper.m_FileExt.Empty();
					ShowMessage();

					m_Ed.SetFocus();
					m_Ed.SetScrollPos(SB_VERT, 0);
					m_Ed.SetCaretPos(CPoint(0, 0));
					m_Ed.SetSel(0, 0);
					m_Ed.m_Saved = FALSE;

					//delete the decrypted file from disc
					if (!DeleteFile(ddlg.m_DecryptedFilename)) AfxMessageBox(_T("Could not clean up temp decrypted file from the disk!\r\nPlease remove it manually\r\nThe file is here:") + ddlg.m_DecryptedFilename, MB_ICONERROR);
				}
			}

		}
	}

	//ddlg.m_EncryptedFilename =
}


void CPredictEdDlg::OnEncryptionSaveasencryptedfile()
{
	CAESEncryptDlg edlg;
	if (!m_Ed.m_Saved)
	{
		int res = AfxMessageBox(_T("The changes are not saved. Do you wish to save before encrypting?"), MB_YESNOCANCEL);
		if (res == IDCANCEL) return;
		if (res == IDYES)
		{
			OnFileSave32772();
			if (m_SaveCanceled) return;
		
		}
		if (m_SysHelper.m_FileName.IsEmpty())
		{
			AfxMessageBox(_T("File must be saved in order to be encrypted!"), MB_ICONERROR);
			return;
		}
	}

	edlg.m_SourceFileName = m_SysHelper.m_FileName;
	edlg.m_BackupFileName = m_Ed.m_AutoBackupFileName;

	edlg.DoModal();

}


void CPredictEdDlg::OnEncryptionEncrypt()
{
	CAESEncryptDlg edlg;
	edlg.m_FileEnc = TRUE;
	edlg.DoModal();

}


void CPredictEdDlg::OnEncryptionSha512hash()
{
	CHashDlg hdlg;
	m_Ed.GetWindowText(hdlg.m_Content);
	hdlg.DoModal();
}


void CPredictEdDlg::OnEncryptionConvertto()
{
	//CBase64Dlg bdlg;
	//bdlg.DoModal();
}


void CPredictEdDlg::OnBase64Encode()
{
	if (m_SysHelper.GetFileNameToOpen(_T("All Files (*.*)|*.*||"), _T("Select a file to encode...")))
	{
		CCryptHelper crypthelper;
		if ((crypthelper.B64Encode(m_SysHelper.m_FileName)) && (!crypthelper.m_OutputFile.IsEmpty()))
		{
			AfxMessageBox(_T("File Encoded and saved as:\r\n\r\n") + crypthelper.m_OutputFile, MB_ICONINFORMATION);
		}
		else AfxMessageBox(_T("Base64 encoding failed!"), MB_ICONERROR);
	}
}


void CPredictEdDlg::OnBase64Decode()
{
	if (m_SysHelper.GetFileNameToOpen(_T("All Files (*.*)|*.*||"), _T("Select a file to decode...")))
	{
		CCryptHelper crypthelper;
		if ((crypthelper.B64Decode(m_SysHelper.m_FileName)) && (!crypthelper.m_OutputFile.IsEmpty()))
		{
			AfxMessageBox(_T("File Decoded and saved as:\r\n\r\n") + crypthelper.m_OutputFile, MB_ICONINFORMATION);
		}
		else AfxMessageBox(_T("Base64 decoding failed!"), MB_ICONERROR);
	}
}


void CPredictEdDlg::OnEncryptionSteg()
{
}


void CPredictEdDlg::OnStegEncode()
{
	CStegEncDlg esdlg;
	esdlg.DoModal();
}


void CPredictEdDlg::OnStegDecode()
{
	CStegDecDlg dsdlg;
	dsdlg.DoModal();
}


void CPredictEdDlg::OnEditSpellingcheck()
{
	CSpellCheckDlg spelldlg;
	spelldlg.m_DicFile = m_Context.m_sDictionary;

	if (!spelldlg.LoadDictionary())
	{
		AfxMessageBox(_T("Error reading the Dictionary file."), MB_ICONERROR);
		return;
	}

	m_Ed.GetWindowText(spelldlg.m_Content);
	spelldlg.m_Content.Replace(_T("\n"), _T("")); //GetSel/SetSel counts NL as one char, so remove \n

	spelldlg.DoModal();

	if (spelldlg.m_IsDicChanged)
	{
		m_Context.m_sDictionary = spelldlg.m_DicFile;
		m_Context.UpdateContext();
	}
}

LONG CPredictEdDlg::OnSelectMisspell(WPARAM wParam, LPARAM lParam)
{
	CString* pstr = (CString*)wParam;

	long nStartChar, nEndChar, n;
	m_Ed.GetSel(nStartChar, nEndChar);

	FINDTEXTEX ft;
	ft.chrg.cpMin = nEndChar;
	ft.chrg.cpMax = -1;
	ft.lpstrText = pstr->GetString();

		n = m_Ed.FindText(FR_WHOLEWORD|FR_DOWN, &ft);
		if (n != -1)
		{
			m_Ed.SetSel(ft.chrgText);
		}
		else m_Ed.SetSel(0, 0); //find from top again

	return 0;
}

LONG CPredictEdDlg::OnSelectSpellSuggestion(WPARAM wParam, LPARAM lParam)
{
	CString* pstr = (CString*)wParam;
	if (!m_Ed.GetSelText().IsEmpty()) m_Ed.ReplaceSel(pstr->GetString());
	return 0;
}

void CPredictEdDlg::OnContextsLoadcontext()
{
	if (m_Context.LoadContext())
	{
		BOOL success = TRUE;

		//load knowledge
		success = success && m_Ed.m_LTM.LoadMap(m_Context.m_sLTMFile);
		success = success && m_Ed.m_STM.LoadMap(m_Context.m_sSTMFile);
		success = success && (!m_Context.m_sDictionary.IsEmpty());

		if(!success)
		{
			AfxMessageBox(_T("Error: Context loading failed."), MB_ICONERROR);
			//try to fall back
			m_Ed.m_LTM.LoadMap(m_Ed.m_LTMFileName); 
			m_Ed.m_STM.LoadMap(m_Ed.m_STMFileName); 
			return;
		}

		m_DictionaryFile = m_Context.m_sDictionary;

		//point to new knowledge files
		m_Ed.m_LTMFileName = m_Context.m_sLTMFile;
		m_Ed.m_STMFileName = m_Context.m_sSTMFile;

		ShowMessage();
	}
	else
	{
		//try to load the default context
		if (m_Abort)
		{
			int res = AfxMessageBox(_T("Error: Context loading failed.\r\nThis can happen if some of the user folders are deleted, or when the program is launched as standalone without installation.\
\r\n\r\nPredictEd will attempt to create a default context.\r\nIf that fails, please reinstall PredictEd.\r\n\r\nDo you wish to proceed?"), MB_ICONERROR|MB_YESNO);
			if (res == IDYES)
			{
				if (!m_Context.CreateDefaultContext()) EndDialog(IDOK);
			}
			else EndDialog(IDOK);
		}
		else
		{
			m_Abort = TRUE;
			m_ContextFile.Empty();
			InitContext();
		}

	}
}


void CPredictEdDlg::OnContextsSavecontext()
{
	if (m_Context.GetNewContextName())
	{
		m_Context.m_sLTMFile = m_Ed.m_LTMFileName;
		m_Context.m_sSTMFile = m_Ed.m_STMFileName;
		m_Context.m_sDictionary = m_DictionaryFile;

		m_Context.CreateContext();
	}
}

void CPredictEdDlg::SetToolTips()
{
	m_ToolTip = new CToolTipCtrl();
	m_ToolTip->Create(this);

	int buttonids[] = { IDC_BUTTON_NEW, IDC_BUTTON_OPEN,IDC_BUTTON_SAVE,IDC_BUTTON_SAVEAS,IDC_BUTTON_COPY,IDC_BUTTON_PASTE,IDC_BUTTON_FONTS,IDC_BUTTON_CLEAR,IDC_BUTTON_TRAIN };
	CString tips[] = { _T("New File"), _T("Open File"),  _T("Save File"),  _T("Save File As"),  _T("Copy"),  _T("Paste"), _T("Set Fonts"), _T("Clear Text Formatting"), _T("Train using text files") };
	int nbuttons = 9;

	for (int i = 0; i < nbuttons; i++)
	{
		CWnd* pWnd = GetDlgItem(buttonids[i]);
		m_ToolTip->AddTool(pWnd, tips[i]);
	}

	m_ToolTip->Activate(TRUE);

	//m_ToolTip->SetTipBkColor(RGB(0,255,0));
	//m_ToolTip->SetTipTextColor(RGB(255, 0, 0));
}

BOOL CPredictEdDlg::PreTranslateMessage(MSG* pMsg)
{
	if (m_ToolTip != NULL)
	{
		m_ToolTip->RelayEvent(pMsg);
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


HBRUSH CPredictEdDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID() == IDC_EDIT_TIPS)
	{
		pDC->SetTextColor(RGB(150, 150, 150));
	}

	if (pWnd->GetDlgCtrlID() == IDC_EDIT_DUR)
	{
		pDC->SetTextColor(RGB(150, 0, 0));
	}

	if (pWnd->GetDlgCtrlID() == IDC_EDIT_WORDS)
	{
		pDC->SetTextColor(RGB(90, 90, 90));
	}

	if (pWnd->GetDlgCtrlID() == IDC_EDIT_PRE)
	{
		pDC->SetTextColor(RGB(50, 50, 50));
	}
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void CPredictEdDlg::InitTips()
{
	m_Tips[0] = _T("Press TAB to cycle through suggestions.");
	m_Tips[1] = _T("You can type **your text** to make it bold.");
	m_Tips[2] = _T("You can type //your text// to make it italics.");
	m_Tips[3] = _T("You can type __your text__ to underline text.");
	m_Tips[4] = _T("You can type **__your text__** to make it underlined bold.");
	m_Tips[5] = _T("For best result train PredictEd using your own writings.");
	m_Tips[6] = _T("Save a context to save the context specific knowledge.");
	m_Tips[7] = _T("Load a context to get suggestions specific to a context.");
	m_Tips[8] = _T("The duration counter helps to stop and relax periodically.");
	//m_Tips[] = _T("");
}

LRESULT CPredictEdDlg::OnUpdateWordCount(WPARAM, LPARAM)
{
	ShowMessage();
	return 0;
}
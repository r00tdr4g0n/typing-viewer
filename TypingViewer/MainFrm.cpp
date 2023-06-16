
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "pch.h"
#include "framework.h"
#include "TypingViewer.h"
#include "TypingViewerDoc.h"
#include "TypingViewerView.h" 
#include "MainFrm.h"
#include "SettingWordDlg.h"
#include "LoginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_TOOLBAR_BUTTON_FONT, &CMainFrame::OnToolbarButtonFont)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_BUTTON_FONT, &CMainFrame::OnUpdateToolbarButtonFont)
	ON_COMMAND(ID_TOOLBAR_BUTTON_INFO, &CMainFrame::OnToolbarButtonInfo)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_BUTTON_INFO, &CMainFrame::OnUpdateToolbarButtonInfo)
	ON_COMMAND(ID_TOOLBAR_BUTTON_MONITOR, &CMainFrame::OnToolbarButtonMonitor)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_BUTTON_MONITOR, &CMainFrame::OnUpdateToolbarButtonMonitor)
	ON_COMMAND(ID_TOOLBAR_BUTTON_SAVE, &CMainFrame::OnToolbarButtonSave)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_BUTTON_SAVE, &CMainFrame::OnUpdateToolbarButtonSave)
	ON_COMMAND(ID_TOOLBAR_BUTTON_SETTING, &CMainFrame::OnToolbarButtonSetting)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_BUTTON_SETTING, &CMainFrame::OnUpdateToolbarButtonSetting)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame() noexcept
	: m_bOutput(false)
	, m_pView(nullptr)
	, m_pDoc(nullptr)
	, m_pFile(nullptr)
	, m_iWordCnt(2)
	, m_bLogin(false)
{
	// TODO: add member initialization code here
	::AfxGetModuleFileName(AfxGetApp()->m_hInstance, m_path);
	m_path.Format(m_path.Left(m_path.ReverseFind(L'\\')));
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC)) {
		TRACE(L"Failed to create tool bar\n");
		return -1;
	}

	CImageList imgList;
	CImage img;
	CBitmap bmp[TOOLBAR_BTN_CNT];
	CToolBarCtrl& toolbarCtrl = m_wndToolBar.GetToolBarCtrl();
	CRect rect;

	imgList.Create(32, 32, ILC_COLOR24 | ILC_MASK, 4, 4);

	LoadBmp(bmp[0], m_path + L"\\" + TOOLBAR_FOLDER_PATH);
	LoadBmp(bmp[1], m_path + L"\\" + TOOLBAR_FONT_PATH);
	LoadBmp(bmp[2], m_path + L"\\" + TOOLBAR_MONITOR_OFF_PATH);
	LoadBmp(bmp[3], m_path + L"\\" + TOOLBAR_MONITOR_ON_PATH);
	LoadBmp(bmp[4], m_path + L"\\" + TOOLBAR_SETTING_PATH);
	LoadBmp(bmp[5], m_path + L"\\" + TOOLBAR_INFO_PATH);

	for (int cnt = 0; cnt < TOOLBAR_BTN_CNT; cnt++) {
		imgList.Add(&bmp[cnt], TOOLBAR_MASK_COLOR);
	}

	toolbarCtrl.SetImageList(&imgList);

	for (int cnt = 0; cnt < TOOLBAR_BTN_CNT; cnt++) {
		bmp[cnt].Detach();
	}

	imgList.Detach();

	toolbarCtrl.SetButtonWidth(100, 150);

	m_wndToolBar.SetButtons(NULL, TOOLBAR_BTN_CNT - 1);
	m_wndToolBar.SetButtonInfo(0, ID_TOOLBAR_BUTTON_SAVE, TBSTYLE_BUTTON | TBSTATE_ENABLED, 0);
	m_wndToolBar.SetButtonText(0, L"저장");
	m_wndToolBar.SetButtonInfo(1, ID_TOOLBAR_BUTTON_FONT, TBSTYLE_BUTTON | TBSTATE_ENABLED, 1);
	m_wndToolBar.SetButtonText(1, L"글꼴");
	m_wndToolBar.SetButtonInfo(2, ID_TOOLBAR_BUTTON_MONITOR, TBSTYLE_BUTTON | TBSTATE_ENABLED, 2);
	m_wndToolBar.SetButtonText(2, L"출력창");
	m_wndToolBar.SetButtonInfo(3, ID_TOOLBAR_BUTTON_SETTING, TBSTYLE_BUTTON | TBSTATE_ENABLED, 4);
	m_wndToolBar.SetButtonText(3, L"설정");
	m_wndToolBar.SetButtonInfo(4, ID_TOOLBAR_BUTTON_INFO, TBSTYLE_BUTTON | TBSTATE_ENABLED, 5);
	m_wndToolBar.SetButtonText(4, L"정보");

	m_wndToolBar.GetItemRect(0, rect);
	m_wndToolBar.SetSizes(rect.Size() + CSize(0, 15), CSize(32, 32));

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	this->SetWindowTextW(L"Typing Viewer");

	CLoginDlg dlg;

	if (dlg.DoModal() == IDOK) {
		m_bLogin = true;
	}
	else {
		m_bLogin = false;
	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style &= ~FWS_ADDTOTITLE;

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::LFtoCF(LOGFONT& a_lf, CHARFORMAT2& a_cf) 
{
	a_cf.dwEffects = CFM_EFFECTS | CFE_AUTOBACKCOLOR;
	a_cf.dwEffects &= ~(CFE_PROTECTED | CFE_LINK | CFM_COLOR);
	a_cf.cbSize = sizeof(CHARFORMAT2);
	a_cf.yHeight = (abs)((ceil)(a_lf.lfHeight * 72 / (double)GetDeviceCaps(CClientDC(this), LOGPIXELSY))) * 20;
	a_cf.yOffset = 0;
	if (a_lf.lfWeight < FW_BOLD) a_cf.dwEffects &= ~CFE_BOLD;
	if (!a_lf.lfItalic) a_cf.dwEffects &= ~CFE_ITALIC;
	if (!a_lf.lfUnderline) a_cf.dwEffects &= ~CFE_UNDERLINE;
	if (!a_lf.lfStrikeOut) a_cf.dwEffects &= ~CFE_STRIKEOUT;
	a_cf.dwMask = CFM_ALL | CFM_BACKCOLOR;
	a_cf.bCharSet = a_lf.lfCharSet;
	a_cf.bPitchAndFamily = a_lf.lfPitchAndFamily;
	wcscpy_s(a_cf.szFaceName, a_lf.lfFaceName);
}

void CMainFrame::CFtoLF(LOGFONT& a_lf, CHARFORMAT2& a_cf)
{
	
}


void CMainFrame::OnToolbarButtonFont()
{
	// TODO: Add your command handler code here
	if (!m_pView) GetTypingViewerView();

	CHARFORMAT2 cf = m_pView->GetCF();
	CFontDialog dlg(cf);

	if (IDOK == dlg.DoModal()) {
		LOGFONT lf;
		CHARFORMAT2 cf;
		memset(&cf, 0, sizeof(cf));
		memcpy(&lf, dlg.m_cf.lpLogFont, sizeof(LOGFONT));

		// cf 변환
		LFtoCF(lf, cf);

		GetTypingViewerView();

		m_pView->SetNewFont(cf);
	}
}


void CMainFrame::OnUpdateToolbarButtonFont(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(true);
}


void CMainFrame::OnToolbarButtonInfo()
{
	// TODO: Add your command handler code here
	theApp.OnAppAbout();
}


void CMainFrame::OnUpdateToolbarButtonInfo(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(true);
}


void CMainFrame::OnToolbarButtonMonitor()
{
	// TODO: Add your command handler code here
	m_bOutput = !m_bOutput;

	ToggleOutput(m_bOutput);

	GetTypingViewerView();
	m_pView->ShowOutput(m_bOutput);
}


void CMainFrame::OnUpdateToolbarButtonMonitor(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(true);
}


void CMainFrame::OnToolbarButtonSave()
{
	// TODO: Add your command handler code here
	GetTypingViewerDoc();
	m_pDoc->OnFileSave();
	
}


void CMainFrame::OnUpdateToolbarButtonSave(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(true);
}


void CMainFrame::OnToolbarButtonSetting()
{
	// TODO: Add your command handler code here
	CSettingWordDlg dlg;

	dlg.SetWordCnt(m_iWordCnt);

	if (IDOK == dlg.DoModal()) {
		m_iWordCnt = dlg.GetWordCnt();
		::SendMessage(GetActiveView()->GetSafeHwnd(), UM_SET_WORD_CNT, m_iWordCnt, 0);
	}
}


void CMainFrame::OnUpdateToolbarButtonSetting(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(true);
}

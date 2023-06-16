// OutputDlg.cpp : implementation file
//

#include "pch.h"
#include "TypingViewer.h"
#include "afxdialogex.h"
#include "OutputDlg.h"
#include "ShellScalingApi.h"
#include "SettingOpacityDlg.h"
#include "SubtitleOptionDlg.h"
#include "cmath"

#pragma comment(lib, "Shcore.lib")
#pragma comment(lib, "D2D1.lib")
#pragma comment(lib, "DWRITE.lib")

// COutputDlg dialog

UINT ThreadFunc(LPVOID a_data)
{
	COutputDlg* p = (COutputDlg*)a_data;
	CString str = L"";

	while (WaitForSingleObject(p->m_event, 100) == WAIT_OBJECT_0) {
		if (p->m_q.empty()) continue;
		
		str.Empty();
		str = (p->m_q).front();
		(p->m_q).pop();

		p->m_pTarget->CreateSolidColorBrush(p->m_colorText, &p->m_pTextBr); 
		p->m_pTarget->CreateSolidColorBrush(p->m_colorBorder, &p->m_pBorderBr);

		if (p->m_pTextFormat && p->m_pTextBr && p->m_pBorderBr) {

			for (int i = 0; i < str.GetLength(); i++) {
				p->m_content = p->m_content + str.GetAt(i);
				p->m_uiContentLen = p->m_content.GetLength();

				p->m_pTarget->BeginDraw();

				p->m_pTarget->Clear(p->m_colorBg);
				if (p->m_bBorder) 
					p->m_pTarget->DrawRectangle(p->m_rectDraw, p->m_pBorderBr);
				p->m_pTarget->DrawText(
					p->m_content,
					p->m_uiContentLen,
					p->m_pTextFormat,
					p->m_rectDraw,
					p->m_pTextBr,
					D2D1_DRAW_TEXT_OPTIONS_CLIP);

				p->m_pTarget->EndDraw();
			}
		}
		
		p->IRelease(&p->m_pBorderBr);
		p->IRelease(&p->m_pTextBr);

		p->m_pBorderBr = p->m_pTextBr = nullptr;
	}

	return true;
}

IMPLEMENT_DYNAMIC(COutputDlg, CDialogEx)

COutputDlg::COutputDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_OUTPUT, pParent)
	, m_uiLineLen(2)
	, m_uiMaxLineLen(10)
	, m_bUp(true)
	, m_bBorder(false)
	, m_pOldBmp(nullptr)
	, m_uiFontHeight(0)
	, m_uiFontSize(20)
	, m_pOldFont(nullptr)
	, m_currFont()
	, m_event(nullptr)
{
	m_space.left = 10;
	m_space.right = 10;
	m_space.top = 10;
	m_space.bottom = 10;
}

COutputDlg::~COutputDlg()
{
	ResetEvent(m_event);
	IRelease(&m_pTarget);
	IRelease(&m_pWriteFactory);
	IRelease(&m_pFactory);
	IRelease(&m_pTextFormat);
	IRelease(&m_pTextLayout);
}

void COutputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COutputDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONUP()
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDOWN()
	ON_WM_GETMINMAXINFO()
	ON_MESSAGE(UM_CHANGE_ALPHA, &COutputDlg::OnUmChangeAlpha)
	ON_MESSAGE(UM_DRAW_BORDER, &COutputDlg::OnUmDrawBorder)
	ON_MESSAGE(UM_SET_SUBTITLE, &COutputDlg::OnUmSetSubtitle)
	ON_MESSAGE(UM_SET_SPACE, &COutputDlg::OnUmSetSpace)
	ON_MESSAGE(UM_SET_LINE, &COutputDlg::OnUmSetLine)
	ON_COMMAND(ID_POPUP_BACKGROUND_COLOR, &COutputDlg::OnPopupBackgroundColor)
	ON_COMMAND(ID_POPUP_FONT, &COutputDlg::OnPopupFont)
	ON_COMMAND(ID_POPUP_OPACITY, &COutputDlg::OnPopupOpacity)
	ON_COMMAND(ID_POPUP_SUBTITLE, &COutputDlg::OnPopupSubtitle)
END_MESSAGE_MAP()


// COutputDlg message handlers

void COutputDlg::RedrawAll()
{
	if (!m_pTarget) return;
	m_pTarget->BeginDraw();
	m_pTarget->Clear(m_colorBg);
	if (m_bBorder) {
		m_pTarget->CreateSolidColorBrush(m_colorBorder, &m_pBorderBr);
		m_pTarget->DrawRectangle(m_rectDraw, m_pBorderBr);
	}
	m_pTarget->CreateSolidColorBrush(m_colorText, &m_pTextBr);
	m_pTarget->DrawText(
		m_content,
		m_uiContentLen,
		m_pTextFormat,
		m_rectDraw,
		m_pTextBr,
		D2D1_DRAW_TEXT_OPTIONS_CLIP);
	m_pTarget->EndDraw();
}

BOOL COutputDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) {
			return true;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL COutputDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	// D2D 관련
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &m_pFactory);

	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_pWriteFactory),
		reinterpret_cast<IUnknown**>(&m_pWriteFactory));

	if (S_OK == m_pWriteFactory->CreateTextFormat(L"맑은 고딕", NULL,
		DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		FLOAT(m_uiFontSize), L"ko-kr", &m_pTextFormat)) {

		m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
	}

	if (m_pTextFormat && (S_OK == m_pWriteFactory->CreateTextLayout(
		L"aA가1 ", 5, m_pTextFormat, FLOAT(m_rect.Width()), FLOAT(m_rect.Height()),
		&m_pTextLayout
	))) {

		DWRITE_TEXT_METRICS dtm;
		m_pTextLayout->GetMetrics(&dtm);
		m_uiFontHeight = UINT(dtm.height);
		m_uiFontSize = UINT(m_pTextLayout->GetFontSize());

		wchar_t strFaceName[64];
		m_pTextLayout->GetFontFamilyName(strFaceName, 64);
		wcscpy_s(m_currFont.lfFaceName, strFaceName);
		m_currFont.lfHeight = -MulDiv(m_uiFontSize, GetDeviceCaps(CClientDC(this), LOGPIXELSY), 72);
		m_currFont.lfWeight = DWRITE_FONT_WEIGHT_NORMAL;
		m_currFont.lfItalic = false;
		m_currFont.lfUnderline = false;
	}

	SetBackgroundColor(RGB(0,0,0));

	m_event = CreateEvent(NULL, true, false, L"ThreadEvent");
	if (m_event) SetEvent(m_event);
	pThread = ::AfxBeginThread(ThreadFunc, (LPVOID)this);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void COutputDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	GetClientRect(m_rect);

	// 렌더 타겟 생성 및 저장
	ID2D1HwndRenderTarget* pHwndTarget = nullptr;
	double scale = GetMonitorScale();

	if (m_pFactory) {
		m_pFactory->CreateHwndRenderTarget(RenderTargetProperties(),
			HwndRenderTargetProperties(this->GetSafeHwnd(), SizeU(UINT32(m_rect.Width() * scale), UINT32(m_rect.Height() * scale))),
			&pHwndTarget);
		m_pTarget = pHwndTarget;
	}

	SetSubtitleSize();
	RedrawAll();
}


void COutputDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages
	SetSubtitleSize();
}


void COutputDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CRect r;
	GetClientRect(r);

	m_movableLeft.top = r.top + 10;
	m_movableLeft.bottom = r.bottom - 10;
	m_movableLeft.left = r.left;
	m_movableLeft.right = r.left + 10;

	m_movableRight.top = r.top + 10;
	m_movableRight.bottom = r.bottom - 10;
	m_movableRight.left = r.right - 10;
	m_movableRight.right = r.right;

	m_movableTop.top = r.top;
	m_movableTop.bottom = r.top + 10;
	m_movableTop.left = r.left + 10;
	m_movableTop.right = r.right - 10;

	m_movableBottom.top = r.bottom - 10;
	m_movableBottom.bottom = r.bottom;
	m_movableBottom.left = r.left + 10;
	m_movableBottom.right = r.right - 10;

	m_movableLT.top = r.top;
	m_movableLT.bottom = r.top + 10;
	m_movableLT.left = r.left;
	m_movableLT.right = r.left + 10;

	m_movableRT.top = r.top;
	m_movableRT.bottom = r.top + 10;
	m_movableRT.left = r.right - 10;
	m_movableRT.right = r.right;

	m_movableLB.top = r.bottom - 10;
	m_movableLB.bottom = r.bottom;
	m_movableLB.left = r.left;
	m_movableLB.right = r.left + 10;

	m_movableRB.top = r.bottom - 10;
	m_movableRB.bottom = r.bottom;
	m_movableRB.left = r.right - 10;
	m_movableRB.right = r.right;

	if (PtInRect(m_movableLeft, point)) {
		SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		if (nFlags & MK_LBUTTON) {
			::SendMessage(this->GetSafeHwnd(),WM_NCLBUTTONDOWN, HTLEFT, 0);
		}
	}
	else if (PtInRect(m_movableTop, point)) {
		SetCursor(::LoadCursor(NULL, IDC_SIZENS));
		if (nFlags & MK_LBUTTON) {
			::SendMessage(this->GetSafeHwnd(), WM_NCLBUTTONDOWN, HTTOP, 0);
		}
	}
	else if (PtInRect(m_movableRight, point)) {
		SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		if (nFlags & MK_LBUTTON) {
			::SendMessage(this->GetSafeHwnd(), WM_NCLBUTTONDOWN, HTRIGHT, 0);
		}
	}
	else if (PtInRect(m_movableBottom, point)) {
		SetCursor(::LoadCursor(NULL, IDC_SIZENS));
		if (nFlags & MK_LBUTTON) {
			::SendMessage(this->GetSafeHwnd(),WM_NCLBUTTONDOWN, HTBOTTOM, 0);
		}
	}
	else if (PtInRect(m_movableLT, point)) {
		SetCursor(::LoadCursor(NULL, IDC_SIZENWSE));
		if (nFlags & MK_LBUTTON) {
			::SendMessage(this->GetSafeHwnd(), WM_NCLBUTTONDOWN, HTTOPLEFT, 0);
		}
	}
	else if (PtInRect(m_movableRT, point)) {
		SetCursor(::LoadCursor(NULL, IDC_SIZENESW));
		if (nFlags & MK_LBUTTON) {
			::SendMessage(this->GetSafeHwnd(), WM_NCLBUTTONDOWN, HTTOPRIGHT, 0);
		}
	}
	else if (PtInRect(m_movableLB, point)) {
		SetCursor(::LoadCursor(NULL, IDC_SIZENESW));
		if (nFlags & MK_LBUTTON) {
			::SendMessage(this->GetSafeHwnd(), WM_NCLBUTTONDOWN, HTBOTTOMLEFT, 0);
		}
	}
	else if (PtInRect(m_movableRB, point)) {
		SetCursor(::LoadCursor(NULL, IDC_SIZENWSE));
		if (nFlags & MK_LBUTTON) {
			::SendMessage(this->GetSafeHwnd(), WM_NCLBUTTONDOWN, HTBOTTOMRIGHT, 0);
		}
	}
	else if (nFlags & MK_LBUTTON) {
		SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}


void COutputDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	ClientToScreen(&point);
	OnContextMenu(CWnd::FromHandle(m_hWnd), point);

	CDialogEx::OnRButtonUp(nFlags, point);
}


void COutputDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: Add your message handler code here
	CMenu popup;
	CMenu* pMenu;

	popup.LoadMenu(IDR_MENU_OUTPUT);
	pMenu = popup.GetSubMenu(0);

	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, pWnd);
}


void COutputDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (PtInRect(m_movableLeft, point)) {
		SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
	}
	else if (PtInRect(m_movableTop, point)) {
		SetCursor(::LoadCursor(NULL, IDC_SIZENS));
	}
	else if (PtInRect(m_movableRight, point)) {
		SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
	}
	else if (PtInRect(m_movableBottom, point)) {
		SetCursor(::LoadCursor(NULL, IDC_SIZENS));
	}
	else if (PtInRect(m_movableLT, point)) {
		SetCursor(::LoadCursor(NULL, IDC_SIZENWSE));
	}
	else if (PtInRect(m_movableRT, point)) {
		SetCursor(::LoadCursor(NULL, IDC_SIZENESW));
	}
	else if (PtInRect(m_movableLB, point)) {
		SetCursor(::LoadCursor(NULL, IDC_SIZENESW));
	}
	else if (PtInRect(m_movableRB, point)) {
		SetCursor(::LoadCursor(NULL, IDC_SIZENWSE));
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


void COutputDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default
	lpMMI->ptMinTrackSize.x = 600;
	lpMMI->ptMinTrackSize.y = 200;

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}


afx_msg LRESULT COutputDlg::OnUmChangeAlpha(WPARAM wParam, LPARAM lParam)
{
	LONG extendedStyle = ::GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE);
	::SetWindowLongW(GetSafeHwnd(), GWL_EXSTYLE, extendedStyle | WS_EX_LAYERED);
	::SetLayeredWindowAttributes(GetSafeHwnd(), 0, (BYTE)wParam, LWA_ALPHA);
	return 0;
}


afx_msg LRESULT COutputDlg::OnUmDrawBorder(WPARAM wParam, LPARAM lParam)
{
	m_bBorder = (bool)wParam;
	SetSubtitleSize();
	RedrawAll();
	return 0;
}


afx_msg LRESULT COutputDlg::OnUmSetSubtitle(WPARAM wParam, LPARAM lParam)
{
	m_bUp = (bool)wParam;
	SetSubtitleSize();
	RedrawAll();
	return 0;
}


afx_msg LRESULT COutputDlg::OnUmSetSpace(WPARAM wParam, LPARAM lParam)
{
	switch (lParam) {
	case 1: // left
		m_space.left = UINT(wParam);
		break;
	case 2: // top
		m_space.top = UINT(wParam);
		break;
	case 3: // right
		m_space.right = UINT(wParam);
		break;
	case 4: // bottom
		m_space.bottom = UINT(wParam);
		break;
	}

	SetSubtitleSize();
	RedrawAll();
	return 0;
}


afx_msg LRESULT COutputDlg::OnUmSetLine(WPARAM wParam, LPARAM lParam)
{
	m_uiLineLen = UINT(wParam);
	SetSubtitleSize();
	RedrawAll();
	return 0;
}


void COutputDlg::OnPopupBackgroundColor()
{
	// TODO: Add your command handler code here
	CColorDialog colorDlg;

	if (IDOK == colorDlg.DoModal()) {
		COLORREF color = colorDlg.GetColor();
		UINT uiColor = (color >> 16) | (color & 0x0000ff00) | (color & 0x000000ff) << 16;
		
		m_colorBg = ColorF(uiColor);
		RedrawAll();
		SetSubtitleSize();
	}
}


void COutputDlg::OnPopupFont()
{
	// TODO: Add your command handler code here
	CFontDialog* pFontDlg = nullptr;
	pFontDlg = new CFontDialog(&m_currFont);
	pFontDlg->m_cf.rgbColors = m_srcColorText;

	if (IDOK == pFontDlg->DoModal()) {
		memcpy(&m_currFont, pFontDlg->m_cf.lpLogFont, sizeof(LOGFONT));
		m_uiFontSize = UINT(ceil(m_currFont.lfHeight * 72 / (double)GetDeviceCaps(CClientDC(this), LOGPIXELSY)) * -1);
		m_srcColorText = pFontDlg->GetColor();
		m_colorText = ColorF(m_srcColorText);

		if (S_OK == m_pWriteFactory->CreateTextFormat(
			m_currFont.lfFaceName,
			NULL,
			m_currFont.lfWeight ? DWRITE_FONT_WEIGHT_BOLD : DWRITE_FONT_WEIGHT_NORMAL,
			m_currFont.lfItalic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			FLOAT(m_uiFontSize),
			L"ko-kr",
			&m_pTextFormat
		)) {
			m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
			m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);

			if (S_OK == m_pWriteFactory->CreateTextLayout(L"aA가1 ", 5, m_pTextFormat, FLOAT(m_rect.Width()), FLOAT(m_rect.Height()),
				&m_pTextLayout)) {
				DWRITE_TEXT_METRICS dtm;
				m_pTextLayout->GetMetrics(&dtm);
				m_uiFontHeight = UINT(dtm.height);
				m_uiFontSize = UINT(m_pTextLayout->GetFontSize());
			}

		}

		SetSubtitleSize();

		// 재출력 작업 해줘야 함
		RedrawAll();
	}

	delete pFontDlg;
}


void COutputDlg::OnPopupOpacity()
{
	// TODO: Add your command handler code here
	CSettingOpacityDlg dlg;
	dlg.SetOpacity(m_uiOpacity);

	if (dlg.DoModal() == IDOK) {
		m_uiOpacity = dlg.GetOpacity();
	}

	OnUmChangeAlpha(m_uiOpacity, 0);
}


void COutputDlg::OnPopupSubtitle()
{
	// TODO: Add your command handler code here
	CSubtitleOptionDlg subtitleDlg;
	bool bOldBorder = m_bBorder;
	bool bOldUp = m_bUp;
	SubtitleSpace oldSpace = m_space;
	UINT uiOldLineLen = m_uiLineLen;

	subtitleDlg.SetBorder(m_bBorder);
	subtitleDlg.SetSpace(m_space);
	subtitleDlg.SetLine(m_uiLineLen, m_uiMaxLineLen);
	subtitleDlg.SetPosition(m_bUp);

	if (subtitleDlg.DoModal() == IDOK) {

	}
	else {
		m_bBorder = bOldBorder;
		m_bUp = bOldUp;
		m_space = oldSpace;
		m_uiLineLen = uiOldLineLen;
	}

	SetSubtitleSize();
	RedrawAll();
}

double COutputDlg::GetMonitorScale()
{
	double scale = 0.0f;
	UINT uiDpiX = 0, uiDpiY = 0;
	HMONITOR hMonitor = ::MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST);
	::GetDpiForMonitor(hMonitor, MDT_EFFECTIVE_DPI, &uiDpiX, &uiDpiY);
	scale = uiDpiX / 96.0f;

	if (scale < 1.0) scale = 1.0f;

	return scale;
}

void COutputDlg::SetSubtitleSize()
{
	if (!m_pTarget) return;

	// 현재 drawtext 사이즈 구하기
	m_rectSubtitle.left = m_space.left;
	m_rectSubtitle.right = m_rect.Width() - m_space.right;

	if (m_bUp) {
		m_rectSubtitle.top = m_space.top;
		m_rectSubtitle.bottom = m_uiFontHeight * m_uiLineLen + m_space.top;
	}
	else {
		m_rectSubtitle.top = m_rect.Height() - (m_uiFontHeight * m_uiLineLen + m_space.bottom);
		m_rectSubtitle.bottom = m_rect.Height() - m_space.bottom;
	}

	m_rectDraw.left = FLOAT(m_rectSubtitle.left);
	m_rectDraw.top = FLOAT(m_rectSubtitle.top);
	m_rectDraw.right = FLOAT(m_rectSubtitle.right);
	m_rectDraw.bottom = FLOAT(m_rectSubtitle.bottom);
}

BOOL COutputDlg::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	ResetEvent(m_event);

	WY_IRelease(&m_pWriteFactory);
	WY_IRelease(&m_pFactory);

	return CDialogEx::DestroyWindow();
}

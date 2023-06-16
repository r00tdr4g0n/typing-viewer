#pragma once
#include "afxdialogex.h"
#include "queue"

using namespace D2D1;

// COutputDlg dialog

class COutputDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COutputDlg)

public:
	COutputDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~COutputDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_OUTPUT };
#endif

private:
	// D2D 관련
	ID2D1Factory* m_pFactory = nullptr;
	IDWriteFactory* m_pWriteFactory = nullptr;
	COLORREF m_srcColorText = RGB(255, 255, 255);

	// 사이즈 조절 관련
	CRect m_movableLeft;
	CRect m_movableRight;
	CRect m_movableTop;
	CRect m_movableBottom;
	CRect m_movableLT;
	CRect m_movableRT;
	CRect m_movableLB;
	CRect m_movableRB;

	CDC m_memDC;
	CBitmap m_bmp, * m_pOldBmp;
	UINT m_uiLineLen;
	UINT m_uiMaxLineLen;
	UINT m_uiFontSize = 0;
	UINT m_uiFontHeight;
	UINT m_uiOpacity = 255;
	SubtitleSpace m_space;
	CFont m_font, * m_pOldFont;
	LOGFONT m_currFont;

	CWinThread* pThread = nullptr;

public:
	bool m_bUp;
	bool m_bBorder;
	CRect m_rectSubtitle, m_rect;
	D2D1_COLOR_F m_colorBorder = ColorF(RGB(200, 200, 200));
	D2D1_COLOR_F m_colorText = ColorF(RGB(255, 255, 255));
	D2D1_COLOR_F m_colorBg = ColorF(RGB(0, 0, 0));
	D2D1_RECT_F m_rectDraw;
	UINT m_uiContentLen = 0;
	CString m_content = L"";
	ID2D1RenderTarget* m_pTarget = nullptr;
	IDWriteTextFormat* m_pTextFormat = nullptr;
	IDWriteTextLayout* m_pTextLayout = nullptr;
	ID2D1SolidColorBrush* m_pBorderBr = nullptr,* m_pTextBr = nullptr;
	HANDLE m_event;
	std::queue<CString> m_q;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	void SetSubtitleSize();
	void RedrawAll();
	double GetMonitorScale();
	void SetContent(CString a_content)
	{
		m_q.push(a_content);

		SetSubtitleSize();
	}

	template<class Interface>
	void IRelease(Interface** a_pInterfaceObject)
	{
		if (*a_pInterfaceObject != NULL) {
			(*a_pInterfaceObject)->Release();
			(*a_pInterfaceObject) = NULL;
		}
	}

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnPopupBackgroundColor();
	afx_msg void OnPopupFont();
	afx_msg void OnPopupOpacity();
	afx_msg void OnPopupSubtitle();

	afx_msg LRESULT OnUmChangeAlpha(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUmDrawBorder(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUmSetSubtitle(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUmSetSpace(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUmSetLine(WPARAM wParam, LPARAM lParam);
	virtual BOOL DestroyWindow();

	template<class Interface>
	void WY_IRelease(Interface** ap_interface_object)
	{
		if (*ap_interface_object != NULL) {
			(*ap_interface_object)->Release();
			(*ap_interface_object) = NULL;
		}
	}
};


// TypingViewerView.cpp : implementation of the CTypingViewerView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "TypingViewer.h"
#endif

#include "CntrItem.h"
#include "resource.h"
#include "TypingViewerView.h"
#include "TypingViewerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTypingViewerView

IMPLEMENT_DYNCREATE(CTypingViewerView, CRichEditView)

BEGIN_MESSAGE_MAP(CTypingViewerView, CRichEditView)
	ON_WM_DESTROY()
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CRichEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CRichEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CRichEditView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_MESSAGE(UM_SET_WORD_CNT, &CTypingViewerView::OnUmSetWordCnt)
	ON_CONTROL_REFLECT(EN_CHANGE, &CTypingViewerView::OnEnChange)
END_MESSAGE_MAP()

// CTypingViewerView construction/destruction

CTypingViewerView::CTypingViewerView() noexcept
	: m_pOutputDlg(nullptr)
{
	// TODO: add construction code here

}

CTypingViewerView::~CTypingViewerView()
{
}

BOOL CTypingViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CRichEditView::PreCreateWindow(cs);
}

void CTypingViewerView::OnInitialUpdate()
{
	CRichEditView::OnInitialUpdate();

	if (m_pOutputDlg == nullptr) {
		m_pOutputDlg = new COutputDlg();

		if (!m_pOutputDlg) return;

		if (!m_pOutputDlg->Create(IDD_DIALOG_OUTPUT)) {
			return;
		}

		CRect wRect, cRect;
		GetWindowRect(wRect);
		GetClientRect(cRect);

		m_pOutputDlg->ShowWindow(SW_HIDE);
		m_pOutputDlg->MoveWindow(wRect.left + cRect.Width() / 2 - 400, wRect.top + cRect.Height() / 2 - 250, 1000, 500);
	}

	// Set the printing margins (720 twips = 1/2 inch)
	SetMargins(CRect(720, 720, 720, 720));

	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));
	GetRichEditCtrl().GetDefaultCharFormat(m_cf);
	m_recordCf = m_cf;
	
	m_recordCf.dwEffects |= CFE_STRIKEOUT;
	m_recordCf.dwEffects &= ~CFE_AUTOCOLOR;
	m_recordCf.crTextColor = RGB(190, 190, 190);
	GetRichEditCtrl().LimitText(0);
	
}


// CTypingViewerView printing

BOOL CTypingViewerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}


void CTypingViewerView::OnDestroy()
{
	// Deactivate the item on destruction; this is important
	// when a splitter view is being used
   COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
   if (pActiveItem != nullptr && pActiveItem->GetActiveView() == this)
   {
      pActiveItem->Deactivate();
      ASSERT(GetDocument()->GetInPlaceActiveItem(this) == nullptr);
   }
   CRichEditView::OnDestroy();
}



// CTypingViewerView diagnostics

#ifdef _DEBUG
void CTypingViewerView::AssertValid() const
{
	CRichEditView::AssertValid();
}

void CTypingViewerView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}

CTypingViewerDoc* CTypingViewerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTypingViewerDoc)));
	return (CTypingViewerDoc*)m_pDocument;
}
#endif //_DEBUG


// CTypingViewerView message handlers

CString CTypingViewerView::GetContent()
{
	CString str;
	GetRichEditCtrl().GetWindowTextW(str);

	return str;
}

void CTypingViewerView::SetNewFont(CHARFORMAT2 cf)
{
	if (m_font.m_hObject) m_font.DeleteObject();
	m_cf = cf;
	GetRichEditCtrl().SetSel(0, 5);
	GetRichEditCtrl().SetSelectionCharFormat(cf);
	GetRichEditCtrl().HideSelection(true, false);
}

void CTypingViewerView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	

	CRichEditView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CTypingViewerView::SetRecord(bool a_bSpace)
{
	m_iRecordPrevOffset = m_iRecordOffset;

	if (a_bSpace) {
		m_iRecordOffset = GetContent().Find(L" ", m_iRecordPrevOffset) + 1;
	}
	else {
		m_iRecordOffset = GetContent().GetLength();
	}

	if (m_bEnter) {
		m_strAdd = 
			GetContent().Mid(
				m_iRecordPrevOffset, 
				(m_iRecordOffset - 2) - m_iRecordPrevOffset
			);
	}
	else {
		m_strAdd =
			GetContent().Mid(
				m_iRecordPrevOffset,
				m_iRecordOffset - m_iRecordPrevOffset
			);
	}

	GetRichEditCtrl().SetSel(0, m_iRecordOffset - m_iEnterCnt);
	GetRichEditCtrl().HideSelection(true, true);
	GetRichEditCtrl().SetSelectionCharFormat(m_recordCf);
	GetRichEditCtrl().SetSel(-1, -1);
	GetRichEditCtrl().SetSelectionCharFormat(m_cf);
}

BOOL CTypingViewerView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	long lStart = 0, lEnd = 0;
	
	if (pMsg->message == WM_KEYDOWN) {
		GetRichEditCtrl().GetSel(lStart, lEnd);

		if (pMsg->wParam == VK_F1) {
			m_bTyping = true;
			m_iCheck = 0;
			SetRecord();
			return true;
		}
		else if (pMsg->wParam == VK_LEFT) {
			if (lStart <= m_iRecordOffset - m_iEnterCnt) return true;
		}
		else if (pMsg->wParam == VK_BACK) {
			if (lEnd <= m_iRecordOffset - m_iEnterCnt) {
				return true;
			}
			else {
				if (GetContent().GetAt(lEnd + m_iEnterCnt - 1) == VK_SPACE) {
					m_iCheck--;
				}
			}
		}
		else if (pMsg->wParam == VK_DELETE) {
			if (lStart < m_iRecordOffset) {
				GetRichEditCtrl().SetSel(lEnd, lEnd);
				return true;
			}
		}
		else if (pMsg->wParam == VK_SPACE) {
			m_bSpace = true;
		}
		else if (pMsg->wParam == VK_RETURN) {
			m_bEnter = true;
		}
		else if (pMsg->wParam == VK_HOME || pMsg->wParam == VK_END) {
			return true;
		}
	}
	else if (pMsg->message == WM_LBUTTONUP) {
		GetRichEditCtrl().GetSel(lStart, lEnd);
		if (lStart < m_iRecordOffset) GetRichEditCtrl().SetSel(m_iRecordOffset, m_iRecordOffset);
	}

	return CRichEditView::PreTranslateMessage(pMsg);
}


afx_msg LRESULT CTypingViewerView::OnUmSetWordCnt(WPARAM wParam, LPARAM lParam)
{
	m_iWordCnt = (int)wParam;
	m_iCheck = 0;
	SetRecord();
	return 0;
}

void CTypingViewerView::OnEnChange()
{
	if (m_bEnter) {
		m_iEnterCnt++;
		m_iCheck = 0;
		m_bTyping = true;
		SetRecord();
	}
	else if (m_bSpace) {
		m_iCheck++;

		if (m_iCheck == m_iWordCnt) {
			m_iCheck--;
			m_bTyping = true;
			SetRecord(true);
		}

		m_bSpace = false;
	}

	if (m_bTyping) {
		m_pOutputDlg->SetContent(m_strEnter + m_strAdd);
		if (m_bEnter) m_strEnter = L"\r\n";
		else m_strEnter.Empty();
		m_bTyping = false;
		m_bEnter = false;
		m_strAdd.Empty();
	}
}

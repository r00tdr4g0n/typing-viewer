
// TypingViewerView.h : interface of the CTypingViewerView class
//

#pragma once
#include "OutputDlg.h"

class CTypingViewerCntrItem;

class CTypingViewerView : public CRichEditView
{
protected: // create from serialization only
	CTypingViewerView() noexcept;
	DECLARE_DYNCREATE(CTypingViewerView)

// Attributes
public:
	CTypingViewerDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CTypingViewerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

public:
	int			m_iEnterCnt = 0;
	int			m_iRecordOffset = 0;
	int			m_iRecordPrevOffset = 0;
	int			m_iLast = 0;
	int			m_iCheck = 0;
	int			m_iWordCnt = 2;
	bool		m_bTyping = false;
	bool		m_bEnter = false;
	bool		m_bSpace = false;
	CFont		m_font;
	CHARFORMAT2	m_cf;
	CHARFORMAT2 m_recordCf;
	COLORREF	m_color = RGB(0, 0, 0);
	COutputDlg* m_pOutputDlg;
	CString		m_strAdd = L"";
	CString		m_strEnter = L"";

	CString GetContent();
	CHARFORMAT2 GetCF() { return m_cf; }
	void SetNewFont(CHARFORMAT2 cf);
	void SetRecord(bool a_bSpace = false);
	void ShowOutput(bool a_bOutput)
	{
		if (a_bOutput) m_pOutputDlg->ShowWindow(SW_SHOW);
		else m_pOutputDlg->ShowWindow(SW_HIDE);
	}

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg LRESULT OnUmSetWordCnt(WPARAM wParam, LPARAM lParam);
protected:
public:
	
	afx_msg void OnEnChange();
};

#ifndef _DEBUG  // debug version in TypingViewerView.cpp
inline CTypingViewerDoc* CTypingViewerView::GetDocument() const
   { return reinterpret_cast<CTypingViewerDoc*>(m_pDocument); }
#endif



// MainFrm.h : interface of the CMainFrame class
//

#pragma once
//#include "TypingViewerDoc.h"
//#include "TypingViewerView.h" 

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
	CTypingViewerDoc*	m_pDoc;
	CTypingViewerView*	m_pView;
	CFile*				m_pFile;
	bool				m_bOutput;
	int					m_iWordCnt;
	bool				m_bLogin;
	CString				m_path;

public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	CFile* GetFile() { return m_pFile; }
	bool GetLoginState() { return m_bLogin; }
	void GetTypingViewerView()
	{
		m_pView = (CTypingViewerView*)GetActiveView();
	}
	void GetTypingViewerDoc()
	{
		m_pDoc = (CTypingViewerDoc*)GetActiveDocument();
	}
	void ToggleOutput(bool a_bOutput)
	{
		CRect rect;

		if (m_bOutput) m_wndToolBar.SetButtonInfo(2, ID_TOOLBAR_BUTTON_MONITOR, TBSTYLE_BUTTON | TBSTATE_ENABLED, 3);
		else m_wndToolBar.SetButtonInfo(2, ID_TOOLBAR_BUTTON_MONITOR, TBSTYLE_BUTTON | TBSTATE_ENABLED, 2);
		m_wndToolBar.GetItemRect(0, rect);
		m_wndToolBar.SetSizes(rect.Size() + CSize(0, 25), CSize(32, 38));
	}
	void LFtoCF(LOGFONT& a_lf, CHARFORMAT2& a_cf);
	void CFtoLF(LOGFONT& a_lf, CHARFORMAT2& a_cf);

	afx_msg void OnToolbarButtonFont();
	afx_msg void OnUpdateToolbarButtonFont(CCmdUI* pCmdUI);
	afx_msg void OnToolbarButtonInfo();
	afx_msg void OnUpdateToolbarButtonInfo(CCmdUI* pCmdUI);
	afx_msg void OnToolbarButtonMonitor();
	afx_msg void OnUpdateToolbarButtonMonitor(CCmdUI* pCmdUI);
	afx_msg void OnToolbarButtonSave();
	afx_msg void OnUpdateToolbarButtonSave(CCmdUI* pCmdUI);
	afx_msg void OnToolbarButtonSetting();
	afx_msg void OnUpdateToolbarButtonSetting(CCmdUI* pCmdUI);
};



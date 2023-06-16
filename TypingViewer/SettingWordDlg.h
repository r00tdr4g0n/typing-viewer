#pragma once
#include "afxdialogex.h"


// CSettingWordDlg dialog

class CSettingWordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingWordDlg)

public:
	CSettingWordDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CSettingWordDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_WORD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_editWord;
	CSpinButtonCtrl m_spinWord;

	void SetWordCnt(int a_cnt) { m_editWord = a_cnt; }
	int GetWordCnt() { return m_editWord; }
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposSpinWord(NMHDR* pNMHDR, LRESULT* pResult);
};

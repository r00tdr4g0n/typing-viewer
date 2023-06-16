#pragma once
#include "afxdialogex.h"


// CLoginDlg dialog

typedef struct LoginInfo {
	CString id;
	CString pw;
}LI;

class CLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CLoginDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LOGIN };
#endif

private:
	CString m_id;
	CString m_pw;
	CRect	m_rect;
	CImage	m_loginImg;
	CStatic m_img;
	LI		m_li;
	CDC		m_memdc;
	CBitmap m_bmp, * m_pOldBmp;
	CString m_path;
	CString m_info;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnSetfocusEditId();
	afx_msg void OnSetfocusEditPw();
	afx_msg void OnClickedButtonLogin();
};

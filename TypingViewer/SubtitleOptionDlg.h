#pragma once
#include "afxdialogex.h"


// CSubtitleOptionDlg dialog

class CSubtitleOptionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSubtitleOptionDlg)

public:
	CSubtitleOptionDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CSubtitleOptionDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SUBTITLE };
#endif
private:
	bool	m_bAuto = false;
	bool	m_bBorder = false;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	int m_editLeftSpace;
	int m_editRightSpace;
	int m_editUpSpace;
	int m_editDownSpace;
	int m_editSubtitleLineNum;
	CSpinButtonCtrl m_spinSubtitleLineNum;
	UINT m_radio;
	bool m_bUp;
	int m_iMaxLine;

	void SetBorder(bool a_bCheck) {
		m_bBorder = a_bCheck;
	}
	SubtitleSpace GetSpace() {
		SubtitleSpace space;
		space.bottom = m_editDownSpace;
		space.top = m_editUpSpace;
		space.left = m_editLeftSpace;
		space.right = m_editRightSpace;

		return space;
	}
	void SetSpace(SubtitleSpace a_space) {
		m_editDownSpace = a_space.bottom;
		m_editUpSpace = a_space.top;
		m_editLeftSpace = a_space.left;
		m_editRightSpace = a_space.right;
	}
	void SetLine(int a_line, int a_maxLine)
	{
		m_editSubtitleLineNum = a_line;
		m_iMaxLine = a_maxLine;
	}
	UINT GetLine()
	{
		return m_editSubtitleLineNum;
	}
	void SetPosition(bool a_bUp)
	{
		m_bUp = a_bUp;
	}
	bool GetPosition()
	{
		return m_bUp;
	}
	virtual BOOL OnInitDialog();
	afx_msg void OnClickedCheckBorder();
	afx_msg void OnChangeEditDownSpace();
	afx_msg void OnChangeEditLeftSpace();
	afx_msg void OnChangeEditRightSpace();
	afx_msg void OnChangeEditUpSpace();
	afx_msg void OnRadio(UINT uID);
	afx_msg void OnDeltaposSpinSubtitleLineNum(NMHDR* pNMHDR, LRESULT* pResult);
};

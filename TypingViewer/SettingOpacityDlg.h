#pragma once
#include "afxdialogex.h"


// CSettingOpacityDlg dialog

class CSettingOpacityDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingOpacityDlg)

public:
	CSettingOpacityDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CSettingOpacityDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SETTING_OPACITY };
#endif
private:
	int m_opacity;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_slider;

	void SetOpacity(int a_val) {
		m_opacity = 255 - a_val;
	}
	int GetOpacity() {
		return (255 - m_opacity);
	}

	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

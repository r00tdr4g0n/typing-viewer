// SettingOpacityDlg.cpp : implementation file
//

#include "pch.h"
#include "TypingViewer.h"
#include "afxdialogex.h"
#include "SettingOpacityDlg.h"


// CSettingOpacityDlg dialog

IMPLEMENT_DYNAMIC(CSettingOpacityDlg, CDialogEx)

CSettingOpacityDlg::CSettingOpacityDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SETTING_OPACITY, pParent)
{

}

CSettingOpacityDlg::~CSettingOpacityDlg()
{
}

void CSettingOpacityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_RANGE, m_slider);
}


BEGIN_MESSAGE_MAP(CSettingOpacityDlg, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CSettingOpacityDlg message handlers


BOOL CSettingOpacityDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_slider.SetRange(0, 255);
	m_slider.SetPos(m_opacity);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CSettingOpacityDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	m_opacity = m_slider.GetPos();
	::SendMessage(GetParent()->GetSafeHwnd(), UM_CHANGE_ALPHA, 255 - m_opacity, 0);


	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

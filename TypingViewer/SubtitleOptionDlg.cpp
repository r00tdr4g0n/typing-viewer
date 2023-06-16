// SubtitleOptionDlg.cpp : implementation file
//

#include "pch.h"
#include "TypingViewer.h"
#include "afxdialogex.h"
#include "SubtitleOptionDlg.h"


// CSubtitleOptionDlg dialog

IMPLEMENT_DYNAMIC(CSubtitleOptionDlg, CDialogEx)

CSubtitleOptionDlg::CSubtitleOptionDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SUBTITLE, pParent)
	, m_editSubtitleLineNum(0)
	, m_editRightSpace(0)
	, m_editUpSpace(0)
	, m_editLeftSpace(0)
	, m_editDownSpace(0)
	, m_iMaxLine(0)
	, m_radio(0)
	, m_bUp(true)
{

}

CSubtitleOptionDlg::~CSubtitleOptionDlg()
{
}

void CSubtitleOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LEFT_SPACE, m_editLeftSpace);
	DDX_Text(pDX, IDC_EDIT_RIGHT_SPACE, m_editRightSpace);
	DDX_Text(pDX, IDC_EDIT_UP_SPACE, m_editUpSpace);
	DDX_Text(pDX, IDC_EDIT_DOWN_SPACE, m_editDownSpace);
	DDX_Text(pDX, IDC_EDIT_SUBTITLE_LINE_NUM, m_editSubtitleLineNum);
	DDX_Control(pDX, IDC_SPIN_SUBTITLE_LINE_NUM, m_spinSubtitleLineNum);
	DDX_Radio(pDX, IDC_RADIO_STICKY_UP, (int&)m_radio);
}


BEGIN_MESSAGE_MAP(CSubtitleOptionDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_BORDER, &CSubtitleOptionDlg::OnClickedCheckBorder)
	ON_EN_CHANGE(IDC_EDIT_DOWN_SPACE, &CSubtitleOptionDlg::OnChangeEditDownSpace)
	ON_EN_CHANGE(IDC_EDIT_LEFT_SPACE, &CSubtitleOptionDlg::OnChangeEditLeftSpace)
	ON_EN_CHANGE(IDC_EDIT_RIGHT_SPACE, &CSubtitleOptionDlg::OnChangeEditRightSpace)
	ON_EN_CHANGE(IDC_EDIT_UP_SPACE, &CSubtitleOptionDlg::OnChangeEditUpSpace)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_STICKY_UP, IDC_RADIO_STICKY_DOWN, &CSubtitleOptionDlg::OnRadio)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SUBTITLE_LINE_NUM, &CSubtitleOptionDlg::OnDeltaposSpinSubtitleLineNum)
END_MESSAGE_MAP()


// CSubtitleOptionDlg message handlers


BOOL CSubtitleOptionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_spinSubtitleLineNum.SetRange(1, m_iMaxLine);
	m_spinSubtitleLineNum.SetPos(m_editSubtitleLineNum);
	((CButton*)GetDlgItem(IDC_CHECK_BORDER))->SetCheck(m_bBorder);

	if (m_bUp) {
		((CButton*)GetDlgItem(IDC_RADIO_STICKY_UP))->SetCheck(true);
		((CButton*)GetDlgItem(IDC_RADIO_STICKY_DOWN))->SetCheck(false);
		(CEdit*)GetDlgItem(IDC_EDIT_DOWN_SPACE)->EnableWindow(false);
	}
	else {
		((CButton*)GetDlgItem(IDC_RADIO_STICKY_UP))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO_STICKY_DOWN))->SetCheck(true);
		(CEdit*)GetDlgItem(IDC_EDIT_UP_SPACE)->EnableWindow(false);
	}


	return TRUE;
}


void CSubtitleOptionDlg::OnClickedCheckBorder()
{
	// TODO: Add your control notification handler code here
	m_bBorder = !m_bBorder;

	::SendMessage(this->GetParent()->GetSafeHwnd(), UM_DRAW_BORDER, m_bBorder, 0);
}


void CSubtitleOptionDlg::OnChangeEditDownSpace()
{
	UpdateData(true);
	::SendMessage(this->GetParent()->GetSafeHwnd(), UM_SET_SPACE, m_editDownSpace, 4);
}


void CSubtitleOptionDlg::OnChangeEditLeftSpace()
{
	UpdateData(true);
	::SendMessage(this->GetParent()->GetSafeHwnd(), UM_SET_SPACE, m_editLeftSpace, 1);
}


void CSubtitleOptionDlg::OnChangeEditRightSpace()
{
	UpdateData(true);
	::SendMessage(this->GetParent()->GetSafeHwnd(), UM_SET_SPACE, m_editRightSpace, 3);
}


void CSubtitleOptionDlg::OnChangeEditUpSpace()
{
	UpdateData(true);
	::SendMessage(this->GetParent()->GetSafeHwnd(), UM_SET_SPACE, m_editUpSpace, 2);
}

void CSubtitleOptionDlg::OnRadio(UINT uID)
{
	UpdateData(true);

	switch (m_radio) {
	case 0:
		m_bUp = true;
		(CEdit*)GetDlgItem(IDC_EDIT_DOWN_SPACE)->EnableWindow(false);
		(CEdit*)GetDlgItem(IDC_EDIT_UP_SPACE)->EnableWindow(true);
		break;

	case 1:
		m_bUp = false;
		(CEdit*)GetDlgItem(IDC_EDIT_DOWN_SPACE)->EnableWindow(true);
		(CEdit*)GetDlgItem(IDC_EDIT_UP_SPACE)->EnableWindow(false);
		break;
	}

	::SendMessage(this->GetParent()->GetSafeHwnd(), UM_SET_SUBTITLE, m_bUp, 0);

	UpdateData(false);
}

void CSubtitleOptionDlg::OnDeltaposSpinSubtitleLineNum(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here

	m_editSubtitleLineNum = pNMUpDown->iPos + pNMUpDown->iDelta;

	if (m_editSubtitleLineNum < 1) {
		m_editSubtitleLineNum++;
		return;
	}
	else if (m_editSubtitleLineNum > m_iMaxLine) {
		m_editSubtitleLineNum--;
		return;
	}

	::SendMessage(this->GetParent()->GetSafeHwnd(), UM_SET_LINE, m_editSubtitleLineNum, 0);


	*pResult = 0;
}

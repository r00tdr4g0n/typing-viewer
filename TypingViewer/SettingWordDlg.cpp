// SettingWordDlg.cpp : implementation file
//

#include "pch.h"
#include "TypingViewer.h"
#include "afxdialogex.h"
#include "SettingWordDlg.h"


// CSettingWordDlg dialog

IMPLEMENT_DYNAMIC(CSettingWordDlg, CDialogEx)

CSettingWordDlg::CSettingWordDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_WORD, pParent)
	, m_editWord(0)
{

}

CSettingWordDlg::~CSettingWordDlg()
{
}

void CSettingWordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_WORD, m_editWord);
	DDX_Control(pDX, IDC_SPIN_WORD, m_spinWord);
}


BEGIN_MESSAGE_MAP(CSettingWordDlg, CDialogEx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_WORD, &CSettingWordDlg::OnDeltaposSpinWord)
END_MESSAGE_MAP()


// CSettingWordDlg message handlers


void CSettingWordDlg::OnDeltaposSpinWord(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	m_editWord = pNMUpDown->iPos + pNMUpDown->iDelta;
	if (m_editWord > 10) m_editWord = 10;
	else if (m_editWord < 1) m_editWord = 1;

	*pResult = 0;
}


BOOL CSettingWordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_spinWord.SetRange(2, 10);
	m_spinWord.SetPos(m_editWord);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

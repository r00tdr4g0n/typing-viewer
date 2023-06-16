// CLoginDlg.cpp : implementation file
//

#include "pch.h"
#include "TypingViewer.h"
#include "afxdialogex.h"
#include "LoginDlg.h"


// CLoginDlg dialog

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_LOGIN, pParent)
	, m_id(_T(""))
	, m_pw(_T(""))
	, m_info(_T(""))
{
	m_li = { L"admin", L"admin" };
	::AfxGetModuleFileName(AfxGetApp()->m_hInstance, m_path);
	m_path.Format(m_path.Left(m_path.ReverseFind(L'\\')));
}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ID, m_id);
	DDX_Text(pDX, IDC_EDIT_PW, m_pw);
	DDX_Control(pDX, IDC_STATIC_IMG, m_img);
	DDX_Text(pDX, IDC_STATIC_INFO, m_info);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_EN_SETFOCUS(IDC_EDIT_ID, &CLoginDlg::OnSetfocusEditId)
	ON_EN_SETFOCUS(IDC_EDIT_PW, &CLoginDlg::OnSetfocusEditPw)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CLoginDlg::OnClickedButtonLogin)
END_MESSAGE_MAP()


// CLoginDlg message handlers
BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN) {
		switch (pMsg->wParam) {
		case VK_ESCAPE:
			return true;
		case VK_RETURN:
			OnClickedButtonLogin();
			return true;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	CClientDC dc(this);
	m_img.GetWindowRect(m_rect);
	ScreenToClient(m_rect);
	m_memdc.CreateCompatibleDC(&dc);
	m_bmp.CreateCompatibleBitmap(&dc, m_rect.Width(), m_rect.Height());
	m_pOldBmp = m_memdc.SelectObject(&m_bmp);
	m_memdc.SelectStockObject(WHITE_BRUSH);
	m_memdc.Rectangle(0, 0, m_rect.Width(), m_rect.Height());
	m_loginImg.Load(m_path + L"\\res\\login.png");

	if (!m_loginImg.IsNull())
		m_loginImg.Draw(m_memdc.m_hDC, 0, 0, m_rect.Width(), m_rect.Height());

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


HBRUSH CLoginDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_INFO) {
		pDC->SetTextColor(RGB(255, 0, 0));
	}
	else if (pWnd->GetDlgCtrlID() == IDC_STATIC_IMG) {
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


void CLoginDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialogEx::OnPaint() for painting messages

	dc.TransparentBlt(
		m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
		&m_memdc, 0, 0, m_rect.Width(), m_rect.Height(),
		RGB(255, 255, 255));
}


void CLoginDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	OnCancel();

	CDialogEx::OnClose();
}


void CLoginDlg::OnSetfocusEditId()
{
	// TODO: Add your control notification handler code here
}


void CLoginDlg::OnSetfocusEditPw()
{
	// TODO: Add your control notification handler code here
}

void CLoginDlg::OnClickedButtonLogin()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);

	if (m_id == m_li.id && m_pw == m_li.pw) {
		OnOK();
	}
	else {
		m_info.Format(L"아이디 또는 비밀번호가 틀렸습니다. 다시 입력하세요.");
		UpdateData(false);
	}
}

// TypingViewerDoc.cpp : implementation of the CTypingViewerDoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "TypingViewer.h"
#endif

#include "TypingViewerDoc.h"
#include "TypingViewerView.h"
#include "CntrItem.h"
#include "MainFrm.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTypingViewerDoc

IMPLEMENT_DYNCREATE(CTypingViewerDoc, CRichEditDoc)

BEGIN_MESSAGE_MAP(CTypingViewerDoc, CRichEditDoc)
	// Enable default OLE container implementation
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_LINKS, &CRichEditDoc::OnUpdateEditLinksMenu)
	ON_UPDATE_COMMAND_UI(ID_OLE_VERB_POPUP, &CTypingViewerDoc::OnUpdateObjectVerbPopup)
	ON_COMMAND(ID_OLE_EDIT_LINKS, &CRichEditDoc::OnEditLinks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OLE_VERB_FIRST, ID_OLE_VERB_LAST, &CRichEditDoc::OnUpdateObjectVerbMenu)
	ON_COMMAND(ID_FILE_SAVE, &CTypingViewerDoc::OnFileSave)
END_MESSAGE_MAP()


// CTypingViewerDoc construction/destruction

CTypingViewerDoc::CTypingViewerDoc() noexcept
{
	// Use OLE compound files
	EnableCompoundFile();

	// TODO: add one-time construction code here

}

CTypingViewerDoc::~CTypingViewerDoc()
{
}

BOOL CTypingViewerDoc::OnNewDocument()
{
	if (!CRichEditDoc::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

CRichEditCntrItem* CTypingViewerDoc::CreateClientItem(REOBJECT* preo) const
{
	return new CTypingViewerCntrItem(preo, const_cast<CTypingViewerDoc*>(this));
}




// CTypingViewerDoc serialization

void CTypingViewerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}

	// Calling the base class CRichEditDoc enables serialization
	//  of the container document's COleClientItem objects.
	// TODO: set CRichEditDoc::m_bRTF = FALSE if you are serializing as text
	CRichEditDoc::Serialize(ar);
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CTypingViewerDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CTypingViewerDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CTypingViewerDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CTypingViewerDoc diagnostics

#ifdef _DEBUG
void CTypingViewerDoc::AssertValid() const
{
	CRichEditDoc::AssertValid();
}

void CTypingViewerDoc::Dump(CDumpContext& dc) const
{
	CRichEditDoc::Dump(dc);
}
#endif //_DEBUG

// CTypingViewerDoc commands


BOOL CTypingViewerDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CRichEditDoc::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  Add your specialized creation code here

	return TRUE;
}


void CTypingViewerDoc::OnFileSave()
{
	// TODO: Add your command handler code here
	if (!m_pFile) {
		wchar_t* filter = L"text files (*.txt)|*.txt|";
		wchar_t* baseExt = L"txt";
		wchar_t* baseName = L"content";
		UINT flag = OFN_OVERWRITEPROMPT;

		CFileDialog dlg(
			false,
			baseExt,
			baseName,
			flag,
			filter
		);

		if (IDOK == dlg.DoModal()) {
			m_name = dlg.GetPathName();
			m_pFile = new CFile(m_name, CFile::modeCreate | CFile::modeReadWrite);
			SetPathName(m_name, true);
		}
		else {
			return;
		}
	}

	UINT bom = 0xfeff;

	CTypingViewerView* pView = (CTypingViewerView*)(((CMainFrame*)AfxGetMainWnd())->GetActiveView());
	CString content = pView->GetContent();
	
	if (m_pFile->m_hFile == CFile::hFileNull) m_pFile->Open(m_name, CFile::modeReadWrite);
	m_pFile->SeekToBegin();
	m_pFile->Write(&bom, 2);
	m_pFile->Write(content.GetBuffer(), content.GetLength() * 2);
	m_pFile->Close();
}


// CntrItem.cpp : implementation of the CTypingViewerCntrItem class
//

#include "pch.h"
#include "framework.h"
#include "TypingViewer.h"

#include "TypingViewerDoc.h"
#include "TypingViewerView.h"
#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTypingViewerCntrItem implementation

IMPLEMENT_SERIAL(CTypingViewerCntrItem, CRichEditCntrItem, 0)

CTypingViewerCntrItem::CTypingViewerCntrItem(REOBJECT* preo, CTypingViewerDoc* pContainer)
	: CRichEditCntrItem(preo, pContainer)
{
	// TODO: add one-time construction code here
}

CTypingViewerCntrItem::~CTypingViewerCntrItem()
{
	// TODO: add cleanup code here
}


// CTypingViewerCntrItem diagnostics

#ifdef _DEBUG
void CTypingViewerCntrItem::AssertValid() const
{
	CRichEditCntrItem::AssertValid();
}

void CTypingViewerCntrItem::Dump(CDumpContext& dc) const
{
	CRichEditCntrItem::Dump(dc);
}
#endif


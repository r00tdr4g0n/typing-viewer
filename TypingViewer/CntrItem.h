
// CntrItem.h : interface of the CTypingViewerCntrItem class
//

#pragma once

class CTypingViewerDoc;
class CTypingViewerView;

class CTypingViewerCntrItem : public CRichEditCntrItem
{
	DECLARE_SERIAL(CTypingViewerCntrItem)

// Constructors
public:
	CTypingViewerCntrItem(REOBJECT* preo = nullptr, CTypingViewerDoc* pContainer = nullptr);
		// Note: pContainer is allowed to be null to enable IMPLEMENT_SERIALIZE
		//  IMPLEMENT_SERIALIZE requires the class have a constructor with
		//  zero arguments.  Normally, OLE items are constructed with a
		//  non-null document pointer

// Attributes
public:
	CTypingViewerDoc* GetDocument()
		{ return reinterpret_cast<CTypingViewerDoc*>(CRichEditCntrItem::GetDocument()); }
	CTypingViewerView* GetActiveView()
		{ return reinterpret_cast<CTypingViewerView*>(CRichEditCntrItem::GetActiveView()); }

// Implementation
public:
	~CTypingViewerCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};


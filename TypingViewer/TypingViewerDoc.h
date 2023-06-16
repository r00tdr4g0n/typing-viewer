
// TypingViewerDoc.h : interface of the CTypingViewerDoc class
//


#pragma once


class CTypingViewerDoc : public CRichEditDoc
{
protected: // create from serialization only
	CTypingViewerDoc() noexcept;
	DECLARE_DYNCREATE(CTypingViewerDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual CRichEditCntrItem* CreateClientItem(REOBJECT* preo) const;
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CTypingViewerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
private:
	CFile* m_pFile = NULL;
	CString m_name;

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	afx_msg void OnFileSave();
};

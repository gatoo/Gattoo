
// GattooDoc.h : interface of the CGattooDoc class
//
#pragma once

#include "core/GattooImg.h"

class CGattooDoc : public CDocument
{
protected: // create from serialization only
	
	CGattooDoc();
	DECLARE_DYNCREATE(CGattooDoc)

// Overrides
public:

	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);

	CSize getImgSize() const;
	void const * getImgData() const;
	
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CGattooDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	
	CGattooImg m_Img;

	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

public:
//	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	afx_msg void OnFileSave();
};

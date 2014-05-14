
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
	std::string const getImgDimension() const;
	std::string const getImgPrintDimension() const;
	std::string const getImgModification() const;
	std::string const getImgDepth() const;
	void const * getImgData() const;

	void PerformDrawing(CDC* pDC);
	
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
	
	CGattooImg m_ImgOrig;
	CGattooImg m_ImgForPrint;

	CGattooImg* m_pCurrImg;

	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

public:
//	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	afx_msg void OnFileSave();
	afx_msg void OnToolsHalftone();
	afx_msg void OnToolsSaveToSD();

	CGattooImg::EImageState GetDocumentState();
	
	void SwitchToOriginal(BOOL bOrig);
	void doInverse();
	bool EraseRect(CRect &rcErase);
	void CropImage(CRect &rc);
};

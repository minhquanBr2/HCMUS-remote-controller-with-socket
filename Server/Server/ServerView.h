
// ServerView.h : interface of the CServerView class
//

#pragma once

#include "ServerDoc.h"


class CServerView : public CView
{
protected: // create from serialization only
	CServerView() noexcept;
	DECLARE_DYNCREATE(CServerView)
	CStringArray m_MsgArray;

// Attributes
public:
	CServerDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CServerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	void AddMsg(CString strMessage);
};

#ifndef _DEBUG  // debug version in ServerView.cpp
inline CServerDoc* CServerView::GetDocument() const
   { return reinterpret_cast<CServerDoc*>(m_pDocument); }
#endif


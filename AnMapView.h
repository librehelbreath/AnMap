// AnMapView.h : interface of the CAnMapView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANMAPVIEW_H__994670CC_691F_42CD_BB74_C8FE6ADE6A06__INCLUDED_)
#define AFX_ANMAPVIEW_H__994670CC_691F_42CD_BB74_C8FE6ADE6A06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAnMapView : public CScrollView
{
protected: // create from serialization only
	CAnMapView();
	DECLARE_DYNCREATE(CAnMapView)

// Attributes
public:
// Operations
public:
	void OnProperties();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnMapView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	CAnMapDoc* GetDocument() { return (CAnMapDoc*)m_pDocument; }
	CAnMapDoc* m_pDoc;
	CDC BufferDC;
	RECT m_rtClient;
	POINT m_ptClipping;
	POINT m_ptScroll;
	UINT m_nAutoSaveTimer;
	UINT m_nCursorTimer;
	BOOL m_bCursor;
	void SetStatusText( int iPane, CString str);
	void DrawLimitLine();
	void RenderBrush();
	void RenderTile();
	void UpdateScreen();
	virtual ~CAnMapView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CAnMapView)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANMAPVIEW_H__994670CC_691F_42CD_BB74_C8FE6ADE6A06__INCLUDED_)

#if !defined(AFX_MINIVIEW_H__BF768FC1_E0D9_4558_A31E_95EEEAFC0219__INCLUDED_)
#define AFX_MINIVIEW_H__BF768FC1_E0D9_4558_A31E_95EEEAFC0219__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MiniView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMiniView view

class CMiniView : public CView
{
protected:
	CMiniView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMiniView)

// Attributes
public:
	CAnMapDoc* GetDocument() { return (CAnMapDoc*)m_pDocument; }
	CAnMapDoc* m_pDoc;
	CDC BufferDC;
	CDC OffDC;
	CDC BackDC;
	short m_sTmp;
	RECT m_rtClient;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMiniView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
public:
	void UpdateScreen();
	void LoadMinimap();
	void DrawMinimap(short sx, short sy);
	virtual ~CMiniView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CMiniView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MINIVIEW_H__BF768FC1_E0D9_4558_A31E_95EEEAFC0219__INCLUDED_)

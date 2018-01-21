#if !defined(AFX_PROGRESSBAR_H__165F8FED_7668_4BFE_BD75_7C28CCF8AFD5__INCLUDED_)
#define AFX_PROGRESSBAR_H__165F8FED_7668_4BFE_BD75_7C28CCF8AFD5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProgressBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProgressBar window

class CProgressBar : public CProgressCtrl
{
// Construction
public:
	CProgressBar();

// Attributes
public:

// Operations
public:
	CStatusBar* GetStatusBar();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgressBar)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CProgressBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CProgressBar)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESSBAR_H__165F8FED_7668_4BFE_BD75_7C28CCF8AFD5__INCLUDED_)

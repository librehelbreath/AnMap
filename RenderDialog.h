#if !defined(AFX_RENDERDIALOG_H__0AD738B2_9842_40A0_9B8B_3F1774F8E4B1__INCLUDED_)
#define AFX_RENDERDIALOG_H__0AD738B2_9842_40A0_9B8B_3F1774F8E4B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RenderDialog.h : header file
//

#include "AnMapDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CRenderDialog dialog

class CRenderDialog : public CDialog
{
// Construction
public:
	CRenderDialog(CAnMapDoc* pDoc, CWnd* pParent = NULL);   // standard constructor
	CAnMapDoc* m_pDoc;
	POINTS m_scrollpt;
	CDC m_bufferDC;
	CDC *m_pOutputDC;
	CBitmap bmpBuffer;
	void MakeImage(short sx, short sy);
	void DrawProgress();
	int m_iProgressX;
	int m_iProgressY;
	int m_iRatio;
	int m_iOutSizeX;
	int m_iOutSizeY;
	int m_iBufferSize;
	POINTS m_lbpt;
	POINTS m_lbscroll;

// Dialog Data
	//{{AFX_DATA(CRenderDialog)
	enum { IDD = IDD_DIALOG_RENDER };
	CProgressCtrl	m_progress;
	int		m_iOutputSize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRenderDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRenderDialog)
	afx_msg void OnPaint();
	afx_msg void OnButtonRender();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnButtonSave();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnButtonPrint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RENDERDIALOG_H__0AD738B2_9842_40A0_9B8B_3F1774F8E4B1__INCLUDED_)

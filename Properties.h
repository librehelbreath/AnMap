#if !defined(AFX_PROPERTIES_H__9CFC5906_473B_445C_9298_B12B8D73220F__INCLUDED_)
#define AFX_PROPERTIES_H__9CFC5906_473B_445C_9298_B12B8D73220F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Properties.h : header file
//

#include "AnMapDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CProperties dialog

class CProperties : public CDialog
{
// Construction
public:
	CProperties(CAnMapDoc* pDoc, CWnd* pParent = NULL);   // standard constructor
	int count;
	CAnMapDoc* m_pDoc;
	BOOL m_bIsOneTile;
	BOOL m_bSameMove;
	BOOL m_bSameTele;
	BOOL m_bSameFarm;
	POINTS m_pt1;
	POINTS m_pt2;

// Dialog Data
	//{{AFX_DATA(CProperties)
	enum { IDD = IDD_DIALOG_PROPERTIES };
	int		m_iMoveable;
	int		m_iTeleport;
	short	m_sObjSpr;
	short	m_sObjFrm;
	short	m_sSpr;
	short	m_sSprFrm;
	CString	m_strXY;
	int		m_iFarm;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProperties)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProperties)
	afx_msg void OnCheckMove();
	afx_msg void OnCheckTeleport();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckFarm();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTIES_H__9CFC5906_473B_445C_9298_B12B8D73220F__INCLUDED_)

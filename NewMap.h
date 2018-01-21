#if !defined(AFX_NEWMAP_H__88F0E13E_D0C5_48EF_BEEC_8DBE9E45593B__INCLUDED_)
#define AFX_NEWMAP_H__88F0E13E_D0C5_48EF_BEEC_8DBE9E45593B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewMap.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewMap dialog

class CNewMap : public CDialog
{
// Construction
public:
	CNewMap(short sizeX, short sizeY, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewMap)
	enum { IDD = IDD_DIALOG_NEWMAP };
	short	m_sMapSizeX;
	short	m_sMapSizeY;
	int		m_iTerrain;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewMap)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewMap)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWMAP_H__88F0E13E_D0C5_48EF_BEEC_8DBE9E45593B__INCLUDED_)

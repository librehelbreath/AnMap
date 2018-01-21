#if !defined(AFX_OPTIONDIALOG_H__1EA22E0E_8D13_44B4_B041_AF3103503B16__INCLUDED_)
#define AFX_OPTIONDIALOG_H__1EA22E0E_8D13_44B4_B041_AF3103503B16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionDialog.h : header file
//

#include "AnMapDoc.h"

/////////////////////////////////////////////////////////////////////////////
// COptionDialog dialog

class COptionDialog : public CDialog
{
// Construction
public:
	COptionDialog(CAnMapDoc* pDoc, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptionDialog)
	enum { IDD = IDD_DIALOG_OPTION };
	DWORD	m_dwAutoSaveTime;
	BOOL	m_bAutoSave;
	BOOL	m_bCursor;
	DWORD	m_dwCursor;
	DWORD	m_dwScrollDelay;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptionDialog)
	afx_msg void OnCheckAutosave();
	afx_msg void OnCheckCursorTwinkle();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONDIALOG_H__1EA22E0E_8D13_44B4_B041_AF3103503B16__INCLUDED_)

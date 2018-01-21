// AnMap.h : main header file for the ANMAP application
//

#if !defined(AFX_ANMAP_H__122ABE2E_F02C_4ED6_BDA0_BACB3DAB7568__INCLUDED_)
#define AFX_ANMAP_H__122ABE2E_F02C_4ED6_BDA0_BACB3DAB7568__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "Splash.h"

/////////////////////////////////////////////////////////////////////////////
// CAnMapApp:
// See AnMap.cpp for the implementation of this class
//

class CAnMapApp : public CWinApp
{
public:
	CAnMapApp();
	char    m_strWorkingFolder[256];
	CString m_strLastFile;
	BOOL	m_bAutoSave;
	DWORD	m_dwAutoSaveDelay;
	DWORD	m_dwCursorDelay;
	DWORD	m_dwScrollDelay;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnMapApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CAnMapApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANMAP_H__122ABE2E_F02C_4ED6_BDA0_BACB3DAB7568__INCLUDED_)

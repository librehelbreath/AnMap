// ProgressBar.cpp : implementation file
//

#include "stdafx.h"
#include "AnMap.h"
#include "ProgressBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgressBar

CProgressBar::CProgressBar()
{
}

CProgressBar::~CProgressBar()
{
}


BEGIN_MESSAGE_MAP(CProgressBar, CProgressCtrl)
	//{{AFX_MSG_MAP(CProgressBar)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgressBar message handlers

BOOL CProgressBar::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// 상태바를 얻음
	CStatusBar *pStatusBar = GetStatusBar();
	if( !pStatusBar ) return FALSE;

	// 상태바 위에 프로그레스 컨트롤 생성
	if( !CProgressCtrl::Create(WS_CHILD | WS_VISIBLE, CRect(0,0,0,0), pStatusBar, 1)) return FALSE;

	// 프로그레스 컨트롤의 범위와 스텝 설정
	SetRange(0, 100);
	SetStep(1);
	
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

CStatusBar* CProgressBar::GetStatusBar()
{
	CFrameWnd *pFrame = (CFrameWnd*)AfxGetMainWnd();
	if(!pFrame || !pFrame->IsKindOf(RUNTIME_CLASS(CFrameWnd))) return NULL;
	CStatusBar *pBar = (CStatusBar*) pFrame->GetMessageBar();
	if( !pBar || !pBar->IsKindOf(RUNTIME_CLASS(CStatusBar))) return NULL;
	return pBar;
}

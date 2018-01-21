// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "AnMap.h"

#include "MainFrm.h"
#include "BrushView.h"
#include "AnMapView.h"
#include "MiniView.h"

#include "AnMapDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetPaneInfo( 0, ID_SEPARATOR, SBPS_NORMAL, 200 );
	m_wndStatusBar.SetPaneInfo( 1, ID_SEPARATOR, SBPS_NORMAL, 120 );
	m_wndStatusBar.SetPaneInfo( 2, ID_SEPARATOR, SBPS_NORMAL, 100 );
	m_wndStatusBar.SetPaneInfo( 3, ID_SEPARATOR, SBPS_NORMAL, 110 );
	m_wndStatusBar.SetPaneInfo( 4, ID_SEPARATOR, SBPS_NORMAL, 70 );
	m_wndStatusBar.SetPaneInfo( 5, ID_SEPARATOR, SBPS_STRETCH, 150 );

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	// create splitter window
	BeginWaitCursor();
	if (!m_wndSplitter.CreateStatic(this, 1, 2)) return FALSE;
	m_wndSplitter.SetColumnInfo( 0, 200, 120 );
	if (!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CAnMapView), CSize(640, 480), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}
	if (!m_wndSplitter2.CreateStatic(&m_wndSplitter, 2, 1)) return FALSE;
	if (!m_wndSplitter2.CreateView(0, 0, RUNTIME_CLASS(CMiniView), CSize(200, 200), pContext) ||
		!m_wndSplitter2.CreateView(1, 0, RUNTIME_CLASS(CBrushView), CSize(200, 200), pContext) )
	{
		m_wndSplitter2.DestroyWindow();
		return FALSE;
	}
	EndWaitCursor();
	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		| WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

CAnMapView* CMainFrame::GetRightPane()
{
	CWnd* pWnd = m_wndSplitter.GetPane(0, 1);
	CAnMapView* pView = DYNAMIC_DOWNCAST(CAnMapView, pWnd);
	return pView;
}

CMiniView* CMainFrame::GetMiniPane()
{
	CWnd* pWnd = m_wndSplitter2.GetPane(0, 0);
	CMiniView* pView = DYNAMIC_DOWNCAST(CMiniView, pWnd);
	return pView;
}

void CMainFrame::OnClose() 
{
	CAnMapDoc* pDoc = (CAnMapDoc*)GetActiveDocument();
	if( pDoc->m_bModified == TRUE )
	{
		int res = MessageBox( "Map has been modified, would you like to save it?", "Notice!", MB_YESNOCANCEL | MB_ICONQUESTION );
		if( res == IDCANCEL ) return;
		else if( res == IDYES ) pDoc->SaveFileAs();
	}
	CFrameWnd::OnClose();
}

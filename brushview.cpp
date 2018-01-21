// BrushView.cpp : implementation of the CBrushView class
//

#include "stdafx.h"
#include "AnMap.h"

#include "AnMapDoc.h"
#include "BrushView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBrushView

IMPLEMENT_DYNCREATE(CBrushView, CTreeView)

BEGIN_MESSAGE_MAP(CBrushView, CTreeView)
	//{{AFX_MSG_MAP(CBrushView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBrushView construction/destruction

CBrushView::CBrushView()
{
	m_pDoc = NULL;
}

CBrushView::~CBrushView()
{
}

BOOL CBrushView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style = WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_BORDER | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES | TVS_SHOWSELALWAYS;
	return CTreeView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CBrushView drawing

void CBrushView::OnDraw(CDC* pDC)
{
}

void CBrushView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();
	m_pDoc = GetDocument();
	if( m_pDoc == NULL ) return;
	CTreeCtrl& pTree = GetTreeCtrl();
	pTree.DeleteAllItems();
	TV_INSERTSTRUCT tvinsert;
	tvinsert.hParent = NULL;
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.mask = TVIF_TEXT;
	tvinsert.item.state = 0;
	tvinsert.item.stateMask = 0;
	tvinsert.item.cchTextMax = 60;
	tvinsert.item.iSelectedImage = 1;
	tvinsert.item.cChildren = 0;
	char cFolderName[256];
	char cFileTitle[12];
	ZeroMemory( cFolderName, sizeof(cFolderName) );
	strcpy( cFolderName, "Brushes" );
	tvinsert.item.pszText = cFolderName;
	tvinsert.item.iImage = 0;
	HTREEITEM hBrush = pTree.InsertItem(&tvinsert);
	pTree.SetItemData( hBrush, -1 );
	for( int i=0; i<DEF_MAXSPRITES ; i++ )
	{
		if( i>= 100 && i<300 ) continue;
		if( m_pDoc->m_pSprite[i] == NULL ) continue;
		tvinsert.hParent = hBrush;
		ZeroMemory( cFileTitle, sizeof(cFileTitle) );
		wsprintf( cFileTitle, "Brush(%d)", i );
		tvinsert.item.pszText = cFileTitle;
		HTREEITEM hItem = pTree.InsertItem(&tvinsert);
		pTree.SetItemData( hItem, i );
		pTree.SetCheck( hItem );
	}
	pTree.Expand( hBrush, TVE_EXPAND );
	m_pDoc->SetBrushView( this );
}

/////////////////////////////////////////////////////////////////////////////
// CBrushView diagnostics

#ifdef _DEBUG
void CBrushView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CBrushView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBrushView message handlers

void CBrushView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	CTreeCtrl& pTree = GetTreeCtrl();
	HTREEITEM hItem = pTree.GetSelectedItem();
	//int a = pTree.GetItemState( hItem, TVIS_SELECTED );
	int iBrushIndex = pTree.GetItemData(hItem);
	if( iBrushIndex < 0 ) return;
	m_pDoc->OnBrushMode(iBrushIndex);
	*pResult = 0;
}

void CBrushView::SetSelectedItem( int iBrushIndex )
{
	CTreeCtrl& pTree = GetTreeCtrl();
	HTREEITEM hRoot = pTree.GetRootItem();
	HTREEITEM hItem = pTree.GetChildItem(hRoot);
	if( hItem == NULL ) return;
	int iData = (int)(pTree.GetItemData( hItem ));
	if( iData == iBrushIndex ) {
		pTree.SelectItem(hItem);
		return;
	}
	while(1)
	{
		hItem = pTree.GetNextItem( hItem, TVGN_NEXT );
		if( hItem == NULL ) return;
		iData = pTree.GetItemData( hItem );
		if( iData == iBrushIndex ) 
		{
			pTree.SelectItem( hItem );
			return;
		}
	}
}

void CBrushView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	m_pDoc->OnKeyDown(nChar);
	//CTreeView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CBrushView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	m_pDoc->OnKeyUp(nChar);	
	//CTreeView::OnKeyUp(nChar, nRepCnt, nFlags);
}

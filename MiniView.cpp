// MiniView.cpp : implementation file
//

#include "stdafx.h"
#include "AnMap.h"

#include "AnMapDoc.h"
#include "MiniView.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMiniView

IMPLEMENT_DYNCREATE(CMiniView, CView)

CMiniView::CMiniView()
{
	m_sTmp = 8;
	m_pDoc = NULL;
}

CMiniView::~CMiniView()
{
}


BEGIN_MESSAGE_MAP(CMiniView, CView)
	//{{AFX_MSG_MAP(CMiniView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMiniView drawing

void CMiniView::OnDraw(CDC* pDC)
{
	if( m_pDoc != NULL )
	{
		pDC->BitBlt( 0, 0, m_rtClient.right, m_rtClient.bottom, &BackDC, 0, 0, SRCCOPY );
		m_pDoc->m_bReadyMiniView = TRUE;
		m_pDoc->ThreadJob();
	}
}

void CMiniView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	m_pDoc = GetDocument();
	ASSERT_VALID(m_pDoc);
	if( BufferDC.m_hDC == NULL ) {
		CDC *pDC = GetDC();
		BufferDC.CreateCompatibleDC(pDC);
		OffDC.CreateCompatibleDC(pDC);
		BackDC.CreateCompatibleDC(pDC);
		CBitmap bmpBuffer, bmpOff, bmpBack;
		bmpBuffer.CreateCompatibleBitmap(pDC, 800, 800);
		BufferDC.SelectObject(&bmpBuffer);
		bmpBuffer.DeleteObject();
		bmpOff.CreateCompatibleBitmap(pDC, DEF_MINIMAPSIZE, DEF_MINIMAPSIZE);
		OffDC.SelectObject(&bmpOff);
		bmpOff.DeleteObject();
		bmpBack.CreateCompatibleBitmap(pDC, DEF_MINIMAPSIZE, DEF_MINIMAPSIZE);
		BackDC.SelectObject(&bmpBack);
		bmpBack.DeleteObject();
		ReleaseDC(pDC);
		m_pDoc->SetMiniView( this );
		LoadMinimap();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMiniView diagnostics

#ifdef _DEBUG
void CMiniView::AssertValid() const
{
	CView::AssertValid();
}

void CMiniView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMiniView message handlers

void CMiniView::LoadMinimap()
{
	int ix, iy;
	short sSpr, sSprFrame, sRatio, sLargeMapSize;
	sLargeMapSize = m_pDoc->m_sMapDataSizeY;
	if( sLargeMapSize < m_pDoc->m_sMapDataSizeX ) sLargeMapSize = m_pDoc->m_sMapDataSizeX;

	if( sLargeMapSize > 400 ) m_sTmp = 1;
	else if( sLargeMapSize > 200 ) m_sTmp = 2;
	else if( sLargeMapSize > 100 ) m_sTmp = 4;
	else if( sLargeMapSize > 50 ) m_sTmp = 8;
	else m_sTmp = 16;

	if( m_sTmp < 2 ) sRatio = 2;
	else sRatio = 1;

	for( iy = 0 ; iy < m_pDoc->m_sMapDataSizeY ; iy+=sRatio )
	{
		for( ix = 0 ; ix < m_pDoc->m_sMapDataSizeX ; ix+=sRatio )
		{
			sSpr = m_pDoc->m_tile[ix][iy].m_sTileSprite;
			if( sSpr < 0 || sSpr >= DEF_MAXSPRITES) continue;
			if( m_pDoc->m_pSprite[sSpr] == NULL ) continue;
			sSprFrame = m_pDoc->m_tile[ix][iy].m_sTileSpriteFrame;
			m_pDoc->m_pSprite[sSpr]->DrawMiniSprite(&BufferDC, ix*m_sTmp, iy*m_sTmp, sSprFrame, m_sTmp*sRatio);
		}
	}

//	if( m_sTmp > 0 )
//	{
		short sObjSpr, sObjSprFrame;
		for( iy = 0 ; iy < m_pDoc->m_sMapDataSizeY ; iy++ )
		{
			for( ix = 0 ; ix < m_pDoc->m_sMapDataSizeX ; ix++ )
			{
				sObjSpr = m_pDoc->m_tile[ix][iy].m_sObjectSprite;
				if( sObjSpr < 1 || sObjSpr >= DEF_MAXSPRITES ) continue;
				if( m_pDoc->m_pSprite[sObjSpr] == NULL ) continue;
				sObjSprFrame = m_pDoc->m_tile[ix][iy].m_sObjectSpriteFrame;
				m_pDoc->m_pSprite[sObjSpr]->DrawMiniObject(&BufferDC, ix*m_sTmp, iy*m_sTmp, sObjSprFrame, m_sTmp);
			}
		}
//	}
	OffDC.SetStretchBltMode( HALFTONE );
	OffDC.StretchBlt( 0, 0, m_rtClient.right, m_rtClient.bottom, &BufferDC, 0, 0, sLargeMapSize*m_sTmp, sLargeMapSize*m_sTmp, SRCCOPY);
}

void CMiniView::DrawMinimap(short sx, short sy)
{
	int ix, iy;
	short sSpr, sSprFrame, sObjSpr, sObjSprFrame;	
	for( iy = sy-6 ; iy < sy+18 ; iy++ )
	{
		if( iy < 0 ) continue;
		if( iy >= m_pDoc->m_sMapDataSizeY ) continue;
		for( ix = sx-9 ; ix < sx+27 ; ix++ )
		{
			if( ix < 0 ) continue;
			if( ix >= m_pDoc->m_sMapDataSizeX ) continue;
			sSpr = m_pDoc->m_tile[ix][iy].m_sTileSprite;
			if( sSpr < 0 ) continue;
			if( m_pDoc->m_pSprite[sSpr] == NULL ) return;
			sSprFrame = m_pDoc->m_tile[ix][iy].m_sTileSpriteFrame;
			m_pDoc->m_pSprite[sSpr]->DrawMiniSprite(&BufferDC, ix*m_sTmp, iy*m_sTmp, sSprFrame, m_sTmp);
		}
	}
	for( iy = sy-6 ; iy < sy+18 ; iy++ )
	{
		if( iy < 0 ) continue;
		if( iy >= m_pDoc->m_sMapDataSizeY ) continue;
		for( ix = sx-9 ; ix < sx+27 ; ix++ )
		{
			if( ix < 0 ) continue;
			if( ix >= m_pDoc->m_sMapDataSizeX ) continue;
			sObjSpr = m_pDoc->m_tile[ix][iy].m_sObjectSprite;
			if( sObjSpr < 1 ) continue;
			if( m_pDoc->m_pSprite[sObjSpr] == NULL ) continue;
			sObjSprFrame = m_pDoc->m_tile[ix][iy].m_sObjectSpriteFrame;
			m_pDoc->m_pSprite[sObjSpr]->DrawMiniObject(&BufferDC, ix*m_sTmp, iy*m_sTmp, sObjSprFrame, m_sTmp);
		}
	}
	OffDC.SetStretchBltMode( HALFTONE );
	sx -= 9;
	sy -= 6;
	if( sx < 0 ) sx = 0;
	if( sy < 0 ) sy = 0;
	if( sx > m_pDoc->m_sMapDataSizeX-36 ) sx = m_pDoc->m_sMapDataSizeX-36;
	if( sy > m_pDoc->m_sMapDataSizeY-24 ) sy = m_pDoc->m_sMapDataSizeY-24;
	OffDC.StretchBlt( sx*m_rtClient.right/m_pDoc->m_sMapDataSizeX, sy*m_rtClient.bottom/m_pDoc->m_sMapDataSizeY, (sx+36)*m_rtClient.right/m_pDoc->m_sMapDataSizeX, (sy+24)*m_rtClient.bottom/m_pDoc->m_sMapDataSizeY, &BufferDC, sx*m_sTmp, sy*m_sTmp, (sx+36)*m_sTmp, (sy+24)*m_sTmp, SRCCOPY);
	//OffDC.StretchBlt( 0, 0, m_rtClient.right, m_rtClient.bottom, &BufferDC, 0, 0, m_pDoc->m_sMapDataSizeX*m_sTmp, m_pDoc->m_sMapDataSizeY*m_sTmp, SRCCOPY);
}

void CMiniView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if( m_pDoc != NULL ) {
		m_pDoc->m_ScrollPt.x = m_pDoc->m_sMapDataSizeX * point.x / m_rtClient.right - m_pDoc->m_pRightView->m_rtClient.right/(DEF_TILESIZE*2);
		m_pDoc->m_ScrollPt.y = m_pDoc->m_sMapDataSizeY * point.y / m_rtClient.bottom - m_pDoc->m_pRightView->m_rtClient.bottom/(DEF_TILESIZE*2);
		m_pDoc->UpdateScreen();
	}
	CView::OnLButtonDown(nFlags, point);
}

void CMiniView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if( m_pDoc != NULL ) {
		m_pDoc->m_ScrollPt.x = m_pDoc->m_sMapDataSizeX * point.x / m_rtClient.right - m_pDoc->m_pRightView->m_rtClient.right/(DEF_TILESIZE*2);
		m_pDoc->m_ScrollPt.y = m_pDoc->m_sMapDataSizeY * point.y / m_rtClient.bottom - m_pDoc->m_pRightView->m_rtClient.bottom/(DEF_TILESIZE*2);
		m_pDoc->UpdateScreen();
	}
	CView::OnLButtonUp(nFlags, point);
}

void CMiniView::OnMouseMove(UINT nFlags, CPoint point) 
{
	if( (nFlags & MK_LBUTTON) && point.x>0 && point.y>0 && point.x<m_rtClient.right && point.y<m_rtClient.bottom ) {
		POINT pt;
		pt.x = m_pDoc->m_sMapDataSizeX * point.x / m_rtClient.right - m_pDoc->m_pRightView->m_rtClient.right/(DEF_TILESIZE*2);
		pt.y = m_pDoc->m_sMapDataSizeY * point.y / m_rtClient.bottom - m_pDoc->m_pRightView->m_rtClient.bottom/(DEF_TILESIZE*2);
		if( abs(m_pDoc->m_ScrollPt.x-pt.x)+abs(m_pDoc->m_ScrollPt.y-pt.y) > 3 ) {
			m_pDoc->m_ScrollPt.x = (short)pt.x;
			m_pDoc->m_ScrollPt.y = (short)pt.y;
			m_pDoc->UpdateScreen();
		}
	}
	//CView::OnMouseMove(nFlags, point);
}

void CMiniView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	m_pDoc->OnKeyDown(nChar);
}

void CMiniView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	RECT rt;
	GetClientRect(&rt);
	if( m_pDoc == NULL ) 
	{
		GetClientRect(&m_rtClient);
		return;
	}
	BOOL bRedraw = TRUE;
	rt.right = rt.right - (rt.right%50);
	rt.bottom = rt.bottom - (rt.bottom%50);
	if( rt.right != m_rtClient.right )
	{
		if( rt.right > DEF_MINIMAPSIZE ) rt.right = DEF_MINIMAPSIZE;
		if( m_rtClient.right == rt.right ) bRedraw = FALSE;
		m_rtClient.right = rt.right;
		m_rtClient.bottom = (m_pDoc->m_sMapDataSizeY * rt.right) / m_pDoc->m_sMapDataSizeX;
	}
	else if( rt.bottom != m_rtClient.bottom )
	{
		if( rt.bottom > DEF_MINIMAPSIZE ) rt.bottom = DEF_MINIMAPSIZE;
		if( m_rtClient.bottom == rt.bottom ) bRedraw = FALSE;
		m_rtClient.bottom = rt.bottom;
		m_rtClient.right = (m_pDoc->m_sMapDataSizeX * rt.bottom) / m_pDoc->m_sMapDataSizeY;
	}
	if( m_rtClient.right > 120 && m_rtClient.right < 200 ) m_rtClient.right = 200;
	if( m_rtClient.bottom > 120 && m_rtClient.bottom < 200 ) m_rtClient.bottom = 200;
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_wndSplitter.SetColumnInfo( 0, m_rtClient.right, 120 );
	pFrame->m_wndSplitter2.SetRowInfo( 0, m_rtClient.bottom, 120 );
	pFrame->m_wndSplitter.RecalcLayout();
	pFrame->m_wndSplitter2.RecalcLayout();
	if( bRedraw ) {
		OffDC.SetStretchBltMode( HALFTONE );
		OffDC.StretchBlt( 0, 0, m_rtClient.right, m_rtClient.bottom, &BufferDC, 0, 0, m_pDoc->m_sMapDataSizeX*m_sTmp, m_pDoc->m_sMapDataSizeY*m_sTmp, SRCCOPY);
		UpdateScreen();
	}
}

void CMiniView::UpdateScreen()
{
	BackDC.BitBlt( 0, 0, m_rtClient.right, m_rtClient.bottom, &OffDC, 0, 0, SRCCOPY );
	int xpos = ( (m_pDoc->m_ScrollPt.x)*m_rtClient.right ) / (m_pDoc->m_sMapDataSizeX);
	int ypos = ( (m_pDoc->m_ScrollPt.y)*m_rtClient.bottom ) / (m_pDoc->m_sMapDataSizeY);
	int xsize = ( (m_pDoc->m_pRightView->m_rtClient.right)*m_rtClient.right ) / (DEF_TILESIZE * m_pDoc->m_sMapDataSizeX);
	int ysize = ( (m_pDoc->m_pRightView->m_rtClient.bottom)*m_rtClient.bottom ) / (DEF_TILESIZE * m_pDoc->m_sMapDataSizeY);
	if( m_pDoc->m_edit1.x>=0 && m_pDoc->m_edit1.y>=0 && m_pDoc->m_edit2.x>=0 && m_pDoc->m_edit2.y>=0 )
	{
		int x = m_pDoc->m_edit1.x*m_rtClient.right/m_pDoc->m_sMapDataSizeX - 2;
		int szx = (m_pDoc->m_edit2.x-m_pDoc->m_edit1.x)*m_rtClient.right/m_pDoc->m_sMapDataSizeX + 4;
		int y = m_pDoc->m_edit1.y*m_rtClient.bottom/m_pDoc->m_sMapDataSizeY - 2;
		int szy = (m_pDoc->m_edit2.y-m_pDoc->m_edit1.y)*m_rtClient.bottom/m_pDoc->m_sMapDataSizeY + 4;
		//if( szx > 2 && szy > 2 )
		BackDC.Draw3dRect( x, y, szx, szy, RGB(0,255,0), RGB(0,255,0) );
	}
	BackDC.Draw3dRect( xpos, ypos, xsize, ysize, RGB(255,255,255), RGB(255,255,255) );
	InvalidateRect(&m_rtClient, FALSE);
}

void CMiniView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	m_pDoc->OnKeyUp(nChar);	
	//CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

// AnMapView.cpp : implementation of the CAnMapView class
//

#include "stdafx.h"
#include "AnMap.h"
#include "AnMapDoc.h"
#include "AnMapView.h"
#include "MainFrm.h"
#include "Properties.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnMapView

IMPLEMENT_DYNCREATE(CAnMapView, CScrollView)

BEGIN_MESSAGE_MAP(CAnMapView, CScrollView)
	//{{AFX_MSG_MAP(CAnMapView)
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_COMMAND( WM_TILE_PROPERTIES, OnProperties)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnMapView construction/destruction

CAnMapView::CAnMapView()
{
	m_ptScroll.x = -1;
	m_ptScroll.y = -1;
	m_pDoc = NULL;
	m_bCursor = TRUE;
	m_nCursorTimer = 0;
}

CAnMapView::~CAnMapView()
{	
}

BOOL CAnMapView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CAnMapView drawing

void CAnMapView::OnDraw(CDC* pDC)
{
	if( m_pDoc != NULL )
	{
		if( m_pDoc->m_bWorkMode )
		{
			pDC->BitBlt( m_pDoc->m_ScrollPt.x<<5, m_pDoc->m_ScrollPt.y<<5, m_rtClient.right, m_rtClient.bottom, &BufferDC, 0, 0, SRCCOPY );
			m_pDoc->m_bReadyAnMapView = TRUE;
			m_pDoc->ThreadJob();
		}
		else pDC->BitBlt( 0, 0, m_rtClient.right, m_rtClient.bottom, &BufferDC, 0, 0, SRCCOPY );
	}
}

void CAnMapView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	m_pDoc = GetDocument();
	ASSERT_VALID(m_pDoc);
	if( BufferDC.m_hDC == NULL ) {
		SetScrollSizes(MM_TEXT, CSize(m_pDoc->m_sMapDataSizeX<<5, m_pDoc->m_sMapDataSizeY<<5));
		RECT rt;
		rt.right = GetSystemMetrics( SM_CXFULLSCREEN );
		rt.bottom = GetSystemMetrics( SM_CYFULLSCREEN );
		CDC *pDC = GetDC();
		BufferDC.CreateCompatibleDC(pDC);
		m_ptClipping.x = (((rt.right-1)>>5)+1);
		m_ptClipping.y = (((rt.bottom-1)>>5)+1);
		CBitmap bmpBuffer;
		bmpBuffer.CreateCompatibleBitmap(pDC, (m_ptClipping.x+DEF_CLIPSPACE)<<5, (m_ptClipping.y+DEF_CLIPSPACE)<<5);
		BufferDC.SelectObject(&bmpBuffer);
		bmpBuffer.DeleteObject();
		m_pDoc->SetRightView( this );
		ReleaseDC(pDC);
		if( m_pDoc->m_bAutoSave ) m_nAutoSaveTimer = SetTimer( WM_TIMER_AUTOSAVE, m_pDoc->m_dwAutoSaveDelay*60000, 0 );
		if( m_pDoc->m_dwCursorDelay > 0 ) m_nCursorTimer = SetTimer( WM_TIMER_CURSOR, m_pDoc->m_dwCursorDelay, 0 );
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAnMapView diagnostics

#ifdef _DEBUG
void CAnMapView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CAnMapView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAnMapView message handlers

void CAnMapView::UpdateScreen()
{
	if( m_pDoc->m_bWorkMode ) RenderTile();
	else RenderBrush();
	SetFocus();
}

void CAnMapView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	m_pDoc->OnKeyDown(nChar);
}

void CAnMapView::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);
	GetClientRect(&m_rtClient);
	m_ptClipping.x = (((m_rtClient.right-1)>>5)+1);
	m_ptClipping.y = (((m_rtClient.bottom-1)>>5)+1);
	if( m_pDoc != NULL ) m_pDoc->UpdateScreen();
}

void CAnMapView::OnMouseMove(UINT nFlags, CPoint point) 
{
	if( GetFocus() != this ) return;
	short px, py;
	px = (short)(point.x>>5);
	py = (short)(point.y>>5);
	if( m_pDoc->m_mousept.x == px && m_pDoc->m_mousept.y == py ) return;
	if( px < 0 ) px = 0;
	if( py < 0 ) py = 0;
	m_pDoc->m_mousept.x = px;
	m_pDoc->m_mousept.y = py;
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	m_pDoc->OnMouseMove( nFlags );
}

void CAnMapView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	m_pDoc->OnKeyUp( nChar );
}

BOOL CAnMapView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if( zDelta > 0 )
	{
		if( zDelta > 120 )
		{
			m_pDoc->m_ScrollPt.y -= 3;
			m_pDoc->SelectBrush(0);
			m_pDoc->UpdateScreen();
		}
		else if( zDelta > 0 ) m_pDoc->OnKeyDown( VK_UP );
	}
	else if( zDelta < 0 )
	{
		if( zDelta < -120 )
		{
			m_pDoc->m_ScrollPt.y += 3;
			m_pDoc->SelectBrush(0);
			m_pDoc->UpdateScreen();
		}
		else m_pDoc->OnKeyDown( VK_DOWN );
	}
	return FALSE;
}

void CAnMapView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_pDoc->OnLButtonDown( nFlags );
	//CScrollView::OnLButtonDown(nFlags, point);
}

BOOL CAnMapView::OnEraseBkgnd(CDC* pDC) 
{
	return FALSE;
	if( m_pDoc != NULL ) return FALSE;
	return CScrollView::OnEraseBkgnd(pDC);
}

BOOL CAnMapView::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll) 
{
	// calc new x position
	int x = GetScrollPos(SB_HORZ);
	int xOrig = x;

	switch (LOBYTE(nScrollCode))
	{
	case SB_TOP:
		x = 0;
		break;
	case SB_BOTTOM:
		x = INT_MAX;
		break;
	case SB_LINEUP:
		x -= m_lineDev.cx;
		break;
	case SB_LINEDOWN:
		x += m_lineDev.cx;
		break;
	case SB_PAGEUP:
		x -= m_pageDev.cx;
		break;
	case SB_PAGEDOWN:
		x += m_pageDev.cx;
		break;
	case SB_THUMBTRACK:
		x = nPos;
		break;
	}

	// calc new y position
	int y = GetScrollPos(SB_VERT);
	int yOrig = y;

	switch (HIBYTE(nScrollCode))
	{
	case SB_TOP:
		y = 0;
		break;
	case SB_BOTTOM:
		y = INT_MAX;
		break;
	case SB_LINEUP:
		y -= m_lineDev.cy;
		break;
	case SB_LINEDOWN:
		y += m_lineDev.cy;
		break;
	case SB_PAGEUP:
		y -= m_pageDev.cy;
		break;
	case SB_PAGEDOWN:
		y += m_pageDev.cy;
		break;
	case SB_THUMBTRACK:
		y = nPos;
		break;
	}
	x = (x>>5)<<5;
	y = (y>>5)<<5;
	if( OnScrollBy(CSize(x - xOrig, y - yOrig), bDoScroll) )
	{
		m_pDoc->m_ScrollPt.x = GetScrollPos(SB_HORZ)>>5;
		m_pDoc->m_ScrollPt.y = GetScrollPos(SB_VERT)>>5;
		m_pDoc->UpdateScreen();
	}
	return FALSE;
}

void CAnMapView::SetStatusText( int iPane, CString str) 
{
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_wndStatusBar.SetPaneText(iPane, str );
}

void CAnMapView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_pDoc->OnLButtonUp( nFlags );
}

void CAnMapView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	m_pDoc->OnRButtonDown( nFlags );
}

void CAnMapView::RenderTile()
{
	int ix, iy, cx, cy;
	short sSpr, sSprFrame, sObjSpr, sObjSprFrame;
	POINT pt;
	if( m_pDoc == NULL ) return;
	if( m_pDoc->m_pSprite[m_pDoc->m_iBrushIndex] == NULL ) return;
	if( m_pDoc->m_ScrollPt.x > m_pDoc->m_sMapDataSizeX-((m_rtClient.right-1)>>5)-1 )
	{
		m_pDoc->m_ScrollPt.x = m_pDoc->m_sMapDataSizeX-((m_rtClient.right-1)>>5)-1;
	}
	if( m_pDoc->m_ScrollPt.x < 0)
	{
		m_pDoc->m_ScrollPt.x = 0;
	}
	if( m_pDoc->m_ScrollPt.y < 0)
	{
		m_pDoc->m_ScrollPt.y = 0;
	}
	if( m_pDoc->m_ScrollPt.y > m_pDoc->m_sMapDataSizeY-((m_rtClient.bottom-1)>>5)-1 )
	{
		m_pDoc->m_ScrollPt.y = m_pDoc->m_sMapDataSizeY-((m_rtClient.bottom-1)>>5)-1;
	}
	cx = (m_pDoc->m_ScrollPt.x);
	cy = (m_pDoc->m_ScrollPt.y);
	m_ptScroll.x = m_pDoc->m_ScrollPt.x;
	m_ptScroll.y = m_pDoc->m_ScrollPt.y;
	for( iy = cy ; iy < m_ptClipping.y+cy ; iy++ )
	{
		for( ix = cx ; ix < m_ptClipping.x+cx ; ix++ )
		{
			sSpr = m_pDoc->m_tile[ix][iy].m_sTileSprite;
			if( sSpr < 0 || sSpr >= DEF_MAXSPRITES ) continue;
			if( m_pDoc->m_pSprite[sSpr] == NULL ) continue;
			sSprFrame = m_pDoc->m_tile[ix][iy].m_sTileSpriteFrame;
			m_pDoc->m_pSprite[sSpr]->DrawRealSprite(&BufferDC, (ix-cx)*32, (iy-cy)*32, sSprFrame);
		}
	}

	if( m_pDoc->m_bGrid )
	{
		for( iy = -(cy%4) ; iy < m_ptClipping.y+(cy%4) ; iy+=4 )
		{
			for( ix = -(cx%6) ; ix < m_ptClipping.x+(cx%6) ; ix+=6 )
			{
				BufferDC.Draw3dRect( (ix<<5)-1, (iy<<5)-1, DEF_TILESIZE*6+1, DEF_TILESIZE*4+1, RGB(0,0,0), RGB(0,0,0) );
			}
		}
	}
	//brush 그리기
	if( m_pDoc->m_cWorkMode == 0 )
	{
		if( m_pDoc->m_iTileMode == 0 )
		{
			if( m_pDoc->m_iBrushIndex >= 0 && m_pDoc->m_iSelBrush >= 0 )
			{
				if( m_bCursor == TRUE )
				{
					int px, py;
					px = ((m_pDoc->m_mousept.x+cx)/6)*6 - (m_pDoc->m_pSprite[m_pDoc->m_iBrushIndex]->m_stBrush[m_pDoc->m_iSelBrush].pvx) - m_pDoc->m_iTempX*6;
					py = ((m_pDoc->m_mousept.y+cy)/4)*4 - (m_pDoc->m_pSprite[m_pDoc->m_iBrushIndex]->m_stBrush[m_pDoc->m_iSelBrush].pvy) - m_pDoc->m_iTempY*4;
					for( iy=0 ; iy<=m_pDoc->m_pSprite[m_pDoc->m_iBrushIndex]->m_stBrush[m_pDoc->m_iSelBrush].szy ; iy++ )
					{
						for( ix=0 ; ix<=m_pDoc->m_pSprite[m_pDoc->m_iBrushIndex]->m_stBrush[m_pDoc->m_iSelBrush].szx ; ix++ )
						{
							sSprFrame = (m_pDoc->m_pSprite[m_pDoc->m_iBrushIndex]->m_stBrush[m_pDoc->m_iSelBrush].y + iy)*20 + m_pDoc->m_pSprite[m_pDoc->m_iBrushIndex]->m_stBrush[m_pDoc->m_iSelBrush].x + ix;
							if( m_pDoc->m_pSprite[m_pDoc->m_iBrushIndex]->IsBlankTile( sSprFrame ) == FALSE )
								m_pDoc->m_pSprite[m_pDoc->m_iBrushIndex]->DrawRealSprite( &BufferDC, (px+ix-cx)<<5, (py+iy-cy)<<5, sSprFrame );
						}
					}
					int tx = (px-cx)<<5;
					int ty = (py-cy)<<5;
					int szx = ((px-cx+m_pDoc->m_pSprite[m_pDoc->m_iBrushIndex]->m_stBrush[m_pDoc->m_iSelBrush].szx+1)<<5);
					int szy = ((py-cy+m_pDoc->m_pSprite[m_pDoc->m_iBrushIndex]->m_stBrush[m_pDoc->m_iSelBrush].szy+1)<<5);
					CPen pen;
					pen.CreatePen( PS_SOLID, 1, RGB(255,0,0) );
					BufferDC.SetBkColor( RGB(0,0,0) );
					BufferDC.SelectObject( pen );
					BufferDC.MoveTo(tx+7 ,ty );
					BufferDC.LineTo(tx ,ty );
					BufferDC.LineTo(tx ,ty+8 );
					BufferDC.MoveTo(szx-8,szy-1);
					BufferDC.LineTo(szx-1,szy-1);
					BufferDC.LineTo(szx-1,szy-9);
				}
			}
		}
	}

	for( iy = cy-7 ; iy < m_ptClipping.y+cy+10 ; iy++ )
	{
		if( iy < 0 ) continue;
		if( iy >= m_pDoc->m_sMapDataSizeY ) break;
		for( ix = cx-6 ; ix < m_ptClipping.x+cx+5 ; ix++ )
		{
			if( ix < 0 ) continue;
			if( ix >= m_pDoc->m_sMapDataSizeX ) break;
			sObjSpr = m_pDoc->m_tile[ix][iy].m_sObjectSprite;
			if( sObjSpr == 0 ) continue;
			if( m_pDoc->m_bTree == FALSE && sObjSpr >= 100 && sObjSpr < 200 ) continue;
			if( m_pDoc->m_pSprite[sObjSpr] == NULL ) continue;
			sObjSprFrame = m_pDoc->m_tile[ix][iy].m_sObjectSpriteFrame;
			m_pDoc->m_pSprite[sObjSpr]->DrawRealObject(&BufferDC, (ix-cx)<<5, (iy-cy)<<5, sObjSprFrame);
		}
	}
	if( m_pDoc->m_cWorkMode == 0 )
	{
		if( m_pDoc->m_iTileMode == 1 )//나무브러쉬
		{
			m_pDoc->m_pSprite[m_pDoc->m_iSelTree]->DrawRealObject(&BufferDC, m_pDoc->m_mousept.x<<5, m_pDoc->m_mousept.y<<5, 0);
		}
		else if( m_pDoc->m_iTileMode == 2 )//구조물브러쉬
		{
			m_pDoc->m_pSprite[m_pDoc->m_iObjectIndex]->DrawRealObject(&BufferDC, m_pDoc->m_mousept.x<<5, m_pDoc->m_mousept.y<<5, m_pDoc->m_iSelObject);
		}
	}

	if( m_pDoc->m_cWorkMode > 0 )
	{
		for( iy = cy ; iy < m_ptClipping.y+cy ; iy++ )
		{
			for( ix = cx ; ix < m_ptClipping.x+cx ; ix++ )
			{
				if( ! m_pDoc->m_tile[ix][iy].m_bIsMoveAllowed ) BufferDC.Draw3dRect( ((ix-cx)<<5)+2, ((iy-cy)<<5)+2, DEF_TILESIZE-4, DEF_TILESIZE-4, RGB(50,50,255), RGB(50,50,255) );
				if( m_pDoc->m_tile[ix][iy].m_bIsTeleport ) BufferDC.Draw3dRect( ((ix-cx)<<5)+2, ((iy-cy)<<5)+2, DEF_TILESIZE-4, DEF_TILESIZE-4, RGB(255,50,50), RGB(255,50,50) );
				if( m_pDoc->m_tile[ix][iy].m_bIsFarming ) BufferDC.Draw3dRect( ((ix-cx)<<5)+2, ((iy-cy)<<5)+2, DEF_TILESIZE-4, DEF_TILESIZE-4, RGB(255,255,50), RGB(255,255,50) );
			}
		}
		if( m_pDoc->m_cWorkMode == 4 )
		{
			for( iy = cy ; iy < m_ptClipping.y+cy ; iy++ )
			{
				for( ix = cx ; ix < m_ptClipping.x+cx ; ix++ )
				{
					if( ix >= m_pDoc->m_edit1.x && ix <= m_pDoc->m_edit2.x && iy >= m_pDoc->m_edit1.y && iy <= m_pDoc->m_edit2.y ) BufferDC.Draw3dRect( ((ix-cx)<<5)+2, ((iy-cy)<<5)+2, DEF_TILESIZE-4, DEF_TILESIZE-4, RGB(0,255,0), RGB(0,255,0) );
				}
			}
		}
		if( m_pDoc->m_cWorkMode == 1 ) BufferDC.Draw3dRect( (m_pDoc->m_mousept.x<<5), (m_pDoc->m_mousept.y<<5), DEF_TILESIZE, DEF_TILESIZE, RGB(150, 150, 255), RGB(150, 150, 255) );
		if( m_pDoc->m_cWorkMode == 2 ) BufferDC.Draw3dRect( (m_pDoc->m_mousept.x<<5), (m_pDoc->m_mousept.y<<5), DEF_TILESIZE, DEF_TILESIZE, RGB(255, 150, 150), RGB(255, 150, 150) );
		if( m_pDoc->m_cWorkMode == 3 ) BufferDC.Draw3dRect( (m_pDoc->m_mousept.x<<5), (m_pDoc->m_mousept.y<<5), DEF_TILESIZE, DEF_TILESIZE, RGB(255, 255, 150), RGB(255, 255, 150) );
		if( m_pDoc->m_cWorkMode == 4 ) BufferDC.Draw3dRect( (m_pDoc->m_mousept.x<<5), (m_pDoc->m_mousept.y<<5), DEF_TILESIZE, DEF_TILESIZE, RGB(150, 255, 150), RGB(150, 255, 150) );
	}
	if( ( m_ptScroll.x < 20 ) || ( m_ptScroll.y < 20 ) || ( m_ptScroll.x > m_pDoc->m_sMapDataSizeX-20-m_rtClient.right/32 ) || ( m_ptScroll.y > m_pDoc->m_sMapDataSizeY-20-m_rtClient.bottom/32 ) ) DrawLimitLine();
	CString cTmp;
	if( m_pDoc->m_cWorkMode == 4 )
	{
		cTmp.Format( "(%d,%d)~(%d,%d) = (%d,%d)", m_pDoc->m_edit1.x, m_pDoc->m_edit1.y, m_pDoc->m_edit2.x, m_pDoc->m_edit2.y, m_pDoc->m_edit2.x-m_pDoc->m_edit1.x+1, m_pDoc->m_edit2.y-m_pDoc->m_edit1.y+1 );
		SetStatusText( 0, cTmp );
	}
	cTmp.Format( "MapSize(%d,%d)", m_pDoc->m_sMapDataSizeX, m_pDoc->m_sMapDataSizeY );
	SetStatusText(1, cTmp);
	cTmp.Format( "Mouse : %d,%d", m_pDoc->m_ScrollPt.x+m_pDoc->m_mousept.x, m_pDoc->m_ScrollPt.y+m_pDoc->m_mousept.y );
	SetStatusText(2, cTmp);
	cTmp.Format( "Object : %d", m_pDoc->m_tile[m_pDoc->m_ScrollPt.x+m_pDoc->m_mousept.x][m_pDoc->m_ScrollPt.y+m_pDoc->m_mousept.y].m_sObjectSprite );
	SetStatusText(3, cTmp);

	pt.x = m_pDoc->m_ScrollPt.x<<5;
	pt.y = m_pDoc->m_ScrollPt.y<<5;
	ScrollToPosition(pt);
	InvalidateRect(&m_rtClient, FALSE);
}

void CAnMapView::RenderBrush()
{
	int i;
	CString cTmp;
	if( m_pDoc == NULL ) return;
	if( m_pDoc->m_pSprite[m_pDoc->m_iBrushIndex] == NULL ) return;
	cTmp.Format( "Total Brush : %d", m_pDoc->m_pSprite[m_pDoc->m_iBrushIndex]->m_sHowManyBrushes );
	SetStatusText(0, cTmp );
	cTmp.Format( "BrushSize(%d,%d)", m_pDoc->m_pSprite[m_pDoc->m_iBrushIndex]->m_iScreenX, m_pDoc->m_pSprite[m_pDoc->m_iBrushIndex]->m_iScreenY );
	SetStatusText(1, cTmp);
	cTmp.Format( "Mouse : %d,%d", m_pDoc->m_mousept.x, m_pDoc->m_mousept.y );
	SetStatusText(2, cTmp );
	if( m_pDoc->m_iTileMode == 2 )
	{
		if( m_pDoc->m_iObjectIndex < 0 ) return;
		if( m_pDoc->m_pSprite[m_pDoc->m_iObjectIndex] == NULL ) return;
		BufferDC.BitBlt( 0, 0, m_rtClient.right, m_rtClient.bottom, NULL, 0, 0, BLACKNESS );
		m_pDoc->m_pSprite[m_pDoc->m_iObjectIndex]->DrawBrush(&BufferDC);
		InvalidateRect(&m_rtClient, TRUE);
		return;
	}
	if( m_pDoc->m_iBrushIndex < 0 ) return;
	BufferDC.BitBlt( 0, 0, m_rtClient.right, m_rtClient.bottom, NULL, 0, 0, BLACKNESS );
	m_pDoc->m_pSprite[m_pDoc->m_iBrushIndex]->DrawBrush(&BufferDC);
	BufferDC.Draw3dRect( 0, 0, m_pDoc->m_pSprite[m_pDoc->m_iBrushIndex]->m_iScreenX, m_pDoc->m_pSprite[m_pDoc->m_iBrushIndex]->m_iScreenY, RGB(255,255,255), RGB(255,255,255) );
	if( (m_pDoc->m_pSprite[m_pDoc->m_iBrushIndex]->m_sHowManyBrushes>0) && (m_pDoc->m_iBrushMode != 2) )
	{
		CPen pen;
		pen.CreatePen( PS_DOT, 0, RGB(255,255,0) );
		BufferDC.SetBkColor( RGB(0,0,0) );
		BufferDC.SelectObject( pen );
		CSprite *pSpr = m_pDoc->m_pSprite[m_pDoc->m_iBrushIndex];
		for( i=0 ; i<pSpr->m_sHowManyBrushes ; i++ )
		{
			if( pSpr->m_stBrush[i].x < 20 && pSpr->m_stBrush[i].y < 15 && pSpr->m_stBrush[i].szx >= 0 && pSpr->m_stBrush[i].szy >= 0 )
			{
				BufferDC.MoveTo( (pSpr->m_stBrush[i].x)<<5, (pSpr->m_stBrush[i].y)<<5 );
				BufferDC.LineTo( (pSpr->m_stBrush[i].x+pSpr->m_stBrush[i].szx+1)<<5, (pSpr->m_stBrush[i].y)<<5 );
				BufferDC.LineTo( (pSpr->m_stBrush[i].x+pSpr->m_stBrush[i].szx+1)<<5, (pSpr->m_stBrush[i].y+pSpr->m_stBrush[i].szy+1)<<5 );
				BufferDC.LineTo( (pSpr->m_stBrush[i].x)<<5, (pSpr->m_stBrush[i].y+pSpr->m_stBrush[i].szy+1)<<5 );
				BufferDC.LineTo( (pSpr->m_stBrush[i].x)<<5, (pSpr->m_stBrush[i].y)<<5 );
			}
		}
		if( m_pDoc->m_iBrushMode != 1 && m_pDoc->m_cWorkMode == 0)
		{
			int iIndex = -1;
			for( i=0 ; i<pSpr->m_sHowManyBrushes ; i++ )
			{
				if( m_pDoc->m_mousept.x >= pSpr->m_stBrush[i].x && m_pDoc->m_mousept.x <= (pSpr->m_stBrush[i].x + pSpr->m_stBrush[i].szx ) )
				{
					if( m_pDoc->m_mousept.y >= pSpr->m_stBrush[i].y && m_pDoc->m_mousept.y <= (pSpr->m_stBrush[i].y + pSpr->m_stBrush[i].szy ) )
					{
						if( iIndex == -1 ) iIndex = i;
						else
						{
							if( pSpr->m_stBrush[i].szx * pSpr->m_stBrush[i].szy < pSpr->m_stBrush[iIndex].szx * pSpr->m_stBrush[iIndex].szy )
							{
								iIndex = i;
							}
						}
					}
				}
			}
			if( iIndex != -1 ) {
				BufferDC.Draw3dRect( (pSpr->m_stBrush[iIndex].x)<<5                                 , (pSpr->m_stBrush[iIndex].y)<<5, (pSpr->m_stBrush[iIndex].szx+1)<<5, (pSpr->m_stBrush[iIndex].szy+1)<<5, RGB(0,255,0), RGB(0,255,0) );
				BufferDC.Draw3dRect( (pSpr->m_stBrush[iIndex].x+pSpr->m_stBrush[iIndex].pvx)*32+2   , (pSpr->m_stBrush[iIndex].y+pSpr->m_stBrush[iIndex].pvy)*32+2   , 96-4, 64-4, RGB(0,0,255), RGB(0,0,255) );
				BufferDC.Draw3dRect( (pSpr->m_stBrush[iIndex].x+pSpr->m_stBrush[iIndex].pvx)*32+2+96, (pSpr->m_stBrush[iIndex].y+pSpr->m_stBrush[iIndex].pvy)*32+2   , 96-4, 64-4, RGB(0,0,255), RGB(0,0,255) );
				BufferDC.Draw3dRect( (pSpr->m_stBrush[iIndex].x+pSpr->m_stBrush[iIndex].pvx)*32+2   , (pSpr->m_stBrush[iIndex].y+pSpr->m_stBrush[iIndex].pvy)*32+2+64, 96-4, 64-4, RGB(0,0,255), RGB(0,0,255) );
				BufferDC.Draw3dRect( (pSpr->m_stBrush[iIndex].x+pSpr->m_stBrush[iIndex].pvx)*32+2+96, (pSpr->m_stBrush[iIndex].y+pSpr->m_stBrush[iIndex].pvy)*32+2+64, 96-4, 64-4, RGB(0,0,255), RGB(0,0,255) );
			}
		}
	}
	if( m_pDoc->m_cWorkMode > 0 )
	{
		for( i=0 ; i<300 ; i++ )
		{
			if( m_pDoc->m_pSprite[m_pDoc->m_iBrushIndex]->m_bMove[i] == true )
			{
				BufferDC.Draw3dRect( ((i%20)<<5)+2, ((i/20)<<5)+2, DEF_TILESIZE-4, DEF_TILESIZE-4, RGB(0,0,255), RGB(0,0,255) );
			}
		}
		BufferDC.Draw3dRect( (m_pDoc->m_mousept.x<<5), (m_pDoc->m_mousept.y<<5), DEF_TILESIZE, DEF_TILESIZE, RGB(150, 150, 255), RGB(150, 150, 255) );
		InvalidateRect(&m_rtClient, TRUE);
		return;
	}

	if( m_pDoc->m_iBrushMode == 0 )
	{
		BufferDC.Draw3dRect( m_pDoc->m_mousept.x<<5, m_pDoc->m_mousept.y<<5, 32, 32, RGB(200,0,0), RGB(200,0,0) );
	}
	else if( m_pDoc->m_iBrushMode == 1 )
	{
		short sx, sy, szx, szy;
		if( m_pDoc->m_mousept.x > m_pDoc->m_sBrsX1 )
		{
			sx = m_pDoc->m_sBrsX1*32;
			szx = (m_pDoc->m_mousept.x-m_pDoc->m_sBrsX1+1)*32;
		}
		else
		{
			sx = m_pDoc->m_mousept.x*32;
			szx = (m_pDoc->m_sBrsX1-m_pDoc->m_mousept.x+1)*32;
		}
		if( m_pDoc->m_mousept.y > m_pDoc->m_sBrsY1 )
		{
			sy = m_pDoc->m_sBrsY1*32;
			szy = (m_pDoc->m_mousept.y-m_pDoc->m_sBrsY1+1)*32;
		}
		else
		{
			sy = m_pDoc->m_mousept.y*32;
			szy = (m_pDoc->m_sBrsY1-m_pDoc->m_mousept.y+1)*32;
		}
		BufferDC.Draw3dRect( sx, sy, szx, szy, RGB(200,0,0), RGB(200,0,0) );
	}
	else if( m_pDoc->m_iBrushMode == 2 )
	{
		short sx, sy, szx, szy, px, py;
		sx = m_pDoc->m_sBrsX1*32;
		sy = m_pDoc->m_sBrsY1*32;
		szx = ( m_pDoc->m_sBrsX2 - m_pDoc->m_sBrsX1 + 1 ) *32;
		szy = ( m_pDoc->m_sBrsY2 - m_pDoc->m_sBrsY1 + 1 ) *32;
		BufferDC.Draw3dRect( sx, sy, szx, szy, RGB(0,255,0), RGB(0,255,0) );
		px = m_pDoc->m_mousept.x*32;
		py = m_pDoc->m_mousept.y*32;
		BufferDC.Draw3dRect( px-96+2, py-64+2, 96-4, 64-4, RGB(200,0,0), RGB(200,0,0) );
		BufferDC.Draw3dRect( px   +2, py-64+2, 96-4, 64-4, RGB(200,0,0), RGB(200,0,0) );
		BufferDC.Draw3dRect( px-96+2, py   +2, 96-4, 64-4, RGB(200,0,0), RGB(200,0,0) );
		BufferDC.Draw3dRect( px   +2, py   +2, 96-4, 64-4, RGB(200,0,0), RGB(200,0,0) );
	}
	InvalidateRect(&m_rtClient, TRUE);
}

void CAnMapView::DrawLimitLine()
{
	int i;
	CPen pen;
	pen.CreatePen( PS_SOLID , 1, RGB(255,255,0) );
	BufferDC.SetBkColor( RGB(0,0,0) );
	BufferDC.SelectObject( pen );
	if( m_ptScroll.x < 20 )
	{
		for( i=0 ; i<m_rtClient.bottom ; i+=32 )
		{
			BufferDC.MoveTo((20-m_ptScroll.x)*32 , i+12 );
			BufferDC.LineTo((20-m_ptScroll.x)*32 , i+20 );
		}
	}
	if( m_ptScroll.y < 20 )
	{
		for( i=0 ; i<m_rtClient.right ; i+=32 )
		{
			BufferDC.MoveTo( i+12, (20-m_ptScroll.y)*32 );
			BufferDC.LineTo( i+20, (20-m_ptScroll.y)*32 );
		}
	}
	if( m_ptScroll.x > m_pDoc->m_sMapDataSizeX-20-m_rtClient.right/32 )
	{
		for( i=0 ; i<m_rtClient.bottom ; i+=32 )
		{
			BufferDC.MoveTo((m_pDoc->m_sMapDataSizeX-20-m_ptScroll.x)*32 , i+12 );
			BufferDC.LineTo((m_pDoc->m_sMapDataSizeX-20-m_ptScroll.x)*32 , i+20 );
		}
	}
	if( m_ptScroll.y > m_pDoc->m_sMapDataSizeY-20-m_rtClient.bottom/32 )
	{
		for( i=0 ; i<m_rtClient.right ; i+=32 )
		{
			BufferDC.MoveTo( i+12, (m_pDoc->m_sMapDataSizeY-20-m_ptScroll.y)*32 );
			BufferDC.LineTo( i+20, (m_pDoc->m_sMapDataSizeY-20-m_ptScroll.y)*32 );
		}
	}
}

void CAnMapView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	m_pDoc->OnRButtonUp( nFlags );
	if( m_pDoc->m_cWorkMode == 4 )
	{
		if( m_pDoc->m_edit1.x >= 0 && m_pDoc->m_edit1.y >= 0 && m_pDoc->m_edit2.x >= 0 && m_pDoc->m_edit2.y >= 0 )
		{
			ClientToScreen(&point);
			CMenu menu;
			menu.CreatePopupMenu();
			menu.AppendMenu( MF_STRING, WM_TILE_PROPERTIES, "Tile P&roperties" );
			SetForegroundWindow();
			menu.TrackPopupMenu( TPM_LEFTALIGN, point.x, point.y, this, NULL );
		}
	}
}

void CAnMapView::OnProperties()
{
	CProperties dlg(m_pDoc);
	dlg.DoModal();
}

void CAnMapView::OnTimer(UINT nIDEvent) 
{
	if( GetFocus() != this ) return;
	if( nIDEvent == WM_TIMER_CURSOR )
	{
		if( m_pDoc->m_bWorkMode )
		{
			if( m_pDoc->m_cWorkMode == 0 )
			{
				if( m_bCursor ) m_bCursor = FALSE;
				else m_bCursor = TRUE;
				UpdateScreen();
			}
		}
	}
	if( nIDEvent == WM_TIMER_AUTOSAVE )
	{
		if( m_pDoc->m_bModified )
		{
			m_pDoc->SaveMapData( "Backup\\"+m_pDoc->m_filename );
			SetStatusText( 4, "Saved" );
		}
	}
	CScrollView::OnTimer(nIDEvent);
}

void CAnMapView::OnDestroy() 
{
	if( m_nAutoSaveTimer != 0 )
	{
		KillTimer( m_nAutoSaveTimer );
		m_nAutoSaveTimer = 0;
	}
	if( m_nCursorTimer != 0 )
	{
		KillTimer( m_nCursorTimer );
		m_nCursorTimer = 0;
	}
	CScrollView::OnDestroy();
}

void CAnMapView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);
	if( m_pDoc == NULL ) return;
	if( bActivate == FALSE && pDeactiveView == this )
	{
		m_pDoc->m_bActivated = FALSE;
	}
	else if( bActivate == TRUE && pActivateView == this )
	{
		m_pDoc->m_bActivated = TRUE;
	}
}

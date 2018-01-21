// RenderDialog.cpp : implementation file
//

#include "stdafx.h"
#include "AnMap.h"
#include "RenderDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRenderDialog dialog

CRenderDialog::CRenderDialog(CAnMapDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CDialog(CRenderDialog::IDD, pParent)
{
	m_pDoc = pDoc;
	m_scrollpt.x = 0;
	m_scrollpt.y = 0;
	m_pOutputDC = NULL;
	m_iRatio = 0;
	m_iProgressX = -1;
	m_iProgressY = -1;
	m_iOutSizeX = 0;
	m_iOutSizeY = 0;
	m_lbscroll.x = -1;
	m_lbscroll.y = -1;
	int iTemp = m_pDoc->m_sMapDataSizeX;
	if( m_pDoc->m_sMapDataSizeX < m_pDoc->m_sMapDataSizeY ) iTemp = m_pDoc->m_sMapDataSizeY;

	if( iTemp%16 == 0 ) m_iBufferSize = iTemp/16;
	else if( iTemp%8 == 0 ) m_iBufferSize = iTemp/8;
	else if( iTemp%4 == 0 ) m_iBufferSize = iTemp/4;
	else if( iTemp%2 == 0 ) m_iBufferSize = iTemp/2;
	else m_iBufferSize = iTemp/16;

	if( m_iBufferSize < 30 ) m_iBufferSize = m_iBufferSize*2;
	//{{AFX_DATA_INIT(CRenderDialog)
	m_iOutputSize = 640;
	//}}AFX_DATA_INIT
}

void CRenderDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRenderDialog)
	DDX_Control(pDX, IDC_PROGRESS_JOB, m_progress);
	DDX_Text(pDX, IDC_EDIT_OUTPUTSIZE, m_iOutputSize);
	DDV_MinMaxInt(pDX, m_iOutputSize, 64, 6400);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRenderDialog, CDialog)
	//{{AFX_MSG_MAP(CRenderDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_RENDER, OnButtonRender)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON_PRINT, OnButtonPrint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRenderDialog message handlers

BOOL CRenderDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CDC* pDC = GetDC();
	m_bufferDC.CreateCompatibleDC( pDC );
	CBitmap bmpBuffer;
	bmpBuffer.CreateCompatibleBitmap(pDC, m_iBufferSize*DEF_TILESIZE, m_iBufferSize*DEF_TILESIZE);
	m_bufferDC.SelectObject(&bmpBuffer);
	bmpBuffer.DeleteObject();
	GetDlgItem( IDC_BUTTON_SAVE )->ShowWindow(FALSE);
	GetDlgItem( IDC_BUTTON_PRINT )->ShowWindow(FALSE);
	ReleaseDC(pDC);
	char cStatic[10];
	int iTemp = m_pDoc->m_sMapDataSizeX;
	if( m_pDoc->m_sMapDataSizeY > m_pDoc->m_sMapDataSizeX ) iTemp = m_pDoc->m_sMapDataSizeY;
	wsprintf( cStatic, "/%d = %d.%d%%", iTemp*32, (200000/iTemp)/100, (200000/iTemp)%100 );
	SetWindowPos( NULL, 0, 0, 657, 705, SWP_NOMOVE | SWP_NOZORDER );
	GetDlgItem( IDC_STATIC_RATIO )->SetWindowText( cStatic );
	return TRUE;
}

void CRenderDialog::OnPaint() 
{
	CPaintDC dc(this);
	if( m_pOutputDC != NULL )
	{
		if( m_iProgressX == -1 ) dc.BitBlt( 5, 35, 640, 640, NULL, 0, 0, SRCCOPY );
		dc.BitBlt( 5, 35, 640, 640, m_pOutputDC, m_scrollpt.x, m_scrollpt.y, SRCCOPY );
		if( m_iProgressX != -1 ) DrawProgress();
	}
	else
	{
		if( m_pDoc != NULL )
		{
			int iTemp = m_pDoc->m_sMapDataSizeX;
			if( m_pDoc->m_sMapDataSizeY > m_pDoc->m_sMapDataSizeX ) iTemp = m_pDoc->m_sMapDataSizeY;
			dc.SetStretchBltMode( HALFTONE );
			dc.StretchBlt( 5, 35, 640, 640, &(m_pDoc->m_pMiniView->BufferDC), 0, 0, iTemp*m_pDoc->m_pMiniView->m_sTmp, iTemp*m_pDoc->m_pMiniView->m_sTmp, SRCCOPY );
		}
	}
}

void CRenderDialog::OnButtonRender() 
{
	if( UpdateData( TRUE ) == FALSE ) return;
	if( m_iProgressX != -1 ) return;
	if( m_iProgressY != -1 ) return;
	int iTemp = m_pDoc->m_sMapDataSizeX;
	if( m_pDoc->m_sMapDataSizeY > m_pDoc->m_sMapDataSizeX ) iTemp = m_pDoc->m_sMapDataSizeY;
	m_iRatio = (m_iOutputSize*m_iBufferSize) / iTemp;
	if( m_iRatio > m_iBufferSize*DEF_TILESIZE ) return;
	m_iOutSizeX = m_iRatio*m_pDoc->m_sMapDataSizeX/m_iBufferSize;
	m_iOutSizeY = m_iRatio*m_pDoc->m_sMapDataSizeY/m_iBufferSize;
	CDC* pDC = GetDC();
	if( bmpBuffer.m_hObject != NULL ) bmpBuffer.DeleteObject();
	if( bmpBuffer.CreateCompatibleBitmap(pDC, m_iOutSizeX, m_iOutSizeY) == 0 )
	{
		ReleaseDC(pDC);
		MessageBox( "Sorry! Memory is in short." );
		return;
	}
	m_scrollpt.x = 0;
	m_scrollpt.y = 0;
	char cStatic[20];
	int sangha = (m_iOutSizeX*10000)/(m_pDoc->m_sMapDataSizeX*DEF_TILESIZE);
	wsprintf( cStatic, "/%d = %d.%d%%", iTemp*DEF_TILESIZE, sangha/100, sangha%100 );
	GetDlgItem( IDC_STATIC_RATIO )->SetWindowText( cStatic );
	if( m_iOutSizeY > m_iOutSizeX ) m_iOutputSize = m_iOutSizeY;
	else m_iOutputSize = m_iOutSizeX;
	UpdateData( FALSE );
	if( m_pOutputDC != NULL )
	{
		delete m_pOutputDC;
		m_pOutputDC = NULL;
	}
	m_pOutputDC = new CDC;
	m_pOutputDC->CreateCompatibleDC( pDC );
	m_pOutputDC->SelectObject(&bmpBuffer);
	pDC->BitBlt( 5, 35, 640, 640, NULL, 0, 0, BLACKNESS );
	ReleaseDC(pDC);

	m_pOutputDC->SetStretchBltMode( HALFTONE );
	m_iProgressX = 0;
	m_iProgressY = 0;
	GetDlgItem( IDC_BUTTON_RENDER )->ShowWindow(FALSE);
	GetDlgItem( IDC_BUTTON_SAVE )->ShowWindow(FALSE);
	GetDlgItem( IDC_BUTTON_PRINT )->ShowWindow(FALSE);
	m_progress.ShowWindow(TRUE);
	SetWindowPos( NULL, 0, 0, 657, 723, SWP_NOMOVE | SWP_NOZORDER );
	DrawProgress();
}

void CRenderDialog::DrawProgress()
{
	BeginWaitCursor();
	MakeImage( m_iProgressX*m_iBufferSize, m_iProgressY*m_iBufferSize );
	m_pOutputDC->StretchBlt( m_iProgressX*m_iRatio, m_iProgressY*m_iRatio, m_iRatio, m_iRatio, &m_bufferDC, 0, 0, m_iBufferSize*DEF_TILESIZE, m_iBufferSize*DEF_TILESIZE, SRCCOPY );

	m_iProgressX++;
	if( m_iProgressX*m_iBufferSize >= m_pDoc->m_sMapDataSizeX )
	{
		m_iProgressX = 0;
		m_iProgressY ++;
	}
	if( m_iProgressY*m_iBufferSize >= m_pDoc->m_sMapDataSizeY )
	{
		m_iProgressX = -1;
		m_iProgressY = -1;
		GetDlgItem( IDC_BUTTON_RENDER )->ShowWindow(TRUE);
		GetDlgItem( IDC_BUTTON_SAVE )->ShowWindow(TRUE);
		GetDlgItem( IDC_BUTTON_PRINT )->ShowWindow(TRUE);
		EndWaitCursor();
		m_progress.ShowWindow(FALSE);
		SetWindowPos( NULL, 0, 0, 657, 705, SWP_NOMOVE | SWP_NOZORDER );
	}
	else
	{
		int iProgress = (m_iProgressY*m_pDoc->m_sMapDataSizeX + (m_iProgressX+1)*m_iBufferSize)*m_iBufferSize*100/(m_pDoc->m_sMapDataSizeX*m_pDoc->m_sMapDataSizeY);
		if( iProgress < 0 ) iProgress = 0;
		if( iProgress > 100 ) iProgress = 100;
		m_progress.SetPos( iProgress );
	}	
	RECT rt;
	SetRect( &rt, 0, 0, 700, 700 );
	InvalidateRect( &rt, FALSE );
}

void CRenderDialog::MakeImage(short sx, short sy)
{
	int ix, iy;
	short sSpr, sFrm;
	for( iy=0 ; iy<m_iBufferSize ; iy++ )
	{
		if( sy+iy < 0 || sy+iy >= m_pDoc->m_sMapDataSizeY ) continue;
		for( ix=0 ; ix<m_iBufferSize ; ix++ )
		{
			if( sx+ix < 0 || sx+ix >= m_pDoc->m_sMapDataSizeX ) continue;
			sSpr = m_pDoc->m_tile[sx+ix][sy+iy].m_sTileSprite;
			if( sSpr < 0 || sSpr >= DEF_MAXSPRITES ) continue;
			if( m_pDoc->m_pSprite[sSpr] == NULL ) continue;
			sFrm = m_pDoc->m_tile[sx+ix][sy+iy].m_sTileSpriteFrame;
			m_pDoc->m_pSprite[sSpr]->DrawRealSprite(&m_bufferDC, ix<<5, iy<<5, sFrm);
		}
	}
	for( iy=-4 ; iy<m_iBufferSize+10 ; iy++ )
	{
		if( sy+iy < 0 || sy+iy >= m_pDoc->m_sMapDataSizeY ) continue;
		for( ix=-6 ; ix<m_iBufferSize+4 ; ix++ )
		{
			if( sx+ix < 0 || sx+ix >= m_pDoc->m_sMapDataSizeX ) continue;
			sSpr = m_pDoc->m_tile[sx+ix][sy+iy].m_sObjectSprite;
			if( sSpr <= 0 || sSpr >= DEF_MAXSPRITES ) continue;
			if( m_pDoc->m_pSprite[sSpr] == NULL ) continue;
			sFrm = m_pDoc->m_tile[sx+ix][sy+iy].m_sObjectSpriteFrame;
			m_pDoc->m_pSprite[sSpr]->DrawRealObject(&m_bufferDC, ix<<5, iy<<5, sFrm);
		}
	}
}

void CRenderDialog::OnClose() 
{
	if( bmpBuffer.m_hObject != NULL ) bmpBuffer.DeleteObject();
	if( m_pOutputDC != NULL )
	{
		delete m_pOutputDC;
		m_pOutputDC = NULL;
	}
	CDialog::OnClose();
}

void CRenderDialog::OnButtonSave() 
{
	if( m_pOutputDC == NULL ) return;
	if( m_iProgressX != -1 ) return;
	if( m_iProgressY != -1 ) return;
	char cDir[256];
	GetCurrentDirectory( sizeof(cDir), cDir );
	CFileDialog dlg( FALSE, "bmp", NULL, OFN_OVERWRITEPROMPT, "Image Files (*.bmp)|*.bmp|", NULL );
	if( dlg.DoModal() != IDOK ) return;
	BeginWaitCursor();
	SetCurrentDirectory(cDir);
	CString cFn;
	HANDLE hFileWrite;
	DWORD nCount;
	cFn = dlg.GetPathName();
	hFileWrite = CreateFile(cFn, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, NULL, NULL);
	if (hFileWrite == INVALID_HANDLE_VALUE) return;
	BITMAPFILEHEADER fh;
	BITMAPINFOHEADER ih;
	fh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	fh.bfReserved1 = 0;
	fh.bfReserved2 = 0;
	fh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 3*m_iOutSizeX*m_iOutSizeY;
	fh.bfType = 0x4d42;
	ih.biBitCount = 24;
	ih.biClrImportant = 0;
	ih.biClrUsed = 0;
	ih.biCompression = BI_RGB;
	ih.biHeight = m_iOutSizeY;
	ih.biPlanes = 1;
	ih.biSize = sizeof(BITMAPINFOHEADER);
	ih.biSizeImage = 0;
	ih.biWidth = m_iOutSizeX;
	ih.biXPelsPerMeter = 0;
	ih.biYPelsPerMeter = 0;
	char *cpRaster, *cp;
	int iOutX = 0;
	if( m_iOutSizeX%4 != 0 )
	{
		iOutX = m_iOutSizeX + 4 - (m_iOutSizeX%4);
	}
	else
	{
		iOutX = m_iOutSizeX;
	}
	cpRaster = new char[3*iOutX*m_iOutSizeY];
	ZeroMemory( cpRaster, 3*iOutX*m_iOutSizeY );
	cp = cpRaster;
	GetDIBits( m_pOutputDC->m_hDC, (HBITMAP)bmpBuffer, 0, m_iOutSizeY, cpRaster, (LPBITMAPINFO)&ih, DIB_RGB_COLORS );
	SetFilePointer( hFileWrite, 0, NULL, FILE_BEGIN);
	WriteFile( hFileWrite, &fh, sizeof(BITMAPFILEHEADER), &nCount, NULL);
	WriteFile( hFileWrite, &ih, sizeof(BITMAPINFOHEADER), &nCount, NULL);
	WriteFile( hFileWrite, cpRaster, 3*iOutX*m_iOutSizeY, &nCount, NULL);
	CloseHandle( hFileWrite );
	delete[] cpRaster;
	EndWaitCursor();
	wsprintf( cDir, "%s has been saved succesfully!", dlg.GetFileName() );
	MessageBox( cDir );
}

void CRenderDialog::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if( m_iProgressX != -1 ) return;
	if( m_iProgressY != -1 ) return;
	if( point.x > 10 && point.x < 650 && point.y > 50 && point.y < 690 )
	{
		m_lbpt.x = (short)point.x;
		m_lbpt.y = (short)point.y;
		m_lbscroll.x = m_scrollpt.x;
		m_lbscroll.y = m_scrollpt.y;
	}	
	CDialog::OnLButtonDown(nFlags, point);
}

void CRenderDialog::OnMouseMove(UINT nFlags, CPoint point) 
{
	if( m_iProgressX != -1 ) return;
	if( m_iProgressY != -1 ) return;
	if( m_lbscroll.x == -1 ) return;
	if( m_lbscroll.y == -1 ) return;
	if( nFlags & MK_LBUTTON )
	{
		m_scrollpt.x = m_lbscroll.x - ( point.x - m_lbpt.x );
		m_scrollpt.y = m_lbscroll.y - ( point.y - m_lbpt.y );
		if( m_scrollpt.x < 0 ) m_scrollpt.x = 0;
		if( m_scrollpt.x > m_iOutSizeX-640 ) m_scrollpt.x = m_iOutSizeX-640;
		if( m_scrollpt.y < 0 ) m_scrollpt.y = 0;
		if( m_scrollpt.y > m_iOutSizeY-640 ) m_scrollpt.y = m_iOutSizeY-640;
		RECT rt;
		SetRect( &rt, 0, 0, 700, 700 );
		InvalidateRect( &rt, FALSE );
	}
	CDialog::OnMouseMove(nFlags, point);
}

void CRenderDialog::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_lbscroll.x = -1;
	m_lbscroll.y = -1;
	CDialog::OnLButtonUp(nFlags, point);
}

void CRenderDialog::OnButtonPrint() 
{
	if( m_pOutputDC == NULL ) return;
	if( m_iProgressX != -1 ) return;
	if( m_iProgressY != -1 ) return;
	CPrintDialog printDlg(FALSE);
	if( printDlg.DoModal() == IDCANCEL ) return;
	BeginWaitCursor();
	CDC printdc;
	printdc.Attach(printDlg.GetPrinterDC());
	printdc.m_bPrinting = TRUE;	
	DOCINFO di;
	ZeroMemory( &di, sizeof(DOCINFO) );
	di.cbSize = sizeof(DOCINFO);
	di.lpszDocName = m_pDoc->m_filename;
	printdc.StartDoc(&di);
	CPrintInfo Info;
	Info.m_rectDraw.SetRect( 0, 0, printdc.GetDeviceCaps(HORZRES), printdc.GetDeviceCaps(VERTRES) );
	printdc.StartPage();
	printdc.SetMapMode(MM_LOMETRIC);
	CRect rect(0, -100, 1970, -300 );
	printdc.DrawText( m_pDoc->m_filename, &rect, DT_CENTER );
	printdc.StretchBlt( 0, -400, 1970, -1970, m_pOutputDC, 0, 0, m_iOutSizeX, m_iOutSizeX, SRCCOPY );
	printdc.EndPage();
	printdc.EndDoc();
	printdc.Detach();
	EndWaitCursor();
}

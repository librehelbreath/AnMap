// Sprite.cpp: implementation of the CSprite class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AnMap.h"
#include "Sprite.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSprite::CSprite(HANDLE hFileRead, short sSprID, short NthFile, char* FileName)
{
	int i;
	DWORD nCount, dwASDfileLoc;
	for( i=0 ; i < 60 ; i++ )
	{
		m_stBrush[i].x = -1;
		m_stBrush[i].y = -1;
		m_stBrush[i].szx = -1;
		m_stBrush[i].szy = -1;
		m_stBrush[i].pvx = -1;
		m_stBrush[i].pvy = -1;
	}
	m_sSprID = sSprID;
	ZeroMemory( m_cFileName, sizeof(m_cFileName) );
	strcpy( m_cFileName, FileName );
	SetFilePointer(hFileRead, 24 + NthFile*8, NULL, FILE_BEGIN);
	ReadFile(hFileRead, &dwASDfileLoc,  4, &nCount, NULL);
	SetFilePointer(hFileRead, dwASDfileLoc+100, NULL, FILE_BEGIN);
	ReadFile(hFileRead, &m_iTotalFrame,  4, &nCount, NULL);
	ReadFile(hFileRead, m_stSpd, m_iTotalFrame*12, &nCount, NULL);
	m_dwBmpStartPos = dwASDfileLoc + 108 + m_iTotalFrame*12;
	m_bIsDCEmpty = TRUE;
}

CSprite::~CSprite()
{
}

void CSprite::DrawRealSprite(CDC *pDC, short dx, short dy, short sSprFrame)
{
	if( sSprFrame >= m_iTotalFrame ) return;
	if( m_bIsDCEmpty ) MakeMemDC(pDC);
	pDC->BitBlt( dx, dy, DEF_TILESIZE, DEF_TILESIZE, &BufferDC, m_stSpd[sSprFrame].sx, m_stSpd[sSprFrame].sy, SRCCOPY );
}

void CSprite::DrawRealObject(CDC *pDC, short dx, short dy, short sSprFrame)
{
	if( sSprFrame >= m_iTotalFrame ) return;
	if( m_bIsDCEmpty ) MakeMemDC(pDC);
	TransparentBlt( pDC->GetSafeHdc(), dx+m_stSpd[sSprFrame].pvx, dy+m_stSpd[sSprFrame].pvy, m_stSpd[sSprFrame].szx, m_stSpd[sSprFrame].szy, BufferDC.GetSafeHdc(), m_stSpd[sSprFrame].sx, m_stSpd[sSprFrame].sy, m_stSpd[sSprFrame].szx, m_stSpd[sSprFrame].szy, m_color );
}

void CSprite::DrawMiniSprite(CDC *pDC, short dx, short dy, short sSprFrame, short sSize)
{
	if( sSprFrame >= m_iTotalFrame )
	{
		//MessageBox( NULL, "Error", NULL, MB_OK );
		return;
	}
	if( m_bIsDCEmpty ) MakeMemDC(pDC);
	pDC->BitBlt( dx, dy, sSize, sSize, &BufferDC, m_stSpd[sSprFrame].sx, m_stSpd[sSprFrame].sy, SRCCOPY );
}

void CSprite::DrawMiniObject(CDC *pDC, short dx, short dy, short sSprFrame, short sSize)
{
	if( sSprFrame >= m_iTotalFrame )
	{
		//MessageBox( NULL, "Error", NULL, MB_OK );
		return;
	}
	if( m_stSpd[sSprFrame].szy < DEF_TILESIZE ) return;
	if( m_bIsDCEmpty ) MakeMemDC(pDC);
	TransparentBlt( pDC->GetSafeHdc(), dx + (m_stSpd[sSprFrame].pvx*sSize>>5), dy + (m_stSpd[sSprFrame].pvy*sSize>>5), ((m_stSpd[sSprFrame].szx*sSize)>>5), ((m_stSpd[sSprFrame].szy*sSize)>>5), BufferDC.GetSafeHdc(), m_stSpd[sSprFrame].sx, m_stSpd[sSprFrame].sy, m_stSpd[sSprFrame].szx, m_stSpd[sSprFrame].szy, m_color );
}

void CSprite::DrawBrush(CDC *pDC)
{
	if( m_bIsDCEmpty ) MakeMemDC(pDC);
	pDC->BitBlt( 0, 0, 640, 480, &BufferDC, 0, 0, SRCCOPY );
}

BOOL CSprite::MakeMemDC(CDC *pDC)
{
	if( !m_bIsDCEmpty ) return FALSE;
	char PathName[30];
	BITMAPFILEHEADER fh;
	DWORD nCount;
	wsprintf( PathName, "sprites\\%s.pak", m_cFileName );
	HANDLE hFileRead = CreateFile(PathName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if( hFileRead == INVALID_HANDLE_VALUE ) return FALSE;
	SetFilePointer( hFileRead, m_dwBmpStartPos, NULL, FILE_BEGIN );
	ReadFile( hFileRead, &fh, 14, &nCount, NULL );
	char* lpDib = new char[fh.bfSize-14];
	ReadFile( hFileRead, lpDib, fh.bfSize-14, &nCount, NULL );
	CloseHandle( hFileRead );
	LPBITMAPINFO lpBmpInfo = (LPBITMAPINFO)lpDib;
	m_iScreenX = lpBmpInfo->bmiHeader.biWidth;
	m_iScreenY = lpBmpInfo->bmiHeader.biHeight;
	BufferDC.CreateCompatibleDC(pDC);
	CBitmap bmpBuffer;
	if( bmpBuffer.CreateCompatibleBitmap(pDC, m_iScreenX, m_iScreenY) == FALSE ) MessageBox( NULL, "Error", NULL, MB_OK );
	if( BufferDC.SelectObject(&bmpBuffer) == NULL ) return FALSE;
	bmpBuffer.DeleteObject();
	SetDIBitsToDevice( BufferDC.GetSafeHdc(), 0, 0, m_iScreenX, m_iScreenY, 0, 0, 0, m_iScreenY, lpDib+fh.bfOffBits-14, lpBmpInfo, DIB_RGB_COLORS );
	m_color = BufferDC.GetPixel( 0, 0 );
	delete[] lpDib;
	m_bIsDCEmpty = FALSE;
	return TRUE;
}

BOOL CSprite::InsertBrush(short x1, short y1, short x2, short y2, short x3, short y3)
{
	short x, y, szx, szy, pvx, pvy;
	if( x1 < x2 )
	{
		x = x1;
		szx = x2 - x1;
	}
	else
	{
		x = x2;
		szx = x1 - x2;
	}
	if( y1 < y2 )
	{
		y = y1;
		szy = y2 - y1;
	}
	else
	{
		y = y2;
		szy = y1 - y2;
	}
	pvx = x3 - x;
	pvy = y3 - y;
	for( int i=0 ; i<60 ; i++ )
	{
		if( m_stBrush[i].szx < 0 || m_stBrush[i].szy < 0 )
		{
			m_stBrush[i].x = (char)x;
			m_stBrush[i].y = (char)y;
			m_stBrush[i].szx = (char)szx;
			m_stBrush[i].szy = (char)szy;
			m_stBrush[i].pvx = (char)pvx;
			m_stBrush[i].pvy = (char)pvy;
			m_sHowManyBrushes++;
			return TRUE;
		}
	}
	return FALSE;
}

void CSprite::DeleteBrush(int index)
{
	if( m_sHowManyBrushes < 1 ) return;
	if( index >= m_sHowManyBrushes ) return;
	m_stBrush[index].x = -1;
	m_stBrush[index].y = -1;
	m_stBrush[index].szx = -1;
	m_stBrush[index].szy = -1;
	m_stBrush[index].pvx = -1;
	m_stBrush[index].pvy = -1;
	if( index < m_sHowManyBrushes-1 )
	{
		for( int i = index ; i< m_sHowManyBrushes-1 ; i++ )
		{
			m_stBrush[i].x   = m_stBrush[i+1].x;
			m_stBrush[i].y   = m_stBrush[i+1].y;
			m_stBrush[i].szx = m_stBrush[i+1].szx;
			m_stBrush[i].szy = m_stBrush[i+1].szy;
			m_stBrush[i].pvx = m_stBrush[i+1].pvx;
			m_stBrush[i].pvy = m_stBrush[i+1].pvy;
		}
	}
	m_stBrush[m_sHowManyBrushes-1].x = -1;
	m_stBrush[m_sHowManyBrushes-1].y = -1;
	m_stBrush[m_sHowManyBrushes-1].szx = -1;
	m_stBrush[m_sHowManyBrushes-1].szy = -1;
	m_stBrush[m_sHowManyBrushes-1].pvx = -1;
	m_stBrush[m_sHowManyBrushes-1].pvy = -1;
	m_sHowManyBrushes--;
}

BOOL CSprite::IsBlankTile(short sSprFrame)
{
	if( m_bIsDCEmpty ) return FALSE;
	short sx = m_stSpd[sSprFrame].sx;
	short sy = m_stSpd[sSprFrame].sy;
	COLORREF clr;

	for( int y=0 ; y<32 ; y+=10 )
	{
		for( int x=0 ; x<32 ; x+=10 )
		{
			clr = BufferDC.GetPixel(sx+x, sy+y);
			if( GetRValue( clr ) > 1 ) return FALSE;
			if( GetGValue( clr ) > 1 ) return FALSE;
			if( GetBValue( clr ) > 1 ) return FALSE;
			//if( BufferDC.GetPixel(sx+x, sy+y) != 0 ) return FALSE;
		}
	}
	return TRUE;
}
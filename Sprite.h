// Sprite.h: interface for the CSprite class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPRITE_H__E107475F_9EFB_45E0_A26E_03F95AE045CF__INCLUDED_)
#define AFX_SPRITE_H__E107475F_9EFB_45E0_A26E_03F95AE045CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSprite  
{
public:
	CSprite(HANDLE hFileRead, short sSprID, short NthFile, char* FileName);
	virtual ~CSprite();
	BOOL MakeMemDC(CDC *pDC);
	void DrawRealSprite(CDC *pDC, short dx, short dy, short sSprFrame);
	void DrawRealObject(CDC *pDC, short dx, short dy, short sSprFrame);
	void DrawMiniSprite(CDC *pDC, short dx, short dy, short sSprFrame, short sSize);
	void DrawMiniObject(CDC *pDC, short dx, short dy, short sSprFrame, short sSize);
	void DrawBrush(CDC *pDC);
	BOOL InsertBrush(short x1, short y1, short x2, short y2, short pvx, short pvy);
	void DeleteBrush(int index);
	BOOL IsBlankTile(short sSprFrame);
	DWORD m_dwBmpStartPos;
	CDC   BufferDC;
	char  m_cFileName[16];
	int   m_iScreenX;
	int   m_iScreenY;
	int   m_iTotalFrame;
	BOOL  m_bIsDCEmpty;
	short m_sHowManyBrushes;
	short m_sSprID;
	COLORREF m_color;
	struct {
		char x;
		char y;
		char szx;
		char szy;
		char pvx;
		char pvy;
	} m_stBrush[60];

	struct {
		short sx;
		short sy;
		short szx;
		short szy;
		short pvx;
		short pvy;
	} m_stSpd[300];
	bool m_bMove[300];
};

#endif // !defined(AFX_SPRITE_H__E107475F_9EFB_45E0_A26E_03F95AE045CF__INCLUDED_)

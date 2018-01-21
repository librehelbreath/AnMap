// TileSpr.h: interface for the CTileSpr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TILESPR_H__429CB9AE_4F84_46C8_8BD4_BB22AC8D28CC__INCLUDED_)
#define AFX_TILESPR_H__429CB9AE_4F84_46C8_8BD4_BB22AC8D28CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTileSpr  
{
public:
	CTileSpr();
	virtual ~CTileSpr();
	short m_sTileSprite;
	short m_sTileSpriteFrame;
	short m_sObjectSprite;
	short m_sObjectSpriteFrame;
	bool  m_bIsMoveAllowed;
	bool  m_bIsTeleport;
	bool  m_bIsFarming;
};

#endif // !defined(AFX_TILESPR_H__429CB9AE_4F84_46C8_8BD4_BB22AC8D28CC__INCLUDED_)

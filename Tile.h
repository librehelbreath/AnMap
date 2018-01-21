// Tile.h: interface for the CTile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TILE_H__7FDCCB0E_111E_49ED_B54A_E277A9785EFA__INCLUDED_)
#define AFX_TILE_H__7FDCCB0E_111E_49ED_B54A_E277A9785EFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TileSpr.h"

class CTile  
{
public:
	CTile(short sx, short sy);
	virtual ~CTile();
	short x, y;
	CTileSpr tile;
};

#endif // !defined(AFX_TILE_H__7FDCCB0E_111E_49ED_B54A_E277A9785EFA__INCLUDED_)

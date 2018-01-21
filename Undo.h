// Undo.h: interface for the CUndo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UNDO_H__0EF3C81B_65BE_4A94_BEEB_8731C7FEB223__INCLUDED_)
#define AFX_UNDO_H__0EF3C81B_65BE_4A94_BEEB_8731C7FEB223__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Tile.h"

class CUndo  
{
public:
	CUndo();
	virtual ~CUndo();
	CTile *m_pUndoTile[DEF_MAX_MEMTILE];
	CTile *m_pRedoTile[DEF_MAX_MEMTILE];
};

#endif // !defined(AFX_UNDO_H__0EF3C81B_65BE_4A94_BEEB_8731C7FEB223__INCLUDED_)

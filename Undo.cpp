// Undo.cpp: implementation of the CUndo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AnMap.h"
#include "Undo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUndo::CUndo()
{
	for( int i=0 ; i<DEF_MAX_MEMTILE ; i++ )
	{
		m_pUndoTile[i] = NULL;
		m_pRedoTile[i] = NULL;
	}
}

CUndo::~CUndo()
{
	for( int i=0 ; i<DEF_MAX_MEMTILE ; i++ )
	{
		if( m_pUndoTile[i] != NULL )
		{
			delete m_pUndoTile[i];
			m_pUndoTile[i] = NULL;
		}
		if( m_pRedoTile[i] != NULL )
		{
			delete m_pRedoTile[i];
			m_pRedoTile[i] = NULL;
		}
	}
}

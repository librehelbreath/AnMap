// AnMapDoc.cpp : implementation of the CAnMapDoc class
//

#include "stdafx.h"
#include "AnMap.h"
#include "AnMapDoc.h"
#include "NewMap.h"
#include "RenderDialog.h"
#include "OptionDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnMapDoc

IMPLEMENT_DYNCREATE(CAnMapDoc, CDocument)

BEGIN_MESSAGE_MAP(CAnMapDoc, CDocument)
	//{{AFX_MSG_MAP(CAnMapDoc)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_MOVE_LEFT, OnMoveLeft)
	ON_COMMAND(ID_MOVE_RIGHT, OnMoveRight)
	ON_COMMAND(ID_MOVE_UP, OnMoveUp)
	ON_COMMAND(ID_MOVE_DOWN, OnMoveDown)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_VIEW_GRID, OnViewGrid)
	ON_COMMAND(ID_VIEW_TREE, OnViewTree)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GRID, OnUpdateViewGrid)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TREE, OnUpdateViewTree)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_COMMAND(ID_TOOL_CHECK_BLANK, OnCheckBlank)
	ON_COMMAND(ID_VIEW_RENDER, OnViewRender)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_TOOL_OPTION, OnToolOption)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnMapDoc construction/destruction

CAnMapDoc::CAnMapDoc()
{
	int i;
	m_pApp = (CAnMapApp*)AfxGetApp();
	SetCurrentDirectory( m_pApp->m_strWorkingFolder );
	m_bAutoSave = m_pApp->m_bAutoSave;
	m_dwAutoSaveDelay = m_pApp->m_dwAutoSaveDelay;
	m_dwCursorDelay = m_pApp->m_dwCursorDelay;
	m_dwScrollDelay = m_pApp->m_dwScrollDelay;
	m_bActivated = TRUE;
	m_hMutex = CreateMutex(NULL, FALSE, "AnMap");
	m_bGrid = FALSE;
	m_bTree = TRUE;
	m_bModified = FALSE;
	m_cWorkMode = 0;
	m_pRightView = NULL;
	m_pMiniView = NULL;
	m_pBrushView = NULL;
	m_mousept.x = 0;
	m_mousept.y = 0;
	m_iTileMode = 0;
	for( i=0 ; i < DEF_MAXSPRITES ; i++ ) m_pSprite[i] = NULL;
	m_iIndexUndo = 0;
	m_iHowManyUndo = 0;
	for( i=0 ; i < DEF_MAX_UNDO ; i++ ) m_pUndo[i] = NULL;
	m_ScrollPt.x = 0;
	m_ScrollPt.y = 0;
	m_edit1.x = m_edit1.y = m_edit2.x = m_edit2.y = -1;
	m_iSelTree = 100;
	m_iObjectIndex = 200;
	m_iSelObject = 0;
	m_sMapDataSizeX = 0;
	m_sMapDataSizeY = 0;
	//m_iBrushIndex = -1;
	m_iBrushIndex = 0;
	m_iSelBrush = 0;
	m_bWorkMode = TRUE;//TRUE이면 타일찍기 모드, FALSE이면 브러쉬 선택 모드
	if( m_pApp->m_strLastFile == "" )
	{
		m_pathname = "";
		m_filename = "Untitled.amd";
		m_sMapDataSizeX = 100;
		m_sMapDataSizeY = 100;
		SelectTerrain(0);
	}
	else
	{
		m_pathname = m_pApp->m_strLastFile;
		m_filename = m_pathname.Mid( m_pathname.ReverseFind( '\\' ) +1 );
	}
	SetTitle(m_filename);
}

CAnMapDoc::~CAnMapDoc()
{
	int i;
	HANDLE hFileBrush;
	DWORD nCount;
	short *sp;
	char *cp;
	char cBuffer[662*DEF_MAXSPRITES];//(2+6*60+20*15)
	ZeroMemory( cBuffer, sizeof(cBuffer) );
	for( i=0 ; i < DEF_MAXSPRITES ; i++ )
	{
		if( m_pSprite[i] != NULL )
		{
			cp = cBuffer + i*662;

			sp = (short *)cp;
			*sp = m_pSprite[i]->m_sHowManyBrushes;
			cp += 2;

			memcpy( cp, m_pSprite[i]->m_stBrush, 6*m_pSprite[i]->m_sHowManyBrushes );
			cp += 360;
			
			memcpy( cp, m_pSprite[i]->m_bMove, 300 );
		}
	}

	hFileBrush = CreateFile("Brush.brs", GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, NULL, NULL);
	if (hFileBrush != INVALID_HANDLE_VALUE)
		WriteFile(hFileBrush, &cBuffer, 662*DEF_MAXSPRITES, &nCount, NULL);

	if( m_pApp != NULL && m_pathname != "" )
	{
		m_pApp->WriteProfileString( "Settings", "LastFile", m_pathname );
	}
	m_pApp->WriteProfileInt( "Settings", "AutoSave", m_bAutoSave );
	m_pApp->WriteProfileInt( "Settings", "AutoSaveDelay", m_dwAutoSaveDelay );
	m_pApp->WriteProfileInt( "Settings", "Cursor", m_dwCursorDelay );
	m_pApp->WriteProfileInt( "Settings", "ScrollDelay", m_dwScrollDelay );

	for( i=0 ; i < DEF_MAXSPRITES ; i++ )
	{
		if( m_pSprite[i] != NULL ) delete m_pSprite[i];
	}
	for( i=0 ; i < DEF_MAX_UNDO ; i++ )
	{
		if( m_pUndo[i] != NULL ) delete m_pUndo[i];
	}
	ReleaseMutex(m_hMutex);
	CloseHandle(m_hMutex);
}

void CAnMapDoc::SetRightView( CAnMapView* pView )
{
	HANDLE hFileRead;
	DWORD nCount;
	m_pRightView = pView;
	MakeSprite( "maptiles1",       0, 32 );
	hFileRead = CreateFile("sprites\\structures1.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if( hFileRead != INVALID_HANDLE_VALUE )
	{
		m_pSprite[51] = new class CSprite(hFileRead, 51,  1, "structures1");
		m_pSprite[55] = new class CSprite(hFileRead, 55,  5, "structures1");
		CloseHandle(hFileRead);
	}
	MakeSprite( "Sinside1",			70, 27 );
	MakeSprite( "trees1",			100, 46 );
//	MakeSprite( "objects1",			200, 8 );//200,10
	MakeSprite( "objects1",			200, 10 );//200,10
//	MakeSprite( "objects5",			201, 7 );
	MakeSprite( "objects2",			211, 5 );
	MakeSprite( "objects3",			216, 4 );
	MakeSprite( "objects4",			220, 1 );
	MakeSprite( "Tile223-225",		223, 3 );
	MakeSprite( "Tile226-229",		226, 4 );
	MakeSprite( "maptiles2",		300, 15 );
	MakeSprite( "maptiles4",		320, 10 );
	MakeSprite( "maptiles5",		330, 19 );
	MakeSprite( "maptiles6",		349, 4 );
	MakeSprite( "maptiles353-361",	353, 9 );
	MakeSprite( "Tile363-366",		363, 4 );
	MakeSprite( "Tile370-381",		370, 12 );
	MakeSprite( "Tile382-387",		382, 6 );
	MakeSprite( "Tile388-402",		388, 15 );
	MakeSprite( "Tile403-405",		403, 3 );
	MakeSprite( "Tile406-421",		406, 16 );
	MakeSprite( "Tile422-429",		422, 8 );
	MakeSprite( "Tile430-443",		430, 14 );
	MakeSprite( "Tile444-444",		444, 1 );
	MakeSprite( "Tile445-461",		445, 17 );
	//MakeSprite( "Tile462-464",		462, 3 );
	MakeSprite( "Tile462-475",		462, 14 );

	hFileRead = CreateFile("Brush.brs", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if( hFileRead != INVALID_HANDLE_VALUE )
	{
		for( int i=0 ; i < DEF_MAXSPRITES ; i++ )
		{
			if( m_pSprite[i] != NULL )
			{
				SetFilePointer(hFileRead, 662*i, NULL, FILE_BEGIN);
				ReadFile(hFileRead, &(m_pSprite[i]->m_sHowManyBrushes),  2, &nCount, NULL);
				ReadFile(hFileRead, &(m_pSprite[i]->m_stBrush), 6*(m_pSprite[i]->m_sHowManyBrushes), &nCount, NULL );
				SetFilePointer(hFileRead, 662*i + 362, NULL, FILE_BEGIN);
				ReadFile(hFileRead, m_pSprite[i]->m_bMove, 300, &nCount, NULL);
			}
		}
		CloseHandle( hFileRead );
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAnMapDoc diagnostics

#ifdef _DEBUG
void CAnMapDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAnMapDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAnMapDoc commands

BOOL CAnMapDoc::_bDecodeMapInfo(char * pHeader)
{
 int i;
 char * token, cReadMode;
 char seps[] = "= ,\t\n";
 BOOL bCheck1, bCheck2;
 bCheck1 = FALSE;
 bCheck2 = FALSE;

	// 먼저 널문자를 없앤다.
	for (i = 0; i < 256; i++) 
		if (pHeader[i] == NULL) pHeader[i] = ' ';

	cReadMode = 0;

	token = strtok( pHeader, seps );
	while( token != NULL )
	{
		if (cReadMode != 0)
		{
			switch (cReadMode)
			{
			case 1:
				m_sMapDataSizeX = atoi(token);
				cReadMode = 0;
				bCheck1 = TRUE;
				break;
			case 2:
				m_sMapDataSizeY = atoi(token);
				cReadMode = 0;
				bCheck2 = TRUE;
				break;
			}
		}	
		else
		{
			if (memcmp(token, "MAPSIZEX",8) == 0) cReadMode = 1;
			if (memcmp(token, "MAPSIZEY",8) == 0) cReadMode = 2;
		}
		token = strtok( NULL, seps );
	}
	if( bCheck1 && bCheck2 ) return TRUE;
	else return FALSE;
}

void CAnMapDoc::OnFileOpen() 
{
	if( m_bModified == TRUE )
	{
		int res = MessageBox( m_pRightView->m_hWnd, "Map has been modified, would you like to save it?", "Notice!", MB_YESNOCANCEL | MB_ICONQUESTION );
		if( res == IDCANCEL ) return;
		else if( res == IDYES ) SaveFileAs();
	}
	CFileDialog dlg( TRUE, "amd", m_filename, OFN_FILEMUSTEXIST, "Map Files (*.amd)|*.amd|", NULL );
	char cDir[256];
	GetCurrentDirectory( sizeof(cDir), cDir );
	if( dlg.DoModal() != IDOK ) return;
	m_filename = dlg.GetFileName();
	m_pathname = dlg.GetPathName();
	SetTitle(m_filename);
	SetCurrentDirectory(cDir);
	if( bOpenMapDataFile( dlg.GetPathName() ) == FALSE )
	{
		m_sMapDataSizeX = 0;
		m_sMapDataSizeY = 0;
		return;
	}
	if( m_pMiniView != NULL )
	{
		m_pMiniView->LoadMinimap();
	}
	if( m_pRightView != NULL )
	{
		m_ScrollPt.x = 0;
		m_ScrollPt.y = 0;
		m_pRightView->SetScrollSizes(MM_TEXT, CSize(m_sMapDataSizeX<<5, m_sMapDataSizeY<<5));
		m_bWorkMode = TRUE;
	}

	m_iHowManyUndo = 0;
	m_iIndexUndo = 0;
	for( int i=0 ; i < DEF_MAX_UNDO ; i++ )
	{
		if( m_pUndo[i] != NULL )
		{
			delete m_pUndo[i];
			m_pUndo[i] = NULL;
		}
	}
	m_bModified = FALSE;
	UpdateScreen();
}

void CAnMapDoc::SetBrushView( CBrushView* pView )
{
	m_pBrushView = pView;
	if( m_pApp->m_strLastFile == "" ) return;
	m_pathname = m_pApp->m_strLastFile;
	m_filename = m_pathname.Mid( m_pathname.ReverseFind( '\\' ) +1 );
	SetTitle(m_filename);
	if( bOpenMapDataFile( m_pathname ) == FALSE )
	{
		m_sMapDataSizeX = 0;
		m_sMapDataSizeY = 0;
		return;
	}
	if( m_pMiniView != NULL )
	{
		m_pMiniView->LoadMinimap();
	}
	if( m_pRightView != NULL )
	{
		m_ScrollPt.x = 0;
		m_ScrollPt.y = 0;
		m_pRightView->SetScrollSizes(MM_TEXT, CSize(m_sMapDataSizeX<<5, m_sMapDataSizeY<<5));
		m_bWorkMode = TRUE;
	}
}

void CAnMapDoc::SetMiniView( CMiniView* pView )
{
	m_pMiniView = pView;
}

void CAnMapDoc::MakeSprite( char* FileName, short sStart, short sCount)
{
	char PathName[28];
	int  iTotalimage;
	DWORD nCount;
	wsprintf( PathName, "sprites\\%s.pak", FileName );
	HANDLE hFileRead = CreateFile(PathName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if( hFileRead == INVALID_HANDLE_VALUE )
	{
#ifdef _DEBUG
		MessageBox( NULL, "Tile Not Found", NULL, MB_OK );
#endif
		return;
	}
	SetFilePointer(hFileRead, 20, NULL, FILE_BEGIN);
	ReadFile(hFileRead, (char *)&iTotalimage, 4, &nCount, NULL);
	for( short i=0 ; i < sCount ; i++ )
	{
		if( i < iTotalimage ) m_pSprite[i+sStart] = new class CSprite(hFileRead, i+sStart, i, FileName);
	}
	CloseHandle(hFileRead);
}

void CAnMapDoc::OnKeyUp(UINT nChar)
{
	switch( nChar )
	{
	case 77: //'M'
		{
			m_cWorkMode++;
			if( m_bWorkMode )
			{
				if( m_cWorkMode > 4 ) m_cWorkMode = 0;
				m_edit1.x = m_edit1.y = m_edit2.x = m_edit2.y = -1;
			}
			else
			{
				if( m_cWorkMode > 1 ) m_cWorkMode = 0;
			}
			UpdateScreen();
		}
		break;
	case 66: //'B'
		{
			if( m_bWorkMode )
			{
				m_cWorkMode = 0;
				OnBrushMode( m_iBrushIndex );
			}
			else 
			{
				m_bWorkMode = TRUE;
				m_cWorkMode = 0;
				m_pRightView->SetScrollSizes(MM_TEXT, CSize(m_sMapDataSizeX*DEF_TILESIZE, m_sMapDataSizeY*DEF_TILESIZE));
				m_iSelBrush = -1;
				UpdateScreen();
			}
		}
		break;
	case 69: //'E'
		{
		}
		break;
	default:
		break;
	}
}

void CAnMapDoc::OnKeyDown(UINT nChar)
{
	int i;
	if( m_bWorkMode == FALSE )
	{
		int iBrushIndex;
		if( m_iTileMode == 0 )
		{
			switch( nChar )
			{
			case 88: //'X'
				{
					if( (m_iBrushIndex<0) || ( (m_iBrushIndex>=100) && (m_iBrushIndex<300) ) || (m_iBrushIndex>=DEF_MAXSPRITES) ) m_iBrushIndex = 0;
					for( i=0; i < DEF_MAXSPRITES-200 ; i++ )
					{
						iBrushIndex = m_iBrushIndex + i + 1;
						if( iBrushIndex < 0 ) iBrushIndex += DEF_MAXSPRITES;
						if( (iBrushIndex>=100) && (iBrushIndex<300) ) iBrushIndex+=200;
						if( iBrushIndex >= DEF_MAXSPRITES ) iBrushIndex -= DEF_MAXSPRITES;
						if( m_pSprite[iBrushIndex] != NULL )
						{
							OnBrushMode( iBrushIndex );
							return;
						}
					}
				}
				break;
			case 90: //'Z'
				{
					if( (m_iBrushIndex<0) || ( (m_iBrushIndex>=100) && (m_iBrushIndex<300) ) || (m_iBrushIndex>=DEF_MAXSPRITES) ) m_iBrushIndex = 0;
					for( int i=0; i < DEF_MAXSPRITES-200 ; i++ )
					{
						iBrushIndex = m_iBrushIndex - i - 1;
						if( iBrushIndex >= DEF_MAXSPRITES ) iBrushIndex -= DEF_MAXSPRITES;
						if( (iBrushIndex>=100) && (iBrushIndex<300) ) iBrushIndex-=200;
						if( iBrushIndex < 0 ) iBrushIndex += DEF_MAXSPRITES;
						if( m_pSprite[iBrushIndex] != NULL )
						{
							OnBrushMode( iBrushIndex );
							return;
						}
					}
				}
				break;
			default:
				return;
			}
		}
		else if( m_iTileMode == 2 )
		{
			switch( nChar )
			{
			case 88: //'X'
				{
					if( m_iObjectIndex < 200 || m_iObjectIndex >= 300 ) m_iObjectIndex = 200;
					for( i=0 ; i < 100 ; i++ )
					{
						iBrushIndex = m_iObjectIndex + i + 1;

						if( iBrushIndex < 200 ) iBrushIndex += 100;
						if( iBrushIndex >= 300 ) iBrushIndex -= 100;

						if( m_pSprite[iBrushIndex] != NULL )
						{
							m_iObjectIndex = iBrushIndex;
							m_pRightView->UpdateScreen();
							return;
						}
					}
				}
				break;
			case 90: //'Z'
				{
					if( m_iObjectIndex < 200 || m_iObjectIndex >= 300 ) m_iObjectIndex = 200;
					for( i=0 ; i < 100 ; i++ )
					{
						iBrushIndex = m_iObjectIndex - i - 1;

						if( iBrushIndex < 200 ) iBrushIndex += 100;
						if( iBrushIndex >= 300 ) iBrushIndex -= 100;

						if( m_pSprite[iBrushIndex] != NULL )
						{
							m_iObjectIndex = iBrushIndex;
							m_pRightView->UpdateScreen();
							return;
						}
					}
				}
				break;
			}
		}
		return;
	}
	else if( m_bWorkMode == TRUE )
	{
		switch( nChar )
		{
		case 84: //'T'
			if( m_iTileMode == 1 ) m_iTileMode = 0;
			else m_iTileMode = 1;
			break;
		case 72: //'H'
			if( m_iTileMode == 2 ) m_iTileMode = 0;
			else m_iTileMode = 2;
			break;
		case 88: //'X'
			if( m_iTileMode == 0 ) SelectBrush( 1 );
			else ChangeBrush( 1 );
			break;
		case 90: //'Z'
			if( m_iTileMode == 0 ) SelectBrush( -1 );
			else ChangeBrush( -1 );
			break;
		case VK_RIGHT:
		case 68: //'D'
			m_ScrollPt.x++;
			SelectBrush(0);
			break;
		case 65: //'A'
		case VK_LEFT:
			m_ScrollPt.x--;
			SelectBrush(0);
			break;
		case 87: //'W'
		case VK_UP:
			m_ScrollPt.y--;
			SelectBrush(0);
			break;
		case 83: //'S'
		case VK_DOWN:
			m_ScrollPt.y++;
			SelectBrush(0);
			break;
		case VK_NEXT:
			m_ScrollPt.y+=m_pRightView->m_ptClipping.y-1;
			SelectBrush(0);
			break;
		case VK_PRIOR:
			m_ScrollPt.y-=m_pRightView->m_ptClipping.y-1;
			SelectBrush(0);
			break;
		default:
			return;
		}
		UpdateScreen();
	}
}

void CAnMapDoc::OnFileSaveAs() 
{
	SaveFileAs();
}

void CAnMapDoc::SaveFileAs() 
{
	char cDir[256];
	GetCurrentDirectory( sizeof(cDir), cDir );
	CFileDialog dlg( FALSE, "amd", m_filename, OFN_OVERWRITEPROMPT, "Map Files (*.amd)|*.amd|", NULL );
	if( dlg.DoModal() != IDOK ) return;
	m_pathname = dlg.GetPathName();
	m_filename = dlg.GetFileName();
	SetTitle(m_filename);
	SetCurrentDirectory(cDir);
	SaveMapData(m_pathname);
	m_bModified = FALSE;
	for( int i=0 ; i < DEF_MAX_UNDO ; i++ )
	{
		if( m_pUndo[i] != NULL )
		{
			delete m_pUndo[i];
			m_pUndo[i] = NULL;
		}
	}
	m_iHowManyUndo = 0;
	m_iIndexUndo = 0;
}

void CAnMapDoc::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( TRUE );
}

void CAnMapDoc::SaveMapData(CString cFn)
{
	HANDLE hFileWrite;
	char cHeader[256];
	DWORD nCount;
	hFileWrite = CreateFile(cFn, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, NULL, NULL);
	if (hFileWrite == INVALID_HANDLE_VALUE) return;
	ZeroMemory( cHeader, sizeof(cHeader) );
	wsprintf( cHeader, "MAPSIZEX =  %d MAPSIZEY =  %d TILESIZE = 10", m_sMapDataSizeX, m_sMapDataSizeY );
	SetFilePointer( hFileWrite, 0, NULL, FILE_BEGIN);
	WriteFile( hFileWrite, cHeader, 256, &nCount, NULL);
	char* cpMapData = new char[m_sMapDataSizeX*m_sMapDataSizeY*10];
	char *cp;
	short *sp;
	cp = cpMapData;
	for( int y=0 ; y<m_sMapDataSizeY ; y++ )
	{
		for( int x=0 ; x<m_sMapDataSizeX ; x++ )
		{
			sp = (short *)cp;
			*sp = m_tile[x][y].m_sTileSprite;
			cp += 2;

			sp = (short *)cp;
			*sp = m_tile[x][y].m_sTileSpriteFrame;
			cp += 2;

			sp = (short *)cp;
			*sp = m_tile[x][y].m_sObjectSprite;
			cp += 2;

			sp = (short *)cp;
			*sp = m_tile[x][y].m_sObjectSpriteFrame;
			cp += 2;

			if( m_tile[x][y].m_bIsMoveAllowed ) *cp = 0;
			else *cp = -128;//0x80;
			if( m_tile[x][y].m_bIsTeleport ) *cp = 64;//0x40;
			else if( m_tile[x][y].m_bIsFarming ) *cp = 32;//0x40;
			cp += 2;
		}
	}
	WriteFile( hFileWrite, cpMapData, m_sMapDataSizeX*m_sMapDataSizeY*10, &nCount, NULL);
	delete[] cpMapData;
	CloseHandle(hFileWrite);
}

void CAnMapDoc::UpdateScreen()
{
	if( m_pRightView != NULL ) m_pRightView->UpdateScreen();
	if( m_pMiniView  != NULL ) m_pMiniView->UpdateScreen();
}

BOOL CAnMapDoc::bOpenMapDataFile(CString cFn)
{
	char cHeader[256];
	DWORD nCount;
	int i;
	ZeroMemory( cHeader, sizeof(cHeader) );
	HANDLE hFileRead = CreateFile(cFn, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (hFileRead == INVALID_HANDLE_VALUE) return FALSE;
	ReadFile( hFileRead, cHeader, 256, &nCount, NULL );
	if( _bDecodeMapInfo(cHeader) == FALSE ) {
		CloseHandle(hFileRead);
		return FALSE;
	}
	char* cpMapData = new char[m_sMapDataSizeX*m_sMapDataSizeY*10];
	ReadFile( hFileRead, cpMapData, m_sMapDataSizeX*m_sMapDataSizeY*10, &nCount, NULL );
	CloseHandle(hFileRead);
	for( i=0 ; i < DEF_MAX_UNDO ; i++ )
	{
		if( m_pUndo[i] != NULL )
		{
			delete m_pUndo[i];
			m_pUndo[i] = NULL;
		}
	}
	m_iHowManyUndo = 0;
	m_iIndexUndo = 0;
	char *cp = cpMapData;
	short *sp;
	for (int y=0 ; y<m_sMapDataSizeY ; y++ )
	{
		for(int x=0 ; x<m_sMapDataSizeX ; x++ )
		{
			sp = (short *)cp;
			m_tile[x][y].m_sTileSprite        = *sp;
			cp += 2;
			sp = (short *)cp;
			m_tile[x][y].m_sTileSpriteFrame   = *sp;
			cp += 2;
			sp = (short *)cp;
			m_tile[x][y].m_sObjectSprite      = *sp;
			cp += 2;
			sp = (short *)cp;
			m_tile[x][y].m_sObjectSpriteFrame = *sp;
			cp += 2;
			if (((*cp) & 0x80) != 0)
				 m_tile[x][y].m_bIsMoveAllowed = FALSE;
			else m_tile[x][y].m_bIsMoveAllowed = TRUE;
			if (((*cp) & 0x40) != 0)
				 m_tile[x][y].m_bIsTeleport = TRUE;
			else m_tile[x][y].m_bIsTeleport = FALSE;
			if (((*cp) & 0x20) != 0)
				 m_tile[x][y].m_bIsFarming = TRUE;
			else m_tile[x][y].m_bIsFarming = FALSE;
			cp += 2;
		}
	}
	delete[] cpMapData;
	return TRUE;
}

void CAnMapDoc::OnBrushMode(int iBrushIndex)
{	
	if( m_iTileMode == 0 )
	{
		m_bWorkMode = FALSE;
		m_iBrushIndex = iBrushIndex;
		if( m_iBrushIndex < 0 ) m_iBrushIndex = 0;
		m_iBrushMode = 0;
		m_iSelBrush = -1;
		m_pRightView->SetScrollSizes(MM_TEXT, CSize(640, 480));
		POINT pt;
		pt.x = 0;
		pt.y = 0;
		m_pRightView->ScrollToPosition(pt);
		m_pBrushView->SetSelectedItem( m_iBrushIndex );
		m_pRightView->BufferDC.BitBlt( 0, 0, m_pRightView->m_rtClient.right, m_pRightView->m_rtClient.bottom, NULL, 0, 0, BLACKNESS );
		if( m_pSprite[m_iBrushIndex] == NULL ) return;
		m_pSprite[m_iBrushIndex]->DrawBrush(&(m_pRightView->BufferDC));
		m_pRightView->UpdateScreen();
	}
	else if( m_iTileMode == 2 )
	{
		m_bWorkMode = FALSE;
		m_pRightView->SetScrollSizes(MM_TEXT, CSize(640, 480));
		POINT pt;
		pt.x = 0;
		pt.y = 0;
		m_pRightView->ScrollToPosition(pt);
		m_pRightView->BufferDC.BitBlt( 0, 0, m_pRightView->m_rtClient.right, m_pRightView->m_rtClient.bottom, NULL, 0, 0, BLACKNESS );
		m_pSprite[m_iObjectIndex]->DrawBrush(&(m_pRightView->BufferDC));
		m_pRightView->UpdateScreen();
	}
}

void CAnMapDoc::PutTile( BOOL bMakeUndo )
{
	if( m_iBrushIndex < 0 ) return;
	if( m_pSprite[m_iBrushIndex] == NULL ) return;
	short sFrame, x, y;
	bool bMove;
	int i;
	CUndo* pUndo;
	if( bMakeUndo ) pUndo = MakeUndo();
	else
	{
		if( m_iIndexUndo < 1 ) return;
		pUndo = m_pUndo[m_iIndexUndo-1];
	}
	if( pUndo == NULL ) return;
	x = m_mousept.x + m_ScrollPt.x;
	y = m_mousept.y + m_ScrollPt.y;
	x = x - x%6 - m_pSprite[m_iBrushIndex]->m_stBrush[m_iSelBrush].pvx - m_iTempX*6;
	y = y - y%4 - m_pSprite[m_iBrushIndex]->m_stBrush[m_iSelBrush].pvy - m_iTempY*4;
	for( int iy=0 ; iy<=m_pSprite[m_iBrushIndex]->m_stBrush[m_iSelBrush].szy ; iy++ )
	{
		if( (y+iy) >= m_sMapDataSizeY ) continue;
		if( (y+iy) < 0 ) continue;
		for( int ix=0 ; ix<=m_pSprite[m_iBrushIndex]->m_stBrush[m_iSelBrush].szx ; ix++ )
		{
			if( (x+ix) < 0 ) continue;
			if( (x+ix) >= m_sMapDataSizeX ) continue;
			sFrame = (m_pSprite[m_iBrushIndex]->m_stBrush[m_iSelBrush].y + iy)*20 + m_pSprite[m_iBrushIndex]->m_stBrush[m_iSelBrush].x + ix;
			bMove = m_pSprite[m_iBrushIndex]->m_bMove[(m_pSprite[m_iBrushIndex]->m_stBrush[m_iSelBrush].y + iy)*20 + m_pSprite[m_iBrushIndex]->m_stBrush[m_iSelBrush].x + ix];
			if( m_pSprite[m_iBrushIndex]->IsBlankTile( sFrame ) == FALSE )
			{
				for( i = 0 ; i < DEF_MAX_MEMTILE ; i++ )
				{
					if( (pUndo->m_pUndoTile[i] == NULL) && (pUndo->m_pUndoTile[i] == NULL) )
					{
						pUndo->m_pUndoTile[i] = new class CTile(x+ix, y+iy);
						CopyTile( &(pUndo->m_pUndoTile[i]->tile), &(m_tile[x+ix][y+iy]) );

						m_tile[x+ix][y+iy].m_sTileSprite = m_iBrushIndex;
						m_tile[x+ix][y+iy].m_sTileSpriteFrame = sFrame;
						m_tile[x+ix][y+iy].m_sObjectSprite = 0;
						m_tile[x+ix][y+iy].m_sObjectSpriteFrame = 0;
						m_tile[x+ix][y+iy].m_bIsTeleport = FALSE;
						m_tile[x+ix][y+iy].m_bIsFarming = FALSE;
						if( bMove ) m_tile[x+ix][y+iy].m_bIsMoveAllowed = false;
						else m_tile[x+ix][y+iy].m_bIsMoveAllowed = true;

						pUndo->m_pRedoTile[i] = new class CTile(x+ix, y+iy);
						CopyTile( &(pUndo->m_pRedoTile[i]->tile), &(m_tile[x+ix][y+iy]) );
						break;
					}
				}
			}
		}
	}
	m_pMiniView->DrawMinimap( x, y );
	SetModify();
	UpdateScreen();
}

void CAnMapDoc::PutTree( BOOL bMakeUndo )
{
	if( m_iSelTree < 100 || m_iSelTree >= 200 ) return;
	if( m_pSprite[m_iSelTree] == NULL ) return;
	int i;
	short x = m_mousept.x + m_ScrollPt.x;
	short y = m_mousept.y + m_ScrollPt.y;
	CUndo* pUndo;
	if( bMakeUndo ) pUndo = MakeUndo();
	else
	{
		if( m_iIndexUndo < 1 ) return;
		pUndo = m_pUndo[m_iIndexUndo-1];
	}
	if( pUndo == NULL ) return;
	for( i = 0 ; i < DEF_MAX_MEMTILE ; i++ )
	{
		if( (pUndo->m_pUndoTile[i] == NULL) && (pUndo->m_pUndoTile[i] == NULL) )
		{
			pUndo->m_pUndoTile[i] = new class CTile(x, y);
			CopyTile( &(pUndo->m_pUndoTile[i]->tile), &(m_tile[x][y]) );
			m_tile[x][y].m_sObjectSprite = m_iSelTree;
			m_tile[x][y].m_sObjectSpriteFrame = 0;
			m_tile[x][y].m_bIsMoveAllowed = FALSE;
			m_tile[x][y].m_bIsTeleport = FALSE;
			m_tile[x][y].m_bIsFarming = FALSE;
			pUndo->m_pRedoTile[i] = new class CTile(x, y);
			CopyTile( &(pUndo->m_pRedoTile[i]->tile), &(m_tile[x][y]) );
			break;
		}
	}
	m_pMiniView->DrawMinimap( x, y );
	SetModify();
	UpdateScreen();
}

void CAnMapDoc::PutObject( BOOL bMakeUndo )
{
	if( m_iObjectIndex < 200 || m_iObjectIndex >= 300 ) return;
	if( m_pSprite[m_iObjectIndex] == NULL ) return;
	if( m_pSprite[m_iObjectIndex]->m_iTotalFrame <= m_iSelObject ) return;
	int i;
	CUndo* pUndo;
	if( bMakeUndo ) pUndo = MakeUndo();
	else
	{
		if( m_iIndexUndo < 1 ) return;
		pUndo = m_pUndo[m_iIndexUndo-1];
	}
	if( pUndo == NULL ) return;
	short x = m_mousept.x + m_ScrollPt.x;
	short y = m_mousept.y + m_ScrollPt.y;
	for( i = 0 ; i < DEF_MAX_MEMTILE ; i++ )
	{
		if( (pUndo->m_pUndoTile[i] == NULL) && (pUndo->m_pUndoTile[i] == NULL) )
		{
			pUndo->m_pUndoTile[i] = new class CTile(x, y);
			CopyTile( &(pUndo->m_pUndoTile[i]->tile), &(m_tile[x][y]) );
			m_tile[x][y].m_sObjectSprite = m_iObjectIndex;
			m_tile[x][y].m_sObjectSpriteFrame = m_iSelObject;
			m_tile[x][y].m_bIsMoveAllowed = FALSE;
			m_tile[x][y].m_bIsTeleport = FALSE;
			m_tile[x][y].m_bIsFarming = FALSE;
			pUndo->m_pRedoTile[i] = new class CTile(x, y);
			CopyTile( &(pUndo->m_pRedoTile[i]->tile), &(m_tile[x][y]) );
			break;
		}
	}
	m_pMiniView->DrawMinimap( x, y );
	SetModify();
	UpdateScreen();
}

void CAnMapDoc::OnFileSave() 
{
	if( m_pathname == "" ) SaveFileAs();
	else
	{
		SaveMapData(m_pathname);
		m_bModified = FALSE;
		for( int i=0 ; i < DEF_MAX_UNDO ; i++ )
		{
			if( m_pUndo[i] != NULL )
			{
				delete m_pUndo[i];
				m_pUndo[i] = NULL;
			}
		}
		m_iHowManyUndo = 0;
		m_iIndexUndo = 0;
		SetTitle(m_filename);
	}
}

void CAnMapDoc::OnMoveLeft() 
{
	if( m_bWorkMode == TRUE )
	{
		m_ScrollPt.x-=3;
		UpdateScreen();
	}
}

void CAnMapDoc::OnMoveRight() 
{
	if( m_bWorkMode == TRUE )
	{
		m_ScrollPt.x+=3;
		UpdateScreen();
	}
}

void CAnMapDoc::OnMoveUp() 
{
	if( m_bWorkMode == TRUE )
	{
		m_ScrollPt.y-=3;
		UpdateScreen();
	}
}

void CAnMapDoc::OnMoveDown() 
{
	if( m_bWorkMode == TRUE )
	{
		m_ScrollPt.y+=3;
		UpdateScreen();
	}
}

void CAnMapDoc::OnFileNew() 
{
	if( m_bModified == TRUE )
	{
		int res = MessageBox( m_pRightView->m_hWnd, "Map has been modified, would you like to save it?", "Notice!", MB_YESNOCANCEL | MB_ICONQUESTION);
		if( res == IDCANCEL ) return;
		else if( res == IDYES ) SaveFileAs();
	}
	CNewMap dlg(m_sMapDataSizeX, m_sMapDataSizeY);
	if( dlg.DoModal() == IDCANCEL ) return;

//	CString szTemp;
//	szTemp.Format("%d", dlg.m_iTerrain);
//	AfxMessageBox(szTemp);
//	return;


	int i;
	for( i=0 ; i < DEF_MAX_UNDO ; i++ )
	{
		if( m_pUndo[i] != NULL )
		{
			delete m_pUndo[i];
			m_pUndo[i] = NULL;
		}
	}
	m_iHowManyUndo = 0;
	m_iIndexUndo = 0;
	m_sMapDataSizeX = dlg.m_sMapSizeX;
	m_sMapDataSizeY = dlg.m_sMapSizeY;
	m_edit1.x = m_edit1.y = m_edit2.x = m_edit2.y = 0;
	SelectTerrain(dlg.m_iTerrain);
	if( m_pMiniView != NULL ) m_pMiniView->LoadMinimap();
	if( m_pRightView != NULL )
	{
		m_ScrollPt.x = 0;
		m_ScrollPt.y = 0;
		m_pRightView->SetScrollSizes(MM_TEXT, CSize(m_sMapDataSizeX<<5, m_sMapDataSizeY<<5));
		m_filename = "Untitled.amd";
		m_pathname = "";
		SetTitle(m_filename);
	}
	UpdateScreen();
}

void CAnMapDoc::OnViewGrid() 
{
	m_bGrid = !m_bGrid;
	UpdateScreen();
}

void CAnMapDoc::OnViewTree() 
{
	m_bTree = !m_bTree;
	UpdateScreen();
}

void CAnMapDoc::OnUpdateViewGrid(CCmdUI* pCmdUI) 
{
	if(m_bGrid) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}

void CAnMapDoc::OnUpdateViewTree(CCmdUI* pCmdUI) 
{
	if(m_bTree) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}

BOOL CAnMapDoc::IsBrushMatched( int i, short mx, short my )
{
	int tx, ty, tx1, tx2, ty1, ty2;
	for( ty = 0; ty <= m_pSprite[m_iBrushIndex]->m_stBrush[i].szy ; ty++ )
	{
		for( tx = 0; tx <= m_pSprite[m_iBrushIndex]->m_stBrush[i].szx ; tx++ )
		{
			tx1 = (tx - m_pSprite[m_iBrushIndex]->m_stBrush[i].pvx)%6;
			tx2 = (tx - m_pSprite[m_iBrushIndex]->m_stBrush[i].pvx)/6;
			ty1 = (ty - m_pSprite[m_iBrushIndex]->m_stBrush[i].pvy)%4;
			ty2 = (ty - m_pSprite[m_iBrushIndex]->m_stBrush[i].pvy)/4;
			if( tx1 < 0 )
			{
				tx2 --;
				tx1 = tx1+6;
			}
			if( ty1 < 0 )
			{
				ty2 --;
				ty1 = ty1+4;
			}
			if( (tx1==mx) && (ty1==my) )
			{
				m_iSelBrush = i;
				m_iTempX = tx2;
				m_iTempY = ty2;
				return TRUE;
			}
		}
	}
	return FALSE;
}

void CAnMapDoc::SelectBrush(int iDir)
{
	int i, px, py, index, iMaxbrush;
	if( m_iBrushIndex < 0 ) return;
	if( m_iBrushIndex >= DEF_MAXSPRITES ) return;
	if( m_iTileMode != 0 ) return;
	if( m_pSprite[m_iBrushIndex] == NULL ) return;
	px = (m_ScrollPt.x + m_mousept.x)%6;
	py = (m_ScrollPt.y + m_mousept.y)%4;
	iMaxbrush = m_pSprite[m_iBrushIndex]->m_sHowManyBrushes;
	if( m_iSelBrush < 0 || m_iSelBrush >= iMaxbrush ) m_iSelBrush = 0;
	if( iDir == 0 )
	{
		for( i=0 ; i < iMaxbrush ; i++ )
		{
			index = m_iSelBrush + i;
			if( index < 0 ) index += iMaxbrush;
			if( index >= iMaxbrush ) index -= iMaxbrush;
			if( (m_pSprite[m_iBrushIndex]->m_stBrush[m_iSelBrush].x + m_pSprite[m_iBrushIndex]->m_stBrush[m_iSelBrush].pvx) == (m_pSprite[m_iBrushIndex]->m_stBrush[index].x + m_pSprite[m_iBrushIndex]->m_stBrush[index].pvx) &&
				(m_pSprite[m_iBrushIndex]->m_stBrush[m_iSelBrush].y + m_pSprite[m_iBrushIndex]->m_stBrush[m_iSelBrush].pvy) == (m_pSprite[m_iBrushIndex]->m_stBrush[index].y + m_pSprite[m_iBrushIndex]->m_stBrush[index].pvy) &&
				(m_pSprite[m_iBrushIndex]->m_stBrush[index].szx + m_pSprite[m_iBrushIndex]->m_stBrush[index].szy < 4 ) )
			{
				if( IsBrushMatched( index, px, py ) ) return;
			}
		}
	}
	for( i=0 ; i < iMaxbrush ; i++ )
	{
		if( iDir < 0 ) index = m_iSelBrush - i + iDir;
		else index = m_iSelBrush + i + iDir;

		if( index < 0 ) index += iMaxbrush;
		if( index >= iMaxbrush ) index -= iMaxbrush;

		if( IsBrushMatched( index, px, py ) ) return;
	}
	m_iSelBrush = -1;
}

void CAnMapDoc::ChangeBrush(int iDir)
{
	int i;
	if( m_iTileMode == 1 )
	{
		int index;
		if( m_iSelTree < 100 || m_iSelTree >= 200 ) m_iSelTree = 100;
		for( i=0 ; i < 100 ; i++ )
		{
			index = m_iSelTree + iDir*i + iDir;
			if( index < 100 ) index += 100;
			if( index >= 200 ) index -= 100;
			if( m_pSprite[index] != NULL )
			{
				m_iSelTree = index;
				return;
			}
		}
		m_iSelTree = 99;
		return;
	}
	else if( m_iTileMode == 2 )
	{
		m_iSelObject += iDir;
		if( m_iSelObject < 0 ) m_iSelObject = m_pSprite[m_iObjectIndex]->m_iTotalFrame-1;
		if( m_iSelObject > m_pSprite[m_iObjectIndex]->m_iTotalFrame-1 ) m_iSelObject = 0;
	}
}

void CAnMapDoc::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	if( m_iIndexUndo == 0 )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	if( m_pUndo[m_iIndexUndo-1] == NULL )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	pCmdUI->Enable(TRUE);	
}

void CAnMapDoc::OnEditUndo() 
{
	if( m_iIndexUndo == 0 ) return;
	if( m_pUndo[m_iIndexUndo-1] == NULL ) return;
	int i, iHowManyUndo;
	iHowManyUndo = 0;
	m_iIndexUndo--;
	CTile *pTile;
	for( i=DEF_MAX_MEMTILE-1 ; i>=0 ; i-- )
	{
		pTile = m_pUndo[m_iIndexUndo]->m_pUndoTile[i];
		if( pTile != NULL )
		{
			CopyTile( &(m_tile[pTile->x][pTile->y]), &(pTile->tile) );
			iHowManyUndo++;
		}
	}
	if( iHowManyUndo >= 500 ) m_pMiniView->LoadMinimap();
	else
	{
		for( i=0 ; i<DEF_MAX_MEMTILE ; i++ )
		{
			pTile = m_pUndo[m_iIndexUndo]->m_pUndoTile[i];
			if( pTile != NULL )
			{
				m_pMiniView->DrawMinimap( pTile->x, pTile->y );
				if( abs(pTile->x-m_ScrollPt.x-12) > 18 || abs( pTile->y-m_ScrollPt.y-8 ) > 12 )
				{
					m_ScrollPt.x = pTile->x - 12;
					m_ScrollPt.y = pTile->y - 8;
				}
				break;
			}
		}
	}
	if( m_iIndexUndo == 0 )
	{
		m_bModified = FALSE;
		SetTitle(m_filename);
	}
	UpdateScreen();
}

void CAnMapDoc::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	if( m_iIndexUndo >= m_iHowManyUndo )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	if( m_pUndo[m_iIndexUndo] == NULL )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	pCmdUI->Enable(TRUE);
}

void CAnMapDoc::OnEditRedo() 
{
	if( m_iIndexUndo >= m_iHowManyUndo ) return;
	if( m_pUndo[m_iIndexUndo] == NULL ) return;
	int i;
	CTile *pTile;
	for( i=0 ; i<DEF_MAX_MEMTILE ; i++ )
	{
		pTile = m_pUndo[m_iIndexUndo]->m_pRedoTile[i];
		if( pTile != NULL )
		{
			CopyTile( &(m_tile[pTile->x][pTile->y]), &(pTile->tile) );
		}
	}
	for( i=0 ; i<DEF_MAX_MEMTILE ; i++ )
	{
		pTile = m_pUndo[m_iIndexUndo]->m_pRedoTile[i];
		if( pTile != NULL )
		{
			m_pMiniView->DrawMinimap( pTile->x, pTile->y );
			if( abs(pTile->x-m_ScrollPt.x-12) > 18 || abs( pTile->y-m_ScrollPt.y-8 ) > 12 )
			{
				m_ScrollPt.x = pTile->x - 12;
				m_ScrollPt.y = pTile->y - 8;
			}
			break;
		}
	}
	SetModify();
	m_iIndexUndo++;
	UpdateScreen();
}

CUndo* CAnMapDoc::MakeUndo()
{
	int i;
	if( m_iIndexUndo < m_iHowManyUndo )
	{
		for( i=m_iIndexUndo ; i<DEF_MAX_UNDO ; i++ )
		{
			if( m_pUndo[i] != NULL )
			{
				delete m_pUndo[i];
				m_pUndo[i] = NULL;
			}
		}
		m_iHowManyUndo = m_iIndexUndo;
	}
	if( m_iIndexUndo == DEF_MAX_UNDO )
	{
		delete m_pUndo[0];
		for( i=0 ; i<DEF_MAX_UNDO-1 ; i++ )
		{
			m_pUndo[i] = m_pUndo[i+1];
		}
		m_iIndexUndo = DEF_MAX_UNDO-1;
		m_iHowManyUndo = DEF_MAX_UNDO-1;
	}
	m_iIndexUndo ++;
	m_iHowManyUndo ++;
	m_pUndo[m_iIndexUndo-1] = new class CUndo;
	return m_pUndo[m_iIndexUndo-1];
}

void CAnMapDoc::CopyTile( CTileSpr* pDstTile, CTileSpr* pSrcTile )
{
	pDstTile->m_sTileSprite			= pSrcTile->m_sTileSprite;
	pDstTile->m_sTileSpriteFrame	= pSrcTile->m_sTileSpriteFrame;
	pDstTile->m_sObjectSprite		= pSrcTile->m_sObjectSprite;
	pDstTile->m_sObjectSpriteFrame	= pSrcTile->m_sObjectSpriteFrame;
	pDstTile->m_bIsMoveAllowed		= pSrcTile->m_bIsMoveAllowed;
	pDstTile->m_bIsTeleport			= pSrcTile->m_bIsTeleport;
	pDstTile->m_bIsFarming			= pSrcTile->m_bIsFarming;
}

void CAnMapDoc::OnRButtonDown(UINT nFlags)
{
	int i;
	if( m_bWorkMode == FALSE )
	{
		if( m_cWorkMode == 1 )
		{
			if( m_pSprite[m_iBrushIndex] == NULL ) return;
			if( m_mousept.x > ((m_pSprite[m_iBrushIndex]->m_iScreenX)>>5) ) return;
			if( m_mousept.y > ((m_pSprite[m_iBrushIndex]->m_iScreenY)>>5) ) return;
			m_pSprite[m_iBrushIndex]->m_bMove[m_mousept.y*20 + m_mousept.x] = false;
			UpdateScreen();
		}
		return;
	}
	switch( m_cWorkMode )
	{
	case 0:
		m_iBrushIndex = m_tile[m_mousept.x+m_ScrollPt.x][m_mousept.y+m_ScrollPt.y].m_sTileSprite;
		short sx, sy;
		sx = m_tile[m_mousept.x+m_ScrollPt.x][m_mousept.y+m_ScrollPt.y].m_sTileSpriteFrame%20;
		sy = m_tile[m_mousept.x+m_ScrollPt.x][m_mousept.y+m_ScrollPt.y].m_sTileSpriteFrame/20;
		m_iSelBrush = -1;
		for( i = 0 ; i<m_pSprite[m_iBrushIndex]->m_sHowManyBrushes ; i++ )
		{
			if( sx >= m_pSprite[m_iBrushIndex]->m_stBrush[i].x && sx <= m_pSprite[m_iBrushIndex]->m_stBrush[i].x+m_pSprite[m_iBrushIndex]->m_stBrush[i].szx && sy >= m_pSprite[m_iBrushIndex]->m_stBrush[i].y && sy <= m_pSprite[m_iBrushIndex]->m_stBrush[i].y+m_pSprite[m_iBrushIndex]->m_stBrush[i].szy )
			{
				m_iSelBrush = i;
				break;
			}
		}
		//m_pDoc->m_pBrushView->SetSelectedItem(m_pDoc->m_iBrushIndex);		
		SelectBrush(0);
		UpdateScreen();
		break;
	case 1:
		{
			CUndo* pUndo = MakeUndo();
			if( pUndo == NULL ) return;
			for( i = 0 ; i < DEF_MAX_MEMTILE ; i++ )
			{
				if( (pUndo->m_pUndoTile[i] == NULL) && (pUndo->m_pUndoTile[i] == NULL) )
				{
					pUndo->m_pUndoTile[i] = new class CTile(m_mousept.x+m_ScrollPt.x, m_mousept.y+m_ScrollPt.y);
					CopyTile( &(pUndo->m_pUndoTile[i]->tile), &(m_tile[m_mousept.x+m_ScrollPt.x][m_mousept.y+m_ScrollPt.y]) );
					m_tile[m_mousept.x+m_ScrollPt.x][m_mousept.y+m_ScrollPt.y].m_bIsMoveAllowed = TRUE;
					pUndo->m_pRedoTile[i] = new class CTile(m_mousept.x+m_ScrollPt.x, m_mousept.y+m_ScrollPt.y);
					CopyTile( &(pUndo->m_pRedoTile[i]->tile), &(m_tile[m_mousept.x+m_ScrollPt.x][m_mousept.y+m_ScrollPt.y]) );
					SetModify();
					break;
				}
			}
			m_pRightView->UpdateScreen();
		}
		break;
	case 2:
		{
			CUndo* pUndo = MakeUndo();
			if( pUndo == NULL ) return;
			for( i = 0 ; i < DEF_MAX_MEMTILE ; i++ )
			{
				if( (pUndo->m_pUndoTile[i] == NULL) && (pUndo->m_pUndoTile[i] == NULL) )
				{
					pUndo->m_pUndoTile[i] = new class CTile(m_mousept.x+m_ScrollPt.x, m_mousept.y+m_ScrollPt.y);
					CopyTile( &(pUndo->m_pUndoTile[i]->tile), &(m_tile[m_mousept.x+m_ScrollPt.x][m_mousept.y+m_ScrollPt.y]) );
					m_tile[m_mousept.x+m_ScrollPt.x][m_mousept.y+m_ScrollPt.y].m_bIsTeleport = FALSE;
					pUndo->m_pRedoTile[i] = new class CTile(m_mousept.x+m_ScrollPt.x, m_mousept.y+m_ScrollPt.y);
					CopyTile( &(pUndo->m_pRedoTile[i]->tile), &(m_tile[m_mousept.x+m_ScrollPt.x][m_mousept.y+m_ScrollPt.y]) );
					SetModify();
					break;
				}
			}
			m_pRightView->UpdateScreen();
		}
		break;
	case 3:
		{
			CUndo* pUndo = MakeUndo();
			if( pUndo == NULL ) return;
			for( i = 0 ; i < DEF_MAX_MEMTILE ; i++ )
			{
				if( (pUndo->m_pUndoTile[i] == NULL) && (pUndo->m_pUndoTile[i] == NULL) )
				{
					pUndo->m_pUndoTile[i] = new class CTile(m_mousept.x+m_ScrollPt.x, m_mousept.y+m_ScrollPt.y);
					CopyTile( &(pUndo->m_pUndoTile[i]->tile), &(m_tile[m_mousept.x+m_ScrollPt.x][m_mousept.y+m_ScrollPt.y]) );
					m_tile[m_mousept.x+m_ScrollPt.x][m_mousept.y+m_ScrollPt.y].m_bIsFarming = FALSE;
					pUndo->m_pRedoTile[i] = new class CTile(m_mousept.x+m_ScrollPt.x, m_mousept.y+m_ScrollPt.y);
					CopyTile( &(pUndo->m_pRedoTile[i]->tile), &(m_tile[m_mousept.x+m_ScrollPt.x][m_mousept.y+m_ScrollPt.y]) );
					SetModify();
					break;
				}
			}
			m_pRightView->UpdateScreen();
		}
		break;
	}
}

void CAnMapDoc::OnRButtonUp(UINT nFlags)
{
}

void CAnMapDoc::OnMouseMove(UINT nFlags)
{
	int i;
	BOOL bLB, bRB;
	bLB = nFlags & MK_LBUTTON;
	bRB = nFlags & MK_RBUTTON;
	if( m_bWorkMode == TRUE )
	{
		switch(m_cWorkMode)
		{
		case 0:
			i = m_iSelBrush;
			SelectBrush(0);
			if( i == -1 || m_iSelBrush != i )
			{
				if( bLB )
				{
					switch( m_iTileMode )
					{
					case 0:
						PutTile( FALSE );
						break;
					case 1:
						PutTree( FALSE );
						break;
					case 2:
						PutObject( FALSE );
						break;
					}
				}
			}
			break;
		case 1:
			if( bLB || bRB )
			{
				if( m_iIndexUndo == 0 ) return;
				CUndo* pUndo = m_pUndo[m_iIndexUndo-1];
				if( pUndo == NULL ) return;
				for( i = 0 ; i < DEF_MAX_MEMTILE ; i++ )
				{
					if( (pUndo->m_pUndoTile[i] == NULL) && (pUndo->m_pUndoTile[i] == NULL) )
					{
						pUndo->m_pUndoTile[i] = new class CTile(m_mousept.x+m_ScrollPt.x, m_mousept.y+m_ScrollPt.y);
						CopyTile( &(pUndo->m_pUndoTile[i]->tile), &(m_tile[m_mousept.x+m_ScrollPt.x][m_mousept.y+m_ScrollPt.y]) );
						if( bLB ) m_tile[m_mousept.x+m_ScrollPt.x][m_mousept.y+m_ScrollPt.y].m_bIsMoveAllowed = FALSE;
						else if( bRB ) m_tile[m_mousept.x+m_ScrollPt.x][m_mousept.y+m_ScrollPt.y].m_bIsMoveAllowed = TRUE;
						pUndo->m_pRedoTile[i] = new class CTile(m_mousept.x+m_ScrollPt.x, m_mousept.y+m_ScrollPt.y);
						CopyTile( &(pUndo->m_pRedoTile[i]->tile), &(m_tile[m_mousept.x+m_ScrollPt.x][m_mousept.y+m_ScrollPt.y]) );
						break;
					}
				}
			}
			break;
		case 2:
			if( bLB || bRB )
			{
				if( m_iIndexUndo == 0 ) return;
				CUndo* pUndo = m_pUndo[m_iIndexUndo-1];
				if( pUndo == NULL ) return;
				for( i = 0 ; i < DEF_MAX_MEMTILE ; i++ )
				{
					if( (pUndo->m_pUndoTile[i] == NULL) && (pUndo->m_pUndoTile[i] == NULL) )
					{
						pUndo->m_pUndoTile[i] = new class CTile(m_mousept.x+m_ScrollPt.x, m_mousept.y+m_ScrollPt.y);
						CopyTile( &(pUndo->m_pUndoTile[i]->tile), &(m_tile[m_mousept.x+m_ScrollPt.x][m_mousept.y+m_ScrollPt.y]) );
						if( bLB ) m_tile[m_mousept.x+m_ScrollPt.x][m_mousept.y+m_ScrollPt.y].m_bIsTeleport = TRUE;
						else if( bRB ) m_tile[m_mousept.x+m_ScrollPt.x][m_mousept.y+m_ScrollPt.y].m_bIsTeleport = FALSE;
						pUndo->m_pRedoTile[i] = new class CTile(m_mousept.x+m_ScrollPt.x, m_mousept.y+m_ScrollPt.y);
						CopyTile( &(pUndo->m_pRedoTile[i]->tile), &(m_tile[m_mousept.x+m_ScrollPt.x][m_mousept.y+m_ScrollPt.y]) );
						break;
					}
				}
			}
			break;
		case 3:
			if( bLB || bRB )
			{
				if( m_iIndexUndo == 0 ) return;
				CUndo* pUndo = m_pUndo[m_iIndexUndo-1];
				if( pUndo == NULL ) return;
				for( i = 0 ; i < DEF_MAX_MEMTILE ; i++ )
				{
					if( (pUndo->m_pUndoTile[i] == NULL) && (pUndo->m_pUndoTile[i] == NULL) )
					{
						pUndo->m_pUndoTile[i] = new class CTile(m_mousept.x+m_ScrollPt.x, m_mousept.y+m_ScrollPt.y);
						CopyTile( &(pUndo->m_pUndoTile[i]->tile), &(m_tile[m_mousept.x+m_ScrollPt.x][m_mousept.y+m_ScrollPt.y]) );
						if( bLB ) m_tile[m_mousept.x+m_ScrollPt.x][m_mousept.y+m_ScrollPt.y].m_bIsFarming = TRUE;
						else if( bRB ) m_tile[m_mousept.x+m_ScrollPt.x][m_mousept.y+m_ScrollPt.y].m_bIsFarming = FALSE;
						pUndo->m_pRedoTile[i] = new class CTile(m_mousept.x+m_ScrollPt.x, m_mousept.y+m_ScrollPt.y);
						CopyTile( &(pUndo->m_pRedoTile[i]->tile), &(m_tile[m_mousept.x+m_ScrollPt.x][m_mousept.y+m_ScrollPt.y]) );
						break;
					}
				}
			}
			break;
		}
	}
	else if( m_bWorkMode == FALSE )
	{
		if( m_pSprite[m_iBrushIndex] == NULL ) return;
		if( m_mousept.x > ((m_pSprite[m_iBrushIndex]->m_iScreenX)>>5) ) return;
		if( m_mousept.y > ((m_pSprite[m_iBrushIndex]->m_iScreenY)>>5) ) return;
		if( m_cWorkMode == 1 )
		{
			if( bLB ) m_pSprite[m_iBrushIndex]->m_bMove[m_mousept.y*20 + m_mousept.x] = true;
			if( bRB ) m_pSprite[m_iBrushIndex]->m_bMove[m_mousept.y*20 + m_mousept.x] = false;
		}
	}
	UpdateScreen();
}

void CAnMapDoc::OnLButtonDown(UINT nFlags)
{
	int i;
	if( m_bWorkMode == FALSE ) 
	{
		if( m_mousept.x > ((m_pSprite[m_iBrushIndex]->m_iScreenX)>>5) ) return;
		if( m_mousept.y > ((m_pSprite[m_iBrushIndex]->m_iScreenY)>>5) ) return;

		if( m_cWorkMode == 1 )
		{
			m_pSprite[m_iBrushIndex]->m_bMove[m_mousept.y*20 + m_mousept.x] = true;
			UpdateScreen();
		}
		else
		{
			if( m_iBrushMode == 0 )
			{
				m_iBrushMode = 1;
				m_sBrsX1 = m_mousept.x;
				m_sBrsY1 = m_mousept.y;
			}
		}
	}
	else
	{
		switch( m_cWorkMode )
		{
		case 0:
			switch( m_iTileMode )
			{
			case 0:
				PutTile( TRUE );
				break;
			case 1:
				PutTree( TRUE );
				break;
			case 2:
				PutObject( TRUE );
				break;
			}
			break;
		case 1:
			{
				CUndo* pUndo = MakeUndo();
				if( pUndo == NULL ) return;
				for( i = 0 ; i < DEF_MAX_MEMTILE ; i++ )
				{
					if( (pUndo->m_pUndoTile[i] == NULL) && (pUndo->m_pUndoTile[i] == NULL) )
					{
						pUndo->m_pUndoTile[i] = new class CTile(m_mousept.x+m_ScrollPt.x, m_mousept.y+m_ScrollPt.y);
						CopyTile( &(pUndo->m_pUndoTile[i]->tile), &(m_tile[m_mousept.x+m_ScrollPt.x][m_mousept.y+m_ScrollPt.y]) );
						m_tile[m_mousept.x+m_ScrollPt.x][m_mousept.y+m_ScrollPt.y].m_bIsMoveAllowed = FALSE;
						pUndo->m_pRedoTile[i] = new class CTile(m_mousept.x+m_ScrollPt.x, m_mousept.y+m_ScrollPt.y);
						CopyTile( &(pUndo->m_pRedoTile[i]->tile), &(m_tile[m_mousept.x+m_ScrollPt.x][m_mousept.y+m_ScrollPt.y]) );
						SetModify();
						break;
					}
				}
				m_pRightView->UpdateScreen();
			}
			break;
		case 2:
			{
				CUndo* pUndo = MakeUndo();
				if( pUndo == NULL ) return;
				for( i = 0 ; i < DEF_MAX_MEMTILE ; i++ )
				{
					if( (pUndo->m_pUndoTile[i] == NULL) && (pUndo->m_pUndoTile[i] == NULL) )
					{
						pUndo->m_pUndoTile[i] = new class CTile(m_mousept.x+m_ScrollPt.x, m_mousept.y+m_ScrollPt.y);
						CopyTile( &(pUndo->m_pUndoTile[i]->tile), &(m_tile[m_mousept.x+m_ScrollPt.x][m_mousept.y+m_ScrollPt.y]) );
						m_tile[m_mousept.x+m_ScrollPt.x][m_mousept.y+m_ScrollPt.y].m_bIsTeleport = TRUE;
						pUndo->m_pRedoTile[i] = new class CTile(m_mousept.x+m_ScrollPt.x, m_mousept.y+m_ScrollPt.y);
						CopyTile( &(pUndo->m_pRedoTile[i]->tile), &(m_tile[m_mousept.x+m_ScrollPt.x][m_mousept.y+m_ScrollPt.y]) );
						SetModify();
						break;
					}
				}
				m_pRightView->UpdateScreen();
			}
			break;
		case 3:
			{
				CUndo* pUndo = MakeUndo();
				if( pUndo == NULL ) return;
				for( i = 0 ; i < DEF_MAX_MEMTILE ; i++ )
				{
					if( (pUndo->m_pUndoTile[i] == NULL) && (pUndo->m_pUndoTile[i] == NULL) )
					{
						pUndo->m_pUndoTile[i] = new class CTile(m_mousept.x+m_ScrollPt.x, m_mousept.y+m_ScrollPt.y);
						CopyTile( &(pUndo->m_pUndoTile[i]->tile), &(m_tile[m_mousept.x+m_ScrollPt.x][m_mousept.y+m_ScrollPt.y]) );
						m_tile[m_mousept.x+m_ScrollPt.x][m_mousept.y+m_ScrollPt.y].m_bIsFarming = TRUE;
						pUndo->m_pRedoTile[i] = new class CTile(m_mousept.x+m_ScrollPt.x, m_mousept.y+m_ScrollPt.y);
						CopyTile( &(pUndo->m_pRedoTile[i]->tile), &(m_tile[m_mousept.x+m_ScrollPt.x][m_mousept.y+m_ScrollPt.y]) );
						SetModify();
						break;
					}
				}
				m_pRightView->UpdateScreen();
			}
			break;
		case 4:
			{
				if( nFlags & MK_SHIFT )
				{
					int iTmpX, iTmpY;
					iTmpX = m_ScrollPt.x + m_mousept.x;
					iTmpY = m_ScrollPt.y + m_mousept.y;
					if( iTmpX < m_edit1.x )
					{
						m_edit2.x = m_edit1.x;
						m_edit1.x = iTmpX;
					}
					else m_edit2.x = iTmpX;
					if( iTmpY < m_edit1.y )
					{
						m_edit2.y = m_edit1.y;
						m_edit1.y = iTmpY;
					}
					else m_edit2.y = iTmpY;
				}
				else
				{
					m_edit1.x = m_ScrollPt.x + m_mousept.x;
					m_edit1.y = m_ScrollPt.y + m_mousept.y;
					m_edit2.x = m_ScrollPt.x + m_mousept.x;
					m_edit2.y = m_ScrollPt.y + m_mousept.y;
				}
				UpdateScreen();
			}
			break;
		}
	}
}

void CAnMapDoc::OnLButtonUp(UINT nFlags)
{
	if( m_bWorkMode == FALSE ) 
	{
		if( m_iBrushMode == 1 )
		{
			m_sBrsX2 = m_mousept.x;
			m_sBrsY2 = m_mousept.y;
			if( (m_sBrsX2+1)*DEF_TILESIZE > m_pSprite[m_iBrushIndex]->m_iScreenX ) m_sBrsX2 = m_pSprite[m_iBrushIndex]->m_iScreenX/32-1;
			if( (m_sBrsY2+1)*DEF_TILESIZE > m_pSprite[m_iBrushIndex]->m_iScreenY ) m_sBrsY2 = m_pSprite[m_iBrushIndex]->m_iScreenY/32-1;
			short x1 = m_sBrsX1;
			short x2 = m_sBrsX2;
			short y1 = m_sBrsY1;
			short y2 = m_sBrsY2;
			short x, y, szx, szy;
			if( x1 < x2 )
			{
				x = x1;
				szx = x2 - x1;
			}
			else
			{
				m_sBrsX1 = x2;
				m_sBrsX2 = x1;
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
				m_sBrsY1 = y2;
				m_sBrsY2 = y1;
				y = y2;
				szy = y1 - y2;
			}
			for( int i=0 ; i<m_pSprite[m_iBrushIndex]->m_sHowManyBrushes ; i++ )
			{
				if( m_pSprite[m_iBrushIndex]->m_stBrush[i].x == x && m_pSprite[m_iBrushIndex]->m_stBrush[i].y == y && m_pSprite[m_iBrushIndex]->m_stBrush[i].szx == szx && m_pSprite[m_iBrushIndex]->m_stBrush[i].szy == szy )
				{
					m_pSprite[m_iBrushIndex]->DeleteBrush( i );
					m_iBrushMode = 0;
					m_pRightView->UpdateScreen();
					return;
				}
			}
			m_iBrushMode = 2;
		}
		else if( m_iBrushMode == 2 )
		{
			m_iBrushMode = 0;
			short pvx = m_mousept.x;
			short pvy = m_mousept.y;
			m_pSprite[m_iBrushIndex]->InsertBrush( m_sBrsX1, m_sBrsY1, m_sBrsX2, m_sBrsY2, pvx-3, pvy-2);
		}
		m_pRightView->UpdateScreen();
	}
}

void CAnMapDoc::OnCheckBlank() 
{
	int x, y, errno;
	for( y=1 ; y<m_sMapDataSizeY-1 ; y++ )
	{
		for( x=1 ; x<m_sMapDataSizeX-1 ; x++ )
		{
			errno = 0;
			if( m_tile[x][y].m_bIsMoveAllowed )
			{
				if( m_tile[x-1][y-1].m_bIsMoveAllowed == FALSE ) errno++;
				if( m_tile[x  ][y-1].m_bIsMoveAllowed == FALSE ) errno++;
				if( m_tile[x+1][y-1].m_bIsMoveAllowed == FALSE ) errno++;
				if( m_tile[x-1][y  ].m_bIsMoveAllowed == FALSE ) errno++;
				if( m_tile[x+1][y  ].m_bIsMoveAllowed == FALSE ) errno++;
				if( m_tile[x-1][y+1].m_bIsMoveAllowed == FALSE ) errno++;
				if( m_tile[x  ][y+1].m_bIsMoveAllowed == FALSE ) errno++;
				if( m_tile[x+1][y+1].m_bIsMoveAllowed == FALSE ) errno++;
				if( errno > 7 )
				{
					m_ScrollPt.x = x-10;
					m_ScrollPt.y = y-10;
					m_cWorkMode = 1;
					UpdateScreen();
					return;
				}
			}
		}
	}	
	for( y=1 ; y<m_sMapDataSizeY-1 ; y++ )
	{
		for( x=1 ; x<m_sMapDataSizeX-1 ; x++ )
		{
			errno = 0;
			if( m_tile[x][y].m_bIsMoveAllowed )
			{
				if( m_tile[x-1][y-1].m_bIsMoveAllowed == FALSE ) errno++;
				if( m_tile[x  ][y-1].m_bIsMoveAllowed == FALSE ) errno++;
				if( m_tile[x+1][y-1].m_bIsMoveAllowed == FALSE ) errno++;
				if( m_tile[x-1][y  ].m_bIsMoveAllowed == FALSE ) errno++;
				if( m_tile[x+1][y  ].m_bIsMoveAllowed == FALSE ) errno++;
				if( m_tile[x-1][y+1].m_bIsMoveAllowed == FALSE ) errno++;
				if( m_tile[x  ][y+1].m_bIsMoveAllowed == FALSE ) errno++;
				if( m_tile[x+1][y+1].m_bIsMoveAllowed == FALSE ) errno++;
				if( errno > 6 )
				{
					m_ScrollPt.x = x-10;
					m_ScrollPt.y = y-10;
					m_cWorkMode = 1;
					UpdateScreen();
					return;
				}
			}
		}
	}
}

void CAnMapDoc::SelectTerrain(int iTerrain)
{
	short sSpr, sFrm;
	switch( iTerrain )
	{
	case 0:
		sSpr = 0;
		sFrm = 20;
		break;
	case 1:
		sSpr = 11;
		sFrm = 1;
		break;
	case 2:
		sSpr = 12;
		sFrm = 80;
		break;
	case 3:
		sSpr = 14;
		sFrm = 1;
		break;
	case 4:
		sSpr = 15;
		sFrm = 80;
		break;
	case 5:
		sSpr = 445;
		sFrm = 6;
		break;
	case 6:
		sSpr = 19;
		sFrm = 100;
		break;
	case 7:
		sSpr = 27;
		sFrm = 20;
		break;
	case 8:
		sSpr = 300;
		sFrm = 0;
		break;
	case 9:
		sSpr = 330;
		sFrm = 1;
		break;
	case 10:
		sSpr = 347;
		sFrm = 93;
		break;
	case 11:
		sSpr = 445;
		sFrm = 0;
		break;
	case 12:
		sSpr = 446;
		sFrm = 0;
		break;
	case 13:
		sSpr = 462;
		sFrm = 0;
		break;
	case 14:
		sSpr = 464;
		sFrm = 0;
		break;
	default:
		sSpr = 0;
		sFrm = 20;
		break;
	}
	int x, y;
	for( y=0 ; y<m_sMapDataSizeY ; y++ )
	{
		for( x=0 ; x<m_sMapDataSizeX ; x++ )
		{
			m_tile[x][y].m_sTileSprite = sSpr;
			m_tile[x][y].m_sTileSpriteFrame = sFrm+(x%6)+(y%4)*20;
			m_tile[x][y].m_sObjectSprite = 0;
			m_tile[x][y].m_sObjectSpriteFrame = 0;
			m_tile[x][y].m_bIsMoveAllowed = TRUE;
			m_tile[x][y].m_bIsTeleport = FALSE;
			m_tile[x][y].m_bIsFarming = FALSE;
		}
	}
}

void CAnMapDoc::OnViewRender() 
{
	CRenderDialog dlg(this);
	dlg.DoModal();	
}

void CAnMapDoc::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	if( m_edit1.x<0 || m_edit1.y<0 || m_edit2.x<0 || m_edit2.y<0 )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	if( m_edit1.x > m_edit2.x || m_edit1.y > m_edit2.y )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	pCmdUI->Enable(TRUE);
}

void CAnMapDoc::OnEditCopy() 
{
	if( m_edit1.x<0 || m_edit1.y<0 || m_edit2.x<0 || m_edit2.y<0 ) return;
	if( m_edit1.x > m_edit2.x || m_edit1.y > m_edit2.y ) return;
	int ix, iy;
	m_memsize.x = m_edit2.x-m_edit1.x+1;
	m_memsize.y = m_edit2.y-m_edit1.y+1;
	for( iy=0 ; iy<m_memsize.y ; iy++ )
	{
		for( ix=0 ; ix<m_memsize.x ; ix++ )
		{
			CopyTile( &(m_mTile[ix][iy]), &(m_tile[ix+m_edit1.x][iy+m_edit1.y]) );
		}
	}
}

void CAnMapDoc::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	if( m_edit1.x<0 || m_edit1.y<0 || m_edit2.x<0 || m_edit2.y<0 )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	if( m_memsize.x < 1 || m_memsize.y < 1 )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	pCmdUI->Enable(TRUE);
}

void CAnMapDoc::OnEditPaste() 
{
	if( m_edit1.x<0 || m_edit1.y<0 || m_edit2.x<0 || m_edit2.y<0 ) return;
	if( m_memsize.x < 1 || m_memsize.y < 1 ) return;
	int i, ix, iy;
	CUndo* pUndo;
	BOOL bTooBig = FALSE;
	if( m_memsize.x*m_memsize.y > DEF_MAX_MEMTILE ) bTooBig = TRUE;
	if( bTooBig == FALSE )
	{
		pUndo = MakeUndo();
		if( pUndo == NULL ) return;
	}
	for( iy=0 ; iy<m_memsize.y ; iy++ )
	{
		if( iy+m_edit2.y >= m_sMapDataSizeY ) continue;
		for( ix=0 ; ix<m_memsize.x ; ix++ )
		{
			if( ix+m_edit2.x >= m_sMapDataSizeX ) continue;
			if( bTooBig ) CopyTile( &(m_tile[ix+m_edit2.x][iy+m_edit2.y]), &(m_mTile[ix][iy]) );
			else
			{
				for( i = 0 ; i < DEF_MAX_MEMTILE ; i++ )
				{
					if( (pUndo->m_pUndoTile[i] == NULL) && (pUndo->m_pRedoTile[i] == NULL) )
					{
						pUndo->m_pUndoTile[i] = new class CTile(ix+m_edit2.x, iy+m_edit2.y);
						CopyTile( &(pUndo->m_pUndoTile[i]->tile), &(m_tile[ix+m_edit2.x][iy+m_edit2.y]) );
						CopyTile( &(m_tile[ix+m_edit2.x][iy+m_edit2.y]), &(m_mTile[ix][iy]) );
						pUndo->m_pRedoTile[i] = new class CTile(ix+m_edit2.x, iy+m_edit2.y);
						CopyTile( &(pUndo->m_pRedoTile[i]->tile), &(m_tile[ix+m_edit2.x][iy+m_edit2.y]) );
						break;
					}
				}
			}
		}
	}
	SetModify();
	if( m_memsize.x*m_memsize.y < 500 ) m_pMiniView->DrawMinimap( m_edit2.x, m_edit2.y );
	else m_pMiniView->LoadMinimap();
	UpdateScreen();
}

void CAnMapDoc::ThreadJob()
{
	if( m_bReadyAnMapView == TRUE && m_bReadyMiniView == TRUE )
	{
		if( m_bActivated )
		{
			POINT pt;
			GetCursorPos( &pt );
			ScreenToClient( m_pRightView->m_hWnd, &pt );
			m_bReadyMiniView = FALSE;
			m_bReadyAnMapView = FALSE;
			if( ( timeGetTime() - m_dwScrollTime ) < m_dwScrollDelay )
			{
				UpdateScreen();
				return;
			}
			if( pt.x > 0 && pt.x < m_pRightView->m_rtClient.right && pt.y > 0 && pt.y < m_pRightView->m_rtClient.bottom )
			{
				if( pt.x > 0 && pt.x < DEF_TILESIZE*2 ) {
					m_dwScrollTime = timeGetTime();
					OnKeyDown( VK_LEFT );
				}
				if( pt.y > 0 && pt.y < DEF_TILESIZE*2 ) {
					m_dwScrollTime = timeGetTime();
					OnKeyDown( VK_UP );
				}
				if( pt.x > m_pRightView->m_rtClient.right-DEF_TILESIZE*2 && pt.x < m_pRightView->m_rtClient.right ) {
					m_dwScrollTime = timeGetTime();
					OnKeyDown( VK_RIGHT );
				}
				if( pt.y > m_pRightView->m_rtClient.bottom-DEF_TILESIZE*2 && pt.y < m_pRightView->m_rtClient.bottom ) {
					m_dwScrollTime = timeGetTime();
					OnKeyDown( VK_DOWN );
				}
			}
		}
	}
}

void CAnMapDoc::SetModify()
{
	if( m_bModified == FALSE ) SetTitle( m_filename+" *" );
	m_bModified = TRUE;
}

void CAnMapDoc::OnToolOption() 
{
	COptionDialog dlg(this);
	if( dlg.DoModal() == IDCANCEL ) return;
	if( m_dwAutoSaveDelay != dlg.m_dwAutoSaveTime || m_bAutoSave != dlg.m_bAutoSave )
	{
		m_dwAutoSaveDelay = dlg.m_dwAutoSaveTime;
		if( dlg.m_bAutoSave == TRUE )
		{
			if( m_bAutoSave )
			{
				m_pRightView->KillTimer( m_pRightView->m_nAutoSaveTimer );
				m_pRightView->m_nAutoSaveTimer = m_pRightView->SetTimer( WM_TIMER_AUTOSAVE, m_dwAutoSaveDelay*60000, 0 );
			}
			else
			{
				m_bAutoSave = TRUE;
				m_pRightView->m_nAutoSaveTimer = m_pRightView->SetTimer( WM_TIMER_AUTOSAVE, m_dwAutoSaveDelay*60000, 0 );
			}
		}
		else
		{
			if( m_pRightView->m_nAutoSaveTimer != 0 )
			{
				m_pRightView->KillTimer( m_pRightView->m_nAutoSaveTimer );
				m_pRightView->m_nAutoSaveTimer = 0;
			}
			m_bAutoSave = FALSE;
		}
	}
	if( dlg.m_bCursor == FALSE ) dlg.m_dwCursor = 0;
	if( dlg.m_dwCursor != m_dwCursorDelay )
	{
		if( m_pRightView->m_nCursorTimer != 0 )
		{
			m_pRightView->KillTimer( m_pRightView->m_nCursorTimer );
			m_pRightView->m_nCursorTimer = 0;
		}
		m_dwCursorDelay = dlg.m_dwCursor;
		if( m_dwCursorDelay == 0 )
		{
			m_pRightView->m_bCursor = TRUE;
		}
		else
		{
			m_pRightView->m_nCursorTimer = m_pRightView->SetTimer( WM_TIMER_CURSOR, m_dwCursorDelay, 0 );
		}
	}
	m_dwScrollDelay = dlg.m_dwScrollDelay;
}


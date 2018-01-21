// Properties.cpp : implementation file
//

#include "stdafx.h"
#include "AnMap.h"
#include "Properties.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProperties dialog

CProperties::CProperties(CAnMapDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CDialog(CProperties::IDD, pParent)
{
	m_pDoc = pDoc;
	m_pt1.x = pDoc->m_edit1.x;
	m_pt1.y = pDoc->m_edit1.y;
	m_pt2.x = pDoc->m_edit2.x;
	m_pt2.y = pDoc->m_edit2.y;
	m_bSameMove = TRUE;
	m_bSameTele = TRUE;
	m_bSameFarm = TRUE;

	//{{AFX_DATA_INIT(CProperties)
	m_sObjSpr = 0;
	m_sObjFrm = 0;
	m_sSpr = 0;
	m_sSprFrm = 0;
	m_strXY = _T("");
	//}}AFX_DATA_INIT
	if( m_pt1.x == m_pt2.x && m_pt1.y == m_pt2.y )
	{
		m_bIsOneTile = TRUE;
		m_sSpr    = pDoc->m_tile[m_pt1.x][m_pt1.y].m_sTileSprite;
		m_sSprFrm = pDoc->m_tile[m_pt1.x][m_pt1.y].m_sTileSpriteFrame;
		m_sObjSpr = pDoc->m_tile[m_pt1.x][m_pt1.y].m_sObjectSprite;
		m_sObjFrm = pDoc->m_tile[m_pt1.x][m_pt1.y].m_sObjectSpriteFrame;
		m_strXY.Format( "%d,%d", m_pt1.x, m_pt1.y );
	}
	else
	{
		m_bIsOneTile = FALSE;
		int ix, iy;
		BOOL bMove = pDoc->m_tile[m_pt1.x][m_pt1.y].m_bIsMoveAllowed;
		BOOL bTele = pDoc->m_tile[m_pt1.x][m_pt1.y].m_bIsTeleport;
		BOOL bFarm = pDoc->m_tile[m_pt1.x][m_pt1.y].m_bIsFarming;
		for( iy=m_pt1.y ; iy<=m_pt2.y ; iy++ )
		{
			for( ix=m_pt1.x ; ix<=m_pt2.x ; ix++ )
			{
				if( pDoc->m_tile[ix][iy].m_bIsMoveAllowed )
				{
					if( bMove == FALSE ) m_bSameMove = FALSE;
				}
				else
				{
					if( bMove == TRUE ) m_bSameMove = FALSE;
				}
				if( pDoc->m_tile[ix][iy].m_bIsTeleport )
				{
					if( bTele == FALSE ) m_bSameTele = FALSE;
				}
				else
				{
					if( bTele == TRUE ) m_bSameTele = FALSE;
				}
				if( pDoc->m_tile[ix][iy].m_bIsFarming )
				{
					if( bFarm == FALSE ) m_bSameFarm = FALSE;
				}
				else
				{
					if( bFarm == TRUE ) m_bSameFarm = FALSE;
				}
			}
		}
	}

	if( m_bSameMove == FALSE )
	{
		m_iMoveable = 2;
	}
	else
	{
		if( m_pDoc->m_tile[m_pt1.x][m_pt1.y].m_bIsMoveAllowed == TRUE ) m_iMoveable = 1;
		else m_iMoveable = 0;
	}

	if( m_bSameTele == FALSE )
	{
		m_iTeleport = 2;
	}
	else
	{
		if( m_pDoc->m_tile[m_pt1.x][m_pt1.y].m_bIsTeleport == TRUE ) m_iTeleport = 1;
		else m_iTeleport = 0;
	}

	if( m_bSameFarm == FALSE )
	{
		m_iFarm = 2;
	}
	else
	{
		if( m_pDoc->m_tile[m_pt1.x][m_pt1.y].m_bIsFarming == TRUE ) m_iFarm = 1;
		else m_iFarm = 0;
	}
}


void CProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProperties)
	DDX_Check(pDX, IDC_CHECK_MOVE, m_iMoveable);
	DDX_Check(pDX, IDC_CHECK_TELEPORT, m_iTeleport);
	DDX_Text(pDX, IDC_EDIT_OBJECT, m_sObjSpr);
	DDX_Text(pDX, IDC_EDIT_OBJECT_FRAME, m_sObjFrm);
	DDX_Text(pDX, IDC_EDIT_SPRITE, m_sSpr);
	DDX_Text(pDX, IDC_EDIT_SPRITE_FRAME, m_sSprFrm);
	DDX_Text(pDX, IDC_STATIC_XY, m_strXY);
	DDV_MaxChars(pDX, m_strXY, 8);
	DDX_Check(pDX, IDC_CHECK_FARM, m_iFarm);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProperties, CDialog)
	//{{AFX_MSG_MAP(CProperties)
	ON_BN_CLICKED(IDC_CHECK_MOVE, OnCheckMove)
	ON_BN_CLICKED(IDC_CHECK_TELEPORT, OnCheckTeleport)
	ON_BN_CLICKED(IDC_CHECK_FARM, OnCheckFarm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProperties message handlers

void CProperties::OnCheckMove() 
{
	m_iMoveable++;
	if( m_bSameMove )
	{		
		if( m_iMoveable > 1 ) m_iMoveable = 0;
	}
	else
	{
		if( m_iMoveable > 2 ) m_iMoveable = 0;
	}
	UpdateData( FALSE );
	//m_edit1
	//m_edit1.x, m_edit1.y, m_edit2.x, m_edit2.y
	//m_pDoc->m_tile[][].
}

void CProperties::OnCheckTeleport() 
{
	m_iTeleport++;
	if( m_bSameTele )
	{		
		if( m_iTeleport > 1 ) m_iTeleport = 0;
	}
	else
	{
		if( m_iTeleport > 2 ) m_iTeleport = 0;
	}
	UpdateData( FALSE );
}

void CProperties::OnOK() 
{
	int ix, iy;
	CDialog::OnOK();
	if( m_bIsOneTile == TRUE )
	{
		m_pDoc->m_tile[m_pt1.x][m_pt1.y].m_sTileSprite = m_sSpr;
		m_pDoc->m_tile[m_pt1.x][m_pt1.y].m_sTileSpriteFrame = m_sSprFrm;
		m_pDoc->m_tile[m_pt1.x][m_pt1.y].m_sObjectSprite = m_sObjSpr;
		m_pDoc->m_tile[m_pt1.x][m_pt1.y].m_sObjectSpriteFrame = m_sObjFrm;
	}
	if( m_iMoveable == 0 )
	{
		for( iy=m_pt1.y ; iy<=m_pt2.y ; iy++ )
		{
			for( ix=m_pt1.x ; ix<=m_pt2.x ; ix++ )
			{
				m_pDoc->m_tile[ix][iy].m_bIsMoveAllowed = FALSE;
			}
		}
	}
	else if( m_iMoveable == 1 )
	{
		for( iy=m_pt1.y ; iy<=m_pt2.y ; iy++ )
		{
			for( ix=m_pt1.x ; ix<=m_pt2.x ; ix++ )
			{
				m_pDoc->m_tile[ix][iy].m_bIsMoveAllowed = TRUE;
			}
		}
	}
	if( m_iTeleport == 0 )
	{
		for( iy=m_pt1.y ; iy<=m_pt2.y ; iy++ )
		{
			for( ix=m_pt1.x ; ix<=m_pt2.x ; ix++ )
			{
				m_pDoc->m_tile[ix][iy].m_bIsTeleport = FALSE;
			}
		}
	}
	else if( m_iTeleport == 1 )
	{
		for( iy=m_pt1.y ; iy<=m_pt2.y ; iy++ )
		{
			for( ix=m_pt1.x ; ix<=m_pt2.x ; ix++ )
			{
				m_pDoc->m_tile[ix][iy].m_bIsTeleport = TRUE;
			}
		}
	}
	if( m_iFarm == 0 )
	{
		for( iy=m_pt1.y ; iy<=m_pt2.y ; iy++ )
		{
			for( ix=m_pt1.x ; ix<=m_pt2.x ; ix++ )
			{
				m_pDoc->m_tile[ix][iy].m_bIsFarming = FALSE;
			}
		}
	}
	else if( m_iFarm == 1 )
	{
		for( iy=m_pt1.y ; iy<=m_pt2.y ; iy++ )
		{
			for( ix=m_pt1.x ; ix<=m_pt2.x ; ix++ )
			{
				m_pDoc->m_tile[ix][iy].m_bIsFarming = TRUE;
			}
		}
	}
}

BOOL CProperties::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( m_bIsOneTile == FALSE )
	{
		GetDlgItem( IDC_EDIT_SPRITE )->EnableWindow( FALSE );
		GetDlgItem( IDC_EDIT_SPRITE_FRAME )->EnableWindow( FALSE );
		GetDlgItem( IDC_EDIT_OBJECT )->EnableWindow( FALSE );
		GetDlgItem( IDC_EDIT_OBJECT_FRAME )->EnableWindow( FALSE );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProperties::OnCheckFarm() 
{
	m_iFarm++;
	if( m_bSameFarm )
	{		
		if( m_iFarm > 1 ) m_iFarm = 0;
	}
	else
	{
		if( m_iFarm > 2 ) m_iFarm = 0;
	}
	UpdateData( FALSE );
}

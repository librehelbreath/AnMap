// NewMap.cpp : implementation file
//

#include "stdafx.h"
#include "AnMap.h"
#include "NewMap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewMap dialog


CNewMap::CNewMap(short sizeX, short sizeY, CWnd* pParent /*=NULL*/)
	: CDialog(CNewMap::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewMap)
	m_sMapSizeX = sizeX;
	m_sMapSizeY = sizeY;
	m_iTerrain = 0;
	//}}AFX_DATA_INIT
}


void CNewMap::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewMap)
	DDX_Text(pDX, IDC_EDIT_MAPSIZEX, m_sMapSizeX);
	DDV_MinMaxInt(pDX, m_sMapSizeX, 50, 752);
	DDX_Text(pDX, IDC_EDIT_MAPSIZEY, m_sMapSizeY);
	DDV_MinMaxInt(pDX, m_sMapSizeY, 50, 752);
	DDX_Radio(pDX, IDC_RADIO_GRASS, m_iTerrain);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewMap, CDialog)
	//{{AFX_MSG_MAP(CNewMap)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewMap message handlers

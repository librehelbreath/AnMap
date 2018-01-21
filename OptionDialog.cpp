// OptionDialog.cpp : implementation file
//

#include "stdafx.h"
#include "AnMap.h"
#include "OptionDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionDialog dialog


COptionDialog::COptionDialog(CAnMapDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CDialog(COptionDialog::IDD, pParent)
{
	m_dwAutoSaveTime = pDoc->m_dwAutoSaveDelay;
	m_bAutoSave = pDoc->m_bAutoSave;
	if( pDoc->m_dwCursorDelay == 0 )
	{
		m_bCursor = FALSE;
		m_dwCursor = 150;
	}
	else
	{
		m_bCursor = TRUE;
		m_dwCursor = pDoc->m_dwCursorDelay;
	}
	m_dwScrollDelay = pDoc->m_dwScrollDelay;
	//{{AFX_DATA_INIT(COptionDialog)
	//}}AFX_DATA_INIT
}


void COptionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionDialog)
	DDX_Text(pDX, IDC_EDIT_AUTOSAVETIME, m_dwAutoSaveTime);
	DDV_MinMaxDWord(pDX, m_dwAutoSaveTime, 1, 60);
	DDX_Check(pDX, IDC_CHECK_AUTOSAVE, m_bAutoSave);
	DDX_Check(pDX, IDC_CHECK_CURSOR_TWINKLE, m_bCursor);
	DDX_Text(pDX, IDC_EDIT_CURSOR_TWINKLE, m_dwCursor);
	DDV_MinMaxDWord(pDX, m_dwCursor, 50, 1000);
	DDX_Text(pDX, IDC_EDIT_SCROLL_DELAY, m_dwScrollDelay);
	DDV_MinMaxDWord(pDX, m_dwScrollDelay, 5, 100);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionDialog, CDialog)
	//{{AFX_MSG_MAP(COptionDialog)
	ON_BN_CLICKED(IDC_CHECK_AUTOSAVE, OnCheckAutosave)
	ON_BN_CLICKED(IDC_CHECK_CURSOR_TWINKLE, OnCheckCursorTwinkle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionDialog message handlers

void COptionDialog::OnCheckAutosave() 
{
	if( UpdateData( TRUE ) == 0 ) return;
	if( m_bAutoSave == FALSE )
	{
		GetDlgItem( IDC_EDIT_AUTOSAVETIME )->EnableWindow( FALSE );
	}
	else GetDlgItem( IDC_EDIT_AUTOSAVETIME )->EnableWindow( TRUE );
}

void COptionDialog::OnCheckCursorTwinkle() 
{
	if( UpdateData( TRUE ) == 0 ) return;
	if( m_bCursor == FALSE )
	{
		GetDlgItem( IDC_EDIT_CURSOR_TWINKLE )->EnableWindow( FALSE );
	}
	else GetDlgItem( IDC_EDIT_CURSOR_TWINKLE )->EnableWindow( TRUE );
}

BOOL COptionDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( m_bAutoSave == FALSE ) GetDlgItem( IDC_EDIT_AUTOSAVETIME )->EnableWindow( FALSE );
	if( m_bCursor == FALSE ) GetDlgItem( IDC_EDIT_CURSOR_TWINKLE )->EnableWindow( FALSE );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionDialog::OnOK() 
{
	if( UpdateData( TRUE ) == 0 ) return;
	CDialog::OnOK();
}

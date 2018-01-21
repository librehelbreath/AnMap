// AnMap.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "AnMap.h"

#include "MainFrm.h"
#include "AnMapDoc.h"
#include "BrushView.h"
#include "AnMapView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnMapApp

BEGIN_MESSAGE_MAP(CAnMapApp, CWinApp)
	//{{AFX_MSG_MAP(CAnMapApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnMapApp construction

CAnMapApp::CAnMapApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAnMapApp object

CAnMapApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CAnMapApp initialization

BOOL CAnMapApp::InitInstance()
{
	// Standard initialization

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	HKEY key;
	DWORD dwDisp;
	DWORD Size = 256;
	if( RegCreateKeyEx( HKEY_LOCAL_MACHINE, "Software\\SiemenTech\\Helbreath", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ, NULL, &key, &dwDisp ) != ERROR_SUCCESS ) return FALSE;

	if( RegQueryValueEx(key, "RootPath", 0, NULL, (LPBYTE)m_strWorkingFolder, &Size) != ERROR_SUCCESS )
	{
		RegCloseKey(key);
		return FALSE;
	}
	if( memcmp( m_strWorkingFolder, "", 1 ) == 0 ) return FALSE;

	if (OpenMutex(MUTEX_ALL_ACCESS, FALSE, "AnMap") != NULL) return FALSE;

	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	m_strLastFile = GetProfileString( "Settings", "LastFile" );
	HANDLE hFile = CreateFile( m_strLastFile, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if( hFile == INVALID_HANDLE_VALUE ) m_strLastFile = "";
	else CloseHandle( hFile );
	m_bAutoSave = GetProfileInt( "Settings", "AutoSave", 1 );
	if( m_bAutoSave == 0 ) m_bAutoSave = FALSE;
	else m_bAutoSave = TRUE;

	m_dwAutoSaveDelay = GetProfileInt( "Settings", "AutoSaveDelay", 10 );//레지스트리에 저장된 값은 Minute, 안에서 쓰는 값은 MiliSecond로 바꾼다.
	if( m_dwAutoSaveDelay < 1 ) m_dwAutoSaveDelay = 1;
	if( m_dwAutoSaveDelay > 60 ) m_dwAutoSaveDelay = 60;

	m_dwCursorDelay = GetProfileInt( "Settings", "Cursor", 150 );
	if( m_dwCursorDelay > 1000 ) m_dwCursorDelay = 1000;

	m_dwScrollDelay = GetProfileInt( "Settings", "ScrollDelay", 20 );
	if( m_dwScrollDelay < 5 ) m_dwScrollDelay = 5;
	if( m_dwScrollDelay > 100 ) m_dwScrollDelay = 100;

	// Register document templates

	CSplash splash;
	//splash.Create(NULL);
	splash.ShowWindow(SW_SHOW);
	splash.UpdateWindow();

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CAnMapDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CAnMapView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	if( cmdInfo.m_nShellCommand == CCommandLineInfo::FileOpen ) m_strLastFile = cmdInfo.m_strFileName;
	if (!ProcessShellCommand(cmdInfo)) return FALSE;
	//m_pMainWnd->DragAcceptFiles();
	splash.DestroyWindow();

	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CAnMapApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CAnMapApp message handlers
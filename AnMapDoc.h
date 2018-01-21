// AnMapDoc.h : interface of the CAnMapDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANMAPDOC_H__843368A7_DBA1_4162_AA84_666F430B9EF4__INCLUDED_)
#define AFX_ANMAPDOC_H__843368A7_DBA1_4162_AA84_666F430B9EF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TileSpr.h"
#include "BrushView.h"
#include "AnMapView.h"
#include "MiniView.h"
#include "Sprite.h"
#include "Undo.h"
#include <Mmsystem.h>

class CAnMapDoc : public CDocument
{
protected: // create from serialization only
	CAnMapDoc();
	DECLARE_DYNCREATE(CAnMapDoc)

// Attributes
public:
	CAnMapApp	*m_pApp;
	BOOL		m_bWorkMode;//TRUE이면 타일찍기 모드, FALSE이면 브러쉬 선택 모드
	char		m_cWorkMode;//0:일반타일 1:Moveable 2:Teleport 3:Farming
	short		m_sMapDataSizeX, m_sMapDataSizeY;
	short		m_sBrsX1, m_sBrsY1;
	short		m_sBrsX2, m_sBrsY2;
	int			m_iTileMode;//TileMode - 0:일반타일 1:나무 2:구조물
	int			m_iHowManyUndo;
	int			m_iIndexUndo;
	int			m_iBrushIndex;
	int			m_iSelBrush;
	int			m_iSelTree;
	int			m_iObjectIndex;
	int			m_iSelObject;
	int			m_iBrushMode;
	int			m_iTempX, m_iTempY;
	BOOL		m_bModified;
	BOOL		m_bGrid;
	BOOL		m_bTree;
	BOOL		m_bActivated;
	BOOL		m_bReadyMiniView;
	BOOL		m_bReadyAnMapView;
	BOOL		m_bAutoSave;
	DWORD		m_dwAutoSaveDelay;
	DWORD		m_dwCursorDelay;
	DWORD		m_dwScrollTime;
	DWORD		m_dwScrollDelay;
	CUndo		*m_pUndo[DEF_MAX_UNDO];
	HANDLE		m_hMutex;
	POINTS		m_ScrollPt;
	POINTS		m_mousept;
	POINTS		m_edit1, m_edit2, m_memsize;
	CString		m_filename;
	CString		m_pathname;
	CSprite		*m_pSprite[DEF_MAXSPRITES];
	CTileSpr	m_tile[752][752];
	CTileSpr	m_mTile[752][752];
	CMiniView	*m_pMiniView;
	CAnMapView	*m_pRightView;
	CBrushView	*m_pBrushView;

// Operations
public:
	CUndo* MakeUndo();
	void CopyTile( CTileSpr* pDstTile, CTileSpr* pSrcTile );
	BOOL IsBrushMatched( int i, short mx, short my );
	void ChangeBrush(int iDir);
	void SelectBrush(int iDir);
	BOOL _bDecodeMapInfo( char * pHeader );
	BOOL bOpenMapDataFile( CString cFn );
	void OnKeyDown( UINT nChar);
	void OnKeyUp(UINT nChar);
	void OnLButtonDown(UINT nFlags);
	void OnLButtonUp(UINT nFlags);
	void OnRButtonDown(UINT nFlags);
	void OnRButtonUp(UINT nFlags);
	void OnMouseMove(UINT nFlags);
	void SaveFileAs();
	void SaveMapData( CString cFn );
	void OnBrushMode( int iBrushIndex );
	void PutTile( BOOL bMakeUndo );
	void PutTree( BOOL bMakeUndo );
	void PutObject( BOOL bMakeUndo );
	void SetRightView( CAnMapView* pView );
	void SetMiniView( CMiniView* pView );
	void SetBrushView( CBrushView* pView);
	void SelectTerrain(int iTerrain);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnMapDoc)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetModify();
	void ThreadJob();
	void UpdateScreen();
	void MakeSprite( char* FileName, short sStart, short sCount);
	virtual ~CAnMapDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CAnMapDoc)
	afx_msg void OnFileSaveAs();
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnMoveLeft();
	afx_msg void OnMoveRight();
	afx_msg void OnMoveUp();
	afx_msg void OnMoveDown();
	afx_msg void OnFileNew();
	afx_msg void OnViewGrid();
	afx_msg void OnViewTree();
	afx_msg void OnUpdateViewGrid(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewTree(CCmdUI* pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnCheckBlank();
	afx_msg void OnViewRender();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnToolOption();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANMAPDOC_H__843368A7_DBA1_4162_AA84_666F430B9EF4__INCLUDED_)

; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CProperties
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "AnMap.h"
LastPage=0

ClassCount=13
Class1=CAnMapApp
Class2=CAnMapDoc
Class3=CAnMapView
Class4=CMainFrame

ResourceCount=14
Resource1=IDD_DIALOG_NEWMAP
Class5=CBrushView
Class6=CAboutDlg
Class7=CMiniView
Resource2=IDD_DIALOG_OPTION
Class8=CProperties
Class9=CProgressBar
Resource3=IDR_MAINFRAME
Class10=CNewMap
Resource4=IDD_DIALOG_RENDER
Class11=CRenderDialog
Resource5=IDD_DIALOG_PROPERTIES
Class12=COptionDialog
Resource6=IDD_ABOUTBOX
Class13=CSplash
Resource7=IDD_SPLASH
Resource8=IDD_DIALOG_OPTION (Korean)
Resource9=IDR_MAINFRAME (Korean)
Resource10=IDD_ABOUTBOX (Korean)
Resource11=IDD_DIALOG_PROPERTIES (Korean)
Resource12=IDD_DIALOG_NEWMAP (Korean)
Resource13=IDD_DIALOG_RENDER (Korean)
Resource14=IDD_SPLASH (Korean)

[CLS:CAnMapApp]
Type=0
HeaderFile=AnMap.h
ImplementationFile=AnMap.cpp
Filter=W
BaseClass=CWinApp
VirtualFilter=AC
LastObject=CAnMapApp

[CLS:CAnMapDoc]
Type=0
HeaderFile=AnMapDoc.h
ImplementationFile=AnMapDoc.cpp
Filter=W
BaseClass=CDocument
VirtualFilter=DC
LastObject=CAnMapDoc

[CLS:CAnMapView]
Type=0
HeaderFile=AnMapView.h
ImplementationFile=AnMapView.cpp
Filter=C
BaseClass=CScrollView
VirtualFilter=VWC
LastObject=CAnMapView


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CFrameWnd
VirtualFilter=fWC
LastObject=CMainFrame



[CLS:CAboutDlg]
Type=0
HeaderFile=AnMap.cpp
ImplementationFile=AnMap.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_APP_EXIT
Command6=ID_EDIT_UNDO
Command7=ID_EDIT_REDO
Command8=ID_EDIT_CUT
Command9=ID_EDIT_COPY
Command10=ID_EDIT_PASTE
Command11=ID_VIEW_TOOLBAR
Command12=ID_VIEW_STATUS_BAR
Command13=ID_VIEW_TREE
Command14=ID_VIEW_GRID
Command15=ID_VIEW_RENDER
Command16=ID_TOOL_CHECK_BLANK
Command17=ID_TOOL_OPTION
Command18=ID_APP_ABOUT
CommandCount=18

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_MOVE_LEFT
Command2=ID_EDIT_COPY
Command3=ID_MOVE_RIGHT
Command4=ID_TOOL_CHECK_BLANK
Command5=ID_VIEW_GRID
Command6=ID_FILE_NEW
Command7=ID_FILE_OPEN
Command8=ID_FILE_SAVE
Command9=ID_MOVE_DOWN
Command10=ID_VIEW_TREE
Command11=ID_EDIT_PASTE
Command12=ID_EDIT_UNDO
Command13=ID_EDIT_CUT
Command14=ID_VIEW_RENDER
Command15=ID_NEXT_PANE
Command16=ID_PREV_PANE
Command17=ID_EDIT_COPY
Command18=ID_EDIT_PASTE
Command19=ID_MOVE_UP
Command20=ID_EDIT_CUT
Command21=ID_EDIT_REDO
Command22=ID_EDIT_UNDO
Command23=ID_EDIT_REDO
CommandCount=23

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_TOOL_CHECK_BLANK
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_REDO
Command10=ID_VIEW_RENDER
Command11=ID_APP_ABOUT
CommandCount=11

[CLS:CMiniView]
Type=0
HeaderFile=MiniView.h
ImplementationFile=MiniView.cpp
BaseClass=CView
Filter=C
VirtualFilter=VWC
LastObject=CMiniView

[DLG:IDD_DIALOG_PROPERTIES]
Type=1
Class=CProperties
ControlCount=14
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_CHECK_MOVE,button,1342242822
Control4=IDC_CHECK_TELEPORT,button,1342242822
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_EDIT_SPRITE,edit,1350639744
Control10=IDC_EDIT_SPRITE_FRAME,edit,1350639744
Control11=IDC_EDIT_OBJECT,edit,1350639744
Control12=IDC_EDIT_OBJECT_FRAME,edit,1350639744
Control13=IDC_STATIC_XY,static,1342308352
Control14=IDC_CHECK_FARM,button,1342242822

[CLS:CProperties]
Type=0
HeaderFile=Properties.h
ImplementationFile=Properties.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_CHECK_FARM

[CLS:CProgressBar]
Type=0
HeaderFile=ProgressBar.h
ImplementationFile=ProgressBar.cpp
BaseClass=CProgressCtrl
Filter=W
VirtualFilter=NWC
LastObject=CProgressBar

[CLS:CBrushView]
Type=0
HeaderFile=brushview.h
ImplementationFile=brushview.cpp
BaseClass=CTreeView
LastObject=CBrushView
Filter=C
VirtualFilter=VWC

[DLG:IDD_DIALOG_NEWMAP]
Type=1
Class=CNewMap
ControlCount=23
Control1=IDC_EDIT_MAPSIZEX,edit,1350639744
Control2=IDC_EDIT_MAPSIZEY,edit,1350639744
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816
Control5=IDC_RADIO_GRASS,button,1342308361
Control6=IDC_DESERT,button,1342177289
Control7=IDC_RADIO_DRYDESERT,button,1342177289
Control8=IDC_RADIO_WASTELAND,button,1342177289
Control9=IDC_RADIO_STONE,button,1342177289
Control10=IDC_RADIO_ICE,button,1342177289
Control11=IDC_RADIO_WATER,button,1342177289
Control12=IDC_RADIO_ROCK,button,1342177289
Control13=IDC_RADIO_DUNGEON,button,1342177289
Control14=IDC_RADIO_PEBBLE,button,1342177289
Control15=IDC_RADIO_TOH,button,1342177289
Control16=IDC_RADIO_SNOW,button,1342177289
Control17=IDC_RADIO_FRIGUS,button,1342177289
Control18=IDC_RADIO_VOLCANO1,button,1342177289
Control19=IDC_RADIO_VOLCANO2,button,1342177289
Control20=IDC_STATIC,button,1342177287
Control21=IDC_STATIC,static,1342308353
Control22=IDC_STATIC,static,1342308353
Control23=IDC_STATIC,button,1342177287

[CLS:CNewMap]
Type=0
HeaderFile=NewMap.h
ImplementationFile=NewMap.cpp
BaseClass=CDialog
Filter=D
LastObject=CNewMap
VirtualFilter=dWC

[DLG:IDD_DIALOG_RENDER]
Type=1
Class=CRenderDialog
ControlCount=7
Control1=IDC_BUTTON_RENDER,button,1342177281
Control2=IDC_BUTTON_SAVE,button,1342242816
Control3=IDC_EDIT_OUTPUTSIZE,edit,1350639745
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC_RATIO,static,1342308352
Control6=IDC_PROGRESS_JOB,msctls_progress32,1350565888
Control7=IDC_BUTTON_PRINT,button,1342242816

[CLS:CRenderDialog]
Type=0
HeaderFile=RenderDialog.h
ImplementationFile=RenderDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=CRenderDialog
VirtualFilter=dWC

[DLG:IDD_DIALOG_OPTION]
Type=1
Class=COptionDialog
ControlCount=14
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_CHECK_AUTOSAVE,button,1342242819
Control5=IDC_EDIT_AUTOSAVETIME,edit,1350639745
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,button,1342177287
Control8=IDC_CHECK_CURSOR_TWINKLE,button,1342242819
Control9=IDC_EDIT_CURSOR_TWINKLE,edit,1350639745
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,button,1342177287
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_EDIT_SCROLL_DELAY,edit,1350639745

[CLS:COptionDialog]
Type=0
HeaderFile=OptionDialog.h
ImplementationFile=OptionDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=IDOK
VirtualFilter=dWC

[DLG:IDD_SPLASH]
Type=1
Class=CSplash
ControlCount=6
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352

[CLS:CSplash]
Type=0
HeaderFile=Splash.h
ImplementationFile=Splash.cpp
BaseClass=CDialog
Filter=D
LastObject=CSplash

[DLG:IDD_ABOUTBOX (Korean)]
Type=1
Class=?
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352

[TB:IDR_MAINFRAME (Korean)]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_TOOL_CHECK_BLANK
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_REDO
Command10=ID_VIEW_RENDER
Command11=ID_APP_ABOUT
CommandCount=11

[MNU:IDR_MAINFRAME (Korean)]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_APP_EXIT
Command6=ID_EDIT_UNDO
Command7=ID_EDIT_REDO
Command8=ID_EDIT_CUT
Command9=ID_EDIT_COPY
Command10=ID_EDIT_PASTE
Command11=ID_VIEW_TOOLBAR
Command12=ID_VIEW_STATUS_BAR
Command13=ID_VIEW_TREE
Command14=ID_VIEW_GRID
Command15=ID_VIEW_RENDER
Command16=ID_TOOL_CHECK_BLANK
Command17=ID_TOOL_OPTION
Command18=ID_APP_ABOUT
CommandCount=18

[ACL:IDR_MAINFRAME (Korean)]
Type=1
Class=?
Command1=ID_MOVE_LEFT
Command2=ID_EDIT_COPY
Command3=ID_MOVE_RIGHT
Command4=ID_TOOL_CHECK_BLANK
Command5=ID_VIEW_GRID
Command6=ID_FILE_NEW
Command7=ID_FILE_OPEN
Command8=ID_FILE_SAVE
Command9=ID_MOVE_DOWN
Command10=ID_VIEW_TREE
Command11=ID_EDIT_PASTE
Command12=ID_EDIT_UNDO
Command13=ID_EDIT_CUT
Command14=ID_VIEW_RENDER
Command15=ID_NEXT_PANE
Command16=ID_PREV_PANE
Command17=ID_EDIT_COPY
Command18=ID_EDIT_PASTE
Command19=ID_MOVE_UP
Command20=ID_EDIT_CUT
Command21=ID_EDIT_REDO
Command22=ID_EDIT_UNDO
Command23=ID_EDIT_REDO
CommandCount=23

[DLG:IDD_DIALOG_PROPERTIES (Korean)]
Type=1
Class=?
ControlCount=14
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_CHECK_MOVE,button,1342242822
Control4=IDC_CHECK_TELEPORT,button,1342242822
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_EDIT_SPRITE,edit,1350639744
Control10=IDC_EDIT_SPRITE_FRAME,edit,1350639744
Control11=IDC_EDIT_OBJECT,edit,1350639744
Control12=IDC_EDIT_OBJECT_FRAME,edit,1350639744
Control13=IDC_STATIC_XY,static,1342308352
Control14=IDC_CHECK_FARM,button,1342242822

[DLG:IDD_DIALOG_NEWMAP (Korean)]
Type=1
Class=?
ControlCount=23
Control1=IDC_EDIT_MAPSIZEX,edit,1350639744
Control2=IDC_EDIT_MAPSIZEY,edit,1350639744
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816
Control5=IDC_RADIO_GRASS,button,1342308361
Control6=IDC_DESERT,button,1342177289
Control7=IDC_RADIO_DRYDESERT,button,1342177289
Control8=IDC_RADIO_WASTELAND,button,1342177289
Control9=IDC_RADIO_STONE,button,1342177289
Control10=IDC_RADIO_ICE,button,1342177289
Control11=IDC_RADIO_WATER,button,1342177289
Control12=IDC_RADIO_ROCK,button,1342177289
Control13=IDC_RADIO_DUNGEON,button,1342177289
Control14=IDC_RADIO_PEBBLE,button,1342177289
Control15=IDC_RADIO_TOH,button,1342177289
Control16=IDC_RADIO_SNOW,button,1342177289
Control17=IDC_RADIO_FRIGUS,button,1342177289
Control18=IDC_RADIO_VOLCANO1,button,1342177289
Control19=IDC_RADIO_VOLCANO2,button,1342177289
Control20=IDC_STATIC,button,1342177287
Control21=IDC_STATIC,static,1342308353
Control22=IDC_STATIC,static,1342308353
Control23=IDC_STATIC,button,1342177287

[DLG:IDD_DIALOG_RENDER (Korean)]
Type=1
Class=?
ControlCount=7
Control1=IDC_BUTTON_RENDER,button,1342177281
Control2=IDC_BUTTON_SAVE,button,1342242816
Control3=IDC_EDIT_OUTPUTSIZE,edit,1350639745
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC_RATIO,static,1342308352
Control6=IDC_PROGRESS_JOB,msctls_progress32,1350565888
Control7=IDC_BUTTON_PRINT,button,1342242816

[DLG:IDD_DIALOG_OPTION (Korean)]
Type=1
Class=?
ControlCount=14
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_CHECK_AUTOSAVE,button,1342242819
Control5=IDC_EDIT_AUTOSAVETIME,edit,1350639745
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,button,1342177287
Control8=IDC_CHECK_CURSOR_TWINKLE,button,1342242819
Control9=IDC_EDIT_CURSOR_TWINKLE,edit,1350639745
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,button,1342177287
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_EDIT_SCROLL_DELAY,edit,1350639745

[DLG:IDD_SPLASH (Korean)]
Type=1
Class=?
ControlCount=6
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352


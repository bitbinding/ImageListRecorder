; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CRecordSettingDialog
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ImageListRecorder.h"
LastPage=0

ClassCount=9
Class1=CImageListRecorderApp
Class2=CImageListRecorderDoc
Class3=CImageListRecorderView
Class4=CMainFrame

ResourceCount=7
Resource1=IDR_MENU1
Class5=CAboutDlg
Resource2=IDD_DIALOG2
Class6=CRecordSettingDialog
Resource3=IDD_DIALOG1
Class7=CSavingImageListDialog
Resource4=IDD_ABOUTBOX
Class8=CAviToolInput
Resource5=IDD_DIALOG3
Class9=CMuxProcessDisplay
Resource6=IDR_MAINFRAME
Resource7=IDD_DIALOG4

[CLS:CImageListRecorderApp]
Type=0
HeaderFile=ImageListRecorder.h
ImplementationFile=ImageListRecorder.cpp
Filter=N

[CLS:CImageListRecorderDoc]
Type=0
HeaderFile=ImageListRecorderDoc.h
ImplementationFile=ImageListRecorderDoc.cpp
Filter=N

[CLS:CImageListRecorderView]
Type=0
HeaderFile=ImageListRecorderView.h
ImplementationFile=ImageListRecorderView.cpp
Filter=C
BaseClass=CScrollView
VirtualFilter=VWC
LastObject=CImageListRecorderView


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=ID_MENU_DISPLAY_WINDOW
BaseClass=CFrameWnd
VirtualFilter=fWC




[CLS:CAboutDlg]
Type=0
HeaderFile=ImageListRecorder.cpp
ImplementationFile=ImageListRecorder.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_TingZhiLuZhi
Command3=ID_AVITOOLSMENUITEM
Command4=ID_FILE_PRINT
Command5=ID_FILE_PRINT_PREVIEW
Command6=ID_FILE_PRINT_SETUP
Command7=ID_APP_EXIT
Command8=ID_BEGINPAGE
Command9=ID_APP_ABOUT
CommandCount=9

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_EDIT_COPY
Command2=ID_FILE_NEW
Command3=ID_FILE_OPEN
Command4=ID_FILE_PRINT
Command5=ID_FILE_SAVE
Command6=ID_AVITOOLSMENUITEM
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_TingZhiLuZhi
Command11=ID_NEXT_PANE
Command12=ID_PREV_PANE
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_EDIT_CUT
Command16=ID_EDIT_UNDO
CommandCount=16

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_TingZhiLuZhi
Command3=ID_AVITOOLSMENUITEM
Command4=ID_FILE_PRINT
Command5=ID_BEGINPAGE
CommandCount=5

[DLG:IDD_DIALOG1]
Type=1
Class=CRecordSettingDialog
ControlCount=31
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,button,1342177287
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT1,edit,1350631552
Control7=IDC_EDIT3,edit,1350631552
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_EDIT2,edit,1350631552
Control11=IDC_EDIT4,edit,1350631552
Control12=IDC_BUTTON1,button,1342242816
Control13=IDC_BUTTON2,button,1342242816
Control14=IDC_STATIC,button,1342177287
Control15=IDC_STATIC,static,1342308352
Control16=IDC_EDIT5,edit,1350631552
Control17=IDC_STATIC,static,1342308352
Control18=IDC_EDIT6,edit,1350631552
Control19=IDC_STATIC,static,1342308352
Control20=IDC_STATIC,static,1342308352
Control21=IDC_CHECK1,button,1342242819
Control22=IDC_STATIC,button,1342177287
Control23=IDC_EDIT7,edit,1350631552
Control24=IDC_EDIT8,edit,1350631552
Control25=IDC_STATIC,static,1342308352
Control26=IDC_STATIC,static,1342308352
Control27=IDC_STATIC,static,1342308352
Control28=IDC_STATIC,static,1342308352
Control29=IDC_EDIT9,edit,1350631552
Control30=IDC_STATIC,static,1342308352
Control31=IDC_COMBO1,combobox,1344340035

[CLS:CRecordSettingDialog]
Type=0
HeaderFile=RecordSettingDialog.h
ImplementationFile=RecordSettingDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_EDIT7
VirtualFilter=dWC

[DLG:IDD_DIALOG2]
Type=1
Class=CSavingImageListDialog
ControlCount=2
Control1=IDCANCEL,button,1342242816
Control2=IDC_PROGRESS1,msctls_progress32,1350565889

[CLS:CSavingImageListDialog]
Type=0
HeaderFile=SavingImageListDialog.h
ImplementationFile=SavingImageListDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=CSavingImageListDialog
VirtualFilter=dWC

[DLG:IDD_DIALOG3]
Type=1
Class=CAviToolInput
ControlCount=16
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC2,button,1342177287
Control4=65535,static,1342308352
Control5=IDC_EDIT6,edit,1350631552
Control6=65535,static,1342308352
Control7=IDC_CHECK1,button,1476460547
Control8=IDC_STATIC1,button,1342177287
Control9=IDC_CHECK2,button,1342242819
Control10=IDC_EDIT7,edit,1350633600
Control11=IDC_EDIT8,edit,1350633600
Control12=65535,static,1342308352
Control13=65535,static,1342308352
Control14=IDC_BUTTON1,button,1342242816
Control15=IDC_BUTTON2,button,1342242816
Control16=IDC_BUTTON3,button,1342242816

[CLS:CAviToolInput]
Type=0
HeaderFile=AviToolInput.h
ImplementationFile=AviToolInput.cpp
BaseClass=CDialog
Filter=D
LastObject=ID_MENU_DISPLAY_WINDOW
VirtualFilter=dWC

[DLG:IDD_DIALOG4]
Type=1
Class=CMuxProcessDisplay
ControlCount=1
Control1=IDC_PROGRESS1,msctls_progress32,1350565889

[CLS:CMuxProcessDisplay]
Type=0
HeaderFile=MuxProcessDisplay.h
ImplementationFile=MuxProcessDisplay.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CMuxProcessDisplay

[MNU:IDR_MENU1]
Type=1
Class=CMainFrame
Command1=ID_TingZhiLuZhi
Command2=ID_MENU_DISPLAY_WINDOW
CommandCount=2


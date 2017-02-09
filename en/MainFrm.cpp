// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "ImageListRecorder.h"
#include "ImageListRecorderView.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_MESSAGE(WM_SETMAINFRAMETEXT,OnSetMfText)
	ON_MESSAGE(WM_ICONMOUSEUP,OnIconMouseUp)
	ON_WM_SYSCOMMAND()
	ON_COMMAND(ID_MENU_DISPLAY_WINDOW, OnMenuDisplayWindow)
	ON_UPDATE_COMMAND_UI(ID_MENU_DISPLAY_WINDOW, OnUpdateMenuDisplayWindow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	notifyIconMode=true;
}

CMainFrame::~CMainFrame()
{
	if(notifyIconMode){		
		Shell_NotifyIcon(NIM_DELETE,&nidata);
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	
	
	initNotifyIconData();
	ApplyNotifyIconMode();
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

LRESULT CMainFrame::OnSetMfText(WPARAM w,LPARAM l){
	int type=(int)w;
	int data=(int)l;
	CString str;
	if(type==0){
		SetWindowText("无标题 - ImageListRecorder");
	}else if(type==1){
		str.Format("准备:%.1f",data/1000.0);
		SetWindowText(str);
	}
	return 0;
}

void initNotifyIconData();
	void ApplyNotifyIconMode();

void CMainFrame::initNotifyIconData(){	
	ZeroMemory(&nidata,sizeof(NOTIFYICONDATA));
	nidata.cbSize =sizeof(NOTIFYICONDATA);
    nidata.hWnd =m_hWnd;
    nidata.uID =IDR_MAINFRAME;
    nidata.uFlags =NIF_ICON |NIF_MESSAGE;
    nidata.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
    nidata.uCallbackMessage =WM_ICONMOUSEUP;
}

void CMainFrame::ApplyNotifyIconMode() 
{
	// TODO: Add your command handler code here
	if(notifyIconMode){
		Shell_NotifyIcon(NIM_ADD,&nidata);
	}else{
		Shell_NotifyIcon(NIM_DELETE,&nidata);
	}
}
/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnIconMouseUp(WPARAM wParam, LPARAM lParam){
	if(lParam!=WM_LBUTTONUP && lParam!=WM_RBUTTONUP){
		return;
	}
	CImageListRecorderView *pView=(CImageListRecorderView *)GetActiveView();
	if(pView!=NULL && pView->recordMode){
		CMenu menu,*pSubMenu;//定义下面要用到的cmenu对象  
		menu.LoadMenu(IDR_MENU1);//装载自定义的右键菜单  
		pSubMenu=menu.GetSubMenu(0);//获取第一个弹出菜单，所以第一个菜单必须有子菜单  
		CPoint mousePoint;//定义一个用于确定鼠标位置的位置  
		GetCursorPos(&mousePoint);//获取当前鼠标的位置，以便使得菜单可以跟随光标
		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN,mousePoint.x,mousePoint.y,this);   //在指定位置显示弹出菜单
	}else{
		ShowWindow(SW_SHOW);
	}
}

void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam){
	if ((nID&0xFFF0)==SC_MINIMIZE && notifyIconMode)
	{
		ShowWindow(SW_HIDE);
	}
	else
	{
		CFrameWnd::OnSysCommand(nID, lParam);
	}
}

void CMainFrame::OnMenuDisplayWindow() 
{
	// TODO: Add your command handler code here
	if(IsIconic()==TRUE || IsWindowVisible()==FALSE){
		ShowWindow(SW_SHOW);
	}else{
		ShowWindow(notifyIconMode?SW_HIDE:SW_SHOWMINIMIZED);
	}
}

void CMainFrame::OnUpdateMenuDisplayWindow(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(IsIconic()==TRUE || IsWindowVisible()==FALSE){
		pCmdUI->SetText("显示窗口(&R)");
	}else{
		pCmdUI->SetText("隐藏窗口(&R)");
	}
}

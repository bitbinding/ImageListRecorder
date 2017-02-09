// RecordSettingDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ImageListRecorder.h"
#include "RecordSettingDialog.h"
#include "ImageListRecorderView.h"
#include "myMP3Enc.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecordSettingDialog dialog
extern PFUNMP3ENCGetDevices GetDevices;
extern PFUNMP3ENCGetLineIn GetLineIn;
extern bool myMP3EncLoadComplete;

CRecordSettingDialog::CRecordSettingDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CRecordSettingDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRecordSettingDialog)
	m_e1 = 0;
	m_e2 = 0;
	m_e3 = 550;
	m_e4 = 400;
	m_e5 = 50;
	m_e6 = 7200;
	m_c1 = TRUE;
	m_e8 = 5;
	m_e7 = 550;
	m_e9 = 0;
	//}}AFX_DATA_INIT
	initialMove=0;
	cancelledWithWrongData=false;
	soundInputType=0;
	deviceString="";
	lineInString="";
	deviceString0="";
	lineInString0="";
	willGetInputDeviceName=true;
}


void CRecordSettingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecordSettingDialog)
	DDX_Control(pDX, IDC_COMBO1, m_co1);
	DDX_Text(pDX, IDC_EDIT1, m_e1);
	DDX_Text(pDX, IDC_EDIT2, m_e2);
	DDX_Text(pDX, IDC_EDIT3, m_e3);
	DDV_MinMaxInt(pDX, m_e3, 1, 32767);
	DDX_Text(pDX, IDC_EDIT4, m_e4);
	DDV_MinMaxInt(pDX, m_e4, 1, 32767);
	DDX_Text(pDX, IDC_EDIT5, m_e5);
	DDV_MinMaxInt(pDX, m_e5, 1, 3600000);
	DDX_Text(pDX, IDC_EDIT6, m_e6);
	DDV_MinMaxInt(pDX, m_e6, 1, 2147483647);
	DDX_Check(pDX, IDC_CHECK1, m_c1);
	DDX_Text(pDX, IDC_EDIT7, m_e8);
	DDV_MinMaxInt(pDX, m_e8, 0, 32767);
	DDX_Text(pDX, IDC_EDIT8, m_e7);
	DDV_MinMaxInt(pDX, m_e7, 1, 1999);
	DDX_Text(pDX, IDC_EDIT9, m_e9);
	DDV_MinMaxInt(pDX, m_e9, 0, 30);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRecordSettingDialog, CDialog)
	//{{AFX_MSG_MAP(CRecordSettingDialog)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_SIZE()
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecordSettingDialog message handlers


BOOL CRecordSettingDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	// TODO: Add extra initialization here
	UpdateData(FALSE);
	int i=0, leng=0;
	if(willGetInputDeviceName){
		char devicesStringArray[MAX_DEVICES][MAX_NAME_BUF];   /* all device name */	
		if(myMP3EncLoadComplete){
			leng=GetDevices(devicesStringArray);
		}else{
			leng=0;
		}
		for (i=0; i<leng; i++)
		{
			m_co1.AddString(devicesStringArray[i]);
		}
		//CString str=devicesStringArray[0];
		if(leng==0 || deviceString==""){
			m_co1.SetCurSel(0);
		}else{
			i=m_co1.FindString(-1,(LPCTSTR)deviceString);
			if(i!=CB_ERR){
				m_co1.SetCurSel(m_co1.FindString(-1,(LPCTSTR)deviceString));
			}else{
				m_co1.SetCurSel(0);
			}
		}
	}else{
		m_co1.AddString("有声音");
		if(!myMP3EncLoadComplete || deviceString==""){
			m_co1.SetCurSel(0);
		}else{
			m_co1.SetCurSel(1);
		}
	}
	soundInputType=m_co1.GetCurSel();
	CRect rect;
	rect.left=m_e1;
	rect.top=m_e2;
	rect.right=m_e1+m_e3;
	rect.bottom=m_e2+m_e4;
	MoveWindow(&rect);
	SetTimer(100,100,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CRecordSettingDialog::OnButton2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CRect rect;
	rect.left=m_e1;
	rect.top=m_e2;
	rect.right=m_e1+m_e3;
	rect.bottom=m_e2+m_e4;
	MoveWindow(&rect);
	GetRecordRect();
	drawWindowRect();
}

void CRecordSettingDialog::OnButton1() 
{
	// TODO: Add your control notification handler code here
	GetRecordRect();
	drawWindowRect();
}

void CRecordSettingDialog::GetRecordRect()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CRect rect;
	CWnd *pWnd;
	pWnd = GetActiveWindow();
	pWnd->GetWindowRect(&rect);
	//ScreenToClient(&rect);
	m_e1=rect.left;
	m_e2=rect.top;
	m_e3=rect.Width();
	m_e4=rect.Height();
	UpdateData(FALSE);	
}

void CRecordSettingDialog::drawWindowRect(){
	UpdateData(TRUE);
	CWnd*  pWnd= CWnd::GetDesktopWindow();	
	CWindowDC winDC(pWnd);
	//pWnd->Invalidate();
	CPen pen;
	pen.CreatePen(PS_SOLID,1,RGB(255,0,0));
	winDC.SelectObject(&pen);
	winDC.MoveTo(m_e1-1,m_e2-1);
	winDC.LineTo(m_e3+m_e1+1,m_e2-1);
	winDC.LineTo(m_e3+m_e1+1,m_e4+m_e2+1);
	winDC.LineTo(m_e1-1,m_e4+m_e2+1);
	winDC.LineTo(m_e1-1,m_e2-1);
}

void CRecordSettingDialog::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);	
	// TODO: Add your message handler code here
	CWnd *pWnd0 = GetDlgItem(IDC_BUTTON1);
	if(nType==SIZE_MINIMIZED) return;   //如果是窗体最小化则什么都不做
	if(nType==SIZE_MAXIMIZED){
		UpdateData(TRUE);
		m_e1=0;
		m_e2=0;
		m_e3=GetSystemMetrics(SM_CXSCREEN);
		m_e4=GetSystemMetrics(SM_CYSCREEN);
		UpdateData(FALSE);
		return;
	}
	if(pWnd0){//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
		GetRecordRect();//获取窗口所在矩形并更新界面
	}
}

void CRecordSettingDialog::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);	
	// TODO: Add your message handler code here
	CWnd *pWnd0 = GetDlgItem(IDC_BUTTON1);
	//if(nType==1) return;   //如果是窗体最小化则什么都不做
	if(pWnd0){//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
		if(initialMove<1){
			/*UpdateData(TRUE);
			m_e1=0;
			m_e2=0;
			m_e3=GetSystemMetrics(SM_CXSCREEN);
			m_e4=GetSystemMetrics(SM_CYSCREEN);
			UpdateData(FALSE);*/			
			initialMove++;
		}else{
			GetRecordRect();
		}
	}
}



void CRecordSettingDialog::OnCheck2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

void CRecordSettingDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent=100){
		GetRecordRect();
		KillTimer(100);
	}
	CDialog::OnTimer(nIDEvent);
}

void CRecordSettingDialog::OnOK() 
{
	// TODO: Add extra validation here
	
	if(soundInputType>0 && willGetInputDeviceName){
		m_co1.GetWindowText(deviceString);
		char lineInStringArray[MAX_LINEIN][MAX_NAME_BUF];
		if(GetLineIn!=NULL && GetLineIn(lineInStringArray, (LPTSTR)(LPCTSTR)deviceString)>0){
			lineInString.Format("%s",lineInStringArray[0]);
		}else{
			lineInString="";
		}
	}else if(soundInputType>0){
		deviceString=deviceString0;
		lineInString=lineInString0;
	}else{
		deviceString="";
		lineInString="";
	}
	if(UpdateData(TRUE)){
		CDialog::OnOK();
	}
}

void CRecordSettingDialog::OnCancel() 
{
	// TODO: Add extra cleanup here
	if(UpdateData(TRUE)){
		CDialog::OnCancel();
	}else{
		cancelledWithWrongData=true;
		CDialog::OnCancel();
	}
}

void CRecordSettingDialog::OnSelchangeCombo1() 
{
	// TODO: Add your control notification handler code here
	soundInputType=m_co1.GetCurSel();
	if(soundInputType>0 && willGetInputDeviceName){
		m_co1.GetWindowText(deviceString);
	}else if(soundInputType>0){
		deviceString=deviceString0;
	}else{
		deviceString="";
	}
}

// MuxProcessDisplay.cpp : implementation file
//

#include "stdafx.h"
#include "ImageListRecorder.h"
#include "MuxProcessDisplay.h"
#include "ImageListRecorderView.h"
#include "AviToolInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMuxProcessDisplay dialog
int CMuxProcessDisplay::muxFinishedCount=0;
extern CStringArray videoAudioFileName;
static bool multiplexing=false;
static bool muxCancelling=false;
CMuxProcessDisplay::CMuxProcessDisplay(CWnd* pParent /*=NULL*/)
	: CDialog(CMuxProcessDisplay::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMuxProcessDisplay)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	multiplexing=false;
}


void CMuxProcessDisplay::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMuxProcessDisplay)
	DDX_Control(pDX, IDC_PROGRESS1, m_p1);
	//}}AFX_DATA_MAP
	muxFinishedCount=0;
}


BEGIN_MESSAGE_MAP(CMuxProcessDisplay, CDialog)
	//{{AFX_MSG_MAP(CMuxProcessDisplay)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMuxProcessDisplay message handlers

static UINT muxThreadFunction(LPVOID pParam){
	int i=0;
	int muxResult=0;
	int leng=videoAudioFileName.GetSize();
	CString strOutput;
	for(i=0;i<leng-1;i+=2){
		strOutput=videoAudioFileName[i].Left(videoAudioFileName[i].GetLength()-4);
		strOutput=strOutput+"_mux.avi";
		while(PathFileExists(strOutput)==TRUE){
			strOutput=strOutput.Left(strOutput.GetLength()-4);
			strOutput=strOutput+"_mux.avi";
		}
		muxResult=CAviToolInput::aviMux((LPTSTR)(LPCTSTR)(videoAudioFileName[i]),(LPTSTR)(LPCTSTR)(videoAudioFileName[i+1]),(LPTSTR)(LPCTSTR)strOutput);
		if(muxCancelling){
			muxCancelling=false;
			(CMuxProcessDisplay::muxFinishedCount)=(leng-1)>>1;
			multiplexing=false;
			return 0;
		}
		if(muxResult==1){
			DeleteFile(videoAudioFileName[i]);
			DeleteFile(videoAudioFileName[i+1]);
			MoveFile(strOutput,videoAudioFileName[i]);
		}
		(CMuxProcessDisplay::muxFinishedCount)++;
	}
	multiplexing=false;
	return 1;
}

BOOL CMuxProcessDisplay::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	multiplexing=true;
	AfxBeginThread(muxThreadFunction,NULL);
	
	SetTimer(10,100,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMuxProcessDisplay::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	m_p1.SetPos((int)((muxFinishedCount+(CAviToolInput::aviMuxProgress))*200.0/videoAudioFileName.GetSize()));
	if(!multiplexing){//(muxFinishedCount*2>=videoAudioFileName.GetSize()-1){
		KillTimer(10);
		CDialog::OnOK();
		CDialog::OnTimer(nIDEvent);
		return;
	}
	CDialog::OnTimer(nIDEvent);
}

void CMuxProcessDisplay::OnOK(){
	KillTimer(10);
	if(multiplexing){
		SetWindowText("正在取消，请稍候...");
		muxCancelling=true;
	}else{
		CDialog::OnOK();
	}
	
}
void CMuxProcessDisplay::OnCancel(){
	KillTimer(10);
	if(multiplexing){
		SetWindowText("正在取消，请稍候...");
		muxCancelling=true;
	}else{
		CDialog::OnCancel();
	}
	
}

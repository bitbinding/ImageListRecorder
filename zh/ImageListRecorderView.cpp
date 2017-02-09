// ImageListRecorderView.cpp : implementation of the CImageListRecorderView class
//

#include "stdafx.h"
#include "ImageListRecorder.h"

#include "ImageListRecorderDoc.h"
#include "ImageListRecorderView.h"
#include "RecordSettingDialog.h"
#include "SavingImageListDialog.h"
#include "MainFrm.h"
#include "AviToolInput.h"
#include "myxvid.h"
#include "myMP3Enc.h"
#include "MuxProcessDisplay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
 
extern CImageListRecorderApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CImageListRecorderView
static CImage *imaget;
static CImage *imageBackup;
static CString fileStr;
static CImageListRecorderView *pView;
static HANDLE hThreadWait;
//CString recordThreadStr;

PFUNxvid_global xvid_global;
PFUNxvid_decore xvid_decore;
PFUNxvid_encore xvid_encore;

extern PFUNMP3ENCInitAll MP3ENCInitAll;
extern PFUNMP3ENCUninitAll MP3ENCUninitAll;
//extern PFUNMP3ENCSetMP3Inf SetMP3Inf;
//extern PFUNMP3ENCGetMP3Inf GetMP3Inf;
extern PFUNMP3ENCSetInput SetInput;
extern PFUNMP3ENCSetInWavFmt SetInWavFmt;
extern PFUNMP3ENCStartRec StartRec;
extern PFUNMP3ENCPauseRec PauseRec;
extern PFUNMP3ENCStopRec StopRec;
//extern PFUNMP3ENCGetRecSec GetRecSec;
//extern PFUNMP3ENCGetRecStatus GetRecStatus;

int maxBufferLength;
int keyFrameStep;
int CImageListRecorderView::xvidVersion=0;
CStringArray videoAudioFileName;


//static HANDLE hThreadSave;
//static DWORD ThreadSaveID;

IMPLEMENT_DYNCREATE(CImageListRecorderView, CScrollView);\

BEGIN_MESSAGE_MAP(CImageListRecorderView, CScrollView)
	//{{AFX_MSG_MAP(CImageListRecorderView)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_TingZhiLuZhi, OnTingZhiLuZhi)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateFileNew)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateFileOpen)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_TingZhiLuZhi, OnUpdateTingZhiLuZhi)
	ON_COMMAND(ID_BEGINPAGE, OnBeginpage)
	ON_COMMAND(ID_AVITOOLSMENUITEM, OnAvitoolsmenuitem)
	ON_MESSAGE(WM_RECORDED,OnRecorded)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageListRecorderView construction/destruction

CImageListRecorderView::CImageListRecorderView()
{
	// TODO: add construction code here
	exiting=false;
	imgLeng=1;	
	cimage.LoadFromResource(AfxGetInstanceHandle(),   MAKEINTRESOURCE(IDB_BITMAP1));
	recordRect.top=0;
	recordRect.bottom=400;
	recordRect.left=0;
	recordRect.right=550;
	jiange=50;
	shichang=60;
	//filePath="D:";
	fileName0="";
	recordMode=false;
	showCursor=true;
	aviIsNull=true;
	aviFile=NULL;
	bitStream=NULL;

	imaget=NULL;
	imageBackup=NULL;
	fileStr="";
	pView=this;

	currentBufferLength=0;
	maxFileSize=50<<20;
	avi0Handle=NULL;
	memset(&avi0,0,sizeof(xvid_enc_create_t));
	memset(&aviFrame,0,sizeof(xvid_enc_frame_t));
	memset(&aviStates,0,sizeof(xvid_enc_stats_t));
	idx1=NULL;

	maxBufferLength=5000000;
	keyFrameStep=100;

	recordAudio=false;
	soundName0="";
	recordingSound=false;
	recordedSound=false;

	char dllFileName[1000]="xvidcore.dll";
	HMODULE hModule = ::LoadLibrary(dllFileName);
	if(hModule==NULL){
		MessageBox("程序目录或系统缺少xvidcore.dll文件，请在程序目录下加入。");
		xvid_global=NULL;
		xvid_decore=NULL;
		xvid_encore=NULL;
		xvidVersion=XVID_VERSION;
		return;
	}
	int dllFileNameLength=strlen(dllFileName);
	if(dllFileNameLength>=17 && dllFileName[dllFileNameLength-9]=='1'){
		xvidVersion=XVID_MAKE_VERSION(dllFileName[dllFileNameLength-9]-0x30,dllFileName[dllFileNameLength-7]-0x30,dllFileName[dllFileNameLength-5]-0x30);
	}else{
		xvidVersion=XVID_VERSION;
	}
	
	xvid_global = (PFUNxvid_global)::GetProcAddress(hModule,"xvid_global");
	xvid_decore = (PFUNxvid_decore)::GetProcAddress(hModule,"xvid_decore");
	xvid_encore = (PFUNxvid_encore)::GetProcAddress(hModule,"xvid_encore");
	
	memset(&xvidGblInit,0,sizeof(xvid_gbl_init_t));
	xvidGblInit.version=xvidVersion;
	if(xvid_global==NULL || xvid_decore==NULL || xvid_encore==NULL){
		MessageBox("xvid版本不兼容。");
	}
	if(xvid_global(NULL,XVID_GBL_INIT,&xvidGblInit,NULL)<0){
		MessageBox("xvid库初始化失败，可能是xvidcord.dll的版本不对。");
	}
	if(!myMp3EncLoadDllFile("mp3enc.dll")){
		MessageBox("找不到mp3enc.dll或mp3enc.dll的格式不正确。");
	}
	if(MP3ENCInitAll!=NULL){
		MP3ENCInitAll();
	}
	//recordThreadStr="";
	recordDelay=5000;
}

CImageListRecorderView::~CImageListRecorderView()
{
	exiting=true;
	recordMode=false;
	WaitForSingleObject(hThreadWait,5000);
	
	
	/*if(recordingSound && StopRec!=NULL){
		StopRec();
		recordingSound=false;
	}*/

	

	if(!aviIsNull && fileType==5){
		if(aviFile!=NULL){
			fclose(aviFile);
			aviFile=NULL;
		}
		FinishAviFile((LPTSTR)(LPCTSTR)fileName0,imgi+1,idx1);
		
	}
	if(!aviIsNull || aviFile==NULL){
		//cavifile.releaseAviFile();
		aviIsNull=true;
		if(aviFile!=NULL){
			fclose(aviFile);
			aviFile=NULL;
		}
	}
	if(avi0Handle!=NULL){
		xvid_encore (avi0Handle,XVID_ENC_DESTROY,NULL,NULL);//撤消编码实例
		avi0Handle=NULL;
	}
	if(bitStream!=NULL){
		delete []bitStream;
		bitStream=NULL;
	}
	if(idx1!=NULL){
		delete []idx1;
		idx1=NULL;
	}
	videoAudioFileName.RemoveAll();
	if(MP3ENCInitAll!=NULL && MP3ENCUninitAll!=NULL && !recordedSound){
		MP3ENCUninitAll();
	}
}

BOOL CImageListRecorderView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CImageListRecorderView drawing

void CImageListRecorderView::OnDraw(CDC* pDC)
{
	CImageListRecorderDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
		
	cimage.Draw(pDC->m_hDC,0,0);
	
	// TODO: add draw code for native data here
}

void CImageListRecorderView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	
	SetScrollSizes(MM_TEXT, CSize(cimage.GetWidth(),cimage.GetHeight()));
	
	//SetTimer(1,1000,NULL);
}

/////////////////////////////////////////////////////////////////////////////
// CImageListRecorderView printing

BOOL CImageListRecorderView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CImageListRecorderView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CImageListRecorderView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CImageListRecorderView diagnostics

#ifdef _DEBUG
void CImageListRecorderView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImageListRecorderView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CImageListRecorderDoc* CImageListRecorderView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageListRecorderDoc)));
	return (CImageListRecorderDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageListRecorderView message handlers


void savePicture(){
	//HBITMAP hbmp;
	//CDC cbmpdc;
	if(pView->fileType==1){
		fileStr += _T(".bmp");
		imaget->Save(fileStr,Gdiplus::ImageFormatBMP);
	}else if(pView->fileType==2){
		fileStr += _T(".jpg");
		imaget->Save(fileStr,Gdiplus::ImageFormatJPEG);		
	}else if(pView->fileType==3){
		fileStr += _T(".png");
		imaget->Save(fileStr,Gdiplus::ImageFormatPNG);
	}else if(pView->fileType==4){
		fileStr += _T(".gif");
		imaget->Save(fileStr,Gdiplus::ImageFormatGIF);
	}else if(pView->fileType==5 &&  !(pView->aviIsNull)){
		if(pView->aviFile==NULL){
			pView->currentBufferLength=0;
			pView->aviFile=fopen((LPTSTR)(LPCTSTR)(pView->fileName0),"ab");
			if(pView->aviFile==NULL){
				return;
			}
		}
		if(pView->imgi%keyFrameStep==0 && pView->imgi>0){
			if(pView->avi0Handle!=NULL){
				xvid_encore (pView->avi0Handle,XVID_ENC_DESTROY,NULL,NULL);//撤消编码实例
				pView->avi0Handle=NULL;
				pView->avi0.handle=NULL;
			}

			if(xvid_encore(NULL,XVID_ENC_CREATE,&(pView->avi0),NULL)<0){
				pView->recordMode=false;
				return;
			}
			pView->avi0Handle=pView->avi0.handle;
		}
		int leng=CImageListRecorderView::RecordXvidFrame(pView->aviFile,&(pView->cimage),&(pView->avi0),&(pView->aviFrame),&(pView->aviStates));
		if((pView->imgi)>=0 && (pView->imgi)<=(pView->imgLeng)){
			pView->idx1[pView->imgi]=leng;
		}
		if((leng&1)==1){
			leng++;
		}
		leng+=8;
		pView->currentBufferLength+=leng;
		if(pView->aviFile!=NULL && ftell(pView->aviFile)>pView->maxFileSize-(pView->imgi+1)*16-8 && (pView->imgi+1)<(pView->imgLeng)){
			fclose(pView->aviFile);
			pView->aviFile=NULL;
			//CTime timet=CTime::GetCurrentTime();
			//CTimeSpan dtime=timet-(pView->time0);
			//int nzpl=dtime.GetTotalSeconds()>0?((pView->imgi+1)*2+1)/((dtime.GetTotalSeconds())*2):0;
			CImageListRecorderView::FinishAviFile((LPTSTR)(LPCTSTR)(pView->fileName0),pView->imgi+1,pView->idx1,0);
			do{
				if(pView->aviFile!=NULL){
					fclose(pView->aviFile);
					pView->aviFile=NULL;
				}
				pView->fileName0=pView->fileName0.Left(pView->fileName0.GetLength()-4);
				pView->fileName0=pView->fileName0+"_a.avi";
				pView->aviFile=fopen((LPTSTR)(LPCTSTR)(pView->fileName0),"rb");
			}while(pView->aviFile!=NULL);

			if(pView->avi0Handle!=NULL){
				xvid_encore (pView->avi0Handle,XVID_ENC_DESTROY,NULL,NULL);//撤消编码实例
				pView->avi0Handle=NULL;
				pView->avi0.handle=NULL;
			}
			//if(pView->recordingSound && StopRec!=NULL){
			//if(pView->recordAudio && PauseRec!=NULL){
				//PauseRec();
			//}
				//pView->recordingSound=false;
			//}

			if(xvid_encore(NULL,XVID_ENC_CREATE,&(pView->avi0),NULL)<0){
				pView->recordMode=false;
				return;
			}
			pView->avi0Handle=pView->avi0.handle;
			
			pView->aviFile=fopen((LPTSTR)(LPCTSTR)(pView->fileName0),"wb");
			CImageListRecorderView::CreateAviHeader(pView->aviFile,pView->cimage.GetWidth(),pView->cimage.GetHeight(),1000/pView->jiange,(pView->cimage.GetWidth())*(pView->cimage.GetHeight())*3/2,false);
			pView->imgLeng-=pView->imgi;
			pView->imgi=-1;
			pView->currentBufferLength=0;
			fclose(pView->aviFile);
			pView->aviFile=NULL;
			pView->time0=COleDateTime::GetCurrentTime();
			//pView->recordMode=false;
		}
		if(pView->currentBufferLength>=maxBufferLength){
			fclose(pView->aviFile);
			pView->aviFile=NULL;
		}
	}
}

UINT recordPicture(LPVOID pParam=NULL){
	BOOL bFull=true;//是否截全屏,false则只截取当前活动的窗体
	 
	 //CImage image;
	 CWnd*  pWnd;
	 CRect  rect;
	 //BOOL   bStat;
	 if(bFull)
	 pWnd = CWnd::GetDesktopWindow();
	 else
	 pWnd = CWnd::GetActiveWindow();
	 ASSERT(pWnd);
	 if(pWnd == NULL){		 
		 return 0;
	 }

	 CWindowDC winDC(pWnd);

	 //pWnd->GetWindowRect(&rect);
	rect.left=pView->recordRect.left;
	rect.top=pView->recordRect.top;
	rect.right=pView->recordRect.right;
	rect.bottom=pView->recordRect.bottom;

	CImageDC imageDC(*imaget);
	::BitBlt(imageDC, 0, 0, rect.Width(), rect.Height(), winDC, rect.left, rect.top, SRCCOPY);
	if(pView->showCursor){
		CURSORINFO l_ci;
		l_ci.cbSize=sizeof(CURSORINFO);
		::GetCursorInfo(&l_ci);
		HCURSOR hCursor=l_ci.hCursor;
		POINT ptCursor;
		GetCursorPos(&ptCursor);
		ICONINFO IconInfo;
		if (GetIconInfo(hCursor, &IconInfo))
		{
		   ptCursor.x -=  IconInfo.xHotspot+rect.left;
		   ptCursor.y -=  IconInfo.yHotspot+rect.top;
		   if (IconInfo.hbmMask != NULL)
			DeleteObject(IconInfo.hbmMask);
		   if (IconInfo.hbmColor != NULL)
			DeleteObject(IconInfo.hbmColor);
		}
		DrawIconEx(
		 imageDC,          // handle to device context 
		 ptCursor.x, ptCursor.y,
		 hCursor,          // handle to icon to draw 
		 0,0,           // width of the icon 
		 0,            // index of frame in animated cursor 
		NULL,           // handle to background brush 
		 DI_NORMAL | DI_COMPAT       // icon-drawing flags 
		);
	 }
	 if(pView->IsIconic()==FALSE){
		pView->Invalidate();
	 }
	 ::ReleaseDC(pWnd->m_hWnd,winDC);
	
	 CString str2="";
	 if(pView->fileType<=4){
		fileStr.Format(_T("%s"),pView->fileName0);
		if(fileStr.GetLength()-1>=0 && fileStr.Right(1)!=_T("0")){
			str2.Format(_T("%d"),pView->imgi+1);
		}else if(fileStr.GetLength()-1>=0){
			fileStr.Delete(fileStr.GetLength()-1,1);
			if(pView->imgLeng/10==0){
				str2.Format(_T("%01d"),pView->imgi+1);
			}else if(pView->imgLeng/100==0){
				str2.Format(_T("%02d"),pView->imgi+1);
			}else if(pView->imgLeng/1000==0){
				str2.Format(_T("%03d"),pView->imgi+1);
			}else if(pView->imgLeng/10000==0){
				str2.Format(_T("%04d"),pView->imgi+1);
			}else if(pView->imgLeng/100000==0){
				str2.Format(_T("%05d"),pView->imgi+1);
			}else if(pView->imgLeng/1000000==0){
				str2.Format(_T("%06d"),pView->imgi+1);
			}else if(pView->imgLeng/10000000==0){
				str2.Format(_T("%07d"),pView->imgi+1);
			}else if(pView->imgLeng/100000000==0){
				str2.Format(_T("%08d"),pView->imgi+1);
			}else if(pView->imgLeng/1000000000==0){
				str2.Format(_T("%09d"),pView->imgi+1);
			}else{
				str2.Format(_T("%d"),pView->imgi+1);
			}
		}		
		fileStr+=str2;
	}
	savePicture();
	return 1;
}


void CImageListRecorderView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent!=1){
		CScrollView::OnTimer(nIDEvent);
		return;
	}
	
	CMainFrame* pMf = (CMainFrame*)AfxGetMainWnd();
	if(imgi>=imgLeng){
		if(IsIconic()!=FALSE){
			pMf->SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
		}
		OnTingZhiLuZhi();
		CScrollView::OnTimer(nIDEvent);
		return;
	}
	 imgi++;
	 recordPicture();
	
	CScrollView::OnTimer(nIDEvent);
}

UINT RecordThread(LPVOID pParam) 
{
	// TODO: Add your message handler code here and/or call default
	int i=0;
	CMainFrame* pMf=NULL;
	clock_t recordStartTime=0;//上一帧的开始录制时间
	clock_t recordCurrentTime=0;//上一帧的结束录制时间
	if(pView->fileType==5){
		if(xvid_encore(NULL,XVID_ENC_CREATE,&(pView->avi0),NULL)<0){
			AfxMessageBox("xvid库初始化编码失败，可能是xvidcord.dll的版本不对。",MB_OK,MB_ICONSTOP);
			pView->recordMode=false;
			return 0;
		}
		pView->avi0Handle=pView->avi0.handle;
		if(pView->avi0Handle==NULL){
			AfxMessageBox("创建avi句柄失败。",MB_OK,MB_ICONSTOP);
			pView->recordMode=false;
			return 0;
		}
	}

	if((pView->recordDelay)>0){
		int recordDelayTick=100;
		pMf=(CMainFrame*)AfxGetMainWnd();
		recordStartTime=clock();
		recordCurrentTime=recordStartTime;
		while(recordCurrentTime-recordStartTime<(pView->recordDelay)){
			recordCurrentTime=clock();
			::PostMessage(pMf->m_hWnd,WM_SETMAINFRAMETEXT,1,(pView->recordDelay)-(recordCurrentTime-recordStartTime));
			if(!(pView->recordMode)){
				::PostMessage(pMf->m_hWnd,WM_SETMAINFRAMETEXT,0,0);
				return 0;
			}
			Sleep(recordDelayTick);
			recordCurrentTime=clock();
			if(recordCurrentTime-recordStartTime>=(pView->recordDelay)){
				break;	
			}
		}
		pView->time0=COleDateTime::GetCurrentTime();
		::PostMessage(pMf->m_hWnd,WM_SETMAINFRAMETEXT,0,0);
		::PostMessage(pMf->m_hWnd,WM_SYSCOMMAND, SC_MINIMIZE, 0);
	}

	int fps = pView->jiange>0 ? 1000/pView->jiange : 30;//帧频率
	
	if(pView->fileType==5){
		pView->aviFile=fopen((LPTSTR)(LPCTSTR)(pView->fileName0),"wb");
		if(pView->aviFile==NULL){
			AfxMessageBox("创建文件失败！",MB_OK,MB_ICONSTOP);
			pView->recordMode=false;
			return 0;
		}
		if(CImageListRecorderView::CreateAviHeader(pView->aviFile,pView->cimage.GetWidth(),pView->cimage.GetHeight(),fps,(pView->cimage.GetWidth())*(pView->cimage.GetHeight())*3/2,false)==FALSE){
			if(pView->aviFile!=NULL){
				fclose(pView->aviFile);
				pView->aviFile=NULL;
			}
			pView->recordMode=false;
			return 0;
		}
		if(fclose(pView->aviFile)!=0){
			AfxMessageBox("初始化文件失败！",MB_OK,MB_ICONSTOP);
			pView->aviFile=NULL;
			pView->recordMode=false;
			return 0;
		}
		pView->aviFile=NULL;
		pView->currentBufferLength=0;
		pView->aviIsNull=false;
	}else{
		pView->aviFile=NULL;
		pView->currentBufferLength=0;
	}

	recordStartTime=0;//上一帧的开始录制时间
	recordCurrentTime=0;//上一帧的结束录制时间
	int shouldSleepTime=0;//线程应当睡眠的时间
	int minSleepTime=pView->jiange/2;//线程应当睡眠的最短时间
	int maxSleepTime=(pView->fileType>=5 || pView->jiange<1000)?1000:pView->jiange;//线程应当睡眠的最长时间
	bool willCalcluateShouldSleepTimeFromFps=(pView->fileType>=5 && 1000%pView->jiange!=0);//是否将按帧频率来计算线程应当睡眠的时间
	videoAudioFileName.RemoveAll();
	clock_t recordPictureStart=0;//执行记录和保存图像函数的开始时间
	clock_t recordPictureFinish=0;//执行记录和保存图像函数的结束时间
	int recordDuration=0;
	while(pView->recordMode){
		//if(pView->exiting){
			//return;
		//}
		recordCurrentTime=clock();
		if(pView->imgi<0){
			/*if(pView->recordingSound && StopRec!=NULL){
				StopRec();
				pView->recordingSound=false;
			}*/
			if(pView->recordAudio && StartRec!=NULL){
				// && !pView->recordingSound && StopRec!=NULL){
				pView->soundName0=pView->fileName0.Left(pView->fileName0.GetLength()-4);
				pView->soundName0=pView->soundName0+".mp3";
				videoAudioFileName.Add(pView->fileName0);
				videoAudioFileName.Add(pView->soundName0);
				StartRec((LPTSTR)(LPCTSTR)(pView->soundName0),0);
				pView->recordingSound=true;
			}
			recordStartTime=recordCurrentTime;
		}
		(pView->imgi)++;
		
		if(willCalcluateShouldSleepTimeFromFps){
			shouldSleepTime=(pView->imgi+1)*1000/fps-(recordCurrentTime-recordStartTime);
		}else{
			shouldSleepTime=(pView->imgi+1)*(pView->jiange)-(recordCurrentTime-recordStartTime);
		}
		if(shouldSleepTime<minSleepTime){
			shouldSleepTime=minSleepTime;
		}
		if(shouldSleepTime>maxSleepTime){
			shouldSleepTime=maxSleepTime;
		}
		recordPictureStart=clock();
		recordPicture();
		recordPictureFinish=clock();
		recordDuration=recordPictureFinish-recordPictureStart;
		if(recordDuration<0){
			recordDuration=0;
		}
		if(shouldSleepTime-recordDuration>0){
			Sleep(shouldSleepTime-recordDuration);
		}
		//CloseHandle(hThreadRecord);
		if((pView->imgi)%keyFrameStep==keyFrameStep-1 && pView->fileType==5){
			xvid_encore (pView->avi0Handle,XVID_ENC_DESTROY,NULL,NULL);
			pView->avi0.handle=NULL;
			xvid_encore(NULL,XVID_ENC_CREATE,&(pView->avi0),NULL);
			pView->avi0Handle=pView->avi0.handle;
		}
		if((pView->imgi)>=(pView->imgLeng)-1){
			if(pView->recordingSound && StopRec!=NULL){
				StopRec();
				pView->recordingSound=false;
			}
			pMf = (CMainFrame*)AfxGetMainWnd();
			if(pView->IsIconic()!=FALSE){
				pMf->SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
			}
			pView->recordMode=false;
			pView->TingZhiLuZhi();
		}
	}
	
	if(!pView->exiting && !pView->aviIsNull && pView->fileType>=5){
		if(pView->recordingSound && MP3ENCInitAll!=NULL && MP3ENCUninitAll!=NULL){
			MP3ENCUninitAll();
		}
		pView->TingZhiLuZhi();
	}
	return 1;
}




void CImageListRecorderView::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CScrollView::OnShowWindow(bShow, nStatus);
	// TODO: Add your message handler code here			
}

void CImageListRecorderView::OnFileNew() 
{
	// TODO: Add your command handler code here
	static CString deviceStr="";
	static CString lineInStr="";
	CMainFrame* pMf = (CMainFrame*)AfxGetMainWnd();
	CRect rect;
	pMf->GetWindowRect(&rect);
	rect.right=rect.left+100;
	rect.bottom=rect.top+100;
	pMf->MoveWindow(&rect);
	
	CRecordSettingDialog dlg;
	if(recordedSound){
		dlg.deviceString0=deviceStr;
		dlg.lineInString0=lineInStr;
		dlg.willGetInputDeviceName=false;
	}

	CFileDialog dlg2(FALSE,NULL,NULL,NULL,"BMP序列(*.bmp)|*.bmp|JPG序列(*.jpg;jpeg)|*.jpg;*.jpeg|PNG序列(*.png)|*.png|GIF序列(*.gif)|*.gif|XVID格式压缩的AVI文件(*.avi)|*.avi||");//无压缩的AVI文件(*.avi)|*.avi||");
	int nStructSize = 0;
    DWORD dwVersion, dwWindowsMajorVersion, dwWindwosMinorVersion;
    dwVersion = GetVersion();
    dwWindowsMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
    dwWindwosMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));
    if(dwVersion < 0x80000000)
    {
    nStructSize = 88; //2k,xp下显示新的版本
    }
    else
    {
    nStructSize = 76; //98下显示老版本，如图1
    }
    dlg2.m_ofn.lStructSize = nStructSize; //------用来检测目前的操作系统的版本信息
	dlg2.m_ofn.nFilterIndex=5;

	readRecordLogFile(&dlg,dlg2.m_ofn.nFilterIndex);
	unsigned long fileTypet=dlg2.m_ofn.nFilterIndex;

	if(dlg.DoModal()==IDOK && dlg2.DoModal()==IDOK){
		recordRect.left=dlg.m_e1;
		recordRect.top=dlg.m_e2;
		recordRect.right=dlg.m_e1+dlg.m_e3;
		recordRect.bottom=dlg.m_e2+dlg.m_e4;
		jiange=dlg.m_e5;
		fileName0=dlg2.GetPathName();
		FILE *fp=fopen((LPTSTR)(LPCTSTR)fileName0,"r");
		if(fp!=NULL){
			fclose(fp);
			fp=NULL;
			if(MessageBox(fileName0+"文件已存在，是否替换？","注意",MB_YESNO|MB_ICONWARNING)!=IDYES){
				return;
			}
		}
		CString fileName0R4=fileName0.Right(4);
		fileName0R4.MakeLower();
		CString fileName0R5=fileName0.Right(5);
		fileName0R5.MakeLower();
		if(fileName0R4==".bmp"){
			fileName0=fileName0.Left(fileName0.GetLength()-4);
			fileType=1;
		}else if(fileName0R4==".jpg"){
			fileName0=fileName0.Left(fileName0.GetLength()-4);
			fileType=2;
		}else if(fileName0R5==".jpeg"){
			fileName0=fileName0.Left(fileName0.GetLength()-5);
			fileType=2;
		}else if(fileName0R4==".png"){
			fileName0=fileName0.Left(fileName0.GetLength()-4);
			fileType=3;
		}else if(fileName0R4==".gif"){
			fileName0=fileName0.Left(fileName0.GetLength()-4);
			fileType=4;
		}else if(fileName0R4==".avi"){
			fileType=5;
		}else{
			fileType=dlg2.m_ofn.nFilterIndex;
		}
		if(fileType<=0){
			fileType=1;
		}
		if(fileType>6){
			fileType=6;
		}
		fileTypet=fileType;
		
		
		if(jiange<=0){
			jiange=1;
		}else if(fileType>=5 && jiange>1000){
			jiange=1;
		}
		int fps=1000/jiange;
		

		shichang=dlg.m_e6;
		showCursor=dlg.m_c1==TRUE?true:false;		
		
		imgLeng=shichang*1000/jiange;
		
		int imageWidth=recordRect.Width();
		int imageHeight=recordRect.Height();
		if(fileType==5 && (imageWidth&1)==1){
			imageWidth++;
			recordRect.right++;
		}
		if(fileType==5 && (imageHeight&1)==1){
			imageHeight++;
			recordRect.bottom++;
		}
		if(fileType==5 && imageWidth==640 && imageHeight==480){
			imageWidth+=2;
			recordRect.right++;
		}

		if(!cimage.IsNull()){
			cimage.Destroy();
		}
		if(cimage.Create(imageWidth,imageHeight,24)!=TRUE){
			MessageBox("创建位图失败");
			return;
		}
		if(!cimage2.IsNull()){
			cimage2.Destroy();
		}
		if(cimage2.Create(imageWidth,imageHeight,24)!=TRUE){
			MessageBox("创建位图失败");
			return;
		}
		imaget=&cimage;
		imageBackup=&cimage2;
		if(imageBackup==NULL){
			MessageBox("!");
		}

		recordAudio=((dlg.soundInputType>0) && fileType>=5);
		//autoChangeFps=(dlg.m_c2==TRUE);
		maxFileSize=(dlg.m_e7<<20);
		recordDelay=(dlg.m_e8)*1000;
		
		if(fileType==5){
			if(xvid_global==NULL || xvid_decore==NULL || xvid_encore==NULL){
				MessageBox("找不到兼容的xvidcore.dll文件，因而不能进行录制，请在程序目录上加入。");
				return;
			}
			if(fileName0R4!=".avi"){
				fileName0 += ".avi";
			}
			
			if(!aviIsNull){
				//cavifile.releaseAviFile();
				aviIsNull=true;
			}
			
			
			

			memset(&avi0,0,sizeof(xvid_enc_create_t));
			avi0.version=xvidVersion;
			avi0.profile=0xf5;//XVID_PROFILE_AS_L4;
			
			avi0.width=imageWidth;
			avi0.height=imageHeight;
			avi0.fincr=1;
			avi0.fbase=fps;
		
			
			memset(&aviFrame,0,sizeof(xvid_enc_frame_t));
			aviFrame.version=xvidVersion;
			aviFrame.input.csp=XVID_CSP_BGR;
			int pitch=cimage.GetPitch();
			
			
			if(pitch<0){
				pitch=-pitch;
				aviFrame.input.stride[0]=pitch;
				aviFrame.input.plane[0]=(BYTE *)cimage.GetPixelAddress(0,imageHeight-1);
			}else{
				aviFrame.input.stride[0]=pitch;
				aviFrame.input.plane[0]=(BYTE *)(cimage.GetBits());
			}
			if(bitStream!=NULL){
				delete []bitStream;
				bitStream=NULL;
			}
			bitStream=new unsigned char[(imageWidth*imageHeight<<2)+1000];
			aviFrame.bitstream=bitStream;
			aviFrame.length=(imageWidth*imageHeight<<2)+1000;
			aviFrame.type=XVID_TYPE_AUTO;
			aviFrame.quant=dlg.m_e9;
			memset(&aviStates,0,sizeof(xvid_enc_stats_t));
			aviStates.version=xvidVersion;
			if(imgLeng>0 && imgLeng<(16<<20)){
				idx1=new int[imgLeng];
			}else if(idx1!=NULL){
				delete []idx1;
				idx1=NULL;
			}
			if(recordAudio && !recordedSound && SetInput!=NULL && SetInWavFmt!=NULL){
				SetInput((LPTSTR)(LPCTSTR)(dlg.deviceString),(LPTSTR)(LPCTSTR)(dlg.lineInString));
				SetInWavFmt(2,44100);
				deviceStr=dlg.deviceString;
				lineInStr=dlg.lineInString;
			}
		}
		imgi=-1;
		if(recordDelay==0){
			pMf->SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		}
		SetScrollSizes(MM_TEXT, CSize(imageWidth,imageHeight));		
		recordMode=true;
		time0=COleDateTime::GetCurrentTime();
		/*if(!firstRecord){
			KillTimer(2);
		}*/
		//SetTimer(1,jiange,NULL);
		hThreadWait=AfxBeginThread(RecordThread,NULL)->m_hThread;
	}
	if(!dlg.cancelledWithWrongData){
		writeRecordLogFile(&dlg,fileTypet);
	}
}

BOOL CImageListRecorderView::CreateAviHeader(FILE *fp,int wid,int hei,int fps,int size,bool recordAudio){
	recordAudio=false;
	if(fp==NULL){
		return FALSE;
	}
	int i=0,j=0,leng=0;
	fputc('R',fp);
	fputc('I',fp);
	fputc('F',fp);
	fputc('F',fp);
	fputc(0,fp);//需要在录完后进行修改(4H)
	fputc(0,fp);
	fputc(0,fp);
	fputc(0,fp);
	fputc('A',fp);
	fputc('V',fp);
	fputc('I',fp);
	fputc(' ',fp);

	//---------------------------------------------------------------------------
	fputc('L',fp);//hdrl 列表
	fputc('I',fp);
	fputc('S',fp);
	fputc('T',fp);
	fputc(0x0c,fp);
	fputc(0x01,fp);
	fputc(0,fp);
	fputc(0,fp);
	fputc('h',fp);
	fputc('d',fp);
	fputc('r',fp);
	fputc('l',fp);

	//---------------------------------------------------------------------------
	fputc('a',fp);//avi头部
	fputc('v',fp);
	fputc('i',fp);
	fputc('h',fp);

	fputc(56,fp);//avi主信息头大小
	fputc(0,fp);
	fputc(0,fp);
	fputc(0,fp);

	fputc(0x40,fp);//avi显示每帧所需的时间为40000ns
	fputc(0x9c,fp);
	fputc(0,fp);
	fputc(0,fp);

	fputc(0,fp);//avi文件的最大数据率
	fputc(0,fp);
	fputc(0,fp);
	fputc(0,fp);

	fputc(0,fp);//avi数据填充的粒度
	fputc(0,fp);
	fputc(0,fp);
	fputc(0,fp);

	fputc(0x10,fp);//avi全局标记
	fputc(0x09,fp);
	fputc(0,fp);
	fputc(0,fp);

	fputc(0,fp);//总帧数，需要在录完后进行修改(30H)
	fputc(0,fp);
	fputc(0,fp);
	fputc(0,fp);

	fputc(0,fp);//交互格式时的初始帧数
	fputc(0,fp);
	fputc(0,fp);
	fputc(0,fp);

	fputc(recordAudio?2:1,fp);//流的个数（有声音为2，无声音为1）
	fputc(0,fp);
	fputc(0,fp);
	fputc(0,fp);

	fputc(0,fp);//建议读取本文件的缓存大小
	fputc(0,fp);
	fputc(0,fp);
	fputc(0,fp);
	
	
	fputc(wid&0xff,fp);//视频图像的宽
	fputc(wid>>8&0xff,fp);
	fputc(0,fp);
	fputc(0,fp);

	fputc(hei&0xff,fp);//视频图像的高
	fputc(hei>>8&0xff,fp);
	fputc(0,fp);
	fputc(0
		,fp);
	
	//保留字节，设为0
	for(i=0;i<16;i++){
		fputc(0,fp);
	}
	
	//---------------------------------------------------------------------------
	fputc('L',fp);//视频流子列表strl
	fputc('I',fp);
	fputc('S',fp);
	fputc('T',fp);
	fputc(0xc0,fp);
	fputc(0,fp);
	fputc(0,fp);
	fputc(0,fp);
	fputc('s',fp);
	fputc('t',fp);
	fputc('r',fp);
	fputc('l',fp);

	//---------------------------------------------------------------------------
	fputc('s',fp);//视频流头
	fputc('t',fp);
	fputc('r',fp);
	fputc('h',fp);

	fputc(56,fp);//视频流头大小
	fputc(0,fp);
	fputc(0,fp);
	fputc(0,fp);
	
	fputc('v',fp);//视频流类型
	fputc('i',fp);
	fputc('d',fp);
	fputc('s',fp);

	fputc('X',fp);//视频流的处理者
	fputc('V',fp);
	fputc('I',fp);
	fputc('D',fp);
	
	//12字节的0
	for(i=0;i<12;i++){
		fputc(0,fp);
	}

	fputc(1,fp);//时间尺度
	fputc(0,fp);
	fputc(0,fp);
	fputc(0,fp);

	fputc(fps&0xff,fp);//帧频率，可能需要在录完后进行修改(84H)
	fputc(fps>>8&0xff,fp);
	fputc(fps>>16&0xff,fp);
	fputc(fps>>24&0xff,fp);

	fputc(0,fp);//开始时间
	fputc(0,fp);
	fputc(0,fp);
	fputc(0,fp);

	fputc(0,fp);//总帧数，需要在录完后进行修改(8cH)
	fputc(0,fp);
	fputc(0,fp);
	fputc(0,fp);

	fputc(0xd0,fp);//读取这个流数据建议使用的缓存大小，可能需要在录完后进行修改(90H)
	fputc(0x86,fp);
	fputc(0,fp);
	fputc(0,fp);

	//12字节的0
	for(i=0;i<12;i++){
		fputc(0,fp);
	}

	fputc(wid&0xff,fp);//视频图像的宽
	fputc(wid>>8&0xff,fp);

	fputc(hei&0xff,fp);//视频图像的高
	fputc(hei>>8&0xff,fp);

	//---------------------------------------------------------------------------
	fputc('s',fp);//视频流描述列表
	fputc('t',fp);
	fputc('r',fp);
	fputc('f',fp);

	fputc(0x28,fp);//视频流描述头大小
	fputc(0,fp);
	fputc(0,fp);
	fputc(0,fp);

	//---------------------------------------------------------------------------
	fputc(0x28,fp);//视频流描述头，这里是其大小
	fputc(0,fp);
	fputc(0,fp);
	fputc(0,fp);

	fputc(wid&0xff,fp);//视频图像的宽
	fputc(wid>>8&0xff,fp);
	fputc(wid>>16&0xff,fp);
	fputc(wid>>24&0xff,fp);

	fputc(hei&0xff,fp);//视频图像的高
	fputc(hei>>8&0xff,fp);
	fputc(hei>>16&0xff,fp);
	fputc(hei>>24&0xff,fp);

	fputc(1,fp);//位平面数
	fputc(0,fp);

	fputc(12,fp);//颜色位数
	fputc(0,fp);

	fputc('X',fp);//压缩方式
	fputc('V',fp);
	fputc('I',fp);
	fputc('D',fp);

	fputc(size&0xff,fp);//图像大小
	fputc(size>>8&0xff,fp);
	fputc(size>>16&0xff,fp);
	fputc(size>>24&0xff,fp);

	//16字节的0
	for(i=0;i<16;i++){
		fputc(0,fp);
	}

	//---------------------------------------------------------------------------
	fputc('v',fp);//vprp
	fputc('p',fp);
	fputc('r',fp);
	fputc('p',fp);


	fputc(0x44,fp);
	fputc(0,fp);
	fputc(0,fp);
	fputc(0,fp);

	//8字节的0
	for(i=0;i<8;i++){
		fputc(0,fp);
	}

	fputc(0x19,fp);
	fputc(0,fp);
	fputc(0,fp);
	fputc(0,fp);

	fputc(wid&0xff,fp);//视频图像的宽
	fputc(wid>>8&0xff,fp);
	fputc(wid>>16&0xff,fp);
	fputc(wid>>24&0xff,fp);

	fputc(hei&0xff,fp);//视频图像的高
	fputc(hei>>8&0xff,fp);
	fputc(hei>>16&0xff,fp);
	fputc(hei>>24&0xff,fp);

	fputc(3,fp);//?
	fputc(0,fp);

	fputc(4,fp);//?
	fputc(0,fp);

	fputc(wid&0xff,fp);//视频图像的宽
	fputc(wid>>8&0xff,fp);
	fputc(wid>>16&0xff,fp);
	fputc(wid>>24&0xff,fp);

	fputc(hei&0xff,fp);//视频图像的高
	fputc(hei>>8&0xff,fp);
	fputc(hei>>16&0xff,fp);
	fputc(hei>>24&0xff,fp);

	fputc(1,fp);
	fputc(0,fp);
	fputc(0,fp);
	fputc(0,fp);

	fputc(hei&0xff,fp);//视频图像的高
	fputc(hei>>8&0xff,fp);
	fputc(hei>>16&0xff,fp);
	fputc(hei>>24&0xff,fp);

	fputc(wid&0xff,fp);//视频图像的宽
	fputc(wid>>8&0xff,fp);
	fputc(wid>>16&0xff,fp);
	fputc(wid>>24&0xff,fp);

	fputc(hei&0xff,fp);//视频图像的高
	fputc(hei>>8&0xff,fp);
	fputc(hei>>16&0xff,fp);
	fputc(hei>>24&0xff,fp);

	fputc(wid&0xff,fp);//视频图像的宽
	fputc(wid>>8&0xff,fp);
	fputc(wid>>16&0xff,fp);
	fputc(wid>>24&0xff,fp);

	//16字节的0
	for(i=0;i<16;i++){
		fputc(0,fp);
	}

	//---------------------------------------------------------------------------
	if(recordAudio){
		fputc('L',fp);//音频流子列表strl
		fputc('I',fp);
		fputc('S',fp);
		fputc('T',fp);

		fputc(0x6a,fp);
		fputc(0,fp);
		fputc(0,fp);
		fputc(0,fp);

		fputc('s',fp);
		fputc('t',fp);
		fputc('r',fp);
		fputc('l',fp);

		//---------------------------------------------------------------------------
		fputc('s',fp);
		fputc('t',fp);
		fputc('r',fp);
		fputc('h',fp);

		fputc(38,fp);
		fputc(0,fp);
		fputc(0,fp);
		fputc(0,fp);

		fputc('a',fp);
		fputc('u',fp);
		fputc('d',fp);
		fputc('s',fp);

		fputc(0,fp);//mp3为0x55
		fputc(0,fp);
		fputc(0,fp);
		fputc(0,fp);

		//8字节的0
		for(i=0;i<8;i++){
			fputc(0,fp);
		}

		fputc(0,fp);//初始帧数，可能需要在录完后进行修改(144H)
		fputc(0,fp);
		fputc(0,fp);
		fputc(0,fp);

		fputc(1,fp);//时间尺度
		fputc(0,fp);
		fputc(0,fp);
		fputc(0,fp);

		fputc(0x80,fp);//流的速率
		fputc(2,fp);
		fputc(0,fp);
		fputc(0,fp);

		fputc(0,fp);//流的开始时间，可能需要在录完后进行修改(150H)
		fputc(0,fp);
		fputc(0,fp);
		fputc(0,fp);

		fputc(0,fp);//音频大小，需要在录完后进行修改(154H)
		fputc(0,fp);
		fputc(0,fp);
		fputc(0,fp);

		fputc(0,fp);//读取这个流数据建议使用的缓存大小
		fputc(0x10,fp);
		fputc(0,fp);
		fputc(0,fp);

		fputc(0,fp);//流数据的质量指标
		fputc(0,fp);
		fputc(0,fp);
		fputc(0,fp);

		fputc(1,fp);//采样大小
		fputc(0,fp);
		fputc(0,fp);
		fputc(0,fp);

		//8字节的0
		for(i=0;i<8;i++){
			fputc(0,fp);
		}

		//---------------------------------------------------------------------------
		fputc('s',fp);
		fputc('t',fp);
		fputc('r',fp);
		fputc('f',fp);

		fputc(0x1e,fp);
		fputc(0,fp);
		fputc(0,fp);
		fputc(0,fp);

		//---------------------------------------------------------------------------
		fputc(1,fp);//编码格式，mp3是0x55
		fputc(0,fp);

		fputc(1,fp);//声道格式
		fputc(0,fp);

		fputc(0x44,fp);//采样频率(44100Hz)
		fputc(0xac,fp);
		fputc(0,fp);
		fputc(0,fp);
		
		fputc(0x80,fp);//每秒字节数(16000)
		fputc(0x3e,fp);
		fputc(0,fp);
		fputc(0,fp);

		fputc(1,fp);//每个采样占字节数
		fputc(0,fp);
		fputc(0,fp);
		fputc(0,fp);
	}

	//---------------------------------------------------------------------------
	fputc('L',fp);
	fputc('I',fp);
	fputc('S',fp);
	fputc('T',fp);

	fputc(0x34,fp);
	fputc(0,fp);
	fputc(0,fp);
	fputc(0,fp);

	char movieInfo0[12]="INFOISFT'";
	for(i=strlen(movieInfo0);i<12;i++){
		movieInfo0[i]=0;
	}
	char movieInfo[40]="ImageListRecorder version(1.0)";
	for(i=strlen(movieInfo);i<40;i++){
		movieInfo[i]=0;
	}

	fwrite(movieInfo0,1,12,fp);
	fwrite(movieInfo,1,40,fp);

	//---------------------------------------------------------------------------
	char movieJunk[50]="[= MPlayer junk data! =]";
	char movieJunkLeng=strlen(movieJunk);
	leng=0x1000-ftell(fp)-8;

	fputc('J',fp);
	fputc('U',fp);
	fputc('N',fp);
	fputc('K',fp);

	fputc(leng&0xff,fp);
	fputc(leng>>8&0xff,fp);
	fputc(leng>>16&0xff,fp);
	fputc(leng>>24&0xff,fp);
	j=0;
	for(i=0;i<leng;i++){
		fputc(movieJunk[j],fp);
		j++;
		if(j>=movieJunkLeng){
			j=0;
		}
	}

	//---------------------------------------------------------------------------
	fputc('L',fp);//movi
	fputc('I',fp);
	fputc('S',fp);
	fputc('T',fp);

	fputc(0,fp);
	fputc(0,fp);
	fputc(0,fp);
	fputc(0,fp);//视频流的长度，需要在录完后进行修改(1004H)


	fputc('m',fp);
	fputc('o',fp);
	fputc('v',fp);
	fputc('i',fp);


	
	return TRUE;
}

int CImageListRecorderView::RecordXvidFrame(FILE *fp,CImage *image,xvid_enc_create_t *avi0,xvid_enc_frame_t *frame,xvid_enc_stats_t *states){
	if(fp==NULL || image==NULL || image->IsNull() || avi0==NULL || frame==NULL || states==NULL || frame->bitstream==NULL){
		return -1;
	}
	int width=image->GetWidth();
	int pitch=image->GetPitch();
	int height=image->GetHeight();

	frame->input.csp=pitch<0?(XVID_CSP_BGR|XVID_CSP_VFLIP):XVID_CSP_BGR;
	if(pitch<0){
		pitch=-pitch;
		frame->input.stride[0]=pitch;
		frame->input.plane[0]=(BYTE *)image->GetPixelAddress(0,height-1);
	}else{
		frame->input.stride[0]=pitch;
		frame->input.plane[0]=(BYTE *)(image->GetBits());
	}
	fputc('0',fp);
	fputc('0',fp);
	fputc('d',fp);
	fputc('c',fp);
	memset(states,0,sizeof(xvid_enc_stats_t));
	states->version=xvidVersion;
	int length=xvid_encore(avi0->handle,XVID_ENC_ENCODE,frame,states);
	if(length<0){
		length=0;
	}
	int lengtht=length;
	if((length&1)==1){
		lengtht++;
	}
	fputc(length&0xff,fp);
	fputc(length>>8&0xff,fp);
	fputc(length>>16&0xff,fp);
	fputc(length>>24&0xff,fp);
	//fwrite(handle,1,4,fp);
	//fwrite(avi0,1,sizeof(xvid_enc_create_t),fp);
	//fwrite(frame,1,sizeof(xvid_enc_frame_t),fp);
	//return 8;
	fwrite(frame->bitstream,1,length,fp);
	if((length&1)==1){
		fputc(0,fp);
	}
	return length;
}

BOOL CImageListRecorderView::FinishAviFile(char *fileName,int idx1Leng,int *idx1,int nfps){
	
	if(fileName==NULL){
		return FALSE;
	}
	if(idx1Leng<=0){
		return FALSE;
	}
	int i=0,j=0;
	FILE *fp=fopen(fileName,"rb+");
	if(fp==NULL){
		return FALSE;
	}
	fseek(fp,0,SEEK_END);
	int leng0=ftell(fp);
	//CString cstr="";
	//CString cstr2="";
	//cstr.Format("%d;%x;%x;",feof(fp),ftell(fp),leng0);
	if(leng0<0x1020 || leng0>=0x7fffffff-8-16*idx1Leng){
		fclose(fp);
		fp=NULL;
		return FALSE;
	}
	int lengt=leng0+16*idx1Leng;
	fseek(fp,4,SEEK_SET);
	fwrite(&lengt,1,4,fp);
	fseek(fp,0x30,SEEK_SET);
	
	fwrite(&(idx1Leng),1,4,fp);
	if(nfps>0){
		fseek(fp,0x84,SEEK_SET);
		fwrite(&(nfps),1,4,fp);
	}
	fseek(fp,0x8c,SEEK_SET);
	fwrite(&(idx1Leng),1,4,fp);
	int videoLength=leng0-0x1008;
	fseek(fp,0x1004,SEEK_SET);
	fwrite(&(videoLength),1,4,fp);
	fseek(fp,0x100c,SEEK_SET);

	int idx1t;
	bool zeroMode=false;
	char label[5];
	
	//cstr2.Format("%x;",leng0);
	//cstr+=cstr2;
	bool idx1IsNull0=(idx1==NULL);
	if(idx1IsNull0){
		idx1=new int[idx1Leng];
		for(i=0;i<idx1Leng;i++){
			if(zeroMode){
				idx1[i]=0;
				idx1t=idx1[i];
				//cstr2.Format("%x,%x,%x;",idx1[i],idx1t,ftell(fp));
			}else{
				fread((char *)label,1,4,fp);
				fread(&(idx1[i]),1,4,fp);
				idx1t=((idx1[i])&1)==0?idx1[i]:idx1[i]+1;
				if(label[0]=='0' && label[1]=='1' && label[2]=='w' && label[3]=='b'){
					idx1[i]=-idx1[i]-1;
				}
				//cstr2.Format("%x,%x,%x;",idx1[i],idx1t,ftell(fp));
				if(ftell(fp)+idx1t+8>=leng0){
					zeroMode=true;
				}else{
					fseek(fp,idx1t,SEEK_CUR);
				}
			}
		}
	}
	
	//::MessageBox(NULL,cstr,"",MB_OK);
	fseek(fp,0,SEEK_SET);
	fclose(fp);
	fp=NULL;

	fp=fopen(fileName,"ab");
	if(fp==NULL){
		if(idx1IsNull0){
			delete []idx1;
			idx1=NULL;
		}
		return FALSE;
	}
	fputc('i',fp);
	fputc('d',fp);
	fputc('x',fp);
	fputc('1',fp);
	int idx1BytesLeng=idx1Leng*16;
	fwrite(&(idx1BytesLeng),1,4,fp);
	int idx1Sum=4;
	j=-1;
	for(i=0;i<idx1Leng;i++){
		if(idx1[i]>=0){
			j++;
			fputc('0',fp);
			fputc('0',fp);
			fputc('d',fp);
			fputc('c',fp);
		}else{
			fputc('0',fp);
			fputc('1',fp);
			fputc('w',fp);
			fputc('b',fp);
		}

		fputc(j%keyFrameStep==0 || idx1[i]<0 ? 0x10 :0,fp);
		fputc(0,fp);
		fputc(0,fp);
		fputc(0,fp);
		
		fwrite(&idx1Sum,1,4,fp);
		idx1t=(idx1[i])>=0?idx1[i]:-(idx1[i]+1);
		fwrite(&idx1t,1,4,fp);
		
		idx1t=((idx1[i])&1)==0?idx1[i]:idx1[i]+1;
		if(idx1Sum+0x1008+idx1t+8<leng0){
			idx1Sum+=idx1t+8;
		}
	}

	if(idx1IsNull0){
		delete []idx1;
		idx1=NULL;
	}
	fclose(fp);
	fp=NULL;
	
	return TRUE;
}

void CImageListRecorderView::OnTingZhiLuZhi() 
{
	// TODO: Add your command handler code here
	if(recordMode){
		//KillTimer(1);
		//imgLeng=imgi+1;
		//ReplayImgList();
		recordMode=false;
		if(pView->fileType<5){
			TingZhiLuZhi();
		}
	}else{
		imgi=0;
	}
}

void CImageListRecorderView::TingZhiLuZhi(){
	COleDateTime timet=COleDateTime::GetCurrentTime();
	CString wstr="";
	COleDateTimeSpan dtime=timet-time0;
	bool aviIsNull2=aviIsNull;
	if(!aviIsNull2 && fileType==5){
		if(aviFile!=NULL){
			fclose(aviFile);
			aviFile=NULL;
		}
		FinishAviFile((LPTSTR)(LPCTSTR)fileName0,imgi+1,idx1);
		if(avi0Handle!=NULL){
			xvid_encore (avi0Handle,XVID_ENC_DESTROY,NULL,NULL);//撤消编码实例
			avi0Handle=NULL;
		}
		
	}
	if(bitStream!=NULL){
		delete []bitStream;
		bitStream=NULL;
	}
	if(idx1!=NULL){
		delete []idx1;
		idx1=NULL;
	}
	if(!aviIsNull){
		//cavifile.releaseAviFile();
		aviIsNull=true;
	}
	//if(dtime.GetTotalSeconds()<=0){
		//return;
	//}
	int nzpl=((imgi+1)*2+1)/((dtime.GetTotalSeconds())*2);
	if(nzpl<=0){
		nzpl=1;
	}
	imaget=NULL;
	imageBackup=NULL;
	
	if(!exiting){
		wstr.Format("按帧换算录制了%d分%d秒，实际录制了%d分%d秒，帧频率是%d帧每秒。",(imgi+1)*jiange/1000/60,(imgi+1)*jiange/1000%60,(int)dtime.GetTotalMinutes(),(int)dtime.GetSeconds(),nzpl);
		AfxGetMainWnd()->ShowWindow(SW_SHOW);
		MessageBox(wstr);
		if(fileType==5 && recordAudio){
			recordedSound=true;
			PostMessage(WM_RECORDED,0,0);
			//OnRecorded(0,0);
		}else{
			videoAudioFileName.RemoveAll();
		}
	}
}

LRESULT CImageListRecorderView::OnRecorded(WPARAM wParam, LPARAM lParam)
{
    //TODO: Add your message handle code
	if(videoAudioFileName.GetSize()<=0){
		return 0;
	}
	CMuxProcessDisplay dlg;
	dlg.DoModal();
	videoAudioFileName.RemoveAll();
	CString sPath;//获取当前程序路径
	GetModuleFileName(NULL,sPath.GetBufferSetLength (MAX_PATH+1),MAX_PATH);
	sPath.ReleaseBuffer ();
	ShellExecute(NULL,"open",sPath,NULL,NULL,SW_SHOWNORMAL);
	AfxGetMainWnd()->PostMessage(WM_CLOSE); 
    return 1;
}

void CImageListRecorderView::OnFileSave() 
{
	// TODO: Add your command handler code here
	//KillTimer(2);
	//firstRecord=true;
	
	/*if(dlg.DoModal()==IDOK){
		fileName0=dlg.GetPathName();	
		CSavingImageListDialog dlg2;
		dlg2.fileName0=fileName0;
		dlg2.fileType=dlg.m_ofn.nFilterIndex;
		dlg2.img=img;
		dlg2.imgLeng=imgLeng;
		dlg2.DoModal();
	}*/
}

void CImageListRecorderView::OnUpdateFileNew(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(!recordMode && aviIsNull)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CImageListRecorderView::OnUpdateFileOpen(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CImageListRecorderView::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(!recordMode && imgLeng>0)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CImageListRecorderView::OnUpdateTingZhiLuZhi(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(recordMode)
		pCmdUI->SetText("停止录制(&I)\tEsc");
	else
		pCmdUI->SetText("从头播放(&I)\tEsc");
}

void CImageListRecorderView::OnBeginpage() 
{
	// TODO: Add your command handler code here
	if(recordMode || !aviIsNull){
		return;
	}
	if(!(cimage.IsNull())){
		cimage.Destroy();
	}
	cimage.LoadFromResource(AfxGetInstanceHandle(),   MAKEINTRESOURCE(IDB_BITMAP1));
	Invalidate();
}

void scaleFilter0(CImage &image0, CImage &imaget){
	int wid=image0.GetWidth();
	int hei=image0.GetHeight();
	int nw=imaget.GetWidth();
	int nh=imaget.GetHeight();
	BYTE* mByte=(BYTE *)image0.GetPixelAddress(0,0);//原图指针
	BYTE* pByte=(BYTE *)imaget.GetPixelAddress(0,0);//预览图指针
	int i=0,j=0,k=0,l=0,m=0,n=0;
	int ixb1=0,iyb1=0;
	int ix=0,iy=0;
	int ix1=0,iy1=0;
	int ix2=0,iy2=0;
	int dx=0,dy=0;
	float x=0.0f,y=0.0f;	
	int r=0,g=0,b=0,r2=0,g2=0,b2=0;
	int sumarea=0,sumarea2=0,rate=0;
	if(wid<=nw && hei<=nh){
		for(j=0;j<nh;j++){
			for(i=0;i<nw;i++){				
				x=(float)i*wid/nw;
				y=(float)j*hei/nh;
				ix=(int)x;
				iy=(int)y;
				dx=(int)((x-(float)ix)*256);
				dy=(int)((y-(float)iy)*256);
				ix1=ix+1;
				iy1=iy+1;
				if(ix>=wid || ix<0){
					ix=wid-1;
				}
				if(ix1>=wid || ix1<0){
					ix1=wid-1;
				}
				if(iy>=hei || iy<0){
					iy=hei-1;
				}
				if(iy1>=hei || iy1<0){
					iy1=hei-1;
				}
				mByte=(BYTE *)image0.GetPixelAddress(ix,iy);
				r=(256-dx)*(256-dy)*(int)(*(mByte+2));
				g=(256-dx)*(256-dy)*(int)(*(mByte+1));
				b=(256-dx)*(256-dy)*(int)(*mByte);
				mByte=(BYTE *)image0.GetPixelAddress(ix1,iy);
				r+=dx*(256-dy)*(int)(*(mByte+2));
				g+=dx*(256-dy)*(int)(*(mByte+1));
				b+=dx*(256-dy)*(int)(*mByte);
				mByte=(BYTE *)image0.GetPixelAddress(ix,iy1);
				r+=(256-dx)*dy*(int)(*(mByte+2));
				g+=(256-dx)*dy*(int)(*(mByte+1));
				b+=(256-dx)*dy*(int)(*mByte);
				mByte=(BYTE *)image0.GetPixelAddress(ix1,iy1);
				r+=dx*dy*(int)(*(mByte+2));
				g+=dx*dy*(int)(*(mByte+1));
				b+=dx*dy*(int)(*mByte);
				r=r>>16;
				g=g>>16;
				b=b>>16;
				if (r>255) {
					r=255;
				} else if (r<0) {
					r=0;
				}			
				if (g>255) {
					g=255;
				} else if (g<0) {
					g=0;
				}		
				if (b>255) {
					b=255;
				} else if (b<0) {
					b=0;
				}
				pByte=(BYTE *)imaget.GetPixelAddress(i,j);
				*pByte=(BYTE)b;
				pByte++;
				*pByte=(BYTE)g;
				pByte++;
				*pByte=(BYTE)r;
			}
		}
	}else/* if(wid>=nw && hei>=nh)*/{
		for(j=0;j<nh;j++){
			iy=j*hei/nh;
			iy1=(j+1)*hei/nh;
			if(iy>=iy1){iy1=iy+1;}
			for(i=0;i<nw;i++){
				ix=i*wid/nw;
				ix1=(i+1)*wid/nw;
				if(ix>=ix1){ix1=ix+1;}
				r=0;
				g=0;
				b=0;
				sumarea=0;
				for(l=iy;l<iy1;l++){
					for(k=ix;k<ix1;k++){
						if(k<wid && l<hei && k>=0 && l>=0){
							mByte=(BYTE *)image0.GetPixelAddress(k,l);
							r+=*(mByte+2);
							g+=*(mByte+1);
							b+=*mByte;
							sumarea++;
						}
					}
				}
				if(wid<nw){
					r2=0;
					g2=0;
					b2=0;
					sumarea2=0;
					for(l=iy;l<iy1;l++){
						for(k=ix1;k<=ix1;k++){
							if(k<wid && l<hei && k>=0 && l>=0){
								mByte=(BYTE *)image0.GetPixelAddress(k,l);
								r2+=*(mByte+2);
								g2+=*(mByte+1);
								b2+=*mByte;
								sumarea2++;
							}							
						}
					}
					rate=i*wid*256/nw-i*wid/nw*256;
				}else if(hei<nh){
					r2=0;
					g2=0;
					b2=0;
					sumarea2=0;
					for(l=iy1;l<=iy1;l++){
						for(k=ix;k<ix1;k++){
							if(k<wid && l<hei && k>=0 && l>=0){
								mByte=(BYTE *)image0.GetPixelAddress(k,l);
								r2+=*(mByte+2);
								g2+=*(mByte+1);
								b2+=*mByte;
								sumarea2++;
							}							
						}
					}
					rate=j*hei*256/nh-j*hei/nh*256;
				}else{
					rate=0;
				}
				if(sumarea!=0){
					if(rate<=0 || sumarea2==0){
						r/=sumarea;
						g/=sumarea;
						b/=sumarea;
					}else if(sumarea2!=0){
						r=((256-rate)*r/sumarea+rate*r2/sumarea2)/256;
						g=((256-rate)*g/sumarea+rate*g2/sumarea2)/256;
						b=((256-rate)*b/sumarea+rate*b2/sumarea2)/256;
					}
					if (r>255) {
						r=255;
					} else if (r<0) {
						r=0;
					}			
					if (g>255) {
						g=255;
					} else if (g<0) {
						g=0;
					}		
					if (b>255) {
						b=255;
					} else if (b<0) {
						b=0;
					}
					pByte=(BYTE *)imaget.GetPixelAddress(i,j);
					*pByte=(BYTE)b;
					pByte++;
					*pByte=(BYTE)g;
					pByte++;
					*pByte=(BYTE)r;
				}
			}
		}
	}
}

void CImageListRecorderView::OnAvitoolsmenuitem() 
{
	// TODO: Add your command handler code here
	CAviToolInput dlg;
	
	CSavingImageListDialog dlg2;
	CColorDialog dlg3(dlg2.bgc,CC_FULLOPEN);

	CFileDialog dlg4(FALSE,NULL,NULL,OFN_OVERWRITEPROMPT,"XVID格式压缩的AVI文件(*.avi)|*.avi||");
	int nStructSize = 0;
    DWORD dwVersion, dwWindowsMajorVersion, dwWindwosMinorVersion;
    dwVersion = GetVersion();
    dwWindowsMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
    dwWindwosMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));
    if(dwVersion < 0x80000000)
    {
    nStructSize = 88; //2k,xp下显示新的版本
    }
    else
    {
    nStructSize = 76; //98下显示老版本，如图1
    }
    dlg4.m_ofn.lStructSize = nStructSize; //------用来检测目前的操作系统的版本信息
	if(dlg.DoModal()==IDOK){
		if(dlg.aviOpenMode){
			if(CAviToolInput::changeFps(dlg.m_e6,dlg.fnamea)==TRUE){
				MessageBox("调速成功！");
			}
		}else if(dlg.imageFilePathNameList.GetSize()>0){
			dlg2.cimage.Load(dlg.imageFilePathNameList[0]);
			if(dlg2.cimage.GetBPP()==32){
				//dlg3.SetWindowText("加载的位图是32位位图，请指定背景颜色。");
				//MessageBox("加载的位图是32位位图，请指定背景颜色。");
				
				//if(dlg3.DoModal()==IDOK)dlg2.bgc=dlg3.GetColor();
			}
			if(dlg4.DoModal()==IDOK){
				dlg2.fnamea.Format(_T("%s"),dlg4.GetPathName());
				if(dlg2.fnamea.Right(4) !=_T(".avi") && dlg2.fnamea.Right(4) !=_T(".AVI")){
					dlg2.fnamea += _T(".avi");
				}
				dlg2.fileType=dlg4.m_ofn.nFilterIndex;
				dlg2.wid0=dlg.wid0;//视频宽度
				dlg2.hei0=dlg.hei0;//视频高度
				dlg2.nameList.Copy(dlg.imageFilePathNameList);//图片文件名称和路径数组
				dlg2.imgLeng=dlg.imageFilePathNameList.GetSize();//准备拼接的位图的数量，为零时不拼接。
				dlg2.savedCount=0;//已保存图片的数量
				dlg2.fps0=dlg.m_e6;//输出视频的帧频率
				dlg2.willScale=(dlg.m_c2==TRUE)?true:false;//是否进行缩放
				dlg2.DoModal();
			}
		}
	}
}

void CImageListRecorderView::readRecordLogFile(CRecordSettingDialog *dlg,unsigned long &fileFilter){
	CString sPath;//获取当前程序路径
	GetModuleFileName(NULL,sPath.GetBufferSetLength (MAX_PATH+1),MAX_PATH);
	sPath.ReleaseBuffer ();
	int nPos;
	nPos=sPath.ReverseFind ('\\');
	if(nPos>=0){
		sPath=sPath.Left (nPos+1);	
	}else{
		sPath="";
	}
	FILE *fp=fopen(sPath+"NewFileLog.cfg","r");
	if(fp==NULL){
		return;
	}
	int headi;
	headi=fgetc(fp);
	if(headi!='I'){
		fclose(fp);
		return;
	}
	headi=fgetc(fp);
	if(headi!='L'){
		fclose(fp);
		return;
	}
	headi=fgetc(fp);
	if(headi!='R'){
		fclose(fp);
		return;
	}
	headi=fgetc(fp);
	if(headi!=' '){
		fclose(fp);
		return;
	}
	fscanf(fp,"%d",&(dlg->m_e1));
	fscanf(fp,"%d",&(dlg->m_e2));
	fscanf(fp,"%d",&(dlg->m_e3));
	fscanf(fp,"%d",&(dlg->m_e4));
	fscanf(fp,"%d",&(dlg->m_e5));
	fscanf(fp,"%d",&(dlg->m_e6));
	fscanf(fp,"%d",&(dlg->m_e7));
	fscanf(fp,"%d",&(dlg->m_e8));
	fscanf(fp,"%d",&(dlg->m_c1));
	fscanf(fp,"%d",&fileFilter);
	fscanf(fp,"%d",&(dlg->m_e9));
	char deviceStr[1000];
	fgetc(fp);
	int i=0;
	char c;
	while(!feof(fp)){
		c=fgetc(fp);
		if(c=='|' || c==EOF || i>=1000){
			break;
		}
		deviceStr[i]=c;
		i++;
	}
	deviceStr[i]='\0';
	dlg->deviceString.Format("%s",deviceStr);
	fclose(fp);
	
}
void CImageListRecorderView::writeRecordLogFile(CRecordSettingDialog *dlg,unsigned long &fileFilter){
	CString sPath;//获取当前程序路径
	GetModuleFileName(NULL,sPath.GetBufferSetLength (MAX_PATH+1),MAX_PATH);
	sPath.ReleaseBuffer ();
	int nPos;
	nPos=sPath.ReverseFind ('\\');
	if(nPos>=0){
		sPath=sPath.Left (nPos+1);	
	}else{
		sPath="";
	}	
	FILE *fp;/*=fopen(sPath+"NewFileLog.txt","r");
	int headi;
	if(fp!=NULL){		
		headi=fgetc(fp);
		if(headi!='I'){
			fclose(fp);
			return;
		}
		headi=fgetc(fp);
		if(headi!='L'){
			fclose(fp);
			return;
		}
		headi=fgetc(fp);
		if(headi!='R'){
			fclose(fp);
			return;
		}
		headi=fgetc(fp);
		if(headi!=' '){
			fclose(fp);
			return;
		}
		fclose(fp);
	}*/
	fp=fopen(sPath+"NewFileLog.cfg","w");
	fprintf(fp,"ILR %d %d %d %d %d %d %d %d %d %d %d %s",dlg->m_e1,dlg->m_e2,dlg->m_e3,dlg->m_e4,dlg->m_e5,dlg->m_e6,dlg->m_e7,dlg->m_e8,dlg->m_c1,fileFilter,dlg->m_e9,(LPTSTR)(LPCTSTR)dlg->deviceString);	
	fclose(fp);
}

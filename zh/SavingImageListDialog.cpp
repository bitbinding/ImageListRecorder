// SavingImageListDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ImageListRecorder.h"
#include "ImageListRecorderView.h"
#include "SavingImageListDialog.h"
#include "AVIFile.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSavingImageListDialog dialog

extern PFUNxvid_global xvid_global;
extern PFUNxvid_decore xvid_decore;
extern PFUNxvid_encore xvid_encore;

CSavingImageListDialog::CSavingImageListDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSavingImageListDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSavingImageListDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	/*fileName0="";
	fileType=1;
	img=NULL;
	imgLeng=0;
	savedCount=0;*/	
	fnamea="";//avi文件名称和路径
	fileType=0;//avi文件格式
	nameList.SetSize(0);//图片文件名称和路径数组指针
	wid0=550;//视频宽度
	hei0=400;//视频高度
	widt=wid0;//修正后的视频宽度
	heit=hei0;//修正后的视频高度
	imgLeng=0;//准备拼接的位图的数量，为零时不拼接。
	savedCount=0;//已保存图片的数量
	fps0=0;//输出视频的帧频率
	willScale=false;//是否进行缩放
	bgc=RGB(0,0,0);//背景颜色
	working=false;
	//xvid
	avi0Handle=NULL;
	memset(&avi0,0,sizeof(xvid_enc_create_t));
	memset(&aviFrame,0,sizeof(xvid_enc_frame_t));
	memset(&aviStates,0,sizeof(xvid_enc_stats_t));
	bitStream=NULL;
	idx1=NULL;
}


void CSavingImageListDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSavingImageListDialog)
	DDX_Control(pDX, IDC_PROGRESS1, m_p1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSavingImageListDialog, CDialog)
	//{{AFX_MSG_MAP(CSavingImageListDialog)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSavingImageListDialog message handlers
void copyPixels(CImage &image0,CImage &imaget,COLORREF backgroundColor){
	if(image0.IsNull() || imaget.IsNull()){
		return;
	}
	if(image0.GetBPP()<24 || imaget.GetBPP()<24){
		return;
	}
	int i=0,j=0;
	BYTE* mByte=(BYTE *)image0.GetPixelAddress(0,0);//原图指针
	BYTE* pByte=(BYTE *)imaget.GetPixelAddress(0,0);//预览图指针
	bool copyAlpha=(image0.GetBPP()>=32 && imaget.GetBPP()>=32);
	int wid=imaget.GetWidth();
	int hei=imaget.GetHeight();
	int widmin=image0.GetWidth()<=imaget.GetWidth()?image0.GetWidth():imaget.GetWidth();
	int heimin=image0.GetHeight()<=imaget.GetHeight()?image0.GetHeight():imaget.GetHeight();
	for(j=0;j<hei;j++){
		for(i=0;i<wid;i++){
			pByte=(BYTE *)imaget.GetPixelAddress(i,j);
			if(i<widmin && j<heimin){
				mByte=(BYTE *)image0.GetPixelAddress(i,j);
				*pByte=*mByte;
				pByte[1]=mByte[1];
				pByte[2]=mByte[2];
				if(copyAlpha){
					pByte[3]=mByte[3];
				}
			}else{
				*pByte=GetBValue(backgroundColor);
				pByte[1]=GetGValue(backgroundColor);
				pByte[2]=GetRValue(backgroundColor);
			}
		}
	}
}
void bppTransform(CImage &image0,CImage &imaget,COLORREF backgroundColor){
	if(image0.IsNull() || imaget.IsNull()){
		return;
	}
	if(imaget.GetBPP()<24){
		return;
	}
	if(image0.GetWidth()!=imaget.GetWidth() || image0.GetHeight()!=imaget.GetHeight()){
		return;
	}
	
	int i=0,j=0;
	BYTE* mByte=(BYTE *)image0.GetPixelAddress(0,0);//原图指针
	BYTE* pByte=(BYTE *)imaget.GetPixelAddress(0,0);//预览图指针
	COLORREF colour;
	int r,g,b;
	int wid=image0.GetWidth();
	int hei=image0.GetHeight();
	if(image0.GetBPP()<24){
		for(j=0;j<hei;j++){
			for(i=0;i<wid;i++){
				colour=image0.GetPixel(i,j);
				pByte=(BYTE *)imaget.GetPixelAddress(i,j);
				*pByte=GetBValue(colour);
				pByte[1]=GetGValue(colour);
				pByte[2]=GetRValue(colour);
			}
		}
	}else if(image0.GetBPP()>=32 && imaget.GetBPP()==24){
		for(j=0;j<hei;j++){
			for(i=0;i<wid;i++){
				mByte=(BYTE *)image0.GetPixelAddress(i,j);
				b=mByte[0]+((int)(GetBValue(backgroundColor))-mByte[0])*(255-mByte[3])/255;
				g=mByte[1]+((int)(GetGValue(backgroundColor))-mByte[1])*(255-mByte[3])/255;
				r=mByte[2]+((int)(GetRValue(backgroundColor))-mByte[2])*(255-mByte[3])/255;
				if(b<0){
					b=0;
				}
				if(b>255){
					b=255;
				}
				if(g<0){
					g=0;
				}
				if(g>255){
					g=255;
				}
				if(r<0){
					r=0;
				}
				if(r>255){
					r=255;
				}
				pByte=(BYTE *)imaget.GetPixelAddress(i,j);
				pByte[0]=b;
				pByte[1]=g;
				pByte[2]=r;
			}
		}
	}else{
		copyPixels(image0,imaget,backgroundColor);
	}
}
void scaleFilter(CImage &image0, CImage &imaget,int nw,int nh,COLORREF backgroundColor){
	//双线性缩放滤镜
	if(image0.IsNull() || imaget.IsNull()){
		return;
	}
	if(image0.GetBPP()<24 || imaget.GetBPP()<24){
		return;
	}
	int wid=image0.GetWidth();
	int hei=image0.GetHeight();
	int nw0=imaget.GetWidth();
	int nh0=imaget.GetHeight();
	int i=0,j=0,k=0,l=0,m=0,n=0;
	BYTE* mByte=(BYTE *)image0.GetPixelAddress(0,0);//原图指针
	BYTE* pByte=(BYTE *)imaget.GetPixelAddress(0,0);//预览图指针

	if(nw0!=nw || nh0!=nh){
		for(j=0;j<nh0;j++){
			for(i=0;i<nw0;i++){
				pByte=(BYTE *)imaget.GetPixelAddress(i,j);
				*pByte=(BYTE)GetBValue(backgroundColor);
				pByte++;
				*pByte=(BYTE)GetGValue(backgroundColor);
				pByte++;
				*pByte=(BYTE)GetRValue(backgroundColor);
			}
		}
	}
	int nwmin=nw0<=nw?nw0:nw;
	int nhmin=nh0<=nh?nh0:nh;
	
	int ixb1=0,iyb1=0;
	int ix=0,iy=0;
	int ix1=0,iy1=0;
	int ix2=0,iy2=0;
	int dx=0,dy=0;
	float x=0.0f,y=0.0f;	
	int r=0,g=0,b=0,r2=0,g2=0,b2=0;
	int sumarea=0,sumarea2=0,rate=0;
	if(wid<=nw && hei<=nh){
		for(j=0;j<nhmin;j++){
			for(i=0;i<nwmin;i++){				
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
		for(j=0;j<nhmin;j++){
			iy=j*hei/nh;
			iy1=(j+1)*hei/nh;
			if(iy>=iy1){iy1=iy+1;}
			for(i=0;i<nwmin;i++){
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

bool CSavingImageListDialog::enumExtNamesToReadBitmap(CImage &img,LPCTSTR filePathName){
	char str[100]="jpg|png|gif|bmp|tif";
	int i=0;
	int j=strlen(str);
	for(i=0;i<j;i++){
		if(str[i]=='|'){
			str[i]=0;
		}
	}
	i=0;
	CString strt=filePathName;
	CString str0="";
	if(!(img.IsNull())){
		img.Destroy();
	}
	int d;
	while(i<j){
		img.Load(strt);
		if(!(img.IsNull())){
			break;
		}
		if(i==0){
			d=strt.ReverseFind('.');
			if(d<0){
				str0=strt+".";
			}else{
				str0=strt.Left(d+1);
			}
		}
		strt=str0+(str+i);
		AfxMessageBox(strt);
		i+=4;
	}
	return !(img.IsNull());
}

static UINT xvidJointingFunction(LPVOID pParam){
	CSavingImageListDialog *pDlg=(CSavingImageListDialog *)pParam;
	if(xvid_encore(NULL,XVID_ENC_CREATE,&(pDlg->avi0),NULL)<0){
		AfxMessageBox("xvid库初始化编码失败，可能是xvidcord.dll的版本不对。",MB_OK,MB_ICONSTOP);
		pDlg->working=false;
		return 0;
	}
	pDlg->avi0Handle=pDlg->avi0.handle;
	if(pDlg->avi0Handle==NULL){
		AfxMessageBox("创建句柄失败。",MB_OK,MB_ICONSTOP);
		pDlg->working=false;
		return 0;
	}
	FILE *aviFile=fopen((LPTSTR)(LPCTSTR)(pDlg->fnamea),"wb");
	if(aviFile==NULL){
		AfxMessageBox("创建文件失败！",MB_OK,MB_ICONSTOP);
		pDlg->working=false;
		return 0;
	}
	if(CImageListRecorderView::CreateAviHeader(aviFile,pDlg->widt,pDlg->heit,pDlg->fps0,(pDlg->widt)*(pDlg->heit)*3/2,false)==FALSE){
		if(aviFile!=NULL){
			fclose(aviFile);
			aviFile=NULL;
		}
		pDlg->working=false;
		return 0;
	}
	if(fclose(aviFile)!=0){
		AfxMessageBox("初始化文件失败！",MB_OK,MB_ICONSTOP);
		aviFile=NULL;
		pDlg->working=false;
		return 0;
	}

	aviFile=fopen((LPTSTR)(LPCTSTR)(pDlg->fnamea),"ab");
	if(aviFile==NULL){
		AfxMessageBox("写入文件出错",MB_OK,MB_ICONSTOP);
		pDlg->working=false;
		return 0;
	}
	int keyFrameStep=100;
	int frameLength;
	int readCount=0;
	int nw;
	int nh;
	bool bppChanged=false;
	bool sizeChanged=false;
	while( readCount < (pDlg->imgLeng) ){
		if( readCount >= (pDlg->nameList.GetSize()) ){
			break;
		}
		/*if(!(pDlg->cimage.IsNull())){
			pDlg->cimage.Destroy();
		}
		pDlg->cimage.Load(pDlg->nameList[readCount]);*/
		CSavingImageListDialog::enumExtNamesToReadBitmap(pDlg->cimage,pDlg->nameList[readCount]);
		readCount++;
		if(pDlg->cimage.IsNull()){
			continue;
		}
		bppChanged=false;
		sizeChanged=false;
		if(pDlg->cimage.GetBPP()!=24){
			if(pDlg->dimage.IsNull()){
				pDlg->dimage.Create(pDlg->cimage.GetWidth(),pDlg->cimage.GetHeight(),24);
			}else if(pDlg->dimage.GetWidth()!=pDlg->cimage.GetWidth() || pDlg->dimage.GetHeight()!=pDlg->cimage.GetHeight()){
				pDlg->dimage.Destroy();
				pDlg->dimage.Create(pDlg->cimage.GetWidth(),pDlg->cimage.GetHeight(),24);				
			}
			bppTransform(pDlg->cimage,pDlg->dimage,pDlg->bgc);
			bppChanged=true;
		}
		if(pDlg->cimage.GetWidth()!=pDlg->wid0 && pDlg->cimage.GetHeight()!=pDlg->hei0){
			if((pDlg->cimage.GetHeight())*(pDlg->widt)<(pDlg->cimage.GetWidth())*(pDlg->heit)){
				nw=pDlg->widt;
				nh=nw*(pDlg->cimage.GetHeight())/(pDlg->cimage.GetWidth());
			}else{
				nh=pDlg->heit;
				nw=nh*(pDlg->cimage.GetWidth())/(pDlg->cimage.GetHeight());
			}
			if(bppChanged){
				scaleFilter(pDlg->dimage,pDlg->zimage,nw,nh,pDlg->bgc);
			}else{
				scaleFilter(pDlg->cimage,pDlg->zimage,nw,nh,pDlg->bgc);
			}
			sizeChanged=true;
		}else{// if(pDlg->wid0!=pDlg->widt || pDlg->hei0!=pDlg->heit){
			if(bppChanged){
				copyPixels(pDlg->dimage,pDlg->zimage,pDlg->bgc);
			}else{
				copyPixels(pDlg->cimage,pDlg->zimage,pDlg->bgc);
			}
			sizeChanged=true;
		}
		if(pDlg->savedCount%keyFrameStep==0 && pDlg->savedCount>0){
			if(pDlg->avi0Handle!=NULL){
				xvid_encore (pDlg->avi0Handle,XVID_ENC_DESTROY,NULL,NULL);//撤消编码实例
				pDlg->avi0Handle=NULL;
				pDlg->avi0.handle=NULL;
			}

			if(xvid_encore(NULL,XVID_ENC_CREATE,&(pDlg->avi0),NULL)<0){
				pDlg->avi0Handle=NULL;
				pDlg->avi0.handle=NULL;
				CImageListRecorderView::FinishAviFile((LPTSTR)(LPCTSTR)(pDlg->fnamea),pDlg->savedCount,pDlg->idx1,0);
				pDlg->working=false;
				return 0;
			}
			pDlg->avi0Handle=pDlg->avi0.handle;
		}
		if(sizeChanged){
			frameLength=CImageListRecorderView::RecordXvidFrame(aviFile,&(pDlg->zimage),&(pDlg->avi0),&(pDlg->aviFrame),&(pDlg->aviStates));
		}else if(bppChanged){
			frameLength=CImageListRecorderView::RecordXvidFrame(aviFile,&(pDlg->dimage),&(pDlg->avi0),&(pDlg->aviFrame),&(pDlg->aviStates));
		}else{
			frameLength=CImageListRecorderView::RecordXvidFrame(aviFile,&(pDlg->dimage),&(pDlg->avi0),&(pDlg->aviFrame),&(pDlg->aviStates));
		}
		if((pDlg->savedCount)>=0 && (pDlg->savedCount)<(pDlg->imgLeng)){
			pDlg->idx1[pDlg->savedCount]=frameLength;
		}
		pDlg->savedCount++;
	}
	
	fclose(aviFile);
	aviFile=NULL;
	CImageListRecorderView::FinishAviFile((LPTSTR)(LPCTSTR)(pDlg->fnamea),pDlg->savedCount,pDlg->idx1,0);
	if(!(pDlg->cimage.IsNull())){
		pDlg->cimage.Destroy();
	}
	if(!(pDlg->dimage.IsNull())){
		pDlg->dimage.Destroy();
	}
	if(pDlg->avi0Handle!=NULL){
		xvid_encore (pDlg->avi0Handle,XVID_ENC_DESTROY,NULL,NULL);//撤消编码实例
		pDlg->avi0Handle=NULL;
		pDlg->avi0.handle=NULL;
	}
	pDlg->working=false;
	return 1;
}

BOOL CSavingImageListDialog::OnInitDialog() 
{
	// TODO: Add extra initialization here
	if(imgLeng!=0){
		/*CWnd *pWnd=GetDlgItem(IDC_PROGRESS1);
		if(pWnd!=NULL){
			((CProgressCtrl *)pWnd)->SetRange32(0,100);
		}*/
		widt=wid0;
		heit=hei0;
		if(fileType==1){
			if((widt&1)==1){
				widt++;
			}
			if((heit&1)==1){
				heit++;
			}
			if(widt==640 && heit==480){
				widt+=2;
			}
		}
		zimage.Create(widt,heit,24);
		if(fileType==1){
			if(xvid_global==NULL || xvid_decore==NULL || xvid_encore==NULL){
				MessageBox("找不到兼容的xvidcore.dll文件，因而不能进行录制，请在程序目录上加入。");
				return TRUE;
			}
			
			memset(&avi0,0,sizeof(xvid_enc_create_t));
			avi0.version=CImageListRecorderView::xvidVersion;
			avi0.profile=0xf5;//XVID_PROFILE_AS_L4;
			
			avi0.width=widt;
			avi0.height=heit;
			avi0.fincr=1;
			avi0.fbase=fps0;
		
			
			memset(&aviFrame,0,sizeof(xvid_enc_frame_t));
			aviFrame.version=CImageListRecorderView::xvidVersion;
			aviFrame.input.csp=XVID_CSP_BGR;
			int pitch=zimage.GetPitch();
			
			
			if(pitch<0){
				pitch=-pitch;
				aviFrame.input.stride[0]=pitch;
				aviFrame.input.plane[0]=(BYTE *)(zimage.GetBits())-(heit-1)*pitch;
			}else{
				aviFrame.input.stride[0]=pitch;
				aviFrame.input.plane[0]=(BYTE *)(zimage.GetBits());
			}
			bitStream=new unsigned char[(widt*heit<<2)+1000];
			aviFrame.bitstream=bitStream;
			aviFrame.length=(widt*heit<<2)+1000;
			aviFrame.type=XVID_TYPE_AUTO;
			aviFrame.quant=0;
			memset(&aviStates,0,sizeof(xvid_enc_stats_t));
			aviStates.version=CImageListRecorderView::xvidVersion;
			idx1=new int[imgLeng];

			working=true;
			AfxBeginThread(xvidJointingFunction,this);
			//xvidJointingFunction(this);
		}
	}
	if(working){
		SetTimer(3,100,NULL);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CSavingImageListDialog::OnTimer(UINT nIDEvent) 
{
	if(imgLeng>0){
		CWnd *pWnd=GetDlgItem(IDC_PROGRESS1);
		if(pWnd!=NULL){
			((CProgressCtrl *)pWnd)->SetPos(savedCount*100/imgLeng);
		}
	}
	if(!working){
		KillTimer(3);
		CDialog::OnTimer(nIDEvent);
		CDialog::OnCancel();
		return;
	}
	CDialog::OnTimer(nIDEvent);
}

void CSavingImageListDialog::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(bitStream!=NULL){
		delete []bitStream;
		bitStream=NULL;
	}
	if(idx1!=NULL){
		delete []idx1;
		idx1=NULL;
	}
	CDialog::PostNcDestroy();
}

void CSavingImageListDialog::OnCancel() 
{
	// TODO: Add extra cleanup here
	if(working){
		return;
	}
	CDialog::OnCancel();
}

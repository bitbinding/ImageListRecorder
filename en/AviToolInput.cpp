// AviToolInput.cpp : implementation file
//

#include "stdafx.h"
#include "ImageListRecorder.h"
#include "AviToolInput.h"
#include "AVIFile.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//double aviMuxProcess;
/////////////////////////////////////////////////////////////////////////////
// CAviToolInput dialog

double CAviToolInput::aviMuxProgress=0.0;
CAviToolInput::CAviToolInput(CWnd* pParent /*=NULL*/)
	: CDialog(CAviToolInput::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAviToolInput)
	m_c1 = TRUE;
	m_c2 = FALSE;
	m_e7 = 720;
	m_e8 = 576;
	m_e6 = 25;
	//}}AFX_DATA_INIT
	fnamea="";//序列文件名称和路径
	aviOpenMode=false;//是否打开的是avi文件	
	wid0=1;//图片宽度
	hei0=1;//图片高度
	aviFixFileName="";
	isFixing=false;
}


void CAviToolInput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAviToolInput)
	DDX_Control(pDX, IDC_STATIC2, m_s2);
	DDX_Control(pDX, IDC_STATIC1, m_s1);
	DDX_Check(pDX, IDC_CHECK1, m_c1);
	DDX_Check(pDX, IDC_CHECK2, m_c2);
	DDX_Text(pDX, IDC_EDIT7, m_e7);
	DDV_MinMaxInt(pDX, m_e7, 1, 32767);
	DDX_Text(pDX, IDC_EDIT8, m_e8);
	DDV_MinMaxInt(pDX, m_e8, 1, 32767);
	DDX_Text(pDX, IDC_EDIT6, m_e6);
	DDV_MinMaxUInt(pDX, m_e6, 1, 2147483647);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAviToolInput, CDialog)
	//{{AFX_MSG_MAP(CAviToolInput)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAviToolInput message handlers

int strnumcmp(const char *s1,const char *s2){
	//比较可能有编号的字符串s1和s2，不优先区分大小写。
	int i=0;
	int nummode1=0;
	int nummode2=0;
	int numcmp=-2;
	unsigned char c01;
	unsigned char c02;
	unsigned char c1;
	unsigned char c2;
	int casecmp=0;
	for(i=0;s1[i]!=0 && s2[i]!=0;i++){
		c01=s1[i];
		c02=s2[i];
		if(c01>='0' && c01<='9'){
			nummode1=true;
		}else{
			nummode1=false;
		}
		if(c02>='0' && c02<='9'){
			nummode2=true;
		}else{
			nummode2=false;
		}
		if(nummode1 && nummode2 && numcmp==-2){
			numcmp=0;
		}
		if(numcmp==0){
			if(c01>c02){
				numcmp=1;
			}else if(c01<c02){
				numcmp=-1;
			}
		}
		if(numcmp>-2){
			if(nummode1 && !nummode2){
				return 1;
			}else if(!nummode1 && nummode2){
				return -1;
			}else if(!nummode1 && !nummode2){
				if(numcmp!=0){
					return numcmp;
				}
			}else{
				continue;
			}
		}
		if(!nummode1 || !nummode2){
			numcmp=-2;
		}
		if(c01>='A' && c01<='Z'){
			c1=c01+32;
		}else{
			c1=c01;
		}
		if(c02>='A' && c02<='Z'){
			c2=c02+32;
		}else{
			c2=c02;
		}
		if(c1>c2){
			return 1;
		}else if(c1<c2){
			return -1;
		}else if(casecmp==0){
			if(c01>c02){
				casecmp=1;
			}else if(c01<c02){
				casecmp=-1;
			}
		}
	}
	if(nummode1 && nummode2){
		c01=s1[i];
		c02=s2[i];
		if(c01!=0){
			return 1;
		}else if(c02!=0){
			return -1;
		}else if(numcmp==1){
			return 1;
		}else if(numcmp==-1){
			return -1;
		}else{
			return casecmp;
		}
	}else{
		c01=s1[i];
		c02=s2[i];
		if(c01!=0){
			return 1;
		}else if(c02!=0){
			return -1;
		}else{
			return casecmp;
		}
	}
	return 0;
}

int strnumcmp(CString &s1,CString &s2){
	//比较可能有编号的字符串s1和s2，不优先区分大小写。
	return strnumcmp((LPTSTR)(LPCTSTR)s1,(LPTSTR)(LPCTSTR)s2);
}

void strNumSort(const CStringArray &strArrSrc,CStringArray &strArrOut){
	//将CStringArray用strnumcmp的比较方法排序
	int length=strArrSrc.GetSize();
	if(length<=0){
		return;
	}
	
	int *arr=new int[length];
	int *arr2=new int[length];

	int i=0;
	

	for(i=0;i<length;i++){
		arr[i]=i;
		arr2[i]=i;
	}
	i=0;
	
	int j=0;
	int k=0;
	int l=0;
	int m=0;
	int n=0;
	int a=1;
	int b=a<<1;

	while(a<length){
		i=0;
		j=a;
		k=a;
		l=b;
		m=0;
		if(k>length){
			k=length;
		}
		if(l>length){
			l=length;
		}
		while(m<length){
			if(n==0){
				if(i>=k || j<l && strnumcmp(strArrSrc[arr[i]],strArrSrc[arr[j]])>0){
					arr2[m]=arr[j];
					j++;
				}else if(i<k){
					arr2[m]=arr[i];
					i++;
				}
			}else{
				if(i>=k || j<l && strnumcmp(strArrSrc[arr2[i]],strArrSrc[arr2[j]])>0){
					arr[m]=arr2[j];
					j++;
				}else if(i<k){
					arr[m]=arr2[i];
					i++;
				}
			}
			m++;
			if(m>=l){
				i=l;
				j=l+a;
				k+=b;
				l+=b;
				if(k>length){
					k=length;
				}
				if(l>length){
					l=length;
				}
			}
		}
		n=1-n;
		a=b;
		b=b<<1;
	}
	
	if(strArrOut.GetSize()<length){
		strArrOut.SetSize(length);
	}
	for(i=0;i<length;i++){
		if(n==0){
			strArrOut[i]=strArrSrc[arr[i]];
		}else{
			strArrOut[i]=strArrSrc[arr2[i]];
		}
	}
	delete []arr;
	delete []arr2;
}

void strNumSort(CStringArray &strArr){
	CStringArray tempArr;
	int l0=strArr.GetSize();
	tempArr.SetSize(l0);
	strNumSort(strArr,tempArr);
	int i=0;
	for(i=0;i<l0;i++){
		strArr[i]=tempArr[i];
	}
}

void CAviToolInput::OnButton1() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE,NULL,NULL,OFN_ALLOWMULTISELECT,"Image List Files(*.bmp;*.jpg;*.jpeg;*.png;*.gif)|*.bmp;*.jpg;*.jpeg;*.png;*.gif||");
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
    dlg.m_ofn.lStructSize = nStructSize; //------用来检测目前的操作系统的版本信息
	dlg.m_ofn.lpstrTitle="选择多个图片序列文件";
	const int maxSize=1000000;
	dlg.m_ofn.nMaxFile=maxSize;
	char *nameBuffer=new char[maxSize];
	dlg.m_ofn.lpstrFile=nameBuffer;
	dlg.m_ofn.lpstrFile[0]=NULL;
	int result=dlg.DoModal();
	if(result==IDOK){
		CStringArray nameArraySrc;
		nameArraySrc.SetSize(0);
		POSITION pos=dlg.GetStartPosition();   
		while(pos!=NULL)   
		{   
			nameArraySrc.Add(dlg.GetNextPathName(pos));
		}
		int count=nameArraySrc.GetSize();
		imageFilePathNameList.SetSize(count);
		strNumSort(nameArraySrc,imageFilePathNameList);
		/*CString str;
		for(int i=0;i<count;i++){
			str+=imageFilePathNameList[i]+"\r\n";
		}
		MessageBox(str);*/
		
		if(count>0){
			CImage image1;
			image1.Load(imageFilePathNameList[0]);
			if(image1.IsNull()){
				//MessageBox("文件不存在！");
				imageFilePathNameList.SetSize(0);
				delete []nameBuffer;
				return;
			}
			wid0=image1.GetWidth();
			hei0=image1.GetHeight();
			m_e7=wid0;
			m_e8=hei0;

			CString cstr;
			cstr.Format("Basic:%s",(LPTSTR)(LPCTSTR)imageFilePathNameList[0]);
			m_s1.SetWindowText(cstr);
			cstr.Format("Scale:%s",(LPTSTR)(LPCTSTR)imageFilePathNameList[count-1]);
			m_s2.SetWindowText(cstr);
		}

		CButton *pWnd0 = (CButton*)(GetDlgItem(IDC_CHECK2));
		CEdit *pWnd1 = (CEdit*)(GetDlgItem(IDC_EDIT7));
		CEdit *pWnd2 = (CEdit*)(GetDlgItem(IDC_EDIT8));
		pWnd0->EnableWindow(TRUE);
		pWnd1->EnableWindow(TRUE);
		pWnd2->EnableWindow(TRUE);
		UpdateData(FALSE);
		
	}//else if(CommDlgExtendedError()==FNERR_BUFFERTOOSMALL){
		//AfxMessageBox("读取文件名失败，文件数量过多。");
	//}
	delete []nameBuffer;
}

void CAviToolInput::OnButton2() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE,NULL,NULL,NULL,"avi files(*.avi)|*.avi||");
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
    dlg.m_ofn.lStructSize = nStructSize; //------用来检测目前的操作系统的版本信息
	CString cstr="";
	if(dlg.DoModal()==IDOK){		
		aviOpenMode=true;		
		CString fnamea0=dlg.GetPathName();
		cstr.Format("Basic:%s",(LPTSTR)(LPCTSTR)fnamea0);
		fnamea.Format(_T("%s"),fnamea0);
		m_s1.SetWindowText(cstr);
		m_s2.SetWindowText("Scale");

		m_e6=getFps(fnamea);

		CButton *pWnd0 = (CButton*)(GetDlgItem(IDC_CHECK2));
		CEdit *pWnd1 = (CEdit*)(GetDlgItem(IDC_EDIT7));
		CEdit *pWnd2 = (CEdit*)(GetDlgItem(IDC_EDIT8));
		pWnd0->EnableWindow(FALSE);
		pWnd1->EnableWindow(FALSE);
		pWnd2->EnableWindow(FALSE);
		
		UpdateData(FALSE);	
	}
	
}

void CAviToolInput::OnCheck2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CEdit *pWnd1 = (CEdit*)(GetDlgItem(IDC_EDIT7));
	CEdit *pWnd2 = (CEdit*)(GetDlgItem(IDC_EDIT8));
	if(m_c2==TRUE){
		pWnd1->SetReadOnly(FALSE);
		pWnd2->SetReadOnly(FALSE);
	}else{
		pWnd1->SetReadOnly(TRUE);
		pWnd2->SetReadOnly(TRUE);
		m_e7=wid0;
		m_e8=hei0;
	}
	UpdateData(FALSE);
}

void CAviToolInput::OnButton3() 
{
	// TODO: Add your control notification handler code here
	CString title="";
	GetDlgItemText(IDC_BUTTON3,title);
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
	CFileDialog dlg(TRUE,NULL,NULL,NULL,"avi files(*.avi)|*.avi||");
	dlg.m_ofn.lStructSize = nStructSize; //------用来检测目前的操作系统的版本信息
	if(dlg.DoModal()!=IDOK){
		return;
	}
	if(MessageBox("This ?","Warning",MB_YESNO)!=IDYES){
		return;
	}
	aviFixFileName=dlg.GetPathName();
	AfxBeginThread(aviFix,this);
}

BOOL CAviToolInput::changeFps(UINT nfps,LPCSTR lpszFileName){
	BOOL nresult=FALSE;
	FILE *fp;
	fp=fopen(lpszFileName,"rb+");
	long minSize=0x88;
	char ch;
	if(fp==NULL){
		//AfxMessageBox("要更改帧频率的文件不存在！");
		return nresult;
	}
	ch=getc(fp);		
	long i=0;
	while (!feof (fp) && i<minSize){			
		ch=getc(fp);
		i=ftell(fp);
		if(i==minSize){
			fseek(fp,-4L,SEEK_CUR);
			fwrite(&nfps,sizeof(UINT),1,fp);
			i=ftell(fp);
			nresult=TRUE;
			break;
		}
	}
	if(i<minSize){
		//AfxMessageBox("文件过小，无法改变帧频率！");
	}
	fclose(fp);	
	return nresult;
}
UINT CAviToolInput::getFps(LPCSTR lpszFileName){
	UINT nfps=10;		
	FILE *fp;
	fp=fopen(lpszFileName,"rb+");
	long minSize=0x88;
	char ch;
	if(fp==NULL){
		//AfxMessageBox("要获取帧频率的文件不存在！");
		return nfps;
	}
	ch=getc(fp);		
	long i=0;
	while (!feof (fp) && i<minSize){			
		ch=getc(fp);
		i=ftell(fp);
		if(i==minSize){
			fseek(fp,-4L,SEEK_CUR);
			fread(&nfps,sizeof(UINT),1,fp);
			i=ftell(fp);
			break;
		}
	}
	if(i<minSize){
		//AfxMessageBox("文件过小，无法获取帧频率！");
	}
	fclose(fp);		
	
	return nfps;
}


UINT CAviToolInput::aviFix(LPVOID pParam){
	if(pParam==NULL){
		return 0;
	}
	int keyframeInterval=0;
	CAviToolInput *pWnd=(CAviToolInput *)pParam;
	pWnd->isFixing=true;
	CString windowText0;
	pWnd->GetWindowText(windowText0);
	char *fileName=(LPTSTR)(LPCTSTR)(pWnd->aviFixFileName);
	if(fileName==NULL){
		pWnd->isFixing=false;
		return 0;
	}
	int idx1Leng=0;
	int frameCount=0;
	int i=0,j=0,k=0;
	FILE *fp=fopen(fileName,"rb+");
	if(fp==NULL){
		//pWnd->MessageBox("修复失败，文件不存在。");
		pWnd->SetWindowText(windowText0);
		pWnd->isFixing=false;
		return 0;
	}
	pWnd->SetWindowText("Search for the avi header...");
	fseek(fp,0,SEEK_END);
	int leng0=ftell(fp);
	if((leng0&1)==1 && feof(fp)==0){
		leng0++;
	}
	
	CString cstr="";
	CString cstr2="";
	cstr.Format("%d;%x;%x;",feof(fp),ftell(fp),leng0);
	if(leng0<0x1020 || leng0>=0x7fffffff-8-16*idx1Leng){
		fclose(fp);
		fp=NULL;
		//pWnd->MessageBox("文件太小或太大，暂不支持修复。");
		pWnd->SetWindowText(windowText0);
		pWnd->isFixing=false;
		return 0;
	}
	
	if(keyframeInterval<=0){
		fseek(fp,0x70,SEEK_SET);
		char codeType[4];
		fread(codeType,1,4,fp);
		
		fseek(fp,0x134,SEEK_SET);
		char coderStr[40];
		fread(coderStr,1,40,fp);
		char coderCharTemp=0;
		int coderCharTempPos=0;

		if(keyframeInterval<=0){
			coderCharTempPos=17;
			coderCharTemp=coderStr[coderCharTempPos];
			coderStr[coderCharTempPos]=0;
			if(strcmp(coderStr,"ImageListRecorder")==0){
				keyframeInterval=100;
			}else{
				coderStr[coderCharTempPos]=coderCharTemp;
			}
		}
		
		if(keyframeInterval<=0){
			coderCharTempPos=12;
			coderCharTemp=coderStr[coderCharTempPos];
			coderStr[coderCharTempPos]=0;
			if(codeType[0]=='X' && codeType[1]=='V' && codeType[2]=='I' && codeType[3]=='D' && strcmp(coderStr,"MEncoder SVN")==0){
				keyframeInterval=4000;
			}else{
				coderStr[coderCharTempPos]=coderCharTemp;
			}
		}

		if(keyframeInterval<=0){
			fseek(fp,0x1a6,SEEK_SET);
			fread(coderStr,1,40,fp);
		}

		if(keyframeInterval<=0){
			coderCharTempPos=17;
			coderCharTemp=coderStr[coderCharTempPos];
			coderStr[coderCharTempPos]=0;
			if(strcmp(coderStr,"ImageListRecorder")==0){
				keyframeInterval=100;
			}else{
				coderStr[coderCharTempPos]=coderCharTemp;
			}
		}
		
		if(keyframeInterval<=0){
			coderCharTempPos=12;
			coderCharTemp=coderStr[coderCharTempPos];
			coderStr[coderCharTempPos]=0;
			if(codeType[0]=='X' && codeType[1]=='V' && codeType[2]=='I' && codeType[3]=='D' && strcmp(coderStr,"MEncoder SVN")==0){
				keyframeInterval=4000;
			}else{
				coderStr[coderCharTempPos]=coderCharTemp;
			}
		}
	}

	


	fseek(fp,0x1008,SEEK_SET);
	char label[5];
	fread((char *)label,1,4,fp);
	label[4]='\0';
	int movipos=0;
	char c;
	if(label[0]=='m' && label[1]=='o' && label[2]=='v' && label[3]=='i'){
		movipos=ftell(fp);
	}else{
		fseek(fp,0,SEEK_SET);
		j=0;
		
		for(i=0;i<leng0;i++){
			c=fgetc(fp);
			if(j==0 && c=='L'){
				j++;
			}else if(j==1 && c=='I'){
				j++;
			}else if(j==2 && c=='S'){
				j++;
			}else if(j==3 && c=='T'){
				j++;
			}else if(j>=4 && j<=7){
				j++;
			}else if(j==8 && c=='m'){
				j++;
			}else if(j==9 && c=='o'){
				j++;
			}else if(j==10 && c=='v'){
				j++;
			}else if(j==11 && c=='i'){
				j++;
				movipos=ftell(fp);
				break;
			}else{
				j=0;
			}
		}
	}
	if(movipos<=0){
		//pWnd->MessageBox("修复失败，找不到视频流信息开始之处。");
		pWnd->SetWindowText(windowText0);
		pWnd->isFixing=false;
		return 0;
	}
	int idx1t=0;
	int leng=0;
	i=0;
	if(ftell(fp)+8<=leng0){
		while(true){
			leng=ftell(fp);
			
			fread((char *)label,1,4,fp);
			if(label[0]=='i' && label[1]=='d' && label[2]=='x' && label[3]=='1'){
				leng=ftell(fp)-4;
				break;
			}
			
			idx1Leng++;
			if(label[0]=='0' && label[1]=='0' && label[2]=='d' && label[3]=='c'){
				frameCount++;
			}
			if(frameCount%100==0){
				cstr.Format("%d frames found...",frameCount);
				pWnd->SetWindowText(cstr);
			}
			
			fread(&idx1t,1,4,fp);

			
			idx1t=(idx1t&1)==0?idx1t:idx1t+1;
			if(leng+8+idx1t+8<leng0){
				fseek(fp,idx1t,SEEK_CUR);
			}else if(leng+8+idx1t==leng0){
				leng=leng0;
				break;
			}else{
				break;
			}
		}
	}
	
	if(leng>leng0){
		//pWnd->MessageBox("修复失败，找音视频帧时发生错误。");
		pWnd->SetWindowText(windowText0);
		pWnd->isFixing=false;
		return 0;
	}

	int lengt=leng+16*idx1Leng;
	int wid=0;
	int hei=0;
	
	pWnd->SetWindowText("Fixing");
	fseek(fp,4,SEEK_SET);
	fwrite(&lengt,1,4,fp);
	fseek(fp,0x30,SEEK_SET);
	fwrite(&frameCount,1,4,fp);
	
	fseek(fp,0x40,SEEK_SET);
	fread(&wid,1,4,fp);
	fread(&hei,1,4,fp);

	fseek(fp,0x8c,SEEK_SET);
	fwrite(&frameCount,1,4,fp);

	int pitch=(((wid*3)+31)/32)*4;
	int size=pitch*hei;
	//fseek(fp,0xc0,SEEK_SET);
	//fread(&size,1,4,fp);
	//if(size<0){
	fseek(fp,0xba,SEEK_SET);
	fputc(12,fp);
	fputc(0,fp);
		size=wid*hei*3;
		size=(size&1)==0?size/2:(size+1)/2;
		fseek(fp,0xc0,SEEK_SET);
		fwrite(&size,1,4,fp);
	//}
	
	int videoLength=leng-movipos+4;
	if(movipos>=8 && movipos<=leng){
		fseek(fp,movipos-8,SEEK_SET);
		fwrite(&videoLength,1,4,fp);
		fseek(fp,movipos,SEEK_SET);
	}else{
		//pWnd->MessageBox("Fixing Failed.");
		pWnd->SetWindowText(windowText0);
		pWnd->isFixing=false;
		return 0;
	}
	

	int *idx1=new int[idx1Leng];
	bool zeroMode=false;
	
	//cstr2.Format("%x;",leng0);
	//cstr+=cstr2;
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
			if(ftell(fp)+idx1t+8>=leng){
				zeroMode=true;
			}else{
				fseek(fp,idx1t,SEEK_CUR);
			}
		}
		cstr+=cstr2;
		if(i%100==0){
			cstr.Format("Calculating length:%d/%d...",i,idx1Leng);
			pWnd->SetWindowText(cstr);
		}
	}
	//cstr2.Format("%x,%x,%x;",leng0,leng,lengt);
	//cstr+=cstr2;
	//MessageBox(cstr);
	
	fseek(fp,leng,SEEK_SET);
	i=0;
	j=-8;
	k=ftell(fp);
	int idx1BytesLeng=idx1Leng*16;
	int idx1Sum=4;
	int videoFrameCount=-1;
	while(true){
		if(k==leng0 && i<idx1Leng){
			fclose(fp);
			fp=fopen(fileName,"ab");
			if(fp==NULL){
				delete []idx1;
				idx1=NULL;
				pWnd->MessageBox(".");
				pWnd->SetWindowText(windowText0);
				pWnd->isFixing=false;
				return 0;
			}
			k=ftell(fp);
			/*if(k==leng0+1){
				if(j==8 && i<idx1Leng){
					idx1t=idx1[i]>=0?idx1[i]:-(idx1[i]+1);
				}
				if(j==15 && i<idx1Leng){
					idx1t=(idx1t&1)==0?idx1t:idx1t+1;
					if(idx1Sum-4+movipos+idx1t+8<leng){
						idx1Sum+=idx1t+8;
					}
					j=0;
					i++;
				}
				j++;
			}*/
			/*if(k==leng+1){
				fclose(fp);
				fp=fopen(fileName,"rb+");
				fseek(fp,1,SEEK_END);
				leng0++;
			}*/
		}
		if(j<0){
			if(j==-8){
				fputc('i',fp);
				j++;
			}else if(j==-7){
				fputc('d',fp);
				j++;
			}else if(j==-6){
				fputc('x',fp);
				j++;
			}else if(j==-5){
				fputc('1',fp);
				j++;
			}else if(j==-4){
				fputc(idx1BytesLeng&0xff,fp);
				j++;
			}else if(j==-3){
				fputc(idx1BytesLeng>>8&0xff,fp);
				j++;
			}else if(j==-2){
				fputc(idx1BytesLeng>>16&0xff,fp);
				j++;
			}else if(j==-1){
				fputc(idx1BytesLeng>>24&0xff,fp);
				j++;
			}else{
				fseek(fp,1,SEEK_CUR);
			}
		}else if(i<idx1Leng){
			if(j==0){
				fputc('0',fp);
				j++;
			}else if(j==1){
				fputc(idx1[i]>=0?'0':'1',fp);
				j++;
			}else if(j==2){
				fputc(idx1[i]>=0?'d':'w',fp);
				j++;
			}else if(j==3){
				fputc(idx1[i]>=0?'c':'b',fp);
				j++;
				if(idx1[i]>=0){
					videoFrameCount++;
				}
			}else if(j==4){
				fputc(idx1[i]<0 || 
					(keyframeInterval>0 && videoFrameCount%keyframeInterval==0 || videoFrameCount==0)
					?0x10:0,fp);
				j++;
			}else if(j==5){
				fputc(0,fp);
				j++;
			}else if(j==6){
				fputc(0,fp);
				j++;
			}else if(j==7){
				fputc(0,fp);
				j++;
			}else if(j==8){
				fputc(idx1Sum&0xff,fp);
				j++;
			}else if(j==9){
				fputc(idx1Sum>>8&0xff,fp);
				j++;
			}else if(j==10){
				fputc(idx1Sum>>16&0xff,fp);
				j++;
			}else if(j==11){
				fputc(idx1Sum>>24&0xff,fp);
				j++;
			}else if(j==12){
				idx1t=idx1[i]>=0?idx1[i]:-(idx1[i]+1);
				fputc(idx1t&0xff,fp);
				j++;
			}else if(j==13){
				fputc(idx1t>>8&0xff,fp);
				j++;
			}else if(j==14){
				fputc(idx1t>>16&0xff,fp);
				j++;
			}else if(j==15){
				fputc(idx1t>>24&0xff,fp);
				idx1t=(idx1t&1)==0?idx1t:idx1t+1;
				if(idx1Sum-4+movipos+idx1t+8<leng){
					idx1Sum+=idx1t+8;
				}
				j=0;
				if(i%100==0){
					cstr.Format("Regenerating index:%d/%d...",i,idx1Leng);
					pWnd->SetWindowText(cstr);
				}
				i++;
				
			}
		}else if(k<leng0-1){
			fputc(0xff,fp);
		}else{
			break;
		}
		k++;
	}
	
	
	
	delete []idx1;
	idx1=NULL;
	fclose(fp);
	fp=NULL;
	pWnd->SetWindowText(windowText0);
	pWnd->MessageBox("Success.");
	pWnd->isFixing=false;
	return 1;
}

static void writeAudioAndJunkHeader(FILE *fp){
	int i=0;
	if(fp==NULL){
		return;
	}
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

	fputc(0x38,fp);
	fputc(0,fp);
	fputc(0,fp);
	fputc(0,fp);

	fputc('a',fp);
	fputc('u',fp);
	fputc('d',fp);
	fputc('s',fp);

	fputc(0x55,fp);//mp3为0x55
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
	fputc(0x3e,fp);
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
	fputc(0x02,fp);
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
	fputc(0x55,fp);//编码格式，mp3是0x55
	fputc(0,fp);

	fputc(2,fp);//声道格式
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

	fputc(0xc,fp);//采样位数
	fputc(0,fp);

	fputc(1,fp);//?
	fputc(0,fp);

	fputc(2,fp);//?
	fputc(0,fp);
	fputc(0,fp);
	fputc(0,fp);

	fputc(0xa2,fp);//?
	fputc(0x01,fp);
	fputc(0x01,fp);
	fputc(0,fp);

	fputc(0,fp);
	fputc(0,fp);

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
	int leng=0x1000-ftell(fp)-8;

	fputc('J',fp);
	fputc('U',fp);
	fputc('N',fp);
	fputc('K',fp);

	fputc(leng&0xff,fp);
	fputc(leng>>8&0xff,fp);
	fputc(leng>>16&0xff,fp);
	fputc(leng>>24&0xff,fp);
	int j=0;
	for(i=0;i<leng;i++){
		fputc(movieJunk[j],fp);
		j++;
		if(j>=movieJunkLeng){
			j=0;
		}
	}
}

int CAviToolInput::aviMux(char *videoFile,char *audioFile,char *outputFile){
	aviMuxProgress=0.0;
	FILE *fp0=fopen(videoFile,"rb");
	if(fp0==NULL){
		return 0;
	}
	FILE *fp1=fopen(audioFile,"rb");
	if(fp1==NULL){
		fclose(fp0);
		return 0;
	}
	
	fseek(fp0,0,SEEK_END);
	int videoFileLength=ftell(fp0);
	if((videoFileLength&1)==1 && feof(fp0)==0){
		videoFileLength++;
	}
	if(videoFileLength<0x1020){
		fclose(fp0);
		fclose(fp1);
		return 0;
	}
	fseek(fp0,0,SEEK_SET);

	fseek(fp1,0,SEEK_END);
	int audioFileLength=ftell(fp1)+1;
	if(audioFileLength<0x1b0){
		fclose(fp0);
		fclose(fp1);
		return 0;
	}
	fseek(fp0,0,SEEK_SET);

	FILE *fp=fopen(outputFile,"wb");
	if(fp==NULL){
		fclose(fp0);
		fclose(fp1);
		return 0;
	}

	unsigned char aviHead[0x1010];

	fread(aviHead,1,0x10,fp0);
	fwrite(aviHead,1,0x10,fp);
	fseek(fp0,4,SEEK_CUR);
	fputc(0x7e,fp);
	fputc(1,fp);
	fputc(0,fp);
	fputc(0,fp);

	fread(aviHead,1,0x24,fp0);
	fwrite(aviHead,1,0x24,fp);
	int videoFrameCount=(aviHead[0x1c+3]<<24|aviHead[0x1c+2]<<16|aviHead[0x1c+1]<<8|aviHead[0x1c]);
	
	fseek(fp0,4,SEEK_CUR);
	fputc(2,fp);
	fputc(0,fp);
	fputc(0,fp);
	fputc(0,fp);
	
	fread(aviHead,1,0x120-0x3c,fp0);
	fwrite(aviHead,1,0x120-0x3c,fp);
	
	writeAudioAndJunkHeader(fp);

	int i=0;
	unsigned char c=0;
	int frameSize=0;
	fseek(fp1,0x1a1,SEEK_SET);
	do{
		if(i>0){
			fseek(fp1,frameSize-4,SEEK_CUR);
		}

		c=fgetc(fp1);
		if(c!=0xff){
			break;
		}

		c=fgetc(fp1);
		if(c!=0xfb){
			break;
		}

		c=fgetc(fp1);
		if(c!=0x90 && c!=0x92){
			break;
		}
		frameSize=(c==0x90)?0x1a1:0x1a2;

		fgetc(fp1);
		
		i++;
	}while(ftell(fp1)+frameSize<audioFileLength);
	
	if(ftell(fp1)+frameSize-4>audioFileLength){
		i--;
	}
	int audioFrameCount=i;
	//CString str;
	//str.Format("%x",audioFileLength);
	//::MessageBox(NULL,str,"",MB_OK);
	aviMuxProgress=0.1;

	fputc('L',fp);//movi
	fputc('I',fp);
	fputc('S',fp);
	fputc('T',fp);

	fputc(0,fp);
	fputc(0,fp);
	fputc(0,fp);
	fputc(0,fp);//混流后的长度，需要在录完后进行修改(1004H)

	
	fputc('m',fp);
	fputc('o',fp);
	fputc('v',fp);
	fputc('i',fp);

	int idx1Length=videoFrameCount+audioFrameCount;
	int *idx1=new int[videoFrameCount+audioFrameCount];
	int bufferLength=1024*1024;
	unsigned char *buffer=new unsigned char[bufferLength];
	
	int j=0;
	int k=0;
	int l=0;
	i=0;
	fseek(fp0,0x100c,SEEK_SET);
	fseek(fp1,0x1a1,SEEK_SET);
	int audioSize=0;
	do{
		if(((i+1)*(__int64)audioFrameCount<(j+1)*(__int64)videoFrameCount || j>=audioFrameCount) && i<videoFrameCount){
			fseek(fp0,4,SEEK_CUR);
			fread(&frameSize,1,4,fp0);

			l=ftell(fp0)+frameSize;
			if(l>videoFileLength){
				i=videoFrameCount;
				continue;
			}else if(l+8>videoFileLength){
				i=videoFrameCount-1;
			}

			if(frameSize>bufferLength){
				bufferLength=frameSize+1024;
				delete []buffer;
				buffer=new unsigned char[bufferLength];
			}
			fread(buffer,1,frameSize,fp0);

			fputc('0',fp);
			fputc('0',fp);
			fputc('d',fp);
			fputc('c',fp);
			fwrite(&frameSize,1,4,fp);
			fwrite(buffer,1,frameSize,fp);
			if((frameSize&1)==1){
				fgetc(fp0);
				fputc(0,fp);
			}
			i++;

			idx1[k]=frameSize;
			k++;
		}else if(j<audioFrameCount){
			fseek(fp1,2,SEEK_CUR);
			c=fgetc(fp1);
			if(c==0x90){
				frameSize=0x1a1;
			}else{
				frameSize=0x1a2;
			}
			fseek(fp1,-3,SEEK_CUR);

			l=ftell(fp1)+frameSize;
			if(l>audioFileLength){
				j=audioFrameCount;
				continue;
			}else if(l+4>audioFileLength){
				j=audioFrameCount-1;
			}

			fread(buffer,1,frameSize,fp1);
			buffer[3]=0x0c;
			
			fputc('0',fp);
			fputc('1',fp);
			fputc('w',fp);
			fputc('b',fp);
			fwrite(&frameSize,1,4,fp);
			fwrite(buffer,1,frameSize,fp);
			if((frameSize&1)==1){
				fputc(0,fp);
				audioSize+=frameSize+9;
			}else{
				audioSize+=frameSize+8;
			}
			j++;

			idx1[k]=-frameSize-1;
			k++;
		}else{
			break;
		}
		aviMuxProgress=0.8*k/(double)idx1Length+0.1;
	}while(k<idx1Length);
	
	idx1Length=k;
	int leng0=ftell(fp);
	fputc('i',fp);
	fputc('d',fp);
	fputc('x',fp);
	fputc('1',fp);
	int idx1BytesLeng=idx1Length*16;
	fwrite(&(idx1BytesLeng),1,4,fp);
	int idx1Sum=4;
	j=-1;
	int idx1t=0;
	int keyFrameStep=100;
	for(i=0;i<idx1Length;i++){
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
		
		idx1t=(idx1t&1)==0?idx1t:idx1t+1;
		if(idx1Sum+0x1008+idx1t+8<leng0){
			idx1Sum+=idx1t+8;
		}
	}
	int lengt=ftell(fp);
	/*fclose(fp);

	fp=fopen(outputFile,"r+");
	if(fp==NULL){
		delete []buffer;
		delete []idx1;
		fclose(fp0);
		fclose(fp1);
		fclose(fp);
		return 0;
	}*/
	
	fseek(fp,4,SEEK_SET);
	i=lengt-8;
	fwrite(&i,1,4,fp);

	fseek(fp,0x154,SEEK_SET);
	i=audioSize;
	fwrite(&i,1,4,fp);

	fseek(fp,0x1004,SEEK_SET);
	i=leng0-0x1008;
	fwrite(&i,1,4,fp);


	delete []buffer;
	delete []idx1;
	fclose(fp0);
	fclose(fp1);
	fclose(fp);
	aviMuxProgress=1.0;
	return 1;
}

void CAviToolInput::OnOK() 
{
	// TODO: Add extra validation here
	if(!isFixing){
		CDialog::OnOK();
	}
}

void CAviToolInput::OnCancel() 
{
	// TODO: Add extra cleanup here
	if(!isFixing){
		CDialog::OnCancel();
	}
}

BOOL CAviToolInput::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN){
		return TRUE;
	}
	BOOL returnValue=CDialog::PreTranslateMessage(pMsg);
	/*if(pMsg->message==WM_KEYDOWN && (pMsg->wParam==16 || pMsg->wParam==17)){
		GetDlgItem(IDC_BUTTON3)->SetWindowText("视频混流");
	}
	if(pMsg->message==WM_KEYUP && (pMsg->wParam==16 || pMsg->wParam==17)){
		GetDlgItem(IDC_BUTTON3)->SetWindowText("视频修复");
	}*/
	return returnValue;
}

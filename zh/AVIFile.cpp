#include "StdAfx.h"
#include "avifile.h"

BOOL changeFps(UINT nfps,LPCSTR lpszFileName){
	BOOL nresult=FALSE;
	FILE *fp;
	fp=fopen(lpszFileName,"rb+");
	long minSize=0x88;
	char ch;
	if(fp==NULL){
		MessageBox(NULL,"要更改帧频率的文件不存在！","",MB_OK);
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
		MessageBox(NULL,"文件过小，无法改变帧频率！","",MB_OK);
	}
	fclose(fp);	
	return nresult;
}
UINT getFps(LPCSTR lpszFileName){
	UINT nfps=10;		
	FILE *fp;
	fp=fopen(lpszFileName,"rb+");
	long minSize=0x88;
	char ch;
	if(fp==NULL){
		MessageBox(NULL,"要获取帧频率的文件不存在！","",MB_OK);
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
		MessageBox(NULL,"文件过小，无法获取帧频率！","",MB_OK);
	}
	fclose(fp);		
	
	return nfps;
}

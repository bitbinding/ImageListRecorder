#include "StdAfx.h"
#include "avifile.h"

BOOL changeFps(UINT nfps,LPCSTR lpszFileName){
	BOOL nresult=FALSE;
	FILE *fp;
	fp=fopen(lpszFileName,"rb+");
	long minSize=0x88;
	char ch;
	if(fp==NULL){
		MessageBox(NULL,"Ҫ����֡Ƶ�ʵ��ļ������ڣ�","",MB_OK);
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
		MessageBox(NULL,"�ļ���С���޷��ı�֡Ƶ�ʣ�","",MB_OK);
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
		MessageBox(NULL,"Ҫ��ȡ֡Ƶ�ʵ��ļ������ڣ�","",MB_OK);
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
		MessageBox(NULL,"�ļ���С���޷���ȡ֡Ƶ�ʣ�","",MB_OK);
	}
	fclose(fp);		
	
	return nfps;
}

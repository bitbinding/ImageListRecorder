#include "stdafx.h"
#include "myMP3Enc.h"

PFUNMP3ENCInitAll MP3ENCInitAll=NULL;
PFUNMP3ENCUninitAll MP3ENCUninitAll=NULL;
//PFUNMP3ENCSetMP3Inf SetMP3Inf=NULL;
//PFUNMP3ENCGetMP3Inf GetMP3Inf=NULL;
PFUNMP3ENCSetInput SetInput=NULL;
PFUNMP3ENCSetInWavFmt SetInWavFmt=NULL;
PFUNMP3ENCStartRec StartRec=NULL;
PFUNMP3ENCPauseRec PauseRec=NULL;
PFUNMP3ENCStopRec StopRec=NULL;
//PFUNMP3ENCGetRecSec GetRecSec=NULL;
//PFUNMP3ENCGetRecStatus GetRecStatus=NULL;
PFUNMP3ENCGetDevices GetDevices=NULL;
PFUNMP3ENCGetLineIn GetLineIn=NULL;
bool myMP3EncLoadComplete=false;

bool myMp3EncLoadDllFile(char *fileStr){
	HMODULE hModule = ::LoadLibrary(fileStr);
	if(hModule==NULL)return false;

	MP3ENCInitAll=(PFUNMP3ENCInitAll)::GetProcAddress(hModule,"InitAll");
	if(MP3ENCInitAll==NULL)return false;
	MP3ENCUninitAll=(PFUNMP3ENCUninitAll)::GetProcAddress(hModule,"UninitAll");
	if(MP3ENCUninitAll==NULL)return false;
	//SetMP3Inf=(PFUNMP3ENCSetMP3Inf)::GetProcAddress(hModule,"SetMP3Inf")
	//if(SetMP3Inf==NULL)return false;
	//GetMP3Inf=(PFUNMP3ENCGetMP3Inf)::GetProcAddress(hModule,"GetMP3Inf");
	//if(GetMP3Inf==NULL)return false;
	SetInput=(PFUNMP3ENCSetInput)::GetProcAddress(hModule,"SetInput");
	if(SetInput==NULL)return false;
	SetInWavFmt=(PFUNMP3ENCSetInWavFmt)::GetProcAddress(hModule,"SetInWavFmt");
	if(SetInWavFmt==NULL)return false;
	StartRec=(PFUNMP3ENCStartRec)::GetProcAddress(hModule,"StartRec");
	if(StartRec==NULL)return false;
	PauseRec=(PFUNMP3ENCPauseRec)::GetProcAddress(hModule,"PauseRec");
	if(PauseRec==NULL)return false;
	StopRec=(PFUNMP3ENCStopRec)::GetProcAddress(hModule,"StopRec");
	if(StopRec==NULL)return false;
	//GetRecSec=(PFUNMP3ENCGetRecSec)::GetProcAddress(hModule,"GetRecSec");
	//if(GetRecSec==NULL)return false;

	//GetRecStatus=(PFUNMP3ENCGetRecStatus)::GetProcAddress(hModule,"GetRecStatus");
	//if(GetRecStatus==NULL)return false;
	GetDevices=(PFUNMP3ENCGetDevices)::GetProcAddress(hModule,"GetDevices");
	if(GetDevices==NULL)return false;
	GetLineIn=(PFUNMP3ENCGetLineIn)::GetProcAddress(hModule,"GetLineIn");
	if(GetLineIn==NULL)return false;
	myMP3EncLoadComplete=true;
	return true;
}
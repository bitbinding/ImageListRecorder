////////////////////////////////////////////////////////////////////////
// File Name      : MP3Enc.h
// Description    : MP3 audio encoder interface
// Make Date      : 2006/10/18
// Home page	  : www.codefans.net
// E_mail         : shinedeer@126.com
// Copyright      : 2005-2006 Shinedeer Inc. All rights reserved.
// Histoy         : 	
///////////////////////////////////////////////////////////////////////////

#ifndef  MP3ENC_H
#define  MP3ENC_H

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_DEVICES   5      /* default maximum audio device number */
#define MAX_LINEIN    50     /* default maximum line in device number */
#define MAX_NAME_BUF  255    /* device name buffer */

enum  OUTPUT_FILE_TYPE
{
	OUT_FILE_MP3,	/* output file type is MP3 */
	OUT_FILE_WAV	/* output file type is Wave */
};

/* mp3 player status */
enum  PLAY_STATUS
{
	PST_PLAYING,	/* current is playing */
	PST_PAUSE,		/* current is pause */
	PST_STOP		/* current is stop */
};

/* recorder status */
enum  RECORD_STATUS
{
	RECST_RECORD,	/* current is record */
	RECST_PAUSE,	/* current is pause */
	RECST_STOP		/* current is stop */
};

/* mp3 output mode */
enum OUT_MODE 
{
	MODE_GEN_STEREO,	/* general stereo */
	MODE_JOINT_STEREO,	/* joint stereo */	
	MODE_MONO,			/* mono */
};

/* mp3 compress quality */
enum OUT_QUALITY
{
	QT_BEST,		/* best quality */
	QT_BETTER,		/* better quality */
	QT_GEN,			/* general quality */	
	QT_WORST,		/* worst quality, hight speed */
};


/* mp3 bitrate value */
static const unsigned int dwBitrate[3][16] =
{
    {0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, -1},     /* MPEG 2 (khz) */
    {0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, -1}, /* MPEG 1 (khz) */
    {0, 8, 16, 24, 32, 40, 48, 56, 64, -1, -1, -1, -1, -1, -1, -1},		    /* MPEG 2.5 (khz)*/
};

/* mp3 sample rate */
static const long samplerate[9] = 
{ 
	44100, 48000, 32000,  // MPEG 1.0
	22050, 24000, 16000,  // MPEG 2.0
	11025, 12000,  8000   // MPEG 2.5
};

/* output mp3 file information */
typedef struct mp3inf_s 
{
	/***********mp3 audio information***********/
	int out_version;       /* mpeg audio version, 0=MPEG-2/2.5  1=MPEG-1 */
	int out_channels;      /* mp3 audio channels, (1 or 2) */
	int out_samplerate;    /* mp3 audio sample rate, (see samplerate) */
	int out_dwBitrate;     /* bitrate in bit per second,(see dwBitRateValue) */
	int out_useVBR;        /* if use VBR mode */
	int out_VBR_min;       /* a minimum allowed bitrate */
	int out_VBR_max;       /* a maximum allowed bitrate */
	int out_force_mono;    /* if force to mono mode when have 2 channel input */
	int out_mode;          /* output mp3 audio mode stereo or other, see OUT_MODE */
	int out_force_ms;      /* force M/S mode.  only use for join stereo mode */
	int out_cbr_quality;   /* CBR compress quality, see OUT_QUALITY */
	int out_vbr_quality;   /* VBR compress quality, see OUT_QUALITY */

	/***********mp3 id3 information***********/
	char artist[MAX_NAME_BUF];		/* artist */
	char album[MAX_NAME_BUF];		/* album */
	char title[MAX_NAME_BUF];		/* title */
	char track[MAX_NAME_BUF];		/* track */
	char year[MAX_NAME_BUF];		/* year */	
	char genre[MAX_NAME_BUF];		/* genre */
	char comment[MAX_NAME_BUF];		/* comment */

}mp3inf_t;

typedef int (*PFUNMP3ENCInitAll)(void);
typedef void (*PFUNMP3ENCUninitAll)(void);
//typedef int (*PFUNMP3ENCSetMP3Inf)(mp3inf_t*);
//typedef int (*PFUNMP3ENCGetMP3Inf)(mp3inf_t*);
typedef int (*PFUNMP3ENCSetInput)(char*,char*);
typedef int (*PFUNMP3ENCStartRec)(char*, int);
typedef int (*PFUNMP3ENCSetInWavFmt)(int, int);
typedef int (*PFUNMP3ENCPauseRec)(void);
typedef int (*PFUNMP3ENCStopRec)(void);
//typedef int (*PFUNMP3ENCGetRecSec)(void);
typedef int (*PFUNMP3ENCGetDevices)(char[MAX_DEVICES][MAX_NAME_BUF]);
typedef int (*PFUNMP3ENCGetLineIn)(char[MAX_LINEIN][MAX_NAME_BUF],char*);
//typedef int (*PFUNMP3ENCGetRecStatus)(void);

bool myMp3EncLoadDllFile(char *fileStr);


#ifdef __cplusplus
}
#endif

#endif    // MP3ENC_H

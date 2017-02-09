// ImageListRecorderView.h : interface of the CImageListRecorderView class
//
/////////////////////////////////////////////////////////////////////////////
#include "ImageListRecorderDoc.h"
#include "RecordSettingDialog.h"
#include "AVIFile.h"
#include "myxvid.h"
#if !defined(AFX_IMAGELISTRECORDERVIEW_H__B2ED95C0_F29F_4878_9EA8_B7097EF6FF26__INCLUDED_)
#define AFX_IMAGELISTRECORDERVIEW_H__B2ED95C0_F29F_4878_9EA8_B7097EF6FF26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define WM_RECORDED (WM_USER+100)
#pragma comment(linker, "/ignore:4089") 
class CImageListRecorderView : public CScrollView
{
protected: // create from serialization only
	CImageListRecorderView();
	DECLARE_DYNCREATE(CImageListRecorderView)
private:
	
	
// Attributes
public:
	CImageListRecorderDoc* GetDocument();	
	int imgLeng;//最大帧数
	int imgi;//当前帧减一
	CRect recordRect;//要录制的矩形
	int jiange;//录制间隔
	int shichang;//录制时长
	//CString filePath;
	CString fileName0;//主要文件的文件名标号前部分和文件路径

	bool recordMode;//是否为录制模式
	bool showCursor;//是否截取鼠标
	CImage cimage;//要录制原图
	CImage cimage2;//要录制原图
	int fileType;//文件类型
	COleDateTime time0;//录制开始时刻
	xvid_gbl_init_t xvidGblInit;
	xvid_enc_create_t avi0;
	void* avi0Handle;
	xvid_enc_frame_t aviFrame;
	xvid_enc_stats_t aviStates;	
	bool aviIsNull;
	FILE *aviFile;
	unsigned char *bitStream;
	void readRecordLogFile(CRecordSettingDialog *dlg,unsigned long &fileFilter);
	void writeRecordLogFile(CRecordSettingDialog *dlg,unsigned long &fileFilter);
	void TingZhiLuZhi();
	static BOOL CreateAviHeader(FILE *fp,int wid,int hei,int fps,int size,bool recordAudio);
	static int RecordXvidFrame(FILE *fp,CImage *image,xvid_enc_create_t *avi0,xvid_enc_frame_t *frame,xvid_enc_stats_t *states);
	static BOOL FinishAviFile(char *fileName,int idx1Leng,int *idx1=NULL,int nfps=0);
	int currentBufferLength;
	static int xvidVersion;
	
	int maxFileSize;
	int recordDelay;//延迟录制
	int exiting;//正在执行析构方法
	int *idx1;//索引列表
	bool recordAudio;//视频将会包含音频
	CString soundName0;//声音文件的文件名标号前部分和文件路径
	bool recordingSound;//声音是否正在被录制
	bool recordedSound;//声音是否已经被录制过一次
	//bool mp3EncoderInited;//mp3录制器是否已被创建
// Operations
public:	
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageListRecorderView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL
// Implementation
public:
	virtual ~CImageListRecorderView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CImageListRecorderView)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnFileNew();
	afx_msg void OnTingZhiLuZhi();
	afx_msg void OnFileSave();
	afx_msg void OnUpdateFileNew(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileOpen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTingZhiLuZhi(CCmdUI* pCmdUI);
	afx_msg void OnBeginpage();
	afx_msg void OnAvitoolsmenuitem();
	afx_msg LRESULT OnRecorded(WPARAM wParam, LPARAM lParam); 
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#ifndef _DEBUG  // debug version in ImageListRecorderView.cpp
inline CImageListRecorderDoc* CImageListRecorderView::GetDocument()
   { return (CImageListRecorderDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGELISTRECORDERVIEW_H__B2ED95C0_F29F_4878_9EA8_B7097EF6FF26__INCLUDED_)

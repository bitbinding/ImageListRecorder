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
	int imgLeng;//���֡��
	int imgi;//��ǰ֡��һ
	CRect recordRect;//Ҫ¼�Ƶľ���
	int jiange;//¼�Ƽ��
	int shichang;//¼��ʱ��
	//CString filePath;
	CString fileName0;//��Ҫ�ļ����ļ������ǰ���ֺ��ļ�·��

	bool recordMode;//�Ƿ�Ϊ¼��ģʽ
	bool showCursor;//�Ƿ��ȡ���
	CImage cimage;//Ҫ¼��ԭͼ
	CImage cimage2;//Ҫ¼��ԭͼ
	int fileType;//�ļ�����
	COleDateTime time0;//¼�ƿ�ʼʱ��
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
	int recordDelay;//�ӳ�¼��
	int exiting;//����ִ����������
	int *idx1;//�����б�
	bool recordAudio;//��Ƶ���������Ƶ
	CString soundName0;//�����ļ����ļ������ǰ���ֺ��ļ�·��
	bool recordingSound;//�����Ƿ����ڱ�¼��
	bool recordedSound;//�����Ƿ��Ѿ���¼�ƹ�һ��
	//bool mp3EncoderInited;//mp3¼�����Ƿ��ѱ�����
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

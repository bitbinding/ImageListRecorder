#if !defined(AFX_SAVINGIMAGELISTDIALOG_H__94DC412C_DAFF_4A0A_83F2_824E9C29D04B__INCLUDED_)
#define AFX_SAVINGIMAGELISTDIALOG_H__94DC412C_DAFF_4A0A_83F2_824E9C29D04B__INCLUDED_
#include "AVIFile.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SavingImageListDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSavingImageListDialog dialog

class CSavingImageListDialog : public CDialog
{
// Construction
public:
	//General
	CSavingImageListDialog(CWnd* pParent = NULL);   // standard constructor	
	static bool CSavingImageListDialog::enumExtNamesToReadBitmap(CImage &img,LPCTSTR filePathName);
	CImage cimage;//用于拼接的原位图
	CImage dimage;//用于改变位深度的位图
	CImage zimage;//用于拼接的缩放后的位图
	CString fnamea;//avi文件名称和路径
	int fileType;//avi文件格式
	CStringArray nameList;//图片文件名称和路径数组
	int wid0;//视频宽度
	int hei0;//视频高度
	int widt;//修正后的视频宽度
	int heit;//修正后的视频高度
	int imgLeng;//准备拼接的位图的数量，为零时不拼接。
	int savedCount;//已保存图片的数量
	UINT fps0;//输出视频的帧频率
	bool willScale;//是否进行缩放
	COLORREF bgc;//背景颜色
	bool working;//是否正在拼接

	//xvid
	xvid_enc_create_t avi0;
	void* avi0Handle;
	xvid_enc_frame_t aviFrame;
	xvid_enc_stats_t aviStates;	
	unsigned char *bitStream;
	int *idx1;//索引列表
// Dialog Data
	//{{AFX_DATA(CSavingImageListDialog)
	enum { IDD = IDD_DIALOG2 };
	CProgressCtrl	m_p1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSavingImageListDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSavingImageListDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAVINGIMAGELISTDIALOG_H__94DC412C_DAFF_4A0A_83F2_824E9C29D04B__INCLUDED_)

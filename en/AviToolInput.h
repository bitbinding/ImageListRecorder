#if !defined(AFX_AVITOOLINPUT_H__399CD87F_DD15_4DEA_8BE7_38A4325A8A9A__INCLUDED_)
#define AFX_AVITOOLINPUT_H__399CD87F_DD15_4DEA_8BE7_38A4325A8A9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AviToolInput.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CAviToolInput dialog

class CAviToolInput : public CDialog
{
// Construction
public:
	CAviToolInput(CWnd* pParent = NULL);   // standard constructor
	bool aviOpenMode;//是否打开的是avi文件
	CStringArray imageFilePathNameList;//用于拼接的图片的文件路径和文件名数组

	CString fnamea;//文件名称和路径
	int wid0;//图片宽度
	int hei0;//图片高度
	//void strNumSort(const CStringArray &strArrSrc,CStringArray &strArrOut);//将CStringArray用strnumcmp的比较方法排序
	static BOOL changeFps(UINT nfps,LPCSTR	lpszFileName);//改变视频的帧频率
	static UINT getFps(LPCSTR lpszFileName);//获取视频的帧频率
	static UINT aviFix(LPVOID pParam);//视频的修复
	static int aviMux(char *videoFile,char *audioFile,char *outputFile);//视频的混流
	static double aviMuxProgress;//视频的混流进度
	CString aviFixFileName;//视频修复的文件名
	bool isFixing;//视频是否在修复
	// Dialog Data
	//{{AFX_DATA(CAviToolInput)
	enum { IDD = IDD_DIALOG3 };
	CButton	m_s2;
	CButton	m_s1;
	BOOL	m_c1;
	BOOL	m_c2;
	int		m_e7;
	int		m_e8;
	UINT	m_e6;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAviToolInput)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAviToolInput)
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnCheck2();
	afx_msg void OnButton3();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AVITOOLINPUT_H__399CD87F_DD15_4DEA_8BE7_38A4325A8A9A__INCLUDED_)

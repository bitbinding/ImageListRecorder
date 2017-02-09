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
	bool aviOpenMode;//�Ƿ�򿪵���avi�ļ�
	CStringArray imageFilePathNameList;//����ƴ�ӵ�ͼƬ���ļ�·�����ļ�������

	CString fnamea;//�ļ����ƺ�·��
	int wid0;//ͼƬ���
	int hei0;//ͼƬ�߶�
	//void strNumSort(const CStringArray &strArrSrc,CStringArray &strArrOut);//��CStringArray��strnumcmp�ıȽϷ�������
	static BOOL changeFps(UINT nfps,LPCSTR	lpszFileName);//�ı���Ƶ��֡Ƶ��
	static UINT getFps(LPCSTR lpszFileName);//��ȡ��Ƶ��֡Ƶ��
	static UINT aviFix(LPVOID pParam);//��Ƶ���޸�
	static int aviMux(char *videoFile,char *audioFile,char *outputFile);//��Ƶ�Ļ���
	static double aviMuxProgress;//��Ƶ�Ļ�������
	CString aviFixFileName;//��Ƶ�޸����ļ���
	bool isFixing;//��Ƶ�Ƿ����޸�
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

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
	CImage cimage;//����ƴ�ӵ�ԭλͼ
	CImage dimage;//���ڸı�λ��ȵ�λͼ
	CImage zimage;//����ƴ�ӵ����ź��λͼ
	CString fnamea;//avi�ļ����ƺ�·��
	int fileType;//avi�ļ���ʽ
	CStringArray nameList;//ͼƬ�ļ����ƺ�·������
	int wid0;//��Ƶ���
	int hei0;//��Ƶ�߶�
	int widt;//���������Ƶ���
	int heit;//���������Ƶ�߶�
	int imgLeng;//׼��ƴ�ӵ�λͼ��������Ϊ��ʱ��ƴ�ӡ�
	int savedCount;//�ѱ���ͼƬ������
	UINT fps0;//�����Ƶ��֡Ƶ��
	bool willScale;//�Ƿ��������
	COLORREF bgc;//������ɫ
	bool working;//�Ƿ�����ƴ��

	//xvid
	xvid_enc_create_t avi0;
	void* avi0Handle;
	xvid_enc_frame_t aviFrame;
	xvid_enc_stats_t aviStates;	
	unsigned char *bitStream;
	int *idx1;//�����б�
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

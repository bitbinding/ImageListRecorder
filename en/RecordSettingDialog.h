#if !defined(AFX_RECORDSETTINGDIALOG_H__35A720D9_5162_499D_9A73_186AC0C73293__INCLUDED_)
#define AFX_RECORDSETTINGDIALOG_H__35A720D9_5162_499D_9A73_186AC0C73293__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecordSettingDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRecordSettingDialog dialog

class CRecordSettingDialog : public CDialog
{
// Construction
public:
	
	CRecordSettingDialog(CWnd* pParent = NULL);   // standard constructor
	void GetRecordRect();//获取窗口所在矩形并更新界面
	int initialMove;//是否窗口弹出时窗口弹出时自动调用的OnMove方法，可在OnMove里设置截取的默认值
	void drawWindowRect();
	bool cancelledWithWrongData;
	int soundInputType;
	CString deviceString;
	CString lineInString;
	CString deviceString0;
	CString lineInString0;
	bool willGetInputDeviceName;
	// Dialog Data
	//{{AFX_DATA(CRecordSettingDialog)
	enum { IDD = IDD_DIALOG1 };
	CComboBox	m_co1;
	int		m_e1;
	int		m_e2;
	int		m_e3;
	int		m_e4;
	int		m_e5;
	int		m_e6;
	BOOL	m_c1;
	int		m_e8;
	int		m_e7;
	int		m_e9;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecordSettingDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRecordSettingDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnButton2();
	afx_msg void OnButton1();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnCheck2();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSelchangeCombo1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECORDSETTINGDIALOG_H__35A720D9_5162_499D_9A73_186AC0C73293__INCLUDED_)

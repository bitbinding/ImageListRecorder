#if !defined(AFX_MUXPROCESSDISPLAY_H__FCD324B1_BDB3_4E02_BDC8_81F21AE3DC16__INCLUDED_)
#define AFX_MUXPROCESSDISPLAY_H__FCD324B1_BDB3_4E02_BDC8_81F21AE3DC16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MuxProcessDisplay.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMuxProcessDisplay dialog

class CMuxProcessDisplay : public CDialog
{
// Construction
public:
	CMuxProcessDisplay(CWnd* pParent = NULL);   // standard constructor
	static int muxFinishedCount;
	void OnOK();
	void OnCancel();
// Dialog Data
	//{{AFX_DATA(CMuxProcessDisplay)
	enum { IDD = IDD_DIALOG4 };
	CProgressCtrl	m_p1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMuxProcessDisplay)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMuxProcessDisplay)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MUXPROCESSDISPLAY_H__FCD324B1_BDB3_4E02_BDC8_81F21AE3DC16__INCLUDED_)

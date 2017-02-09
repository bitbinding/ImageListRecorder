// ImageListRecorder.h : main header file for the IMAGELISTRECORDER application
//

#if !defined(AFX_IMAGELISTRECORDER_H__3474310E_BC43_4302_AF0A_E81DFA88644F__INCLUDED_)
#define AFX_IMAGELISTRECORDER_H__3474310E_BC43_4302_AF0A_E81DFA88644F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CImageListRecorderApp:
// See ImageListRecorder.cpp for the implementation of this class
//

class CImageListRecorderApp : public CWinApp
{
public:
	CImageListRecorderApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageListRecorderApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CImageListRecorderApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGELISTRECORDER_H__3474310E_BC43_4302_AF0A_E81DFA88644F__INCLUDED_)

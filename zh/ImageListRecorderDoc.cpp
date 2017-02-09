// ImageListRecorderDoc.cpp : implementation of the CImageListRecorderDoc class
//

#include "stdafx.h"
#include "ImageListRecorder.h"
#include "ImageListRecorderDoc.h"
//#include "Image"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageListRecorderDoc

IMPLEMENT_DYNCREATE(CImageListRecorderDoc, CDocument)

BEGIN_MESSAGE_MAP(CImageListRecorderDoc, CDocument)
	//{{AFX_MSG_MAP(CImageListRecorderDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageListRecorderDoc construction/destruction

CImageListRecorderDoc::CImageListRecorderDoc()
{
	// TODO: add one-time construction code here

}

CImageListRecorderDoc::~CImageListRecorderDoc()
{
}

BOOL CImageListRecorderDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	
	
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CImageListRecorderDoc serialization

void CImageListRecorderDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CImageListRecorderDoc diagnostics

#ifdef _DEBUG
void CImageListRecorderDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImageListRecorderDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageListRecorderDoc commands

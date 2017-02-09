// stdafx.cpp : source file that includes just the standard includes
//	ImageToWords.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#if _MSC_VER < 1300
CImage::CInitGDIPlus CImage::s_initGDIPlus;
CImage::CDCCache CImage::s_cache;
#endif

	#ifdef _ATL_STATIC_REGISTRY
#include <statreg.h>
#endif
#include <atlimpl.cpp>
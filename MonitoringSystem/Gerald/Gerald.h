// Gerald.h : main header file for the GERALD application
//

#if !defined(AFX_GERALD_H__89631D4D_5B95_4BA1_A0A8_BF321EE25363__INCLUDED_)
#define AFX_GERALD_H__89631D4D_5B95_4BA1_A0A8_BF321EE25363__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CGeraldApp:
// See Gerald.cpp for the implementation of this class
//

class CGeraldApp : public CWinApp
{
public:
	CGeraldApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGeraldApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGeraldApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GERALD_H__89631D4D_5B95_4BA1_A0A8_BF321EE25363__INCLUDED_)

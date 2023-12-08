// GeraldDlg.h : header file
//

#if !defined(AFX_GERALDDLG_H__685E8FD6_A4EB_4F40_9259_C4D0B9FC6ECC__INCLUDED_)
#define AFX_GERALDDLG_H__685E8FD6_A4EB_4F40_9259_C4D0B9FC6ECC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CGeraldDlg dialog

class CGeraldDlg : public CDialog
{
// Construction
public:
	CGeraldDlg(CWnd* pParent = NULL);	// standard constructor
    int Width1;
	int Width2;     
	int Height1;
	int Height2;

	CWinThread *pthread;
	DWORD Condition;

	int Condition_num;

// Dialog Data
	//{{AFX_DATA(CGeraldDlg)
	enum { IDD = IDD_GERALD_DIALOG };
	CStatic	m_map;
	CComboBox	NameBOX;
	CComboBox	m_Username;
	CListBox	m_listname;
	CString	m_Status;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGeraldDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
	CString Gettitude(CStringArray* S,int num,CString name);

	CFont font;
	CDC m_MemDC;            //内存兼容的dc
	CBitmap m_Bitmap;       //内存兼容的位图
	CRect rect;             //控件的区域

	int data_num;//数据总共有589行，第一行是数据名称
	CStringArray stationID_;		
	CStringArray stationName_;
	CStringArray Address_;
	CStringArray Total_Docks_;
    CStringArray InService_;
    CStringArray Status_;
    CStringArray Location_;
	CStringArray Latitude_;
	CStringArray Longitude_;
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGeraldDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void Oninput();
	afx_msg void Oninputname();
	afx_msg void Ondrawing();
	afx_msg void OnCreatePlace();
	afx_msg void Onpaintting();
	afx_msg void OnCheck_button();
	afx_msg void OnButton7();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GERALDDLG_H__685E8FD6_A4EB_4F40_9259_C4D0B9FC6ECC__INCLUDED_)

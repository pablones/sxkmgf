#if !defined(AFX_DLGCONFIG_H__ABA08C72_8547_41D0_AB86_1C524B4844CA__INCLUDED_)
#define AFX_DLGCONFIG_H__ABA08C72_8547_41D0_AB86_1C524B4844CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgConfig.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgConfig dialog

class CDlgConfig : public CDialog
{
// Construction
public:
	CString GetStringByBool(BOOL bFlag);
	CDlgConfig(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgConfig)
	enum { IDD = IDD_DLG_CONFIG };
	CButton	m_chk_param06;
	CButton	m_chk_param05;
	CButton	m_chk_param04;
	CButton	m_chk_param03;
	CButton	m_chk_param02;
	CButton	m_chk_param01;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgConfig)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgConfig)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCheckParam01();
	afx_msg void OnCheckParam02();
	afx_msg void OnCheckParam03();
	afx_msg void OnCheckParam04();
	afx_msg void OnCheckParam05();
	afx_msg void OnCheckParam06();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCONFIG_H__ABA08C72_8547_41D0_AB86_1C524B4844CA__INCLUDED_)

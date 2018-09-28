// AccountServerDlg.h : header file
//

#if !defined(AFX_ACCOUNTSERVERDLG_H__1C382F39_7153_496B_9F90_3CF520BB02F8__INCLUDED_)
#define AFX_ACCOUNTSERVERDLG_H__1C382F39_7153_496B_9F90_3CF520BB02F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CAccountServerDlg dialog
#include "SupplyVASService.h"

class CAccountServerDlg : public CDialog
{
// Construction
public:
	CAccountServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CAccountServerDlg)
	enum { IDD = IDD_ACCOUNTSERVER_DIALOG };
	CString	m_sText;
	CString	m_sState2;
	CString	m_sState;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAccountServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
//void ProcessPacketTest(const ParamList& paramlist);
protected:
	long m_nLoginAcceptP5;
	long m_nPointFeeP5;
	HICON m_hIcon;

//	CVASSocket m_skVasSocket;
//	CSupplyVASService m_vasService;///增值充值服务
	// Generated message map functions
	//{{AFX_MSG(CAccountServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnCloseButtom();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnCancel();
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACCOUNTSERVERDLG_H__1C382F39_7153_496B_9F90_3CF520BB02F8__INCLUDED_)

// MsgServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "protocol.h"
#include "common.h"
#include "IniFile.h"
#include "LogFile.h"
#include "I_Shell.h"
#include "WorldThread.h"
#include "MessagePort.h"
#include "P_ServerManager.h"
#include "basetype.h"
#include "array.h"
#include "mystack.h"
#include "I_MessagePort.h"
#include "StrRes.h"

#include "MsgServer.h"
#include "MsgServerDlg.h"
#include "DlgConfig.h"

/////////////////////////////////////////////////////////////////////////////
struct STAT_STRUCT	g_stat = {0};
long				g_nRestart		= false;
long				s_nDatabaseTimeSum;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgServerDlg dialog

CMsgServerDlg::CMsgServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMsgServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMsgServerDlg)
	m_sText = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_nState		= SHELLSTATE_NONE;
	m_nTextLines	= 0;
	m_pMsgPort		= NULL;
	m_pSocketThread	= NULL;
	m_pClientThread	= NULL;
	m_pWorldThread	= NULL;


	m_nAllPlayers	= 0;
	m_nMaxPlayers	= 0;
}

void CMsgServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMsgServerDlg)
	DDX_Text(pDX, IDC_TEXT, m_sText);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMsgServerDlg, CDialog)
	//{{AFX_MSG_MAP(CMsgServerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgServerDlg message handlers

BOOL CMsgServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();	

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	DateTime(m_szStartServer, time(NULL));
	//初始化随机数
	srand( (unsigned)time( NULL ) );
	//初始化字符串
	g_objStrRes.Init();


	memset(&g_stat, 0, sizeof(g_stat));	


	bool bRet = false;
	if(!CMessagePort::InitPortSet(MSGPORT_WORLD + 1))
	{		
		MessageBox("Initial intra message port failed!");
		m_nState	= SHELLSTATE_END;
		bRet = true;
	}
	
	if(!bRet)
	{
		m_pMsgPort = CMessagePort::GetInterface(MSGPORT_SHELL);
		m_pMsgPort->Open();
		m_nState	= SHELLSTATE_INIT;

		SetWindowText("日志工具（数据库支持版） V1.1");
	}

	//开始循环
	SetTimer(1, 200, NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMsgServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if ((nID & 0xFFF0) == SC_CLOSE)
	{
		PostMessage(WM_COMMAND, IDCANCEL);
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMsgServerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMsgServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMsgServerDlg::OnTimer(UINT nIDEvent) 
{
	static	int nLock = 0;
	if(nIDEvent == 1 && nLock <= 0)
	{
		nLock++;
		//!!! Can't use 'return' in this block

		if(m_pMsgPort)
			Process();

		extern long				g_nRestart;
		switch(m_nState)
		{
		case	SHELLSTATE_NONE:
			{
				m_nState	= SHELLSTATE_END;
			}
			break;
		case	SHELLSTATE_INIT:
			{
				m_nState	= SHELLSTATE_CLOSING;		// 失败时退出

				PrintText("1、Init world thread...");
				m_pWorldThread = new CWorldThread(CMessagePort::GetInterface(MSGPORT_WORLD));
				if(!m_pWorldThread)
					break;
				if(!m_pWorldThread->CreateThread(false))
					break;

				PrintText("Init OK。");

				m_pWorldThread->ResumeThread();
				PrintText("All thread start OK。");
				m_nState	= SHELLSTATE_RUNNING;
			}
			break;
		case	SHELLSTATE_RUNNING:
			{
			}
			break;
		case	SHELLSTATE_CLOSING:
			{
				// 关闭核心
				PrintText("Kernel closing...");

				if(m_pWorldThread)
				{
					if(m_pWorldThread->CloseThread(CLOSETHREAD_MILLISECS))
						PrintText("Close world thread OK。");
					else
						PrintText("Close world thread failed!");
					SAFE_DELETE(m_pWorldThread);
					Process();
				}

				PrintText("Server is over, close all  after 3 second!");
				Process();

				m_pMsgPort->Close();
				CMessagePort::ClearPortSet();
				m_pMsgPort = NULL;

				m_nState	= SHELLSTATE_END;
			}
			break;

		case	SHELLSTATE_END:
			{
				KillTimer(1);

				Sleep(3000);

				if(g_nRestart)
					::ShellExecute(m_hWnd, NULL, AfxGetAppName(), NULL, NULL, SW_SHOWNORMAL);

				CDialog::OnOK();
			}
			break;
		default:
			ASSERT(!"switch(m_nState)");
		}

		nLock--;	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	}



	CDialog::OnTimer(nIDEvent);
}

void CMsgServerDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	PrintText("CLOSE key pressed");
	if(AfxMessageBox("Are you sure?", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK)
	{
		m_nState	= SHELLSTATE_CLOSING;
		PrintText("Server closing...");
	}

//	CDialog::OnCancel();
}


bool CMsgServerDlg::Process()
{
	char			buf[MAX_MESSAGESIZE];
	CMessageStatus	cStatus;

	DEBUG_TRY	// VVVVVVVVVVVVVV
	while(m_pMsgPort->Recv(PORT_ANY, PACKET_ANY, STRUCT_TYPE(buf), buf, &cStatus))
	{
		if(ProcessMsg(cStatus.m_nPacket, buf, cStatus.m_nVarType, cStatus.m_nPortFrom))
			return true;

		PrintText("ERROR: CMsgServerDlg::Process() ProcessMsg()");
		return false;
	}
	DEBUG_CATCH("CMsgServerDlg::Process()")	// AAAAAAAAAAA

	return true;
}

void CMsgServerDlg::PrintText(LPCTSTR szMsg)
{
	if(m_nTextLines >= TEXTWINDOW_SIZE)
	{
		int nPos = m_sText.Find("\n", 0);
		if(nPos != -1)
			m_sText = m_sText.Mid(nPos + 1);
	}

	char	buf[20];
	DateTime(buf);
	m_sText += buf+11;
	m_sText += "【";
	m_sText += szMsg;
	m_sText += "】";
	m_sText += "\r\n";
	m_nTextLines++;

	UpdateData(false);

//	LOGMSG("SHELL: %s", szMsg);
}

bool CMsgServerDlg::ProcessMsg(OBJID idPacket, void* pMsg, int nType, int nSource)
{
	switch(idPacket)
	{
	case	SHELL_PRINTTEXT:
		{
			PrintText((char*)pMsg);
		}
		break;
	case SHELL_REMOTE_CLOSE:
		{
			PrintText("Remote close server");
			m_nState	= SHELLSTATE_CLOSING;
			PrintText("Server closing...");
		}
	default:
		LOGERROR("Shell process invalid msg [%d].", idPacket);
		return false;
	}
	return true;
}

BOOL CMsgServerDlg::GetBoolByString(CString str)
{
	if(str == "false")
		return FALSE;

	return TRUE;
}

// AccountServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AccountServer.h"
#include "AccountServerDlg.h"
//#include <windows.h>
#include "allheads.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
	afx_msg void OnEnableLogin();
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
	ON_BN_CLICKED(IDC_ENABLE_LOGIN, OnEnableLogin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccountServerDlg dialog

CAccountServerDlg::CAccountServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAccountServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAccountServerDlg)
	m_sText = _T("");
	m_sState2 = _T("");
	m_sState = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nLoginAcceptP5	= 0;
	m_nPointFeeP5		= 0;
}

void CAccountServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccountServerDlg)
	DDX_Text(pDX, IDC_TEXT, m_sText);
	DDX_Text(pDX, IDC_STATE2, m_sState2);
	DDX_Text(pDX, IDC_STATE, m_sState);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAccountServerDlg, CDialog)
	//{{AFX_MSG_MAP(CAccountServerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDCLOSE, OnCloseButtom)
	ON_WM_TIMER()
	ON_WM_RBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccountServerDlg message handlers

BOOL CAccountServerDlg::OnInitDialog()
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
	
//	m_vasService.m_skVasSocket.sltRecievePacket.Connect( this,&CAccountServerDlg::ProcessPacketTest);
	
	// TODO: Add extra initialization here
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	SetWindowText(SERVERTITLE);
	time( &g_tStartServerTime );
	char	bufStart[20];
	DateTime(bufStart, g_tStartServerTime);

	CreateDirectory("dblog", NULL);
	CreateDirectory("gamelog", NULL);
	CreateDirectory("data", NULL);

	InitLog(SERVERTITLE, LOGFILE, g_tStartServerTime);

	LOGMSG(		"\n\n\n================================================================================\n"
				"=== %s\n"
				"=== Start server time is %s\n"
				"================================================================================\n\n"
				, SERVERTITLE
				, bufStart );
	LOGSERVER(	"\n\n\n================================================================================\n"
				"=== %s\n"
				"=== Start server time is %s\n"
				"================================================================================\n\n"
				, SERVERTITLE
				, bufStart );
	LOGACCOUNT(	"\n\n\n================================================================================\n"
				"=== %s\n"
				"=== Start server time is %s\n"
				"================================================================================\n\n"
				, SERVERTITLE
				, bufStart );
	LOGPOINT(	"\n\n\n================================================================================\n"
				"=== %s\n"
				"=== Start server time is %s\n"
				"================================================================================\n\n"
				, SERVERTITLE
				, bufStart );

	LOGMSG("***************************************************************");
	LOGMSG("** �ʺŷ�������ʼ����");
	LOGMSG("***************************************************************");


	if(!GetConfig())	// װ�������ļ�
	{
		LOGERROR("�޷���ȡINI�ļ��������˳�");
		AfxMessageBox("�޷���ȡINI�ļ�");
		return true;
	}
	LOGMSG("��ȡINI���!");

	// reset title
	if(strlen(SERVER_TITLE))
		SetWindowText(SERVER_TITLE);
/*
	// ��ʼ������
	WSADATA		wsaData;
	int	err;
	if((err = WSAStartup(0x0002, &wsaData)) != 0)
	{
		::MessageBox(NULL, "Init WSAStartup() failed.", "Error",MB_OK|MB_ICONERROR);
		return true;
	}
	// ���汾
	if(wsaData.wVersion != 0x0002)
	{
		WSACleanup();
		::MessageBox(NULL, "WSAStartup Version not match 2.0", "Error",MB_OK|MB_ICONERROR);
		return true;
	}
//*/
	// init db
	if(g_db.Init(DBHOSTNAME, DBUSER, DBPASSWORD, DATABASENAME))
	{
/*
		g_hDbMutex	=::CreateMutex(NULL, false, "DBMutex");
		if (!g_hDbMutex)
		{
			LOGERROR("�޷��������ݿ��ڲ�������󣬳����˳�");
			::MessageBox(NULL, "g_hDbMutex����ʧ�ܡ�", "Error",MB_OK|MB_ICONERROR);
			return true;
		}
//*/
		g_xDatabase	=::CreateMutex(NULL, false, "Database");
		if (!g_xDatabase)
		{
			LOGERROR("�޷��������ݿ⻥����󣬳����˳�");
			::MessageBox(NULL, "g_xDatabase����ʧ�ܡ�", "Error",MB_OK|MB_ICONERROR);
			return true;
		}
	}
	else
	{
		LOGERROR("�޷������ݿ�");
		::MessageBox(NULL, "�޷������ݿ�", "Error",MB_OK|MB_ICONERROR);
		return true;
	}
	LOGMSG("��ʼ���������!");

	g_pOnlineTable = new COnlineTable(ONLINETABLESIZE);
	LOGMSG("OnlineTable ���󴴽��ɹ�!");
	g_pPointThread = new CPointThread(POINTLISTENPORT, POINTSOCKETSNDBUF);
	LOGMSG("PointThread ���󴴽��ɹ�!");
	g_pLoginThread = new CLoginThread(LOGINLISTENPORT);
	LOGMSG("LoginThread ���󴴽��ɹ�!");

	int err2 = 0;
	err2 += !g_pPointThread->CreateThread(false);
	LOGMSG("PointThread �̴߳������!");
	err2 += !g_pLoginThread->CreateThread(false);
	LOGMSG("LoginThread �̴߳������!");

//	GetServerAccount();

	if(!err2)
	{
		err2 += !g_pPointThread->ResumeThread();		// �������Ƶ��߳�
		err2 += !g_pLoginThread->ResumeThread();
		if(err2)
		{
			LOGERROR("ResumeThread()�̳߳��������޷�����");
			AfxMessageBox("�������̳߳���");
			return true;
		}
	}
	else
	{
		LOGERROR("�޷��������̣߳������˳�");
		AfxMessageBox("�޷��������߳�");
		return true;
	}

	SetTimer(1, 1000, NULL);//
	
	LOGMSG("SetTimer �������!");

	LOGMSG("������ϣ���������������-------------------------------------");
	if(g_bEnableLogin)
		PrintText("���������������� . . .");
	else
		PrintText("��ͣ��ҵ�¼ %d ��, �ȴ���Ϸ������ͬ�� . . .", ENABLELOGINDELAY);

	SetWindowText("�������˺ŷ�����");

//	if(m_vasService.StartService())
//		PrintText("������ֵ����ɹ�");
//	else
//		PrintText("��ֵ��������ʧ��!����׽����Ƿ�ռ��.");

//	MSG_VASSupply_INFO vasinfo;
//	ZeroMemory(&vasinfo,sizeof(vasinfo));
//	vasinfo.info.MSG_TYPE=0;
//	vasinfo.info.Supply_VAS_Point=5000;
//	vasinfo.info.Type=0;
//	vasinfo.info.WebServerID=0;
//	strcpy(vasinfo.comboString,"3Ӣ��֮��");
//	vasinfo.info.MSG_SIZE=sizeof(vasinfo.info)+strlen(vasinfo.comboString);


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAccountServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if(nID == SC_CLOSE)
	{
		LOGMSG("��X����������");
		if(AfxMessageBox("Ҫ�رշ�����ô��", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK)
		{
			PostMessage(WM_CLOSE);
			LOGMSG("�ر���Ϣ�Ѿ�����");
		}
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAccountServerDlg::OnPaint() 
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
HCURSOR CAccountServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CAccountServerDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	if(nIDEvent == 1)
	{
		// �������߱�
// 		g_pOnlineTable->RemoveOvertime();

		int		nMovieCount = 6;
//		char	bufLoginChar[]	= " .oOUv .oOUv .oOUv .oOUv .oOUv";
		char	bufPointChar[]	= "-<(|)>-<(|)>-<(|)>-<(|)>-<(|)>";
		char	bufLoginChar[]	= "-<(|)>-<(|)>-<(|)>-<(|)>-<(|)>";

//		char	bufScale[21]	= "....................";
		char	bufScale[21]	= "--------------------";
		char	bufScaleArrow[21]	= "===================>";
		int		nLoginCount	= InterlockedExchange(&s_nLoginCount, 0);
		int		nRealProcLogin = InterlockedExchange(&s_nRealProcLogin, 0);
		int		nCurrPlayers	= g_pOnlineTable->GetPlayerCount();

		// ��������
		static int	st_nMaxPlayers = 0;
		static char	st_szMaxPlayersTime[20] = "0000-00-00 00:00:00";
		if(st_nMaxPlayers < nCurrPlayers)
		{
			st_nMaxPlayers	= nCurrPlayers;
			DateTime(st_szMaxPlayersTime, time(NULL));
		}
/*
		// ÿСʱ��������
		static int	st_nMaxPlayersPerHour = 0;
		time_t	tCurr = time(NULL);
		tm * pTm = localtime(&tCurr);
		if(pTm->tm_min + pTm->tm_sec == 0)
		{
			if(st_nMaxPlayersPerHour)
				PrintText("����������%d", st_nMaxPlayersPerHour);
			st_nMaxPlayersPerHour = 0;
		}
		if(st_nMaxPlayersPerHour < nCurrPlayers)
			st_nMaxPlayersPerHour	= nCurrPlayers;
//*/
		int		nMaxServerTime	= InterlockedExchange(&s_nMaxServerTime, 0);
		int		nAvgServerTime	= -1;
		if(nLoginCount)
			nAvgServerTime = InterlockedExchange(&s_nAvgServerTime, 0) / nLoginCount;
		int		nPointCount = InterlockedExchange(&s_nPointCount, 0);
		int		nLoginScale = 20 - (20*nLoginCount*LOGINLOOPDELAY/1000) % 21;
		int		nPointScale = 20 - (20*nPointCount*POINTLOOPDELAY/1000) % 21;
		int		nSocketCount	= InterlockedExchangeAdd(&s_nSocketCount, 0);
		int		nDatabaseTime	= InterlockedExchange(&s_nDatabaseTimeSum, 0);
		int		nAllTime	= InterlockedExchange(&s_nAllTimeSum, 0);
		//20070804����--------------
		int		nPointThreadProcessTime = InterlockedExchange(&s_nPointThreadPocessTime,0);
		int		nPointThreadProcMsgCount = InterlockedExchange(&s_nPointThreadPocMsgCount,0);
		//--------------------------
		///softworms-���VAS�����ʱ
		int		nPointThreadVASProcessTime = InterlockedExchange(&s_nPointThreadVASPocessTime,0);
		int		nPointThreadLockTime = InterlockedExchange(&s_nPointThreadLockTime,0);
		///softworms-2007-10-06
		if(!nAllTime)
			nAllTime = 20;
		int		nDatabaseScale = 20 - ((nDatabaseTime*20 / nAllTime) % 21);
		if(nSocketCount < 0)
			nSocketCount = 0;
		static int	st_nLoginPos = 0;		// rand() % nMovieCount;
		static int	st_nPointPos = 0;		// rand() % nMovieCount;
		if(nLoginCount)
		{
			st_nLoginPos = (st_nLoginPos + 1) % nMovieCount;
		}
		if(nPointCount)
		{
			st_nPointPos = (st_nPointPos + 1) % nMovieCount;
		}

		ASSERT(MAXGAMESERVERS <= 100);			//??? ÿһ�з�����״̬���֧��20��������
		char	bufServerState[100];
		memset(bufServerState, c_flagNone, 100);
#define	BSS bufServerState
		::GetServerState(bufServerState);

		// 5���Ӹ���һ��
		const int	nMinutes_per_times	= 300;
		const int	nFirstLog			= 0;
		static int	nSaveLog = 0;
		nSaveLog	= (++nSaveLog) % nMinutes_per_times;		// ÿ5���Ӵ���һ��
		bool	bSaveLog = (nSaveLog == nFirstLog);		// ?���ʼ��һ�δ���
		if(bSaveLog)
		{
			m_nLoginAcceptP5 = InterlockedExchange(&s_nLoginAccept, 0);
			m_nPointFeeP5  = InterlockedExchange(&s_nPointFee, 0);
		}
		int	nMinute5Scale = 20 - (((nSaveLog+nMinutes_per_times-nFirstLog)%nMinutes_per_times+1)*20/nMinutes_per_times) % 21;


		
		// �������
		time_t	tCurrTime;
		time( &tCurrTime );
		tm *	pTmFile = localtime(& g_tStartServerTime);
		int	nSec = (int)difftime(tCurrTime, g_tStartServerTime);
		m_sState.Format(//"%s\n\n"
						"AccountServer����ʱ�䣺%04d-%02d-%02d  %02d:%02d\n"
						"AccountServer����ʱ�䣺%d day(s)  %02d:%02d:%02d\n"
						"\n"
							"[%c]�Ʒ�״̬��[%-20s][%d]\n"
							"[%c]��¼״̬��[%-20s][%d]\n"
						"\n"
						"������״̬1��[%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c]\n"		// ״̬��Ϊ20��������
						"������״̬2��[%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c]\n"
						"������״̬3��[%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c]\n"
						"������״̬4��[%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c]\n"
						"������״̬5��[%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c]\n"
						"\n"
							"����������  [%-20s]\n"
							"���ݿ��ʱ��[%-20s]\n"
						// ---------------------------------------------
						//, SERVERTITLE
						, pTmFile->tm_year+1900, pTmFile->tm_mon+1, pTmFile->tm_mday
														, pTmFile->tm_hour, pTmFile->tm_min
						, nSec / 86400, (nSec/3600)%24, (nSec/60)%60, nSec%60
							, bufPointChar[st_nPointPos], bufScale + nPointScale,nPointCount
							, bufLoginChar[st_nLoginPos], bufScale + nLoginScale,nLoginCount
						, ' ',BSS[1],BSS[2],BSS[3],BSS[4],BSS[5],BSS[6],BSS[7],BSS[8],BSS[9]
								, BSS[10],BSS[11],BSS[12],BSS[13],BSS[14],BSS[15],BSS[16],BSS[17],BSS[18],BSS[19]
						, BSS[20],BSS[21],BSS[22],BSS[23],BSS[24],BSS[25],BSS[26],BSS[27],BSS[28],BSS[29]
								, BSS[30],BSS[31],BSS[32],BSS[33],BSS[34],BSS[35],BSS[36],BSS[37],BSS[38],BSS[39]
						, BSS[40],BSS[41],BSS[42],BSS[43],BSS[44],BSS[45],BSS[46],BSS[47],BSS[48],BSS[49]
								, BSS[50],BSS[51],BSS[52],BSS[53],BSS[54],BSS[55],BSS[56],BSS[57],BSS[58],BSS[59]
						, BSS[60],BSS[61],BSS[62],BSS[63],BSS[64],BSS[65],BSS[66],BSS[67],BSS[68],BSS[69]
								, BSS[70],BSS[71],BSS[72],BSS[73],BSS[74],BSS[75],BSS[76],BSS[77],BSS[78],BSS[79]
						, BSS[80],BSS[81],BSS[82],BSS[83],BSS[84],BSS[85],BSS[86],BSS[87],BSS[88],BSS[89]
								, BSS[90],BSS[91],BSS[92],BSS[93],BSS[94],BSS[95],BSS[96],BSS[97],BSS[98],BSS[99]
							, bufScaleArrow + 20 - (nSocketCount*20/MAXCONNECTS)%21			//? ��ֻ��һ�����ʱ��Ӧ��ʾһ��
							, bufScaleArrow + nDatabaseScale
						);
		m_sState2.Format(//"%s\n\n"
						"[%-20s]\n"
						"��ҵ�¼��%3d\n"
						"�Ƶ�������%3d\n"
						"�ۼƼƵ�����%4d\n"
						"\n"
							"�������������%d\n"
							"�������������%d\t%s\n"
							"��¼�߳�������max %dms, avg %dms\n"
							"ʵ�ʴ����½���:%d\n"
							"�Ƶ�,��½���߳�onProcess����ʱ:%d\n"
							//20070804����-------
							"�Ƶ��߳�onProcess����ʱ:%d\n"
							"�Ƶ��߳�procMsg����:%d\n"
							//-------------------
							"�Ƶ��߳�VASprocMsg����ʱ:%d\n"
							"�Ƶ��߳�onProcessSleep����ʱ:%d\n"
						//, SERVERTITLE
						, bufScale + nMinute5Scale
						, m_nLoginAcceptP5
						, m_nPointFeeP5
						, InterlockedExchangeAdd(&s_nPointSum, 0)
							, nCurrPlayers
							, st_nMaxPlayers, st_szMaxPlayersTime
							, nMaxServerTime, nAvgServerTime
							,nRealProcLogin
							,nAllTime
							//20070804����-------
							,nPointThreadProcessTime//�Ƶ��߳�onProcess��ʱ
							,nPointThreadProcMsgCount
							//-------------------
							,nPointThreadVASProcessTime
							,nPointThreadLockTime
						);
		char bufText[4096];
		if(LockedGetText(bufText))
		{
			m_sText = bufText;
		}
		UpdateData(false);

		// 5���Ӵ���һ��
		if(bSaveLog)
		{
			LOGMSG(CString("������״̬��\n") + m_sState + "\n" + m_sState2 + "\n" + bufText);
		}

		static int	nOnlineCount = 0;
		if(++nOnlineCount >= OUTPUTONLINESECS)
		{
			nOnlineCount = 0;

			// ͬʱ����������ͳ����
			FILE *	fOnline = fopen(ONLINEFILE, "w");
			if(fOnline)
			{
				char	bufTime[20];
				DateTime(bufTime, time(NULL));
				fprintf(fOnline, SERVERTITLE "\n");
				fprintf(fOnline, "%s\n", bufTime);
				int nAmount = g_pOnlineTable->GetPlayerCount();
				for(int i = 1; i < g_nServerAccount; i++)	// 1: ��1��ʼ��
				{
					LPCTSTR		szServerName = g_aServerAccount[i].m_szServerName;
					int			nCount = g_pOnlineTable->GetPlayerCount(szServerName);
					int			nState = g_pPointThread->GetServerState(szServerName);
					const char*		pState = "";
					if(nState == CPointThread::STATE_OFFLINE)
					{
						nAmount -= nCount;
						nCount = 0;
						pState = "(�ѶϿ�)";
					}
					else if(nState == CPointThread::STATE_BUSY)
						pState = "(æ)";
					else if(nState == CPointThread::STATE_FULL)
						pState = "(����)";
//					fprintf(fOnline, "%2d(%2d). %-8s\t��%4d %s\n", 
					fprintf(fOnline, "%2d. %-4s\t��%4d %s\n", 
								i, 
//								g_pPointThread->GetServerIndex(szServerName), 
								szServerName, 
								nCount, 
								pState
							);
				}
				fprintf(fOnline, "������������%6d\n", nAmount);
				fprintf(fOnline, ".\n");
				fprintf(fOnline, "������״̬��\n%s\n��ʱͳ�ƣ�\n%s\n�����Ϣ��\n%s\n", m_sState, m_sState2, bufText);
				fclose(fOnline);
			}
		}

		// ������������
//--------------------07.5.26------����--------------------------
//��������OnTimer����Ĵ��������Ͻ�
		static DWORD dwStartTime = ::GetTickCount();
		if( ( ::GetTickCount() - dwStartTime ) / 1000 >= HEARTBEATINTERVALSECS )
		{
			g_pPointThread->CheckHeartbeatAll();
			dwStartTime = ::GetTickCount();
		}
//		static int	nHeartbeatCount = 0;
//		//if(++nHeartbeatCount >= HEARTBEATINTERVALSECS / 5)		// 5���ӿ���Ƶ�ʣ��Ա�֤��ʱ�����ߡ�
//		if(++nHeartbeatCount >= HEARTBEATINTERVALSECS)				//300����һ�Σ�1500�볬ʱ
//		{
//			g_pPointThread->CheckHeartbeatAll();
//			nHeartbeatCount = 0;
//		}
//-------------------------------------------------------------------		
		// ����¼�ӳ�ʱ��
		static bool	bLogOK = false;
		if(!g_bEnableLogin && (clock()/CLOCKS_PER_SEC) >= ENABLELOGINDELAY)
		{
			g_bEnableLogin = true;
		}
		if(!bLogOK && g_bEnableLogin)
		{
			bLogOK = true;
			LOGMSG("�ʺŷ�������ʼ������ҵ�¼");
			PrintText("������ҵ�¼");
		}

		// дCOUNT.TXT
		static int	nCountCount = 0;
		if(++nCountCount >= COUNTFILESECS)
		{
			try{
				nCountCount = 0;
				g_pPointThread->LogCount();

				if(STAT_SERIAL)
					g_pPointThread->LogSerialCount();
			}catch(...){ LOGCATCH("дCOUNT.TXT"); }
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CAboutDlg::OnEnableLogin() 
{
	// TODO: Add your control notification handler code here
	g_bEnableLogin = true;

	CDialog::OnOK();
}

void CAccountServerDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	LOGMSG("��ESC����������");
	if(AfxMessageBox("Ҫ�رշ�����ô��", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK)
	{
		PostMessage(WM_CLOSE);
		LOGMSG("�ر���Ϣ�Ѿ�����");
	}
	return;
	
//	CDialog::OnCancel();
}

void CAccountServerDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	static bool	bFlag = false;
	if(!bFlag)	// ����һ��
	{
		g_bEnableLogin = false;		// ֹͣ��¼
		bFlag = true;
		LOGMSG("��������ʼ�˳�------------------------------------------------");
		PrintText("�������˳��� . . .");

		KillTimer(1);
	}

	if(g_pLoginThread->CloseThread(200))		// �ȹر�
	{
		if(g_pPointThread->CloseThread(200))
		{
			// �ͷ�ȫ�ֱ���
			delete	g_pLoginThread;
			delete	g_pPointThread;
			delete	g_pOnlineTable;

			// �ر����ݿ�
			g_db.Destroy();
/*
			g_db.Close();
			if (g_hDbMutex)
			{
				::CloseHandle(g_hDbMutex);
				g_hDbMutex	=NULL;
			}
//*/
			if (g_xDatabase)
			{
				::CloseHandle(g_xDatabase);
				g_xDatabase	=NULL;
			}
			LOGMSG("***************************************************************");
			LOGMSG("**  �����������ر�");
			LOGMSG("***************************************************************\n\n\n");

			CDialog::OnClose();
			CDialog::OnOK();	//paled add
			return;
		}
	}

	Sleep(500);
	PostMessage(WM_CLOSE);
}

void CAccountServerDlg::OnCloseButtom() 
{
	// TODO: Add your control notification handler code here
	LOGMSG("���رա���������");
	if(AfxMessageBox("Ҫ�رշ�����ô��", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK)
	{
		PostMessage(WM_CLOSE);
		LOGMSG("�ر���Ϣ�Ѿ�����");
	}
}

//void CAccountServerDlg::ProcessPacketTest(const ParamList& paramlist)
//{
//	int ttt = 0;
//}

void CAccountServerDlg::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	char *szServerInfo=(char*)nFlags;
	SetWindowText(szServerInfo);	
	CDialog::OnRButtonDblClk(nFlags, point);
}

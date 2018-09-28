// DlgConfig.cpp : implementation file
//

#include "stdafx.h"
#include "../common/BaseFunc.h"
#include "msgserver.h"
#include "DlgConfig.h"
#include "IniFile.h"
#include "StrRes.h"

#include <direct.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgConfig dialog


CDlgConfig::CDlgConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgConfig::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgConfig)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgConfig)
	DDX_Control(pDX, IDC_CHECK_PARAM06, m_chk_param06);
	DDX_Control(pDX, IDC_CHECK_PARAM05, m_chk_param05);
	DDX_Control(pDX, IDC_CHECK_PARAM04, m_chk_param04);
	DDX_Control(pDX, IDC_CHECK_PARAM03, m_chk_param03);
	DDX_Control(pDX, IDC_CHECK_PARAM02, m_chk_param02);
	DDX_Control(pDX, IDC_CHECK_PARAM01, m_chk_param01);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgConfig, CDialog)
	//{{AFX_MSG_MAP(CDlgConfig)
	ON_BN_CLICKED(IDC_CHECK_PARAM01, OnCheckParam01)
	ON_BN_CLICKED(IDC_CHECK_PARAM02, OnCheckParam02)
	ON_BN_CLICKED(IDC_CHECK_PARAM03, OnCheckParam03)
	ON_BN_CLICKED(IDC_CHECK_PARAM04, OnCheckParam04)
	ON_BN_CLICKED(IDC_CHECK_PARAM05, OnCheckParam05)
	ON_BN_CLICKED(IDC_CHECK_PARAM06, OnCheckParam06)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgConfig message handlers

BOOL CDlgConfig::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetDlgItemText(IDC_CHECK_PARAM01, g_objStrRes.GetStr(18001));	//	屏蔽装备打造
	SetDlgItemText(IDC_CHECK_PARAM02, g_objStrRes.GetStr(18002));	//	屏蔽排行榜
	SetDlgItemText(IDC_CHECK_PARAM03, g_objStrRes.GetStr(18003));	//	屏蔽军团招聘与开除功能
	SetDlgItemText(IDC_CHECK_PARAM04, g_objStrRes.GetStr(18004));	//	Item LifeTime Save
	SetDlgItemText(IDC_CHECK_PARAM05, g_objStrRes.GetStr(18005));	//	Savelog
	SetDlgItemText(IDC_CHECK_PARAM06, g_objStrRes.GetStr(18006));	//	Savelog

	m_chk_param01.SetCheck(g_bEnableEquipCreate);
	m_chk_param02.SetCheck(g_bEnableTopListRefresh);
	m_chk_param03.SetCheck(g_bEnableSynModificate);
	m_chk_param04.SetCheck(g_bItemLifeTimeSaveDB);
	m_chk_param05.SetCheck(g_bLogSave);
	m_chk_param06.SetCheck(g_bEnableDatabaseReconnect);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgConfig::OnOK() 
{
	char szPath[MAX_PATH + 1];
//	GetModuleFileName(NULL, szPath, MAX_PATH);
	_getcwd(szPath, MAX_PATH);
	CString str = CString(szPath);
	str += "\\";
	str += CONFIG_FILENAME;

	if(!WritePrivateProfileString("AdditionalConfig", "ENABLE_EQUIPCREATE", GetStringByBool(g_bEnableEquipCreate), str))
	{
		MessageBox(g_objStrRes.GetStr(19030));
		return;
	}
	if(!WritePrivateProfileString("AdditionalConfig", "ENABLE_TOPLIST_REFRESH", GetStringByBool(g_bEnableTopListRefresh), str))
	{
		MessageBox(g_objStrRes.GetStr(19030));
		return;
	}
	if(!WritePrivateProfileString("AdditionalConfig", "ENABLE_SYN_MODIFICATION", GetStringByBool(g_bEnableSynModificate), str))
	{
		MessageBox(g_objStrRes.GetStr(19030));
		return;
	}
	if(!WritePrivateProfileString("AdditionalConfig", "ENABLE_ITEM_LIFETIME_SAVE", GetStringByBool(g_bItemLifeTimeSaveDB), str))
	{
		MessageBox(g_objStrRes.GetStr(19030));
		return;
	}
	if(!WritePrivateProfileString("AdditionalConfig", "ENABLE_LOG", GetStringByBool(g_bLogSave), str))
	{
		MessageBox(g_objStrRes.GetStr(19030));
		return;
	}
	if(!WritePrivateProfileString("AdditionalConfig", "ENABLE_DATABASE_RECONNECT", GetStringByBool(g_bEnableDatabaseReconnect), str))
	{
		MessageBox(g_objStrRes.GetStr(19030));
		return;
	}

	CDialog::OnOK();
}

void CDlgConfig::OnCheckParam01() 
{
	g_bEnableEquipCreate = m_chk_param01.GetCheck();	
}

void CDlgConfig::OnCheckParam02() 
{
	g_bEnableTopListRefresh = m_chk_param02.GetCheck();	
}

void CDlgConfig::OnCheckParam03() 
{
	g_bEnableSynModificate = m_chk_param03.GetCheck();		
}

void CDlgConfig::OnCheckParam04() 
{
	g_bItemLifeTimeSaveDB = m_chk_param04.GetCheck();		
}

void CDlgConfig::OnCheckParam05() 
{
	g_bLogSave = m_chk_param05.GetCheck();	
}

void CDlgConfig::OnCheckParam06() 
{
	g_bEnableDatabaseReconnect = m_chk_param06.GetCheck();
}

CString CDlgConfig::GetStringByBool(BOOL bFlag)
{
	if(bFlag)
		return "true";

	return "false";
}

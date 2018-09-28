// MsgNewCard.cpp: implementation of the CMsgNewCard class.
//
//////////////////////////////////////////////////////////////////////
#include "mapgroup.h"
#include "AllMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgNewCard::CMsgNewCard()
{
	Init();
	m_pInfo	=(MSG_Info* )m_bufMsg;
}

CMsgNewCard::~CMsgNewCard()
{

}

//////////////////////////////////////////////////////////////////////
BOOL CMsgNewCard::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;

	if(_MSG_NEWCARD != this->GetType())
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgNewCard::Create(int nType,OBJID idUser,OBJID idAccount, const char* pszCard)
{
	if (nType == NEWCARDACT_NONE || !pszCard)
		return false;

	this->Init();

	m_pInfo->dwType	= nType;
	m_pInfo->idUser = idUser;
	m_pInfo->idAccount = idAccount;
	strcpy(m_pInfo->szBuf,pszCard);

	m_unMsgType	=_MSG_NEWCARD;
	m_unMsgSize	=sizeof(MSG_Info);

	return true;
}

void CMsgNewCard::Process(void *pInfo)
{
	CUserPtr pUser = UserManager()->GetUser(this);
	if(!pUser)
		return;

	switch(m_pInfo->dwType)
	{
	case NEWCARDACT_PICK:
		{
			pUser->PickupNewCard(m_pInfo->szBuf);
		}
		break;
	case NEWCARDACT_PICKSUC:
		{
			DEBUG_TRY
			if(pUser->ChkNewGiveMask(35))
				return;

			if(pUser->ProcessAction(2035))
			{
				pUser->AddNewGiveMask(35);
				pUser->SendSysMsg("��ȡ�ɹ�");
			}
			else
			{
				pUser->SetPickupNewCard(m_pInfo->szBuf);
				pUser->SendSysMsg("��ȡʧ��");
			}
			DEBUG_CATCH("pUser->ProcessAction(2035)");
			
		}
		break;
	case NEWCARDACT_PICKFAIL:
		{
			pUser->SendSysMsg("��Ч�Ŀ���");
		}
		break;
	case NEWCARDACT2_PICK:
		{
			pUser->PickupNewCard2(m_pInfo->szBuf);
		}
		break;
	case NEWCARDACT2_PICKSUC:
		{
			DEBUG_TRY
			if(pUser->ChkNewGiveMask(35))
				return;
				
			if(pUser->ProcessAction(2035))
			{
				pUser->AddNewGiveMask(35);
				pUser->SendSysMsg("��ȡ�ɹ�");
			}
			else
			{
				pUser->SetPickupNewCard2(m_pInfo->szBuf);
				pUser->SendSysMsg("��ȡʧ��");
			}
			DEBUG_CATCH("pUser->ProcessAction(2035)");
				
		}
		break;
	case NEWCARDACT2_PICKFAIL:
		{
			pUser->SendSysMsg("��Ч�Ŀ���");
		}
		break;
	case VOTCARD_PICK:
		{
			DEBUG_TRY
 			pUser->PickupVotCard(m_pInfo->szBuf);
			DEBUG_CATCH2("VOTCARD_PICK!! CardID = %s", m_pInfo->szBuf)
		}
		break;
	case VOTCARD_PICKSUC:
		{
			DEBUG_TRY
			if (pUser->GetVotingGift())
			{
				pUser->AddNewGiveMask(36);
				pUser->SendSysMsg("��ȡ�ɹ���");
			}
			else
			{
				pUser->SetPickupNewCard(m_pInfo->szBuf, 1);
				pUser->SendSysMsg("��ȡʧ�ܣ�");
			}
			DEBUG_CATCH2("VOTCARD_PICKSUC!! CardID = %s", m_pInfo->szBuf)
		}
		break;
	case VOTCARD_PICKFAIL:
		{
			pUser->SendSysMsg("��Ч�Ŀ���");
		}
		break;
	case VOTCARD2_PICK:
		{
			pUser->PickupVotCard2(m_pInfo->szBuf);
		}
		break;
	case VOTCARD2_PICKSUC:
		{
			if (pUser->GetVotingGift2(m_pInfo->szBuf))
			{
				pUser->SendSysMsg("��ȡ�ɹ���");
			}
			else
			{
				pUser->SetPickupNewCard(m_pInfo->szBuf, 2);
				pUser->SendSysMsg("��ȡʧ�ܣ�");
			}
		}
		break;
	case VOTCARD2_PICKFAIL:
		{
			pUser->SendSysMsg("��Ч�Ŀ���");
		}
		break;
	case VOTCARD3_PICK:
		{
			pUser->PickupVotCard3(m_pInfo->szBuf);
		}
		break;
	case VOTCARD3_PICKSUC:
		{
			if (pUser->GetVotingGift3(m_pInfo->szBuf))
			{
				pUser->SendSysMsg("��ȡ�ɹ���");
			}
			else
			{
				pUser->SetPickupNewCard(m_pInfo->szBuf, 3);
				pUser->SendSysMsg("��ȡʧ�ܣ�");
			}
		}
		break;
	case VOTCARD3_PICKFAIL:
		{
			pUser->SendSysMsg("��Ч�Ŀ���");
		}
		break;
	
	case NEWCARDACT3_PICK:
		{
			pUser->PickupNewCard3(m_pInfo->szBuf);
		}

	}
}

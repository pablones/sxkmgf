// MsgDialog.h: interface for the CMsgDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef MSGDIALOG_H
#define MSGDIALOG_H

#include "NetMsg.h"
#include "NetStringPacker.h"

WORLDKERNEL_BEGIN
enum {
		MSGDIALOG_TEXT		= 101,
		MSGDIALOG_LINK,
		MSGDIALOG_SHOW,
		MSGDIALOG_ANSWER	= 201,
};

class CMsgDialog : public CNetMsg  
{
public:
	CMsgDialog();
	virtual ~CMsgDialog();
	
	BOOL	Create	(int nAction, LPCTSTR szText, int idxTask);
public:
	BOOL			Create		(char* pbufMsg, DWORD dwMsgSize);
	void			Process		(void *pInfo);

private:
	typedef struct {
		MSGHEAD_DEFINE

		union{
			struct{
				USHORT	usPosX;
				USHORT	usPosY;
			};
			DWORD	idTask;			// panel task id, to server only
		};
		USHORT	usData;
		UCHAR	idxTask;
		UCHAR	ucAction;
		char	szBuf[1];
		// szText
	}MSG_Info;

	MSG_Info*	m_pInfo;

private:
	CNetStringPacker	m_StrPacker;

};
WORLDKERNEL_END

#endif 

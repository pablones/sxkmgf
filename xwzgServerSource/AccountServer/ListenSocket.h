// �����̲߳���
// �ɽ���, 2001.10.10
// �������û�а���WSASTARTUP()��WSACLEANUP()

#ifndef LISTENTHREAD_H
#define LISTENTHREAD_H


//	1��CListenSocket���������ڼ����߳��д������������
// ������ظ�ʹ�á������� Close() �رպ󣬿����µ��� Open() �½������˿ڡ�����ϵͳ��ûʹ�øù��ܡ�
class	CListenSocket
{
public:
	//	��Ҫ������
	//	ÿ������ֻ��BIND��һ���˿ڡ�
	CListenSocket(u_short nPort, int nSndSize = 0);
	//	�����ͷ�SOCKET
	~CListenSocket();
	//	��������˼���SOCKET������ѡ���뿴���롣ʧ��ʱ����false��
	bool	Open();		// ���ظ�����
	//	����ʱ����INVALID_SOCKET
	SOCKET	Accept(u_long & nClientIP);
	//	���ظ����á�ͨ�����ã������������ر�SOCKET��
	//	�رպ�����µ���Open()�ظ�ʹ�øö���
	void	Close();		// ���ظ�����
	bool	IsOpen() { return m_bState; }
	bool	Rebuild();
public:
	SOCKET	Socket() { return m_sockListen; }
protected:
	//	��Ҫ��Ա������
	const u_short	m_nPort;
	const int		m_nSndBuf;
	SOCKET	m_sockListen;
	//	����״̬�Ƿ������ı�־��
	//	����ֵΪfalseʱ��ʾ�ö��󲻿��á�����δOpen()��Ҳ�����������
	bool	m_bState;
	time_t	m_tCloseTime;		// �ϴιر�SOCKET��ʱ�䣬�����Զ���ʱ�ؽ�SOCKET
};



#endif	// LISTENTHREAD_H
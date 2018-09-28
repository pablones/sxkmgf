// �̻߳���
#ifndef THREADBASE_H
#define THREADBASE_H

#include <windows.h>


class CThreadBase
{
public:		// ���졢����
	//constructors
	CThreadBase();

	//destructor
	virtual ~CThreadBase();

public:		// �������ɸ��̵߳��á�
	bool	CreateThread(bool bRun = true);		// false: �ݲ����У��� ResumeThread() ����
	bool	ResumeThread();					// return false: ʧ��

	// ֪ͨ���̹߳رգ������� nMilliseconds ���롣����true: �̹߳رճɹ�
	bool	CloseThread(long nMilliseconds = 0);
/////////////////////////////////////////////////////////////////////
protected:	// ������
	//overrideable
	virtual	void	OnInit() { }
	virtual bool	OnProcess() = 0;
	virtual void	OnDestroy() { }

/////////////////////////////////////////////////////////////////////
private:	// �ڲ�ʹ��
	bool	IsCloseEvent(long nMilliseconds = 0);		// ���ر��߳��¼��Ƿ񴥷������� ThreadProc() ��������
	DWORD	ThreadProc();
	bool	IsCreated() { return (m_hThread != NULL); }

protected:
	HANDLE		m_hThread;
	HANDLE		m_hCloseThreadEvent;
	
/////////////////////////////////////////////////////////////////////
private:
	static DWORD WINAPI TrueThreadProc(LPVOID pParam);
};
#endif // THREADBASE_H
// 线程基类
#ifndef THREADBASE_H
#define THREADBASE_H

#include <windows.h>


class CThreadBase
{
public:		// 构造、析构
	//constructors
	CThreadBase();

	//destructor
	virtual ~CThreadBase();

public:		// 公共，由父线程调用。
	bool	CreateThread(bool bRun = true);		// false: 暂不运行，用 ResumeThread() 运行
	bool	ResumeThread();					// return false: 失败

	// 通知子线程关闭，并阻塞 nMilliseconds 毫秒。返回true: 线程关闭成功
	bool	CloseThread(long nMilliseconds = 0);
/////////////////////////////////////////////////////////////////////
protected:	// 派生用
	//overrideable
	virtual	void	OnInit() { }
	virtual bool	OnProcess() = 0;
	virtual void	OnDestroy() { }

/////////////////////////////////////////////////////////////////////
private:	// 内部使用
	bool	IsCloseEvent(long nMilliseconds = 0);		// 检查关闭线程事件是否触发。用于 ThreadProc() 函数调用
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
// IOCPBuffer.h: interface for the CIOCPBuffer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IOCPBUFFER_H__87DEF694_32D6_4F21_8FF5_E16184A9CDC3__INCLUDED_)
#define AFX_IOCPBUFFER_H__87DEF694_32D6_4F21_8FF5_E16184A9CDC3__INCLUDED_

//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000


/*
 * This Class is used to pass around the buffer and the operation that 
 * is done asynchronously. 
 *
 */
//#include <afx.h>
//#include "common.h"
#include "IOCPS.h"
#include <malloc.h>
#include <new.h>
#include <stdlib.h>

//#include "nedmalloc.h"

//using namespace nedalloc;

class CIOCPBuffer   
{
public:
//	void *  operator new(size_t nSize)
//	{
//		return nedmalloc(nSize);
//	}
//	void operator delete(void* p)
//	{
//		nedfree(p);
//	}
//
//	void* operator new[](size_t nSize)
//	{
//		return nedmalloc(nSize);
//	}
//
//	void operator delete[](void* p)
//	{
//		nedfree(p);
//	}
	
	void Init();	
	
	OVERLAPPED			m_ol;
	BOOL Flush(UINT nBytesToRemove);
	 
	WSABUF * GetWSABuffer();
	
//	void SetSequenceNumber(int nr);
//	
//	int GetSequenceNumber();
	
	void SetupWrite();
	void SetupRead();
	void SetupZeroByteRead();
	int GetOperation();
	void SetOperation(int op);
	void EmptyUsed();
	UINT Use(UINT nSize);
	UINT GetUsed();
	//UINT GetSize();
	BOOL IsValid();
	// DUMP the buffer. (For Debug)
	void DUMP();
	// Write different types of variabels to the buffer
	BOOL AddData(const BYTE * pData,UINT nSize);
	BOOL AddData(const char * pData,UINT nSize);
	BOOL AddData(BYTE data);
	BOOL AddData(UINT data);
	BOOL AddData(unsigned short data);
	
	
	short GetPackageSize();
	// Gets The package Type returns 255 if error. 
	short GetPackageType();
	
	PBYTE GetBuffer();
	CIOCPBuffer();
	/*virtual*/ ~CIOCPBuffer();
	
public:
	//int m_iSequenceNumber;
	// The Type of Operation. 
    int		m_Operation;
	// Size of the Buffer 
	//UINT	m_nSize;
	// number of bytes that are Used. 
	UINT    m_nUsed;
	// The buffer. 
	BYTE m_Buffer[MAXIMUMPACKAGESIZE];
	//Holds the Buffer Used for send/receive
	WSABUF m_wsabuf;
	SOCKET m_Accept;
	// Used to remove the buffer from the queue. 
	//POSITION m_pPos;
};

#endif // !defined(AFX_IOCPBUFFER_H__87DEF694_32D6_4F21_8FF5_E16184A9CDC3__INCLUDED_)

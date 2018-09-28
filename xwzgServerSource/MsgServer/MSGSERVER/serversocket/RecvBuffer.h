#ifndef _RECVBUFFER_H_
#define _RECVBUFFER_H_

#include <windows.h>
#include "CircuitQueue.h"
#include "NetBase.h"

//=============================================================================================================================
/// 클라이언트로부터 받은 패킷을 쌓아둘 버퍼
//		recv 호출 후 GQCS에서 completion이 들어오면 Completion(int)를 호출
//=============================================================================================================================

class RecvBuffer
{
public:
	RecvBuffer()			{ m_pQueue = NULL; }
	virtual ~RecvBuffer()	{ if( m_pQueue ) delete m_pQueue; }

	inline VOID	Create( int nBufferSize, DWORD dwExtraBufferSize ) {
		if( m_pQueue ) delete m_pQueue;
		m_pQueue = new CircuitQueue<BYTE>;
		m_pQueue->Create( nBufferSize, dwExtraBufferSize ); }

	inline VOID	Completion( int nBytesRecvd ) { m_pQueue->Enqueue( NULL, nBytesRecvd ); }

	inline VOID	Clear() { m_pQueue->Clear(); }

	inline VOID	GetRecvParam( BYTE **ppRecvPtr, int &nLength ) {
		*ppRecvPtr	= m_pQueue->GetWritePtr();		
		nLength		= m_pQueue->GetWritableLen(); }

	inline BYTE* GetWritePtr(){
		return m_pQueue->GetWritePtr();
	}

	inline BYTE* GetFirstPacketPtr()
	{
		//PACKET_HEADER   header;
		// 버퍼의 데이터 길이가 헤더 길이보다 작으면 NULL 리턴
		//if( !m_pQueue->Peek( (BYTE*)&header, sizeof(PACKET_HEADER) ) )				return NULL;
		// 버퍼의 데이터 길이가 패킷 길이보다 작으면 NULL 리턴(패킷 완성 안됨)
		//if( m_pQueue->GetLength() < (int)( sizeof(PACKET_HEADER) + header.size ) )	return NULL;
		// 패킷이 버퍼 뒤쪽에서 앞으로 나누어질 때 잘린 뒷부분을 여분의 공간에 카피한다.
		//if( m_pQueue->GetBackDataCount() < (int)( header.size + sizeof(header) ) ) {
		//	m_pQueue->CopyHeadDataToExtraBuffer( header.size + sizeof(header) - m_pQueue->GetBackDataCount() ); }
		short header;

		if(!m_pQueue->Peek((BYTE*)&header,sizeof(header)))
			return NULL;
		if(m_pQueue->GetLength()<header)
			return NULL;
		if(m_pQueue->GetBackDataCount()<header)
			m_pQueue->CopyHeadDataToExtraBuffer(header-m_pQueue->GetBackDataCount());

//		if(m_pQueue->GetLength()<PACKETHEADER)
//			return NULL;

		return  m_pQueue->GetReadPtr();	
	}

	inline int GetLength(){return m_pQueue->GetLength();}

	inline VOID	RemoveFirstPacket( WORD wSize ) { m_pQueue->Dequeue( NULL, wSize ); }

	inline DWORD GetSpace() { return m_pQueue->GetSpace();}

private:
	CircuitQueue<BYTE>	*m_pQueue;
};

#endif

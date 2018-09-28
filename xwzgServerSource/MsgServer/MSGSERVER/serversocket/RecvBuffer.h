#ifndef _RECVBUFFER_H_
#define _RECVBUFFER_H_

#include <windows.h>
#include "CircuitQueue.h"
#include "NetBase.h"

//=============================================================================================================================
/// Ŭ���̾�Ʈ�κ��� ���� ��Ŷ�� �׾Ƶ� ����
//		recv ȣ�� �� GQCS���� completion�� ������ Completion(int)�� ȣ��
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
		// ������ ������ ���̰� ��� ���̺��� ������ NULL ����
		//if( !m_pQueue->Peek( (BYTE*)&header, sizeof(PACKET_HEADER) ) )				return NULL;
		// ������ ������ ���̰� ��Ŷ ���̺��� ������ NULL ����(��Ŷ �ϼ� �ȵ�)
		//if( m_pQueue->GetLength() < (int)( sizeof(PACKET_HEADER) + header.size ) )	return NULL;
		// ��Ŷ�� ���� ���ʿ��� ������ �������� �� �߸� �޺κ��� ������ ������ ī���Ѵ�.
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

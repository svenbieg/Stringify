//=============
// TlsSocket.h
//=============

#pragma once


//=======
// Using
//=======

#include <security.h>
#include "Network/Tcp/TcpSocket.h"


//===========
// Namespace
//===========

namespace Network {
	namespace Tls {


//============
// TLS-Socket
//============

class TlsSocket: public Storage::Streams::RandomAccessStream
{
public:
	// Using
	using TcpSocket=Network::Tcp::TcpSocket;

	// Con-/Destructors
	TlsSocket(Handle<TcpSocket> Socket);
	~TlsSocket();

	// Common
	BOOL Accept(Handle<String> HostName);
	VOID Close();
	BOOL Handshake(Handle<String> HostName);
	Handle<TcpSocket> Socket;

	// Input-Stream
	SIZE_T Available()override;
	SIZE_T Read(VOID* Buffer, SIZE_T Size)override;

	// Output-Stream
	VOID Flush()override;
	SIZE_T Write(VOID const* Buffer, SIZE_T Size)override;

private:
	// Settings
	static constexpr UINT TLS_BUF_SIZE=32768;

	// Common
	VOID CloseInternal();
	SecPkgContext_StreamSizes cStreamSizes;
	CredHandle hCredential;
	CtxtHandle hSecurityContext;
	CHAR m_Buffer[TLS_BUF_SIZE];
	PCCERT_CONTEXT pContext;
	CHAR pInput[TLS_BUF_SIZE];
	CHAR pOutput[TLS_BUF_SIZE];
	UINT uBufferSize;
	UINT uInputSize;
	UINT uMissingSize;
	UINT uOutputSize;
};

}}
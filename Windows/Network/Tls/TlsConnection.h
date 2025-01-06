//=================
// TlsConnection.h
//=================

#pragma once


//=======
// Using
//=======

#include <security.h>
#include <WinSock2.h>
#include "Network/Ip/IpAddress.h"
#include "Storage/Streams/RandomAccessStream.h"


//===========
// Namespace
//===========

namespace Network {
	namespace Tls {


//======================
// Forward-Declarations
//======================

class TlsSocket;


//================
// TLS-Connection
//================

class TlsConnection: public Storage::Streams::RandomAccessStream
{
public:
	// Friends
	friend TlsSocket;

	// Using
	using IP_ADDR=Network::Ip::IP_ADDR;

	// Con-/Destructors
	~TlsConnection() { Close(); }

	// Common
	VOID Close();
	VOID Handshake(Handle<String> HostName);

	// Input-Stream
	SIZE_T Available()override;
	SIZE_T Read(VOID* Buffer, SIZE_T Size)override;

	// Output-Stream
	VOID Flush()override;
	SIZE_T Write(VOID const* Buffer, SIZE_T Size)override;

private:
	// Settings
	static constexpr UINT TLS_BUF_SIZE=32768;

	// Con-/Destructors
	TlsConnection(SOCKET Socket, IP_ADDR Address);

	// Common
	VOID ShutdownTls();
	CHAR m_Buffer[TLS_BUF_SIZE];
	UINT m_BufferSize;
	PCCERT_CONTEXT m_Context;
	CredHandle m_Credential;
	CHAR m_Input[TLS_BUF_SIZE];
	UINT m_InputSize;
	UINT m_MissingSize;
	CHAR m_Output[TLS_BUF_SIZE];
	UINT m_OutputSize;
	IP_ADDR m_RemoteAddress;
	CtxtHandle m_SecurityContext;
	SOCKET m_Socket;
	SecPkgContext_StreamSizes m_StreamSizes;
};

}}
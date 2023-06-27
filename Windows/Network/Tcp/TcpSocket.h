//=============
// TcpSocket.h
//=============

#pragma once


//=======
// Using
//=======

#include <WinSock2.h>
#include "Storage/Streams/RandomAccessStream.h"


//===========
// Namespace
//===========

namespace Network {
	namespace Tcp {


//============
// TCP-Socket
//============

class TcpSocket: public Storage::Streams::RandomAccessStream
{
public:
	// Con-/Destructors
	TcpSocket(SOCKET Socket=INVALID_SOCKET, UINT RemoteIp=0);
	~TcpSocket();

	// Common
	Handle<TcpSocket> Accept();
	VOID Close();
	BOOL Connect(Handle<String> HostName, WORD Port);
	VOID Listen(WORD Port);
	UINT RemoteIp;

	// Input-Stream
	SIZE_T Available()override;
	SIZE_T Read(VOID* Buffer, SIZE_T Size)override;

	// Output-Stream
	VOID Flush()override;
	SIZE_T Write(VOID const* Buffer, SIZE_T Size)override;

protected:
	// Common
	SOCKET uSocket;
};

}}
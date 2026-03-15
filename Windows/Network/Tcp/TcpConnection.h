//=================
// TcpConnection.h
//=================

#pragma once


//=======
// Using
//=======

#include <WinSock2.h>
#include "Network/Ip/IpAddress.h"
#include "Storage/Streams/RandomAccessStream.h"


//===========
// Namespace
//===========

namespace Network {
	namespace Tcp {


//======================
// Forward-Declarations
//======================

class TcpSocket;


//================
// TCP-Connection
//================

class TcpConnection: public Storage::Streams::RandomAccessStream
{
public:
	// Friends
	friend TcpSocket;

	// Using
	using IP_ADDR=Network::Ip::IP_ADDR;

	// Con-/Destructors
	~TcpConnection() { Close(); }

	// Common
	VOID Close();

	// Input-Stream
	SIZE_T Available()override;
	SIZE_T Read(VOID* Buffer, SIZE_T Size)override;

	// Output-Stream
	VOID Flush()override;
	SIZE_T Write(VOID const* Buffer, SIZE_T Size)override;

private:
	// Con-/Destructors
	TcpConnection(SOCKET Socket=INVALID_SOCKET, IP_ADDR RemoteIp=0);

	// Common
	IP_ADDR m_RemoteAddress;
	SOCKET m_Socket;
};

}}
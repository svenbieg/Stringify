//===============
// UdpSocket.cpp
//===============

#include "UdpSocket.h"


//=======
// Using
//=======

using namespace Network::Ip;


//===========
// Namespace
//===========

namespace Network {
	namespace Udp {


//========
// Common
//========

VOID UdpSocket::Broadcast(WORD port, Handle<UdpMessage> msg)
{
Send(IP_BROADCAST, port, msg);
}

VOID UdpSocket::Close()
{
}

Handle<UdpMessage> UdpSocket::Receive(WORD port)
{
return nullptr;
}

VOID UdpSocket::Send(IP_ADDR to, WORD port, Handle<UdpMessage> msg)
{
}


//==========================
// Con-/Destructors Private
//==========================

UdpSocket::UdpSocket()
{}

}}
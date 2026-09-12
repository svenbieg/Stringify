//================
// UdpMessage.cpp
//================

#include "UdpMessage.h"


//===========
// Namespace
//===========

namespace Network {
	namespace Udp {


//==================
// Con-/Destructors
//==================

Handle<UdpMessage> UdpMessage::Create(IP_ADDR ip)
{
return Object::Create<UdpMessage>(ip);
}


//========
// Common
//========

BYTE* UdpMessage::Begin()const
{
return nullptr;
}

WORD UdpMessage::GetSize()const
{
return 0;
}



//==========================
// Con-/Destructors Private
//==========================

UdpMessage::UdpMessage(IP_ADDR from):
m_From(from)
{}

}}
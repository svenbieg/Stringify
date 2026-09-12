//=============
// IpAddress.h
//=============

#pragma once


//=======
// Using
//=======

#include "StringClass.h"


//===========
// Namespace
//===========

namespace Network {
	namespace Ip {


//======
// Type
//======

typedef UINT IP_ADDR;


//========
// Common
//========

constexpr IP_ADDR IP_BROADCAST=0xFFFFFFFF;


//============
// IP-Address
//============

class IpAddress
{
public:
	// Common
	static IP_ADDR From(BYTE A0, BYTE A1, BYTE A2, BYTE A3);
	static BOOL FromString(Handle<String> Address, IP_ADDR* Ip);
	static Handle<String> ToString(IP_ADDR Address);
};

}}
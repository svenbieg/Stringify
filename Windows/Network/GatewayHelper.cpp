//===================
// GatewayHelper.cpp
//===================

#include "pch.h"


//=======
// Using
//=======

#pragma comment(lib, "Iphlpapi.lib")

#include <winsock2.h>
#include <iphlpapi.h>
#include "GatewayHelper.h"


//===========
// Namespace
//===========

namespace Network {


//========
// Common
//========

UINT GetGatewayIp()
{
UINT flags=GAA_FLAG_INCLUDE_GATEWAYS;
ULONG size=0;
GetAdaptersAddresses(AF_INET, flags, nullptr, nullptr, &size);
if(!size)
	return 0;
PIP_ADAPTER_ADDRESSES info=(PIP_ADAPTER_ADDRESSES)new BYTE[size];
if(!info)
	return 0;
GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_GATEWAYS, nullptr, info, &size);
PIP_ADAPTER_ADDRESSES addr=info;
UINT ip=0;
while(addr)
	{
	auto gateway=addr->FirstGatewayAddress;
	if(gateway)
		{
		auto paddr=(sockaddr_in*)gateway->Address.lpSockaddr;
		ip=paddr->sin_addr.S_un.S_addr;
		break;
		}
	addr=addr->Next;
	}
delete info;
return ip;
}

}
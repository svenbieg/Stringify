//==================
// TlsCertificate.h
//==================

#pragma once


//=======
// Using
//=======

#include "Resources/Certificates/Certificate.h"


//===========
// Namespace
//===========

namespace Network {
	namespace Tls {


//=================
// TLS-Certificate
//=================

class TlsCertificate
{
public:
	// Using
	using CERTIFICATE=Resources::Certificates::CERTIFICATE;

	// Common
	static VOID Add(CERTIFICATE const& Certificate) {}
	static BOOL Add(Handle<String> HostName, CERTIFICATE const* Store, UINT StoreCount) { return false; }
};

}}
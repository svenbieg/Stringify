//=================
// ProcessClient.h
//=================

#pragma once


//=======
// Using
//=======

#include "Storage/NamedPipe.h"


//===========
// Namespace
//===========

namespace System {


//================
// Process-Client
//================

class ProcessClient: public Object
{
public:
	// Con-/Destructors
	ProcessClient();

	// Common
	VOID ActivateServerApplication();
	BOOL Connect();
	VOID SendCommandLine();

private:
	// Using
	using NamedPipe=Storage::NamedPipe;

	// Common
	DWORD GetServerProcessId();
	Handle<NamedPipe> hNamedPipe;
	DWORD uServerProcessId;
};

}
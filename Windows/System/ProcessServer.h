//=================
// ProcessServer.h
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
// Process-Server
//================

class ProcessServer: public Object
{
public:
	// Con-/Destructors
	ProcessServer();

	// Common
	VOID Listen();
	Event<ProcessServer, Handle<String>> MessageReceived;

private:
	// Using
	using NamedPipe=Storage::NamedPipe;

	// Common
	VOID OnNamedPipeConnectionReceived();
	VOID OnMessageReceived(Handle<String> Message);
	Handle<NamedPipe> hNamedPipe;
};

}
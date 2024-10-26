//=============
// NamedPipe.h
//=============

#pragma once


//=======
// Using
//=======

#include <namedpipeapi.h>
#include "Storage/Streams/RandomAccessStream.h"


//===========
// Namespace
//===========

namespace Storage {


//============
// Named-Pipe
//============

class NamedPipe: public Storage::Streams::RandomAccessStream
{
public:
	// Using
	using Task=Concurrency::Task;

	// Con-/Destructors
	NamedPipe(Handle<String> Name);
	~NamedPipe();

	// Common
	VOID Close();
	BOOL Connect();
	VOID Listen();
	Event<NamedPipe> ConnectionReceived;

	// Input-Stream
	SIZE_T Available()override;
	SIZE_T Read(VOID* Buffer, SIZE_T Size)override;

	// Output-Stream
	VOID Flush()override;
	SIZE_T Write(VOID const* Buffer, SIZE_T Size)override;

private:
	// Common
	VOID ListenProc();
	VOID OnConnectionClosed();
	Handle<Task> hListenTask;
	HANDLE hNamedPipe;
	Handle<String> hPath;
};

}

//=============
// NamedPipe.h
//=============

#pragma once


//=======
// Using
//=======

#include "Concurrency/Task.h"
#include "Storage/Streams/RandomAccessStream.h"
#include "Event.h"
#include <namedpipeapi.h>


//===========
// Namespace
//===========

namespace Storage {


//============
// Named-Pipe
//============

class NamedPipe: public Object, public Streams::RandomAccessStream
{
public:
	// Using
	using Task=Concurrency::Task;

	// Friends
	friend Object;

	// Con-/Destructors
	~NamedPipe();
	static inline Handle<NamedPipe> Create(Handle<String> Name)
		{
		return Object::Create<NamedPipe>(Name);
		}

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
	// Con-/Destructors
	NamedPipe(Handle<String> Name);

	// Common
	VOID DoListen();
	Handle<Task> m_ListenTask;
	HANDLE m_NamedPipe;
	Handle<String> m_Path;
};

}

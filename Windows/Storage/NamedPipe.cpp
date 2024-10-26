//===============
// NamedPipe.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#include "Core/Application.h"
#include "NamedPipe.h"

using namespace Core;


//===========
// Namespace
//===========

namespace Storage {


//==================
// Con-/Destructors
//==================

NamedPipe::NamedPipe(Handle<String> name):
hNamedPipe(NULL)
{
hPath=new String("\\\\.\\pipe\\ipc_%s", name);
}

NamedPipe::~NamedPipe()
{
Close();
}


//========
// Common
//========

VOID NamedPipe::Close()
{
if(hNamedPipe)
	{
	if(hListenTask)
		{
		CancelSynchronousIo(hListenTask->GetHandle());
		hListenTask=nullptr;
		}
	CloseHandle(hNamedPipe);
	hNamedPipe=NULL;
	}
}

BOOL NamedPipe::Connect()
{
assert(hNamedPipe==NULL);
hNamedPipe=CreateFile(hPath->Begin(), GENERIC_READ|GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, NULL);
if(hNamedPipe==INVALID_HANDLE_VALUE)
	hNamedPipe=NULL;
if(hNamedPipe==NULL)
	return false;
return true;
}

VOID NamedPipe::Listen()
{
assert(hNamedPipe==NULL);
DWORD open_mode=PIPE_ACCESS_DUPLEX|FILE_FLAG_FIRST_PIPE_INSTANCE|FILE_FLAG_WRITE_THROUGH;
hNamedPipe=CreateNamedPipe(hPath->Begin(), open_mode, 0, 1, PAGE_SIZE, PAGE_SIZE, 0, nullptr);
if(hNamedPipe==INVALID_HANDLE_VALUE)
	hNamedPipe=NULL;
if(hNamedPipe==NULL)
	return;
hListenTask=CreateTask(this, &NamedPipe::ListenProc);
}


//==============
// Input-Stream
//==============

SIZE_T NamedPipe::Available()
{
return 0;
}

SIZE_T NamedPipe::Read(VOID* buf, SIZE_T size)
{
if(!hNamedPipe)
	return 0;
DWORD read=0;
if(!ReadFile(hNamedPipe, buf, (UINT)size, &read, nullptr))
	{
	CloseHandle(hNamedPipe);
	hNamedPipe=NULL;
	return 0;
	}
return read;
}


//===============
// Output-Stream
//===============

VOID NamedPipe::Flush()
{
if(!hNamedPipe)
	return;
FlushFileBuffers(hNamedPipe);
}

SIZE_T NamedPipe::Write(VOID const* buf, SIZE_T size)
{
if(!hNamedPipe)
	return 0;
DWORD written=0;
if(!WriteFile(hNamedPipe, buf, (DWORD)size, &written, nullptr))
	{
	CloseHandle(hNamedPipe);
	hNamedPipe=NULL;
	return 0;
	}
return written;
}


//================
// Common Private
//================

VOID NamedPipe::ListenProc()
{
SetLastError(0);
BOOL connected=ConnectNamedPipe(hNamedPipe, nullptr);
if(!connected)
	return;
ConnectionReceived(this);
CloseHandle(hNamedPipe);
hNamedPipe=NULL;
Application::Current->Dispatch(this, &NamedPipe::OnConnectionClosed);
}

VOID NamedPipe::OnConnectionClosed()
{
hListenTask=nullptr;
Listen();
}

}

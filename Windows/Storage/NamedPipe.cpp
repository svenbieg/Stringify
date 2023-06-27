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
cOverlapped({ 0 }),
hNamedPipe(NULL)
{
hPath=new String("\\\\.\\pipe\\ipc_%s", name);
}

NamedPipe::~NamedPipe()
{
Destroy();
}


//========
// Common
//========

BOOL NamedPipe::Connect()
{
ASSERT(hNamedPipe==NULL);
while(1)
	{
	SetLastError(0);
	hNamedPipe=CreateFile(hPath->Begin(), GENERIC_READ|GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, NULL);
	if(hNamedPipe==INVALID_HANDLE_VALUE)
		hNamedPipe=NULL;
	if(hNamedPipe==NULL)
		{
		DWORD err=GetLastError();
		if(err!=ERROR_PIPE_BUSY)
			return false;
		if(!WaitNamedPipe(hPath->Begin(), 1000))
			return false;
		continue;
		}
	break;
	}
return true;
}

VOID NamedPipe::Listen()
{
ASSERT(hNamedPipe==NULL);
ZeroMemory(&cOverlapped, sizeof(OVERLAPPED));
cOverlapped.hEvent=CreateEvent(nullptr, true, false, nullptr);
DWORD open_mode=PIPE_ACCESS_DUPLEX|FILE_FLAG_FIRST_PIPE_INSTANCE|FILE_FLAG_OVERLAPPED;
//DWORD pipe_mode=PIPE_TYPE_MESSAGE|PIPE_READMODE_MESSAGE;
hNamedPipe=CreateNamedPipe(hPath->Begin(), open_mode, 0, 1, PAGE_SIZE, PAGE_SIZE, 0, nullptr);
if(hNamedPipe==INVALID_HANDLE_VALUE)
	hNamedPipe=NULL;
if(hNamedPipe==NULL)
	{
	CloseHandle(cOverlapped.hEvent);
	cOverlapped.hEvent=NULL;
	return;
	}
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
	return 0;
return written;
}


//================
// Common Private
//================

VOID NamedPipe::Destroy()
{
if(cOverlapped.hEvent)
	{
	CloseHandle(cOverlapped.hEvent);
	cOverlapped.hEvent=NULL;
	}
if(hNamedPipe)
	{
	CloseHandle(hNamedPipe);
	hNamedPipe=NULL;
	}
}

VOID NamedPipe::ListenProc()
{
SetLastError(0);
BOOL connected=ConnectNamedPipe(hNamedPipe, &cOverlapped);
if(!connected)
	{
	DWORD err=GetLastError();
	if(err==ERROR_IO_PENDING)
		{
		DWORD read=0;
		if(GetOverlappedResult(hNamedPipe, &cOverlapped, &read, true))
			{
			connected=true;
			}
		}
	else if(err==ERROR_PIPE_CONNECTED)
		{
		connected=true;
		}
	}
if(connected)
	ConnectionReceived(this);
Application::Current->Dispatch(this, &NamedPipe::OnConnectionClosed);
}

VOID NamedPipe::OnConnectionClosed()
{
Destroy();
hListenTask=nullptr;
Listen();
}

}
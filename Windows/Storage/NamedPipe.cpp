//===============
// NamedPipe.cpp
//===============

#include "Storage/NamedPipe.h"


//===========
// Namespace
//===========

namespace Storage {


//==================
// Con-/Destructors
//==================

NamedPipe::NamedPipe(Handle<String> name):
m_NamedPipe(NULL)
{
m_Path=String::Create("\\\\.\\pipe\\ipc_%s", name);
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
if(!m_NamedPipe)
	return;
if(m_ListenTask)
	{
	m_ListenTask->Cancel();
	CancelSynchronousIo(m_ListenTask->GetHandle());
	m_ListenTask=nullptr;
	}
CloseHandle(m_NamedPipe);
m_NamedPipe=NULL;
}

BOOL NamedPipe::Connect()
{
assert(m_NamedPipe==NULL);
m_NamedPipe=CreateFile(m_Path->Begin(), GENERIC_READ|GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, NULL);
if(m_NamedPipe==INVALID_HANDLE_VALUE)
	m_NamedPipe=NULL;
if(m_NamedPipe==NULL)
	return false;
return true;
}

VOID NamedPipe::Listen()
{
assert(m_NamedPipe==NULL);
DWORD open_mode=PIPE_ACCESS_DUPLEX|FILE_FLAG_FIRST_PIPE_INSTANCE|FILE_FLAG_WRITE_THROUGH;
m_NamedPipe=CreateNamedPipe(m_Path->Begin(), open_mode, 0, 1, MemoryHelper::PAGE_SIZE, MemoryHelper::PAGE_SIZE, 0, nullptr);
if(m_NamedPipe==INVALID_HANDLE_VALUE)
	m_NamedPipe=NULL;
if(m_NamedPipe==NULL)
	return;
m_ListenTask=Task::Create(this, &NamedPipe::DoListen);
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
if(!m_NamedPipe)
	return 0;
DWORD read=0;
if(!ReadFile(m_NamedPipe, buf, (UINT)size, &read, nullptr))
	{
	CloseHandle(m_NamedPipe);
	m_NamedPipe=NULL;
	return 0;
	}
return read;
}


//===============
// Output-Stream
//===============

VOID NamedPipe::Flush()
{
if(!m_NamedPipe)
	return;
FlushFileBuffers(m_NamedPipe);
}

SIZE_T NamedPipe::Write(VOID const* buf, SIZE_T size)
{
if(!m_NamedPipe)
	return 0;
DWORD written=0;
if(!WriteFile(m_NamedPipe, buf, (DWORD)size, &written, nullptr))
	{
	CloseHandle(m_NamedPipe);
	m_NamedPipe=NULL;
	return 0;
	}
return written;
}


//================
// Common Private
//================

VOID NamedPipe::DoListen()
{
auto task=Task::Get();
while(!task->Cancelled)
	{
	BOOL connected=ConnectNamedPipe(m_NamedPipe, nullptr);
	if(!connected)
		break;
	ConnectionReceived(this);
	CloseHandle(m_NamedPipe);
	m_NamedPipe=NULL;
	}
}

}

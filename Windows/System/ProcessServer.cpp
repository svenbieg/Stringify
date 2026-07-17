//===================
// ProcessServer.cpp
//===================

#include "ProcessServer.h"


//=======
// Using
//=======

#include "Concurrency/DispatchedQueue.h"
#include "Storage/Streams/StreamReader.h"
#include "Storage/Streams/StreamWriter.h"
#include "PathHelper.h"

using namespace Concurrency;
using namespace Storage::Streams;


//===========
// Namespace
//===========

namespace System {


//==================
// Con-/Destructors
//==================

ProcessServer::ProcessServer()
{}


//========
// Common
//========

VOID ProcessServer::Listen()
{
TCHAR exe_path[MAX_PATH];
GetModuleFileName(NULL, exe_path, MAX_PATH);
auto exe_name=PathHelper::GetLastComponent(exe_path);
m_NamedPipe=NamedPipe::Create(exe_name);
m_NamedPipe->ConnectionReceived.Add(this, &ProcessServer::OnNamedPipeConnectionReceived);
m_NamedPipe->Listen();
}


//================
// Common Private
//================

VOID ProcessServer::OnNamedPipeConnectionReceived()
{
StreamReader reader(m_NamedPipe);
while(1)
	{
	auto msg=reader.ReadString(nullptr, "\n");
	if(!msg)
		return;
	if(msg=="GetProcessId")
		{
		UINT id=GetCurrentProcessId();
		CHAR id_str[32];
		StringHelper::Print(id_str, 32, "0x%x\n", id);
		StreamWriter writer(m_NamedPipe);
		writer.Print(id_str);
		m_NamedPipe->Flush();
		continue;
		}
	DispatchedQueue::Append(this, [this, msg]() { OnMessageReceived(msg); });
	}
}

VOID ProcessServer::OnMessageReceived(Handle<String> msg)
{
MessageReceived(this, msg);
}

}
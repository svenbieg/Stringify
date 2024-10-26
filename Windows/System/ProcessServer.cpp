//===================
// ProcessServer.cpp
//===================

#include "pch.h"


//=======
// Using
//=======

#include "Core/Application.h"
#include "Storage/Streams/StreamReader.h"
#include "Storage/Streams/StreamWriter.h"
#include "ProcessServer.h"

using namespace Core;
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
CHAR exe_path[MAX_PATH];
GetModuleFileNameA(NULL, exe_path, MAX_PATH);
auto exe_name=PathGetLastComponent(exe_path);
hNamedPipe=new NamedPipe(exe_name);
hNamedPipe->ConnectionReceived.Add(this, &ProcessServer::OnNamedPipeConnectionReceived);
hNamedPipe->Listen();
}


//================
// Common Private
//================

VOID ProcessServer::OnNamedPipeConnectionReceived()
{
StreamReader reader(hNamedPipe);
while(1)
	{
	auto msg=reader.ReadString(nullptr, "\n");
	if(!msg)
		return;
	if(msg=="GetProcessId")
		{
		UINT id=GetCurrentProcessId();
		CHAR id_str[32];
		StringPrint(id_str, 32, "0x%x\n", id);
		StreamWriter writer(hNamedPipe);
		writer.Print(id_str);
		hNamedPipe->Flush();
		continue;
		}
	Application::Current->Dispatch(this, [this, msg]() { OnMessageReceived(msg); });
	}
}

VOID ProcessServer::OnMessageReceived(Handle<String> msg)
{
MessageReceived(this, msg);
}

}
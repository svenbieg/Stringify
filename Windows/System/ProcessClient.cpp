//===================
// ProcessClient.cpp
//===================

#include "pch.h"


//=======
// Using
//=======

#include "Storage/Streams/StringReader.h"
#include "Storage/Streams/StringWriter.h"
#include "CommandLine.h"
#include "ProcessClient.h"

using namespace Storage::Streams;


//===========
// Namespace
//===========

namespace System {


//==================
// Con-/Destructors
//==================

ProcessClient::ProcessClient():
uServerProcessId(0)
{}


//========
// Common
//========

VOID ProcessClient::ActivateServerApplication()
{
if(!hNamedPipe)
	return;
AllowSetForegroundWindow(uServerProcessId);
StringWriter writer(hNamedPipe);
writer.Print("ActivateApp\n");
hNamedPipe->Flush();
}

BOOL ProcessClient::Connect()
{
CHAR exe_path[MAX_PATH];
GetModuleFileNameA(NULL, exe_path, MAX_PATH);
auto exe_name=PathGetLastComponent(exe_path);
hNamedPipe=new NamedPipe(exe_name);
if(hNamedPipe->Connect())
	{
	uServerProcessId=GetServerProcessId();
	if(uServerProcessId!=0)
		return true;
	}
hNamedPipe=nullptr;
return false;
}

VOID ProcessClient::SendCommandLine()
{
if(!hNamedPipe)
	return;
StringWriter writer(hNamedPipe);
auto cmd_line=CommandLine::Current;
UINT count=cmd_line->GetArgumentCount();
for(UINT u=0; u<count; u++)
	{
	auto arg=cmd_line->GetArgument(u);
	writer.Print(arg);
	writer.PrintChar('\n');
	}
hNamedPipe->Flush();
}


//================
// Common Private
//================

DWORD ProcessClient::GetServerProcessId()
{
StringWriter writer(hNamedPipe);
writer.Print("GetProcessId\n");
hNamedPipe->Flush();
StringReader reader(hNamedPipe);
TCHAR id_str[32];
reader.ReadString(id_str, 32, '\n');
UINT id=0;
if(StringScan(id_str, "0x%x", &id)!=1)
	return 0;
return id;
}

}
//===================
// ProcessClient.cpp
//===================

#include "pch.h"


//=======
// Using
//=======

#include "Storage/Streams/StreamReader.h"
#include "Storage/Streams/StreamWriter.h"
#include "CommandLine.h"
#include "PathHelper.h"
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
StreamWriter writer(hNamedPipe);
writer.Print("ActivateApp\n");
hNamedPipe->Flush();
}

BOOL ProcessClient::Connect()
{
CHAR exe_path[MAX_PATH];
GetModuleFileNameA(NULL, exe_path, MAX_PATH);
auto exe_name=PathHelper::GetLastComponent(exe_path);
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
StreamWriter writer(hNamedPipe);
auto cmd_line=CommandLine::Get();
auto args=cmd_line->Arguments;
for(auto it=args->Begin(); it->HasCurrent(); it->MoveNext())
	{
	auto arg=it->GetCurrent();
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
StreamWriter writer(hNamedPipe);
writer.Print("GetProcessId\n");
hNamedPipe->Flush();
StreamReader reader(hNamedPipe);
TCHAR id_str[32];
reader.ReadString(id_str, 32, '\n');
UINT id=0;
if(StringHelper::Scan(id_str, "0x%x", &id)!=1)
	return 0;
return id;
}

}

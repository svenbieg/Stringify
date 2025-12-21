//===================
// ProcessClient.cpp
//===================

#include "ProcessClient.h"


//=======
// Using
//=======

#include "Storage/Streams/StreamReader.h"
#include "Storage/Streams/StreamWriter.h"
#include "CommandLine.h"
#include "PathHelper.h"

using namespace Storage::Streams;


//===========
// Namespace
//===========

namespace System {


//==================
// Con-/Destructors
//==================

ProcessClient::ProcessClient():
m_ServerProcessId(0)
{}


//========
// Common
//========

VOID ProcessClient::ActivateServerApplication()
{
if(!m_NamedPipe)
	return;
AllowSetForegroundWindow(m_ServerProcessId);
StreamWriter writer(m_NamedPipe);
writer.Print("ActivateApp\n");
m_NamedPipe->Flush();
}

BOOL ProcessClient::Connect()
{
CHAR exe_path[MAX_PATH];
GetModuleFileNameA(NULL, exe_path, MAX_PATH);
auto exe_name=PathHelper::GetLastComponent(exe_path);
m_NamedPipe=new NamedPipe(exe_name);
if(m_NamedPipe->Connect())
	{
	m_ServerProcessId=GetServerProcessId();
	if(m_ServerProcessId!=0)
		return true;
	}
m_NamedPipe=nullptr;
return false;
}

VOID ProcessClient::SendCommandLine()
{
if(!m_NamedPipe)
	return;
StreamWriter writer(m_NamedPipe);
auto cmd_line=CommandLine::Get();
auto args=cmd_line->Arguments;
for(auto it=args->Begin(); it->HasCurrent(); it->MoveNext())
	{
	auto arg=it->GetCurrent();
	writer.Print(arg);
	writer.PrintChar('\n');
	}
m_NamedPipe->Flush();
}


//================
// Common Private
//================

DWORD ProcessClient::GetServerProcessId()
{
StreamWriter writer(m_NamedPipe);
writer.Print("GetProcessId\n");
m_NamedPipe->Flush();
StreamReader reader(m_NamedPipe);
TCHAR id_str[32];
reader.ReadString(id_str, 32, '\n');
UINT id=0;
if(StringHelper::Scan(id_str, "0x%x", &id)!=1)
	return 0;
return id;
}

}

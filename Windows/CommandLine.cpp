//=================
// CommandLine.cpp
//=================

#include "pch.h"


//=======
// Using
//=======

#include "CommandLine.h"


//==================
// Con-/Destructors
//==================

CommandLine::CommandLine(LPSTR cmd_line):
ppArgs(nullptr),
uCount(0)
{
Current=this;
if(!cmd_line||!cmd_line[0])
	return;
LPSTR next=cmd_line;
UINT count=0;
while(next)
	{
	next=ScanArgument(next, nullptr, nullptr);
	count++;
	}
ppArgs=new LPSTR[count];
next=cmd_line;
count=0;
while(next)
	{
	LPSTR cmd=nullptr;
	UINT len=0;
	next=ScanArgument(next, &cmd, &len);
	cmd[len]=0;
	ppArgs[count++]=cmd;
	}
uCount=count;
}

CommandLine::~CommandLine()
{
if(ppArgs)
	delete ppArgs;
}


//========
// Common
//========

Handle<CommandLine> CommandLine::Current;

LPCSTR CommandLine::GetArgument(UINT id)const
{
if(id>=uCount)
	return nullptr;
return ppArgs[id];
}

BOOL CommandLine::HasArgument(LPCTSTR arg)
{
if(!arg)
	return false;
for(UINT u=0; u<uCount; u++)
	{
	if(StringCompare(arg, ppArgs[u], 0, false)==0)
		return true;
	}
return false;
}

LPSTR CommandLine::ScanArgument(LPSTR cmd_line, LPSTR* arg_ptr, UINT* len_ptr)
{
if(!cmd_line||!cmd_line[0])
	return nullptr;
UINT pos=0;
while(CharEqual(cmd_line[pos], ' '))
	pos++;
UINT len=1;
if(CharEqual(cmd_line[pos], '\"'))
	{
	pos++;
	if(!StringFindChar(&cmd_line[pos], '\"', &len))
		return nullptr;
	}
else
	{
	while(cmd_line[pos+len])
		{
		if(CharEqual(cmd_line[pos+len], ' '))
			break;
		len++;
		}
	}
if(arg_ptr)
	*arg_ptr=&cmd_line[pos];
if(len_ptr)
	*len_ptr=len;
LPSTR next=&cmd_line[pos+len+1];
while(CharEqual(next[0], ' '))
	next++;
if(next[0])
	return next;
return nullptr;
}

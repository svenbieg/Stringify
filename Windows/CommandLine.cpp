//=================
// CommandLine.cpp
//=================

#include "CommandLine.h"



//========
// Common
//========

LPCTSTR CommandLine::Begin()
{
return GetCommandLine();
}


//==========================
// Con-/Destructors Private
//==========================

CommandLine::CommandLine()
{
LPTSTR cmd_line=GetCommandLine();
Arguments=StringList::Create();
if(!cmd_line||!cmd_line[0])
	return;
LPCTSTR next=cmd_line;
UINT count=0;
while(next)
	{
	LPCTSTR str=nullptr;
	UINT len=0;
	next=ScanArgument(next, &str, &len);
	auto cmd=String::Create(len, str);
	Arguments->Append(cmd, false);
	}
}


//================
// Common Private
//================

LPCTSTR CommandLine::ScanArgument(LPCTSTR cmd_line, LPCTSTR* arg_ptr, UINT* len_ptr)
{
if(!cmd_line||!cmd_line[0])
	return nullptr;
UINT pos=0;
while(CharHelper::Equal(cmd_line[pos], ' '))
	pos++;
UINT len=1;
if(CharHelper::Equal(cmd_line[pos], '\"'))
	{
	pos++;
	if(!StringHelper::FindChar(&cmd_line[pos], '\"', &len))
		return nullptr;
	}
else
	{
	while(cmd_line[pos+len])
		{
		if(CharHelper::Equal(cmd_line[pos+len], ' '))
			break;
		len++;
		}
	}
if(arg_ptr)
	*arg_ptr=&cmd_line[pos];
if(len_ptr)
	*len_ptr=len;
LPCTSTR next=&cmd_line[pos+len+1];
while(CharHelper::Equal(next[0], ' '))
	next++;
if(next[0])
	return next;
return nullptr;
}

//================
// SerialPort.cpp
//================

#include "pch.h"


//=======
// Using
//=======

#include "SerialPort.h"


//===========
// Namespace
//===========

namespace Devices {
	namespace Serial {


//==================
// Con-/Destructors
//==================

SerialPort::SerialPort(UINT id):
hFile(NULL)
{
Handle<String> path=new String("\\\\.\\COM%u", id);
hFile=CreateFile(path->Begin(), GENERIC_READ|GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, NULL);
if(hFile==INVALID_HANDLE_VALUE)
	hFile=NULL;
}

SerialPort::~SerialPort()
{
Close();
}


//========
// Common
//========

VOID SerialPort::Close()
{
if(hFile!=NULL)
	CloseHandle(hFile);
hFile=NULL;
}

VOID SerialPort::SetBaudRate(BaudRate baud)
{
if(hFile==NULL)
	return;
DCB params;
ZeroMemory(&params, sizeof(DCB));
params.BaudRate=(DWORD)baud;
params.ByteSize=8;
params.StopBits=ONESTOPBIT;
params.Parity=NOPARITY;
SetCommState(hFile, &params);
}


//==============
// Input-Stream
//==============

SIZE_T SerialPort::Available()
{
return 0;
}

SIZE_T SerialPort::Read(VOID* buf, SIZE_T size)
{
return 0;
}


//===============
// Output-Stream
//===============

VOID SerialPort::Flush()
{
}

SIZE_T SerialPort::Write(VOID const* buf, SIZE_T size)
{
if(!buf||!size)
	return 0;
DWORD written=0;
WriteFile(hFile, buf, (UINT)size, &written, nullptr);
return written;
}

}}
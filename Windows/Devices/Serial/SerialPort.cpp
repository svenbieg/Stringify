//================
// SerialPort.cpp
//================

#include "Devices/Serial/SerialPort.h"


//=======
// Using
//=======

#include "StringClass.h"


//===========
// Namespace
//===========

namespace Devices {
	namespace Serial {


//==================
// Con-/Destructors
//==================

SerialPort::~SerialPort()
{
Close();
}


//========
// Common
//========

VOID SerialPort::Close()
{
if(m_File!=NULL)
	CloseHandle(m_File);
m_File=NULL;
}

VOID SerialPort::SetBaudRate(BaudRate baud)
{
if(m_File==NULL)
	return;
DCB params={ 0 };
params.BaudRate=(DWORD)baud;
params.ByteSize=8;
params.StopBits=ONESTOPBIT;
params.Parity=NOPARITY;
SetCommState(m_File, &params);
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
WriteFile(m_File, buf, (UINT)size, &written, nullptr);
return written;
}


//==========================
// Con-/Destructors Private
//==========================

SerialPort::SerialPort(UINT id):
m_File(NULL)
{
auto path=String::Create("\\\\.\\COM%u", id);
m_File=CreateFile(path->Begin(), GENERIC_READ|GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, NULL);
if(m_File==INVALID_HANDLE_VALUE)
	m_File=NULL;
if(m_File==NULL)
	throw DeviceNotReadyException();
}

}}
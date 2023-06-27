//==============
// SerialPort.h
//==============

#pragma once


//=======
// Using
//=======

#include "Storage/Streams/RandomAccessStream.h"


//===========
// Namespace
//===========

namespace Devices {
	namespace Serial {


//===========
// Baud-Rate
//===========

enum class BaudRate: DWORD
{
Baud4800=CBR_4800,
Baud9600=CBR_9600,
Baud14400=CBR_14400,
Baud19200=CBR_19200,
Baud38400=CBR_38400,
Baud56000=CBR_56000,
Baud57600=CBR_57600,
Baud115200=CBR_115200,
Baud128000=CBR_128000,
Baud256000=CBR_256000
};


//=============
// Serial-Port
//=============

class SerialPort: public Storage::Streams::RandomAccessStream
{
public:
	// Con-/Destructors
	SerialPort(UINT Id);
	~SerialPort();

	// Common
	VOID Close();
	VOID SetBaudRate(BaudRate BaudRate);

	// Input-Stream
	SIZE_T Available()override;
	SIZE_T Read(VOID* Buffer, SIZE_T Size)override;

	// Output-Stream
	VOID Flush()override;
	SIZE_T Write(VOID const* Buffer, SIZE_T Size)override;

private:
	// Common
	HANDLE hFile;
};

}}
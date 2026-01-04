//==============
// VolumeFile.h
//==============

#pragma once


//=======
// Using
//=======

#include "Storage/File.h"
#include "Storage/Volume.h"
#include "Status.h"
#include "StringClass.h"


//===========
// Namespace
//===========

namespace Storage {
	namespace Filesystem {


//=============
// Volume-File
//=============

class VolumeFile: public Volume
{
public:
	// Con-/Destructors
	VolumeFile();
	~VolumeFile();

	// Common
	VOID Close();
	Status Create(Handle<String> Path, FileCreateMode Create=FileCreateMode::OpenExisting);
	BOOL IsTemporary();
	Handle<String> Path;

	// Volume
	UINT GetBlockSize()override;
	UINT64 GetSize()override;
	SIZE_T Read(UINT64 Offset, VOID* Buffer, SIZE_T Size)override;
	BOOL SetSize(UINT64 Size)override;
	SIZE_T Write(UINT64 Offset, VOID const* Buffer, SIZE_T Size)override;

private:
	// Flags
	enum class VolumeFileFlags: UINT
		{
		None=0,
		Temporary=1
		};

	// Common
	OVERLAPPED cOverlapped;
	HANDLE hFile;
	VolumeFileFlags m_Flags;
};

}}
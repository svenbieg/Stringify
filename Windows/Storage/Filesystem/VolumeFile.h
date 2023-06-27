//==============
// VolumeFile.h
//==============

#pragma once


//=======
// Using
//=======

#include "Storage/File.h"
#include "Storage/Volume.h"


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
	FILE_SIZE GetSize()override;
	SIZE_T Read(FILE_SIZE Offset, VOID* Buffer, SIZE_T Size)override;
	BOOL SetSize(FILE_SIZE Size)override;
	SIZE_T Write(FILE_SIZE Offset, VOID const* Buffer, SIZE_T Size)override;

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
	VolumeFileFlags uFlags;
};

}}
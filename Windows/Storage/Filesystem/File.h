//========
// File.h
//========

#pragma once


//=======
// Using
//=======

#include <shared_mutex>
#include "Storage/File.h"
#include "FileHelper.h"


//===========
// Namespace
//===========

namespace Storage {
	namespace Filesystem {


//======
// File
//======

class File: public Storage::File
{
public:
	// Con-/Destructors
	File(Handle<String> Path);

	// Input-Stream
	SIZE_T Available()override;
	SIZE_T Read(VOID* Buffer, SIZE_T Size)override;
	SIZE_T Read(VOID* Buffer, SIZE_T Size, BOOL* Cancel);

	// Output-Stream
	VOID Flush()override;
	SIZE_T Write(VOID const* Buffer, SIZE_T Size)override;
	SIZE_T Write(VOID const* Buffer, SIZE_T Size, BOOL* Cancel);

	// Container
	FILE_SIZE GetSize()override;
	BOOL Seek(UINT64 Position)override;

	// File
	VOID Close()override;
	Status Create(FileCreateMode Create=FileCreateMode::OpenExisting, FileAccessMode Access=FileAccessMode::ReadOnly, FileShareMode Share=FileShareMode::ShareRead)override;
	BOOL SetSize(FILE_SIZE Size)override;

private:
	// Using
	using unique_lock=std::unique_lock<std::shared_mutex>;

	// Common
	SIZE_T AvailableInternal();
	VOID CloseInternal();
	std::shared_mutex cMutex;
	HANDLE hFile;
	UINT64 uPosition;
};

}}
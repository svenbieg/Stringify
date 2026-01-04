//========
// File.h
//========

#pragma once


//=======
// Using
//=======

#include "Concurrency/Task.h"
#include "Storage/Filesystem/FileHelper.h"
#include "Storage/File.h"


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
	static inline Handle<File> Create(Handle<String> Path) { return new File(Path); }
	static Handle<File> Create(Handle<String> Path, FileCreateMode Create, FileAccessMode Access=FileAccessMode::ReadOnly, FileShareMode Share=FileShareMode::ShareRead);

	// Common
	VOID Close();
	Status Create(FileCreateMode Create=FileCreateMode::OpenExisting, FileAccessMode Access=FileAccessMode::ReadOnly, FileShareMode Share=FileShareMode::ShareRead);

	// Input-Stream
	SIZE_T Available()override;
	SIZE_T Read(VOID* Buffer, SIZE_T Size)override;
	SIZE_T Read(VOID* Buffer, SIZE_T Size, BOOL* Cancel);

	// Output-Stream
	VOID Flush()override;
	SIZE_T Write(VOID const* Buffer, SIZE_T Size)override;
	SIZE_T Write(VOID const* Buffer, SIZE_T Size, BOOL* Cancel);

	// Seekable
	SIZE_T GetSize()override;
	BOOL Seek(SIZE_T Position)override;

	// Storage.File
	Handle<String> GetName()override;
	Handle<String> GetPath()override;
	BOOL SetSize(SIZE_T Size)override;

private:
	// Con-/Destructors
	File(Handle<String> Path);

	// Common
	SIZE_T AvailableInternal();
	VOID CloseInternal();
	HANDLE m_File;
	Concurrency::Mutex m_Mutex;
	Handle<String> m_Path;
	UINT64 m_Position;
};

}}
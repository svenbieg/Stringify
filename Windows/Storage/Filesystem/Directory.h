//=============
// Directory.h
//=============

#pragma once


//=======
// Using
//=======

#include "Storage/Directory.h"


//===========
// Namespace
//===========

namespace Storage {
	namespace Filesystem {


//======================
// Forward-Declarations
//======================

class DirectoryIterator;


//===========
// Directory
//===========

class Directory: public Storage::Directory
{
public:
	// Friends
	friend DirectoryIterator;

	// Con-/Destructors
	Directory(Handle<String> Path);

	// Storage.Directory
	Handle<Storage::File> CreateFile(Handle<String> Path, FileCreateMode CreateMode=FileCreateMode::OpenExisting, FileAccessMode AccessMode=FileAccessMode::ReadWrite, FileShareMode ShareMode=FileShareMode::ShareRead)override;
	Handle<Storage::DirectoryIterator> First()override;
	static Handle<Directory> Open(Handle<String> Path);
	Handle<Object> Get(Handle<String> Path)override;

private:
	// Common
	Mutex cMutex;
};


//==========
// Iterator
//==========

class DirectoryIterator: public Storage::DirectoryIterator
{
public:
	// Con-/Destructors
	DirectoryIterator(Handle<Directory> Directory);
	~DirectoryIterator();

	// Common
	BOOL First();
	Handle<Object> GetCurrent()const override { return hCurrent; }
	BOOL HasCurrent()const override { return hCurrent!=nullptr; }
	BOOL MoveNext()override;

private:
	// Common
	Handle<Object> hCurrent;
	Handle<Directory> hDirectory;
	HANDLE hFind;
};

}}
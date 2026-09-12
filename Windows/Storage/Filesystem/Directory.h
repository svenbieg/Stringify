//=============
// Directory.h
//=============

#pragma once


//=======
// Using
//=======

#include "Concurrency/Task.h"
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
	friend Object;

	// Con-/Destructors
	static inline Handle<Directory> Create(Handle<String> Path) { return Object::Create<Directory>(Path); }

	// Storage.Directory
	Handle<Storage::DirectoryIterator> Begin()override;
	Handle<Storage::File> CreateFile(Handle<String> Path, FileCreateMode CreateMode=FileCreateMode::OpenExisting, FileAccessMode AccessMode=FileAccessMode::ReadWrite, FileShareMode ShareMode=FileShareMode::ShareRead)override;
	static Handle<Directory> Open(Handle<String> Path);
	Handle<Object> Get(Handle<String> Path)override;
	Handle<String> GetName()override;
	inline Handle<String> GetPath()const { return m_Path; }
	Handle<Storage::Directory> GetParent()const override;

private:
	// Con-/Destructors
	Directory(Handle<String> Path);

	// Common
	Concurrency::Mutex m_Mutex;
	Handle<String> m_Path;
};


//==========
// Iterator
//==========

class DirectoryIterator: public Storage::DirectoryIterator
{
public:
	// Friends
	friend Directory;
	friend Object;

	// Con-/Destructors
	~DirectoryIterator();

	// Common
	BOOL First();
	Handle<Object> GetCurrent()const override { return m_Current; }
	BOOL HasCurrent()const override { return m_Current!=nullptr; }
	BOOL MoveNext()override;

private:
	// Con-/Destructors
	DirectoryIterator(Directory* Directory);
	static inline Handle<DirectoryIterator> Create(Directory* Directory) { return Object::Create<DirectoryIterator>(Directory); }

	// Common
	Handle<Object> m_Current;
	Handle<Directory> m_Directory;
	HANDLE m_Find;
};

}}
//===============
// Directory.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#include "Directory.h"
#include "File.h"
#include "PathHelper.h"


//===========
// Namespace
//===========

namespace Storage {
	namespace Filesystem {


//==================
// Con-/Destructors
//==================

Directory::Directory(Handle<String> path):
Storage::Directory(path)
{}


//===================
// Storage.Directory
//===================

Handle<Storage::File> Directory::CreateFile(Handle<String> path, FileCreateMode create, FileAccessMode access, FileShareMode share)
{
ScopedLock lock(cMutex);
Handle<String> file_path=new String("%s\\%s", hPath, path);
Handle<File> file=new File(file_path);
if(Failed(file->Create(create, access, share)))
	return nullptr;
return file;
}

Handle<Storage::DirectoryIterator> Directory::First()
{
Handle<DirectoryIterator> it=new DirectoryIterator(this);
return it;
}

Handle<Object> Directory::Get(Handle<String> path)
{
if(!path)
	return nullptr;
ScopedLock lock(cMutex);
Handle<String> item_path=new String("%s\\%s", hPath, path);
WIN32_FIND_DATA fd;
ZeroMemory(&fd, sizeof(WIN32_FIND_DATA));
HANDLE hfind=FindFirstFileEx(item_path->Begin(), FindExInfoBasic, &fd, FindExSearchNameMatch, nullptr, FIND_FIRST_EX_LARGE_FETCH);
if(hfind==INVALID_HANDLE_VALUE)
	hfind=NULL;
if(!hfind)
	return nullptr;
FindClose(hfind);
if(fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
	{
	return new Directory(item_path);
	}
return new File(item_path);
}

Handle<Directory> Directory::Open(Handle<String> path)
{
if(!DirectoryExists(path->Begin()))
	return nullptr;
return new Directory(path);
}


//===========================
// Iterator Con-/Destructors
//===========================

DirectoryIterator::DirectoryIterator(Handle<Directory> dir):
hDirectory(dir),
hFind(NULL)
{
hDirectory->cMutex.Lock();
First();
}

DirectoryIterator::~DirectoryIterator()
{
if(hFind)
	FindClose(hFind);
hDirectory->cMutex.Unlock();
}


//=================
// Iterator Common
//=================

BOOL DirectoryIterator::First()
{
hCurrent=nullptr;
if(hFind)
	{
	FindClose(hFind);
	hFind=NULL;
	}
auto path=hDirectory->GetPath();
WIN32_FIND_DATA fd;
ZeroMemory(&fd, sizeof(WIN32_FIND_DATA));
Handle<String> mask=new String("%s\\*.*", path->Begin());
hFind=FindFirstFileEx(mask->Begin(), FindExInfoBasic, &fd, FindExSearchNameMatch, nullptr, FIND_FIRST_EX_LARGE_FETCH);
if(hFind==INVALID_HANDLE_VALUE)
	hFind=NULL;
if(!hFind)
	return false;
while(fd.cFileName[0]=='.')
	{
	if(!FindNextFile(hFind, &fd))
		{
		FindClose(hFind);
		hFind=NULL;
		return false;
		}
	}
Handle<String> item_path=new String("%s\\%s", path->Begin(), fd.cFileName);
if(fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
	{
	hCurrent=new Directory(item_path);
	}
else
	{
	hCurrent=new File(item_path);
	}
return true;
}

BOOL DirectoryIterator::MoveNext()
{
if(!hFind)
	{
	hCurrent=nullptr;
	return false;
	}
WIN32_FIND_DATA fd;
ZeroMemory(&fd, sizeof(WIN32_FIND_DATA));
if(!FindNextFile(hFind, &fd))
	{
	FindClose(hFind);
	hFind=NULL;
	hCurrent=nullptr;
	return false;
	}
auto path=hDirectory->GetPath();
Handle<String> item_path=new String("%s\\%s", path->Begin(), fd.cFileName);
if(fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
	{
	hCurrent=new Directory(item_path);
	}
else
	{
	hCurrent=new File(item_path);
	}
return true;
}

}}
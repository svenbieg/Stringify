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
#include "StatusHelper.h"

using namespace Concurrency;


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

Handle<Storage::DirectoryIterator> Directory::Begin()
{
return new DirectoryIterator(this);
}

Handle<Storage::File> Directory::CreateFile(Handle<String> path, FileCreateMode create, FileAccessMode access, FileShareMode share)
{
ScopedLock lock(m_Mutex);
auto file_path=String::Create("%s\\%s", m_Path, path);
Handle<File> file=new File(file_path);
if(StatusHelper::Failed(file->Create(create, access, share)))
	return nullptr;
return file;
}

Handle<Object> Directory::Get(Handle<String> path)
{
if(!path)
	return nullptr;
ScopedLock lock(m_Mutex);
auto item_path=String::Create("%s\\%s", m_Path, path);
WIN32_FIND_DATA fd={ 0 };
HANDLE find=FindFirstFileEx(item_path->Begin(), FindExInfoBasic, &fd, FindExSearchNameMatch, nullptr, FIND_FIRST_EX_LARGE_FETCH);
if(find==INVALID_HANDLE_VALUE)
	find=NULL;
if(!find)
	return nullptr;
FindClose(find);
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
m_Directory(dir),
hFind(NULL)
{
m_Directory->m_Mutex.Lock();
First();
}

DirectoryIterator::~DirectoryIterator()
{
if(hFind)
	FindClose(hFind);
m_Directory->m_Mutex.Unlock();
}


//=================
// Iterator Common
//=================

BOOL DirectoryIterator::First()
{
m_Current=nullptr;
if(hFind)
	{
	FindClose(hFind);
	hFind=NULL;
	}
auto path=m_Directory->GetPath();
WIN32_FIND_DATA fd={ 0 };
auto mask=String::Create("%s\\*.*", path->Begin());
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
auto item_path=String::Create("%s\\%s", path->Begin(), fd.cFileName);
if(fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
	{
	m_Current=new Directory(item_path);
	}
else
	{
	m_Current=new File(item_path);
	}
return true;
}

BOOL DirectoryIterator::MoveNext()
{
if(!hFind)
	{
	m_Current=nullptr;
	return false;
	}
WIN32_FIND_DATA fd={ 0 };
if(!FindNextFile(hFind, &fd))
	{
	FindClose(hFind);
	hFind=NULL;
	m_Current=nullptr;
	return false;
	}
auto path=m_Directory->GetPath();
auto item_path=String::Create("%s\\%s", path->Begin(), fd.cFileName);
if(fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
	{
	m_Current=new Directory(item_path);
	}
else
	{
	m_Current=new File(item_path);
	}
return true;
}

}}

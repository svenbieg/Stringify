//===============
// Directory.cpp
//===============

#include "Directory.h"


//=======
// Using
//=======

#include "Storage/Filesystem/File.h"
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
m_Path(path)
{}


//===================
// Storage.Directory
//===================

Handle<Storage::DirectoryIterator> Directory::Begin()
{
return DirectoryIterator::Create(this);
}

Handle<Storage::File> Directory::CreateFile(Handle<String> path, FileCreateMode create, FileAccessMode access, FileShareMode share)
{
auto file_path=String::Create("%s\\%s", m_Path, path);
return File::Create(file_path, create, access, share);
}

Handle<Object> Directory::Get(Handle<String> path)
{
if(!path)
	return nullptr;
WriteLock lock(m_Mutex);
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
	return Directory::Create(item_path);
	}
return File::Create(item_path);
}

Handle<String> Directory::GetName()
{
auto path=m_Path->Begin();
return PathHelper::GetLastComponent(path);
}

Handle<Storage::Directory> Directory::GetParent()const
{
return nullptr;
}

Handle<Directory> Directory::Open(Handle<String> path)
{
if(!FileHelper::DirectoryExists(path->Begin()))
	return nullptr;
return Directory::Create(path);
}


//===========================
// Iterator Con-/Destructors
//===========================

DirectoryIterator::DirectoryIterator(Directory* dir):
m_Directory(dir),
m_Find(NULL)
{
m_Directory->m_Mutex.Lock();
First();
}

DirectoryIterator::~DirectoryIterator()
{
if(m_Find)
	FindClose(m_Find);
m_Directory->m_Mutex.Unlock();
}


//=================
// Iterator Common
//=================

BOOL DirectoryIterator::First()
{
m_Current=nullptr;
if(m_Find)
	{
	FindClose(m_Find);
	m_Find=NULL;
	}
auto path=m_Directory->GetPath();
WIN32_FIND_DATA fd={ 0 };
auto mask=String::Create("%s\\*.*", path->Begin());
m_Find=FindFirstFileEx(mask->Begin(), FindExInfoBasic, &fd, FindExSearchNameMatch, nullptr, FIND_FIRST_EX_LARGE_FETCH);
if(m_Find==INVALID_HANDLE_VALUE)
	m_Find=NULL;
if(!m_Find)
	return false;
while(fd.cFileName[0]=='.')
	{
	if(!FindNextFile(m_Find, &fd))
		{
		FindClose(m_Find);
		m_Find=NULL;
		return false;
		}
	}
auto item_path=String::Create("%s\\%s", path->Begin(), fd.cFileName);
if(fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
	{
	m_Current=Directory::Create(item_path);
	}
else
	{
	m_Current=File::Create(item_path);
	}
return true;
}

BOOL DirectoryIterator::MoveNext()
{
if(!m_Find)
	{
	m_Current=nullptr;
	return false;
	}
WIN32_FIND_DATA fd={ 0 };
if(!FindNextFile(m_Find, &fd))
	{
	FindClose(m_Find);
	m_Find=NULL;
	m_Current=nullptr;
	return false;
	}
auto path=m_Directory->GetPath();
auto item_path=String::Create("%s\\%s", path->Begin(), fd.cFileName);
if(fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
	{
	m_Current=Directory::Create(item_path);
	}
else
	{
	m_Current=File::Create(item_path);
	}
return true;
}

}}
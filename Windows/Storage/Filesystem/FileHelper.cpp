//================
// FileHelper.cpp
//================

#include "Storage/Filesystem/FileHelper.h"


//=======
// Using
//=======

#pragma comment(lib, "Shlwapi")

#include <fileapi.h>
#include <Shlwapi.h>
#include "PathHelper.h"


//===========
// Namespace
//===========

namespace Storage {
	namespace Filesystem {


//========
// Common
//========

BOOL CreateDirectoryTree(LPCTSTR path)
{
if(!path)
	return false;
if(DirectoryExists(path))
	return true;
UINT dir_len=PathHelper::GetDirectoryLength(path);
if(dir_len>0)
	{
	auto parent=String::Create(dir_len-1, path);
	if(!CreateDirectoryTree(parent->Begin()))
		return false;
	}
return CreateDirectory(&path[dir_len], nullptr);
}

BOOL DeleteDirectoryTree(LPCTSTR path)
{
if(!path)
	return false;
if(!DirectoryExists(path))
	return false;
return RemoveDirectory(path);
}

BOOL DirectoryExists(LPCTSTR path)
{
if(!path)
	return false;
return PathIsDirectory(path);
}

BOOL FileDelete(LPCTSTR path)
{
if(!path)
	return false;
if(FileExists(path))
	return DeleteFile(path);
if(DirectoryExists(path))
	return DeleteDirectoryTree(path);
return false;
}

BOOL FileExists(LPCTSTR path)
{
DWORD att=GetFileAttributes(path);
if(att==INVALID_FILE_ATTRIBUTES)
	return false;
if(att&FILE_ATTRIBUTE_DIRECTORY)
	return false;
return true;
}

UINT FileGetAccessMode(FileAccessMode access)
{
switch(access)
	{
	case FileAccessMode::ReadOnly:
		return GENERIC_READ;
	case FileAccessMode::ReadWrite:
		return GENERIC_READ|GENERIC_WRITE;
	}
return 0;
}

BOOL FileGetBasicInfo(LPCTSTR path, FILE_BASIC_INFO& info)
{
if(!path)
	return false;
#ifndef _UNICODE
WCHAR wpath[MAX_PATH];
StringHelper::Copy(wpath, MAX_PATH, path);
HANDLE file=CreateFile2(wpath, FILE_READ_ATTRIBUTES, FILE_SHARE_ALL, OPEN_EXISTING, nullptr);
#else
HANDLE file=CreateFile2(path, FILE_READ_ATTRIBUTES, FILE_SHARE_ALL, OPEN_EXISTING, nullptr);
#endif
if(file==INVALID_HANDLE_VALUE)
	return false;
if(!file)
	return false;
BOOL status=GetFileInformationByHandleEx(file, FileBasicInfo, &info, sizeof(FILE_BASIC_INFO));
CloseHandle(file);
return status;
}

UINT FileGetCreateMode(FileCreateMode create)
{
switch(create)
	{
	case FileCreateMode::CreateAlways:
		return CREATE_ALWAYS;
	case FileCreateMode::CreateNew:
		return CREATE_NEW;
	case FileCreateMode::OpenAlways:
		return OPEN_ALWAYS;
	case FileCreateMode::OpenExisting:
		return OPEN_EXISTING;
	}
return 0;
}

UINT FileGetShareMode(FileShareMode share)
{
switch(share)
	{
	case FileShareMode::Exclusive:
		return 0;
	case FileShareMode::ShareRead:
		return FILE_SHARE_READ;
	case FileShareMode::ShareWrite:
		return FILE_SHARE_READ|FILE_SHARE_WRITE;
	}
return 0;
}

Handle<String> FileGetTemporaryPath(LPCTSTR prefix)
{
TCHAR tmp_path[MAX_PATH];
GetTempPath(MAX_PATH, tmp_path);
TCHAR path[MAX_PATH];
GetTempFileName(tmp_path, prefix, 0, path);
return path;
}

UINT GetFileCount(LPCTSTR mask)
{
if(!mask)
	return 0;
WIN32_FIND_DATA fd={ 0 };
HANDLE find=FindFirstFileEx(mask, FindExInfoBasic, &fd, FindExSearchNameMatch, nullptr, FIND_FIRST_EX_LARGE_FETCH);
if(!find||find==INVALID_HANDLE_VALUE)
	return 0;
UINT count=1;
while(FindNextFile(find, &fd)!=0)
	count++;
FindClose(find);
return count;
}

UINT64 GetFileSize(HANDLE file)
{
if(!file)
	return 0;
LARGE_INTEGER li;
if(!GetFileSizeEx(file, &li))
	return 0;
return li.QuadPart;
}

Handle<String> GetNextFileName(LPCTSTR path)
{
if(!path)
	return nullptr;
UINT dir_len=PathHelper::GetDirectoryLength(path);
Handle<String> dir;
LPCTSTR dir_ptr=nullptr;
if(dir_len>0)
	{
	dir=String::Create(dir_len, path);
	dir_ptr=dir->Begin();
	}
auto file_name=&path[dir_len];
Handle<String> new_name;
TCHAR name[MAX_PATH];
TCHAR ext[8];
UINT id=0;
if(StringHelper::Scan(file_name, "%s (%u).%s", name, MAX_PATH, &id, ext, 8)==3)
	return String::Create("%s%s (%u).%s", dir_ptr, name, id+1, ext);
if(StringHelper::Scan(file_name, "%s.%s", name, MAX_PATH, ext, 8)==2)
	 return String::Create("%s%s (%u).%s", dir_ptr, name, 1, ext);
return nullptr;
}

}}
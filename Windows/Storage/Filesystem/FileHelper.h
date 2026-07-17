//==============
// FileHelper.h
//==============

#pragma once


//=======
// Using
//=======

#include "Storage/File.h"
#include "BitHelper.h"
#include "StringClass.h"


//=============
// Definitions
//=============

#define FILE_SHARE_ALL (FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE)


//===========
// Namespace
//===========

namespace Storage {
	namespace Filesystem {


//========
// Common
//========

class FileHelper
{
public:
	static BOOL CreateDirectoryTree(LPCTSTR Path);
	static BOOL Delete(LPCTSTR Path);
	static BOOL DeleteDirectoryTree(LPCTSTR Path);
	static BOOL DirectoryExists(LPCTSTR Path);
	static BOOL FileExists(LPCTSTR Path);
	static UINT GetAccessMode(FileAccessMode Access);
	static BOOL GetBasicInfo(LPCTSTR Path, FILE_BASIC_INFO& Info);
	static UINT GetCreateMode(FileCreateMode Create);
	static UINT GetFileCount(LPCTSTR Mask);
	static UINT64 GetFileSize(HANDLE File);
	static Handle<String> GetNextFileName(LPCTSTR Path);
	static UINT GetShareMode(FileShareMode Share);
	static Handle<String> GetTemporaryPath(LPCTSTR Prefix);
	static BOOL IsAccessReadOnly(UINT Access);
};

}}
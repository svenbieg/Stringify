//==============
// FileHelper.h
//==============

#pragma once


//=======
// Using
//=======

#include "Storage/File.h"


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

BOOL CreateDirectoryTree(LPCTSTR Path);
BOOL DeleteDirectoryTree(LPCTSTR Path);
BOOL DirectoryExists(LPCTSTR Path);
BOOL FileDelete(LPCTSTR Path);
BOOL FileExists(LPCTSTR Path);
UINT FileGetAccessMode(FileAccessMode Access);
BOOL FileGetBasicInfo(LPCTSTR Path, FILE_BASIC_INFO& Info);
UINT FileGetCreateMode(FileCreateMode Create);
UINT FileGetShareMode(FileShareMode Share);
Handle<String> FileGetTemporaryPath(LPCTSTR Prefix);
UINT GetFileCount(LPCTSTR Mask);
UINT64 GetFileSize(HANDLE File);
Handle<String> GetNextFileName(LPCTSTR Path);
inline BOOL IsAccessReadOnly(UINT Access) { return !(Access&(GENERIC_WRITE|FILE_WRITE_DATA|FILE_APPEND_DATA)); }

}}
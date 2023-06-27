//================
// VolumeFile.cpp
//================

#include "pch.h"


//=======
// Using
//=======

#include "FileHelper.h"
#include "VolumeFile.h"


//===========
// Namespace
//===========

namespace Storage {
	namespace Filesystem {


//==================
// Con-/Destructors
//==================

VolumeFile::VolumeFile():
cOverlapped({ 0 }),
hFile(NULL),
uFlags(VolumeFileFlags::None)
{}

VolumeFile::~VolumeFile()
{
Close();
}


//========
// Common
//========

VOID VolumeFile::Close()
{
if(hFile)
	{
	CloseHandle(hFile);
	if(GetFlag(uFlags, VolumeFileFlags::Temporary))
		DeleteFile(Path->Begin());
	}
Path=nullptr;
ZeroMemory(&cOverlapped, sizeof(OVERLAPPED));
uFlags=VolumeFileFlags::None;
}

Status VolumeFile::Create(Handle<String> path, FileCreateMode create_mode)
{
UINT create=FileGetCreateMode(create_mode);
UINT att=0;
if(!path)
	{
	path=FileGetTemporaryPath(TEXT("vol"));
	att=FILE_ATTRIBUTE_TEMPORARY;
	create=OPEN_EXISTING;
	SetFlag(uFlags, VolumeFileFlags::Temporary);
	}
SetLastError(0);
hFile=CreateFile(path->Begin(), GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, nullptr, create, 0, NULL);
if(hFile==INVALID_HANDLE_VALUE)
	hFile=NULL;
if(hFile==NULL)
	{
	Close();
	return Status::Error;
	}
Path=path;
return Status::Success;
}

BOOL VolumeFile::IsTemporary()
{
return GetFlag(uFlags, VolumeFileFlags::Temporary);
}


//========
// Volume
//========

UINT VolumeFile::GetBlockSize()
{
return PAGE_SIZE;
}

FILE_SIZE VolumeFile::GetSize()
{
return 0;
}

SIZE_T VolumeFile::Read(FILE_SIZE offset, VOID* buf, SIZE_T size)
{
if(!hFile)
	return 0;
cOverlapped.Offset=LOLONG(offset);
cOverlapped.OffsetHigh=HILONG(offset);
DWORD read=0;
if(!ReadFile(hFile, buf, (DWORD)size, &read, &cOverlapped))
	return 0;
return read;
}

BOOL VolumeFile::SetSize(FILE_SIZE size)
{
if(!hFile)
	return false;
LONG size_lo=LOLONG(size);
LONG size_hi=HILONG(size);
DWORD status=SetFilePointer(hFile, size_lo, &size_hi, FILE_BEGIN);
if(status==INVALID_SET_FILE_POINTER)
	return false;
return SetEndOfFile(hFile);
}

SIZE_T VolumeFile::Write(FILE_SIZE offset, VOID const* bufv, SIZE_T size)
{
if(!hFile)
	return 0;
auto buf=(BYTE const*)bufv;
BYTE tmp[PAGE_SIZE];
SIZE_T pos=0;
while(pos<size)
	{
	BYTE const* src=buf;
	SIZE_T copy=size-pos;
	SIZE_T write=copy;
	FILE_SIZE end=offset+copy;
	FILE_SIZE block_end=BlockAlign(end, PAGE_SIZE);
	if(end<block_end)
		{
		FILE_SIZE block_start=block_end-PAGE_SIZE;
		if(offset==block_start)
			{
			FillMemory(tmp, PAGE_SIZE, 0xFF);
			CopyMemory(tmp, buf, copy);
			src=tmp;
			write=PAGE_SIZE;
			}
		else if(offset<block_start)
			{
			copy=(SIZE_T)(block_start-offset);
			write=copy;
			}
		}
	cOverlapped.Offset=LOLONG(offset);
	cOverlapped.OffsetHigh=HILONG(offset);
	DWORD written=0;
	if(!WriteFile(hFile, src, (DWORD)write, &written, &cOverlapped))
		break;
	if(written!=write)
		break;
	offset+=copy;
	buf+=copy;
	pos+=copy;
	}
return pos;
}

}}
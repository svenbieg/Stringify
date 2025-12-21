//================
// VolumeFile.cpp
//================

#include "Storage/Filesystem/VolumeFile.h"


//=======
// Using
//=======

#include "Storage/Filesystem/FileHelper.h"
#include "FlagHelper.h"


//===========
// Namespace
//===========

namespace Storage {
	namespace Filesystem {


//==========
// Settings
//==========

constexpr UINT BLOCK_SIZE=4096;


//==================
// Con-/Destructors
//==================

VolumeFile::VolumeFile():
cOverlapped({ 0 }),
hFile(NULL),
m_Flags(VolumeFileFlags::None)
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
	if(FlagHelper::Get(m_Flags, VolumeFileFlags::Temporary))
		DeleteFile(Path->Begin());
	}
Path=nullptr;
MemoryHelper::Fill(&cOverlapped, sizeof(OVERLAPPED), 0);
m_Flags=VolumeFileFlags::None;
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
	FlagHelper::Set(m_Flags, VolumeFileFlags::Temporary);
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
return FlagHelper::Get(m_Flags, VolumeFileFlags::Temporary);
}


//========
// Volume
//========

UINT VolumeFile::GetBlockSize()
{
return BLOCK_SIZE;
}

FILE_SIZE VolumeFile::GetSize()
{
return 0;
}

SIZE_T VolumeFile::Read(FILE_SIZE offset, VOID* buf, SIZE_T size)
{
if(!hFile)
	return 0;
cOverlapped.Offset=TypeHelper::LowLong(offset);
cOverlapped.OffsetHigh=TypeHelper::HighLong(offset);
DWORD read=0;
if(!ReadFile(hFile, buf, (DWORD)size, &read, &cOverlapped))
	return 0;
return read;
}

BOOL VolumeFile::SetSize(FILE_SIZE size)
{
if(!hFile)
	return false;
LONG size_lo=TypeHelper::LowLong(size);
LONG size_hi=TypeHelper::HighLong(size);
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
BYTE tmp[BLOCK_SIZE];
SIZE_T pos=0;
while(pos<size)
	{
	BYTE const* src=buf;
	SIZE_T copy=size-pos;
	SIZE_T write=copy;
	FILE_SIZE end=offset+copy;
	FILE_SIZE block_end=TypeHelper::AlignUp(end, BLOCK_SIZE);
	if(end<block_end)
		{
		FILE_SIZE block_start=block_end-BLOCK_SIZE;
		if(offset==block_start)
			{
			MemoryHelper::Fill(tmp, BLOCK_SIZE, 0xFF);
			MemoryHelper::Copy(tmp, buf, copy);
			src=tmp;
			write=BLOCK_SIZE;
			}
		else if(offset<block_start)
			{
			copy=(SIZE_T)(block_start-offset);
			write=copy;
			}
		}
	cOverlapped.Offset=TypeHelper::LowLong(offset);
	cOverlapped.OffsetHigh=TypeHelper::HighLong(offset);
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

//==========
// File.cpp
//==========

#include "pch.h"


//=======
// Using
//=======

#include "File.h"


//===========
// Namespace
//===========

namespace Storage {
	namespace Filesystem {


//==================
// Con-/Destructors
//==================

File::File(Handle<String> path):
Storage::File(path),
hFile(NULL),
m_Position(0)
{}


//========
// Common
//========

VOID File::Close()
{
unique_lock lock(m_Mutex);
CloseInternal();
}

Handle<File> File::Create(Handle<String> path, FileCreateMode create, FileAccessMode access, FileShareMode share)
{
Handle<File> file=new File(path);
if(Failed(file->Create(create, access, share)))
	return nullptr;
return file;
}

Status File::Create(FileCreateMode create, FileAccessMode access, FileShareMode share)
{
unique_lock lock(m_Mutex);
CloseInternal();
UINT ucreate=FileGetCreateMode(create);
UINT uacc=FileGetAccessMode(access);
UINT ushare=FileGetShareMode(share);
LPCTSTR path=m_Path->Begin();
SetLastError(0);
HANDLE file=CreateFile(path, uacc, ushare, nullptr, ucreate, 0, NULL);
if(file==INVALID_HANDLE_VALUE)
	file=NULL;
if(file==NULL)
	{
	DWORD err=GetLastError();
	switch(err)
		{
		case ERROR_ACCESS_DENIED:
			return Status::AccessDenied;
		case ERROR_FILE_NOT_FOUND:
			return Status::FileNotFound;
		default:
			break;
		}
	return Status::Error;
	}
hFile=file;
return Status::Success;
}

BOOL File::SetSize(FILE_SIZE size)
{
unique_lock lock(m_Mutex);
if(!hFile)
	return false;
if(!Seek(size))
	return false;
if(!SetEndOfFile(hFile))
	return false;
return true;
}


//==============
// Input-Stream
//==============

SIZE_T File::Available()
{
unique_lock lock(m_Mutex);
return AvailableInternal();
}

SIZE_T File::Read(VOID* buf, SIZE_T size)
{
return Read(buf, size, nullptr);
}

SIZE_T File::Read(VOID* buf, SIZE_T size, BOOL* cancel_ptr)
{
unique_lock lock(m_Mutex);
if(!hFile)
	return 0;
if(!buf)
	{
	SIZE_T available=AvailableInternal();
	SIZE_T copy=Min(size, available);
	m_Position+=copy;
	return copy;
	}
auto buf_ptr=(BYTE*)buf;
OVERLAPPED ov;
ZeroMemory(&ov, sizeof(OVERLAPPED));
SIZE_T pos=0;
while(pos<size)
	{
	UINT copy=Min((UINT)(size-pos), 0x1000000U);
	DWORD read=0;
	ov.Offset=(UINT)m_Position;
	ov.OffsetHigh=(UINT)(m_Position>>32);
	if(!ReadFile(hFile, &buf_ptr[pos], copy, &read, &ov))
		break;
	pos+=read;
	m_Position+=read;
	if(read<copy)
		break;
	if(cancel_ptr)
		{
		if(*cancel_ptr)
			break;
		}
	}
return pos;
}


//===============
// Output-Stream
//===============

VOID File::Flush()
{
unique_lock lock(m_Mutex);
if(!hFile)
	return;
FlushFileBuffers(hFile);
}

SIZE_T File::Write(VOID const* buf, SIZE_T size)
{
return Write(buf, size, nullptr);
}

SIZE_T File::Write(VOID const* buf, SIZE_T size, BOOL* cancel_ptr)
{
unique_lock lock(m_Mutex);
if(!hFile)
	return 0;
auto buf_ptr=(BYTE const*)buf;
OVERLAPPED ov={ 0 };
SIZE_T pos=0;
while(pos<size)
	{
	UINT copy=Min((UINT)(size-pos), 0x1000000U);
	DWORD written=0;
	ov.Offset=(UINT)m_Position;
	ov.OffsetHigh=(UINT)(m_Position>>32);
	if(!WriteFile(hFile, &buf_ptr[pos], copy, &written, &ov))
		break;
	pos+=written;
	m_Position+=written;
	if(written<copy)
		break;
	if(cancel_ptr)
		{
		if(*cancel_ptr)
			break;
		}
	}
return pos;
}


//===========
// Container
//===========

FILE_SIZE File::GetSize()
{
unique_lock lock(m_Mutex);
if(!hFile)
	return 0;
return GetFileSize(hFile);
}

BOOL File::Seek(UINT64 pos)
{
unique_lock lock(m_Mutex);
if(!hFile)
	return false;
LONG lo=(LONG)pos;
LONG hi=(LONG)(pos>>32);
if(SetFilePointer(hFile, lo, &hi, FILE_BEGIN)==INVALID_SET_FILE_POINTER)
	return false;
m_Position=pos;
return true;
}


//================
// Common Private
//================

SIZE_T File::AvailableInternal()
{
if(!hFile)
	return 0;
UINT64 size=GetFileSize(hFile);
UINT64 available=size-m_Position;
if(available>SIZE_MAX)
	return SIZE_MAX;
return (SIZE_T)available;
}

VOID File::CloseInternal()
{
if(hFile!=NULL)
	{
	CloseHandle(hFile);
	hFile=NULL;
	}
}

}}

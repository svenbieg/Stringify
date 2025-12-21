//==========
// File.cpp
//==========

#include "Storage/Filesystem/File.h"


//=======
// Using
//=======

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

Handle<File> File::Create(Handle<String> path, FileCreateMode create, FileAccessMode access, FileShareMode share)
{
Handle<File> file=new File(path);
if(StatusHelper::Failed(file->Create(create, access, share)))
	return nullptr;
return file;
}


//========
// Common
//========

VOID File::Close()
{
WriteLock lock(m_Mutex);
CloseInternal();
}

Status File::Create(FileCreateMode create, FileAccessMode access, FileShareMode share)
{
WriteLock lock(m_Mutex);
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
			return Status::NotFound;
		default:
			break;
		}
	return Status::Error;
	}
m_File=file;
return Status::Success;
}


//==============
// Input-Stream
//==============

SIZE_T File::Available()
{
WriteLock lock(m_Mutex);
return AvailableInternal();
}

SIZE_T File::Read(VOID* buf, SIZE_T size)
{
return Read(buf, size, nullptr);
}

SIZE_T File::Read(VOID* buf, SIZE_T size, BOOL* cancel_ptr)
{
WriteLock lock(m_Mutex);
if(!m_File)
	return 0;
if(!buf)
	{
	SIZE_T available=AvailableInternal();
	SIZE_T copy=TypeHelper::Min(size, available);
	m_Position+=copy;
	return copy;
	}
auto buf_ptr=(BYTE*)buf;
OVERLAPPED ov={ 0 };
SIZE_T pos=0;
while(pos<size)
	{
	UINT copy=TypeHelper::Min((UINT)(size-pos), 0x1000000U);
	DWORD read=0;
	ov.Offset=(UINT)m_Position;
	ov.OffsetHigh=(UINT)(m_Position>>32);
	if(!ReadFile(m_File, &buf_ptr[pos], copy, &read, &ov))
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
WriteLock lock(m_Mutex);
if(!m_File)
	return;
FlushFileBuffers(m_File);
}

SIZE_T File::Write(VOID const* buf, SIZE_T size)
{
return Write(buf, size, nullptr);
}

SIZE_T File::Write(VOID const* buf, SIZE_T size, BOOL* cancel_ptr)
{
WriteLock lock(m_Mutex);
if(!m_File)
	return 0;
auto buf_ptr=(BYTE const*)buf;
OVERLAPPED ov={ 0 };
SIZE_T pos=0;
while(pos<size)
	{
	UINT copy=TypeHelper::Min((UINT)(size-pos), 0x1000000U);
	DWORD written=0;
	ov.Offset=(UINT)m_Position;
	ov.OffsetHigh=(UINT)(m_Position>>32);
	if(!WriteFile(m_File, &buf_ptr[pos], copy, &written, &ov))
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


//==========
// Seekable
//==========

FILE_SIZE File::GetSize()
{
WriteLock lock(m_Mutex);
if(!m_File)
	return 0;
return GetFileSize(m_File);
}

BOOL File::Seek(FILE_SIZE pos)
{
WriteLock lock(m_Mutex);
if(!m_File)
	return false;
LONG lo=(LONG)pos;
LONG hi=(LONG)(pos>>32);
if(SetFilePointer(m_File, lo, &hi, FILE_BEGIN)==INVALID_SET_FILE_POINTER)
	return false;
m_Position=pos;
return true;
}


//==============
// Storage.File
//==============

Handle<String> File::GetName()
{
auto path=m_Path->Begin();
return PathHelper::GetLastComponent(path);
}

Handle<String> File::GetPath()
{
return m_Path;
}

BOOL File::SetSize(FILE_SIZE size)
{
WriteLock lock(m_Mutex);
if(!m_File)
	return false;
if(!Seek(size))
	return false;
if(!SetEndOfFile(m_File))
	return false;
return true;
}


//==========================
// Con-/Destructors Private
//==========================

File::File(Handle<String> path):
m_File(NULL),
m_Path(path),
m_Position(0)
{}


//================
// Common Private
//================

SIZE_T File::AvailableInternal()
{
if(!m_File)
	return 0;
UINT64 size=GetFileSize(m_File);
UINT64 available=size-m_Position;
if(available>SIZE_MAX)
	return SIZE_MAX;
return (SIZE_T)available;
}

VOID File::CloseInternal()
{
if(m_File!=NULL)
	{
	CloseHandle(m_File);
	m_File=NULL;
	}
}

}}
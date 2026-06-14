//====================
// ResourceHelper.cpp
//====================

#include "ResourceHelper.h"


//=======
// Using
//=======

#include "Collections/list.hpp"
#include "Collections/map.hpp"
#include "Graphics/Direct2D/ImagingFactory.h"
#include "Resources/Icons/Icon.h"
#include "Storage/Filesystem/File.h"
#include "Storage/Streams/StreamReader.h"
#include "CommandLine.h"
#include "PathHelper.h"
#include <assert.h>

using namespace Collections;
using namespace Graphics;
using namespace Graphics::Direct2D;
using namespace Resources::Icons;
using namespace Storage;
using namespace Storage::Filesystem;
using namespace Storage::Streams;


//=======
// Types
//=======

#define RC_TYPE_STRING 6
#define RC_TYPE_ICON 3
#define RC_TYPE_ICONGROUP 14


//===========
// Namespace
//===========

namespace Resources {


//============
// Entrypoint
//============

BYTE* g_Base=nullptr;


//=========
// Unicode
//=========

#ifdef _UNICODE
typedef IMAGE_RESOURCE_DIR_STRING_U IMAGE_RESOURCE_DIR_STRING_T;
#else
typedef IMAGE_RESOURCE_DIRECTORY_STRING IMAGE_RESOURCE_DIR_STRING_T;
#endif


//======================
// Forward-Declarations
//======================

IMAGE_RESOURCE_DATA_ENTRY* GetResourceData(IMAGE_RESOURCE_DIRECTORY* Parent, UINT Id);
IMAGE_RESOURCE_DATA_ENTRY* GetResourceData(IMAGE_RESOURCE_DIRECTORY* Parent, LPCSTR Name);
IMAGE_RESOURCE_DIRECTORY* GetResourceDirectory(IMAGE_RESOURCE_DIRECTORY* Parent, UINT Id);
IMAGE_RESOURCE_DIRECTORY* GetResourceDirectory(IMAGE_RESOURCE_DIRECTORY* Parent, LPCSTR Name);
IMAGE_RESOURCE_DIRECTORY* GetResourceRoot();


//============
// Ressources
//============

IMAGE_RESOURCE_DIRECTORY* g_Root=nullptr;

IMAGE_RESOURCE_DATA_ENTRY* GetResourceData(IMAGE_RESOURCE_DIRECTORY* parent, UINT id)
{
BYTE* dir=(BYTE*)parent;
UINT pos=sizeof(IMAGE_RESOURCE_DIRECTORY);
pos+=parent->NumberOfNamedEntries*sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY);
BYTE* root=(BYTE*)g_Root;
IMAGE_RESOURCE_DIRECTORY_ENTRY* entry=(IMAGE_RESOURCE_DIRECTORY_ENTRY*)&dir[pos];
for(UINT u=1; u<parent->NumberOfIdEntries; u++)
	{
	if(entry->Name==id)
		break;
	pos+=sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY);
	entry=(IMAGE_RESOURCE_DIRECTORY_ENTRY*)&dir[pos];
	}
if(entry->DataIsDirectory)
	throw(EINVAL);
return (IMAGE_RESOURCE_DATA_ENTRY*)&root[entry->OffsetToData];
}

IMAGE_RESOURCE_DATA_ENTRY* GetResourceData(IMAGE_RESOURCE_DIRECTORY* parent, LPCSTR name)
{
UINT name_len=StringHelper::Length(name);
assert(name_len>0);
BYTE* dir=(BYTE*)parent;
UINT pos=sizeof(IMAGE_RESOURCE_DIRECTORY);
for(UINT u=0; u<parent->NumberOfNamedEntries; u++)
	{
	IMAGE_RESOURCE_DIRECTORY_ENTRY* entry=(IMAGE_RESOURCE_DIRECTORY_ENTRY*)&dir[pos];
	IMAGE_RESOURCE_DIR_STRING_T* str=(IMAGE_RESOURCE_DIR_STRING_T*)&dir[entry->NameOffset];
	if(str->Length==name_len&&StringHelper::Compare(str->NameString, name, name_len)==0)
		{
		if(entry->DataIsDirectory)
			throw(EINVAL);
		BYTE* root=(BYTE*)g_Root;
		return (IMAGE_RESOURCE_DATA_ENTRY*)&root[entry->OffsetToData];
		}
	pos+=sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY);
	}
throw(EINVAL);
return 0;
}

IMAGE_RESOURCE_DIRECTORY* GetResourceDirectory(IMAGE_RESOURCE_DIRECTORY* parent, UINT id)
{
BYTE* dir=(BYTE*)parent;
UINT pos=sizeof(IMAGE_RESOURCE_DIRECTORY);
pos+=parent->NumberOfNamedEntries*sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY);
for(UINT u=0; u<parent->NumberOfIdEntries; u++)
	{
	IMAGE_RESOURCE_DIRECTORY_ENTRY* entry=(IMAGE_RESOURCE_DIRECTORY_ENTRY*)&dir[pos];
	if(entry->Name==id)
		{
		if(!entry->DataIsDirectory)
			throw(EINVAL);
		BYTE* root=(BYTE*)g_Root;
		return (IMAGE_RESOURCE_DIRECTORY*)&root[entry->OffsetToDirectory];
		}
	pos+=sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY);
	}
throw(EINVAL);
return 0;
}

IMAGE_RESOURCE_DIRECTORY* GetResourceDirectory(IMAGE_RESOURCE_DIRECTORY* parent, LPCSTR name)
{
UINT name_len=StringHelper::Length(name);
assert(name_len>0);
BYTE* dir=(BYTE*)parent;
UINT pos=sizeof(IMAGE_RESOURCE_DIRECTORY);
for(UINT u=0; u<parent->NumberOfNamedEntries; u++)
	{
	IMAGE_RESOURCE_DIRECTORY_ENTRY* entry=(IMAGE_RESOURCE_DIRECTORY_ENTRY*)&dir[pos];
	IMAGE_RESOURCE_DIR_STRING_T* str=(IMAGE_RESOURCE_DIR_STRING_T*)&dir[entry->NameOffset];
	if(str->Length==name_len&&StringHelper::Compare(str->NameString, name, name_len)==0)
		{
		if(!entry->DataIsDirectory)
			throw(EINVAL);
		BYTE* root=(BYTE*)g_Root;
		return (IMAGE_RESOURCE_DIRECTORY*)&root[entry->OffsetToDirectory];
		}
	pos+=sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY);
	}
throw(EINVAL);
return 0;
}

IMAGE_RESOURCE_DIRECTORY* GetResourceRoot()
{
if(g_Root)
	return g_Root;
MEMORY_BASIC_INFORMATION mbi;
VirtualQuery(&g_Root, &mbi, sizeof(MEMORY_BASIC_INFORMATION));
g_Base=(BYTE*)mbi.AllocationBase;
IMAGE_DOS_HEADER* dos=(IMAGE_DOS_HEADER*)g_Base;
if(dos->e_magic!=IMAGE_DOS_SIGNATURE)
	throw(EINVAL);
IMAGE_NT_HEADERS* nt=(IMAGE_NT_HEADERS*)&g_Base[dos->e_lfanew];
if(nt->Signature!=IMAGE_NT_SIGNATURE)
	throw(EINVAL);
UINT file=dos->e_lfanew+4;
UINT section_dir=file+sizeof(IMAGE_FILE_HEADER)+nt->FileHeader.SizeOfOptionalHeader;
UINT section_entry=section_dir;
IMAGE_SECTION_HEADER* rc_section=nullptr;
for(UINT u=0; u<nt->FileHeader.NumberOfSections; u++)
	{
	IMAGE_SECTION_HEADER* p=(IMAGE_SECTION_HEADER*)&g_Base[section_entry];
	if(StringHelper::Compare((LPCSTR)p->Name, ".rsrc", 5)==0)
		{
		rc_section=p;
		break;
		}
	section_entry+=sizeof(IMAGE_SECTION_HEADER);
	}
if(!rc_section)
	throw(EINVAL);
UINT root=rc_section->VirtualAddress;
g_Root=(IMAGE_RESOURCE_DIRECTORY*)&g_Base[root];
return g_Root;
}


//=======
// Icons
//=======

#pragma pack(push)
#pragma pack(2)

typedef struct ICONGROUPENTRY
{
BYTE Width;
BYTE Height;
BYTE ColorCount;
BYTE Reserved;
WORD Planes;
WORD BitCount;
DWORD Size;
WORD Id;
}ICONGROUPENTRY;

typedef struct ICONGROUP
{
WORD Reserved;
WORD Type;
WORD Count;
ICONGROUPENTRY Entries[1];
}ICONGROUP;

#pragma pack(pop)

typedef struct ICONDIR
{
WORD Reserved;
WORD Type;
WORD Count;
}ICONDIR;

typedef struct {
BYTE Width;
BYTE Height;
BYTE ColorCount;
BYTE Reserved;
WORD Planes;
WORD BitCount;
DWORD BytesInRes;
DWORD ImageOffset;
}ICONDIRENTRY;

typedef struct
{
UINT ImageOffset;
UINT Size;
}ICONINFO;


//========
// Common
//========

Handle<Bitmap> ResourceHelper::CreateBitmap(Handle<String> path)
{
path=Lookup(path);
auto factory=ImagingFactory::Create();
auto source=factory->CreateBitmap(path);
UINT width=0;
UINT height=0;
source->GetSize(&width, &height);
ComPointer<IWICBitmapLock> lock;
source->Lock(nullptr, WICBitmapLockRead, lock.AddressOf());
UINT size=0;
WICInProcPointer ptr=nullptr;
lock->GetDataPointer(&size, &ptr);
assert(size==width*height*4);
UINT stride=0;
lock->GetStride(&stride);
auto bmp=Bitmap::Create(width, height, 32);
auto buf=const_cast<BYTE*>(bmp->Begin());
source->CopyPixels(nullptr, stride, size, buf);
return bmp;
}

Handle<Icon> ResourceHelper::CreateIcon(Handle<String> path)
{
path=Lookup(path);
auto file=Filesystem::File::Create(path, FileCreateMode::OpenExisting);
list<ICONINFO> icon_info;
StreamReader reader(file);
ICONDIR dir;
reader.Read(&dir, sizeof(ICONDIR));
for(WORD u=0; u<dir.Count; u++)
	{
	ICONDIRENTRY entry;
	reader.Read(&entry, sizeof(ICONDIRENTRY));
	if(entry.BitCount!=32)
		continue;
	auto& info=icon_info.append();
	info.ImageOffset=entry.ImageOffset;
	info.Size=entry.Width;
	}
UINT count=icon_info.get_count();
if(!count)
	throw NotFoundException();
Icon::IconMap bitmaps;
for(auto& icon: icon_info)
	{
	auto bmp=Bitmap::Create(icon.Size, icon.Size, 32);
	auto buf=const_cast<BYTE*>(bmp->Begin());
	SIZE_T size=icon.Size*icon.Size*4;
	file->Seek(icon.ImageOffset);
	file->Read(buf, size);
	bitmaps.add(icon.Size, bmp);
	}
return Icon::Create(bitmaps);
}

BITMAPINFO* ResourceHelper::GetIcon(UINT id, UINT size)
{
assert(id);
IMAGE_RESOURCE_DIRECTORY* root=GetResourceRoot();
IMAGE_RESOURCE_DIRECTORY* name_dir=GetResourceDirectory(root, RC_TYPE_ICONGROUP);
IMAGE_RESOURCE_DIRECTORY* lng_dir=GetResourceDirectory(name_dir, id);
DWORD lng=0;
GetLocaleInfoEx(LOCALE_NAME_USER_DEFAULT, LOCALE_ILANGUAGE|LOCALE_RETURN_NUMBER, (LPWSTR)&lng, 4);
IMAGE_RESOURCE_DATA_ENTRY* data=nullptr;
try
	{
	data=GetResourceData(lng_dir, lng);
	}
catch(UINT err)
	{
	if(err!=EINVAL)
		throw err;
	lng=0;
	data=GetResourceData(lng_dir, lng);
	}
ICONGROUP* icon_group=(ICONGROUP*)&g_Base[data->OffsetToData];
ICONGROUPENTRY* icon_entries=icon_group->Entries;
UINT icon_count=icon_group->Count;
UINT ico=0;
for(; ico<icon_count; ico++)
	{
	if(!size||icon_entries[ico].Width==size)
		break;
	}
if(ico==icon_count)
	ico=0;
root=GetResourceRoot();
name_dir=GetResourceDirectory(root, RC_TYPE_ICON);
lng_dir=GetResourceDirectory(name_dir, icon_entries[ico].Id);
data=GetResourceData(lng_dir, lng);
return (BITMAPINFO*)&g_Base[data->OffsetToData];
}

UINT ResourceHelper::GetIconCount(UINT id)
{
return GetIconSize(id, nullptr, 0);
}

UINT ResourceHelper::GetIconSize(UINT id, UINT* size_ptr, UINT count)
{
assert(id);
IMAGE_RESOURCE_DIRECTORY* root=GetResourceRoot();
IMAGE_RESOURCE_DIRECTORY* name_dir=GetResourceDirectory(root, RC_TYPE_ICONGROUP);
IMAGE_RESOURCE_DIRECTORY* lng_dir=GetResourceDirectory(name_dir, id);
DWORD lng=0;
GetLocaleInfoEx(LOCALE_NAME_USER_DEFAULT, LOCALE_ILANGUAGE|LOCALE_RETURN_NUMBER, (LPWSTR)&lng, 4);
IMAGE_RESOURCE_DATA_ENTRY* data=nullptr;
try
	{
	data=GetResourceData(lng_dir, lng);
	}
catch(UINT err)
	{
	if(err!=EINVAL)
		throw err;
	lng=0;
	data=GetResourceData(lng_dir, lng);
	}
ICONGROUP* icon_group=(ICONGROUP*)&g_Base[data->OffsetToData];
ICONGROUPENTRY* icon_entries=icon_group->Entries;
UINT icon_count=icon_group->Count;
UINT pos=0;
for(UINT u=0; u<icon_count; u++)
	{
	UINT width=icon_entries[u].Width;
	if(size_ptr)
		{
		if(pos>=count)
			break;
		size_ptr[pos]=width;
		}
	pos++;
	}
return pos;
}


//================
// Common Private
//================

Handle<String> ResourceHelper::Lookup(Handle<String> path)
{
if(FileHelper::FileExists(path))
	return path;
auto cmd_line=CommandLine::Create();
auto exe=cmd_line->Arguments->GetAt(0);
auto root=PathHelper::GetDirectory(exe->Begin());
auto full_path=String::Create("%s\\..\\%s", root, path);
if(FileHelper::FileExists(full_path))
	return full_path;
TCHAR current[MAX_PATH];
GetCurrentDirectory(MAX_PATH, current);
full_path=String::Create("%s\\..\\AppX\\%s", current, path);
if(FileHelper::FileExists(full_path))
	return full_path;
throw NotFoundException();
}

}
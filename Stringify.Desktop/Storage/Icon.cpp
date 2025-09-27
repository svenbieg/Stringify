//==========
// Icon.cpp
//==========

#include "pch.h"


//=======
// Using
//=======

#include "Storage/Filesystem/File.h"
#include "Storage/Streams/StreamReader.h"
#include "Storage/Icon.h"

using namespace Storage::Streams;


//===========
// Namespace
//===========

namespace Storage {


//========
// Header
//========

typedef struct
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


//==========================
// Con-/Destructors Private
//==========================

Icon::Icon(Handle<String> path)
{
auto file=Filesystem::File::Create(path, FileCreateMode::OpenExisting);
if(!file)
	return;
StreamReader reader(file);
ICONDIR dir;
reader.Read(&dir, sizeof(ICONDIR));
for(WORD u=0; u<dir.Count; u++)
	{
	ICONDIRENTRY entry;
	reader.Read(&entry, sizeof(ICONDIRENTRY));
	if(entry.BitCount!=32)
		continue;
	ICON& icon=m_Icons.append();
	icon.Position=entry.ImageOffset+0x28;
	icon.Size=entry.Width*entry.Height*4;
	icon.Width=entry.Width;
	}
for(auto it=m_Icons.begin(); it.has_current(); it.move_next())
	{
	ICON& icon=it.get_current();
	icon.Buffer=Buffer::Create(icon.Size);
	auto buf=icon.Buffer->Begin();
	UINT end=icon.Position+icon.Size;
	UINT height=icon.Width;
	UINT line_size=icon.Width*4;
	for(UINT line=0; line<height; line++)
		{
		file->Seek(end-(line+1)*line_size);
		file->Read(&buf[line*line_size], line_size);
		}
	}
}

}
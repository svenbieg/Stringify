﻿//=================
// Application.cpp
//=================

#include "pch.h"


//=======
// Using
//=======

#include "Concurrency/DispatchedQueue.h"
#include "Concurrency/Task.h"
#include "Resources/Strings/Application.h"
#include "Storage/Filesystem/File.h"
#include "Storage/Streams/StreamReader.h"
#include "Storage/Streams/StreamWriter.h"
#include "Storage/Buffer.h"
#include "Application.h"
#include "PathHelper.h"

using namespace Concurrency;
using namespace Resources::Strings;
using namespace Storage;
using namespace Storage::Streams;
using namespace UI::Controls;


//=============
// Entry-Point
//=============

INT Main()
{
Handle<Stringify::Application> app=new Stringify::Application();
return app->Run();
}


//===========
// Namespace
//===========

namespace Stringify {


//==========
// Settings
//==========

constexpr UINT LINE_LEN=128;


//=============
// Translation
//=============

LPCSTR StringTable[256]=
	{
	//   0        1        2        3        4        5        6        7        8        9        A        B        C        D        E        F
	  "\\0",     "",     "",     "",     "",     "",     "",   "\\a",   "\\b",   "\\t",   "\\n",   "\\v",   "\\f",   "\\r",     "",     "", // 0x00
	    "",     "",     "",     "",     "",     "",     "",     "",     "",     "", "\\x1A",     "",     "",     "",     "",     "", // 0x10
	    " ",     "!",  "\\\"",     "#",     "$",     "%",     "&",     "'",     "(",     ")",     "*",     "+",     ",",     "-",     ".",     "/", // 0x20
	    "0",     "1",     "2",     "3",     "4",     "5",     "6",     "7",     "8",     "9",     ":",     ";",     "<",     "=",     ">",     "?", // 0x30
	    "@",     "A",     "B",     "C",     "D",     "E",     "F",     "G",     "H",     "I",     "J",     "K",     "L",     "M",     "N",     "O", // 0x40
	    "P",     "Q",     "R",     "S",     "T",     "U",     "V",     "W",     "X",     "Y",     "Z",     "[",  "\\\\",     "]",     "^",     "_", // 0x50
	    "`",     "a",     "b",     "c",     "d",     "e",     "f",     "g",     "h",     "i",     "j",     "k",     "l",     "m",     "n",     "o", // 0x60
	    "p",     "q",     "r",     "s",     "t",     "u",     "v",     "w",     "x",     "y",     "z",     "{",     "|",     "}",     "~", "\\x7F", // 0x70
	    "€", "\\x81",     "‚",     "ƒ",     "„",     "…",     "†",     "‡",     "ˆ",     "‰",     "Š",     "‹",     "Œ", "\\x8D",     "Ž", "\\x8F", // 0x80
	"\\x90",     "‘",     "’",     "“",     "”",     "•",     "–",     "—",     "˜",     "™",     "š",     "›",     "œ", "\\x9D",     "ž",     "Ÿ", // 0x90
	"\\xA0",     "¡",     "¢",     "£",     "¤",     "¥",     "¦",     "§",     "¨",     "©",     "ª",     "«",     "¬", "\\xAD",     "®",     "¯", // 0xA0
	    "°",     "±",     "²",     "³",     "´",     "µ",     "¶",     "·",     "¸",     "¹",     "º",     "»",     "¼",     "½",     "¾",     "¿", // 0xB0
	    "À",     "Á",     "Â",     "Ã",     "Ä",     "Å",     "Æ",     "Ç",     "È",     "É",     "Ê",     "Ë",     "Ì",     "Í",     "Î",     "Ï", // 0xC0
	    "Ð",     "Ñ",     "Ò",     "Ó",     "Ô",     "Õ",     "Ö",     "×",     "Ø",     "Ù",     "Ú",     "Û",     "Ü",     "Ý",     "Þ",     "ß", // 0xD0
	    "à",     "á",     "â",     "ã",     "ä",     "å",     "æ",     "ç",     "è",     "é",     "ê",     "ë",     "ì",     "í",     "î",     "ï", // 0xE0
	    "ð",     "ñ",     "ò",     "ó",     "ô",     "õ",     "ö",     "÷",     "ø",     "ù",     "ú",     "û",     "ü",     "ý",     "þ",     "ÿ", // 0xF0
	};


//==================
// Con-/Destructors
//==================

Application::Application():
Desktop::Application(STR_APP_TITLE)
{
Current=this;
m_Window=new AppWindow();
}


//========
// Common
//========

Application* Application::Current=nullptr;

VOID Application::Open(Handle<String> path)
{
if(!path)
	return;
LPCTSTR ext=PathHelper::GetExtension(path->Begin());
if(!ext)
	return;
if(StringHelper::Compare(ext, "ico", 0, false)==0)
	{
	OpenIcon(path);
	return;
	}
OpenBinary(path);
}


//================
// Common Private
//================

VOID Application::Convert(Handle<InputStream> stream)
{
auto result_box=m_Window->ResultBox;
result_box->Clear();
result_box->Enabled=true;
result_box->ReadOnly=true;
Handle<Intermediate> buf=Intermediate::Create(PAGE_SIZE);
buf->SetFormat(StreamFormat::Ansi);
m_ConvertTask=Task::Create(this, [this, buf, stream](){ DoConvert(buf, stream); });
m_ParseTask=Task::Create(this, [this, buf](){ DoParse(buf); });
}

VOID Application::DoConvert(Handle<Intermediate> dst, Handle<InputStream> src)
{
auto task=Task::Get();
StreamWriter writer(dst);
writer.PrintChar('\"');
SIZE_T line_len=0;
BYTE buf[PAGE_SIZE];
while(!task->Cancelled)
	{
	SIZE_T read=src->Read(buf, PAGE_SIZE);
	if(!read||task->Cancelled)
		break;
	for(UINT u=0; u<read; u++)
		{
		if(line_len>=LINE_LEN)
			{
			writer.Print("\"\r\n\"");
			dst->Flush();
			line_len=0;
			}
		SIZE_T written=writer.Print(StringTable[buf[u]]);
		if(!written)
			break;
		line_len+=written;
		}
	}
writer.PrintChar('\"');
writer.PrintChar('\0');
dst->Flush();
}

VOID Application::DoParse(Handle<Intermediate> stream)
{
ScopedLock lock(m_Mutex);
auto task=Task::Get();
auto result_box=m_Window->ResultBox;
StreamReader reader(stream);
CHAR buf[LINE_LEN+8];
while(!task->Cancelled)
	{
	lock.Unlock();
	UINT len=reader.ReadString(buf, LINE_LEN+8, '\r');
	lock.Lock();
	if(!len||task->Cancelled)
		break;
	if(reader.LastChar=='\r')
		reader.Skip();
	auto line=String::Create(buf);
	DispatchedQueue::Append(result_box, [result_box, line]() { result_box->AppendLine(line); });
	if(reader.LastChar==0)
		break;
	}
if(!task->Cancelled)
	DispatchedQueue::Append(this, &Application::OnComplete);
}

VOID Application::OnComplete()
{
m_ConvertTask=nullptr;
m_ParseTask=nullptr;
auto result_box=m_Window->ResultBox;
result_box->ReadOnly=false;
result_box->SetFocus();
result_box->SelectAll();
}

VOID Application::OpenBinary(Handle<String> path)
{
Handle<Filesystem::File> file=new Filesystem::File(path);
if(file->Create()!=Status::Success)
	return;
Convert(file);
}

VOID Application::OpenBitmap(HBITMAP hbmp)
{
BITMAP bmp;
INT status=GetObject(hbmp, sizeof(BITMAP), &bmp);
if(status==0)
	return;
UINT size=bmp.bmWidthBytes*bmp.bmHeight;
auto buf=Buffer::Create(size);
GetBitmapBits(hbmp, size, buf->Begin());
Convert(buf);
}

VOID Application::OpenIcon(Handle<String> path)
{
HICON icon=(HICON)LoadImage(NULL, path->Begin(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE|LR_LOADTRANSPARENT);
if(icon==INVALID_HANDLE_VALUE)
	icon=NULL;
if(icon==NULL)
	return;
ICONINFO info;
GetIconInfo(icon, &info);
OpenBitmap(info.hbmColor);
DestroyIcon(icon);
}

}

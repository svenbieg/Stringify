//=================
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
#include "Storage/Icon.h"
#include "Storage/StaticBuffer.h"
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
auto app=Stringify::Application::Create();
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
	    "",     "",     "",     "",     "",     "",     "",     "",     "",     "",  "\\32",     "",     "",     "",     "",     "", // 0x10
	    " ",     "!",  "\\\"",     "#",     "$",     "%",     "&",     "'",     "(",     ")",     "*",     "+",     ",",     "-",     ".",     "/", // 0x20
	    "0",     "1",     "2",     "3",     "4",     "5",     "6",     "7",     "8",     "9",     ":",     ";",     "<",     "=",     ">",     "?", // 0x30
	    "@",     "A",     "B",     "C",     "D",     "E",     "F",     "G",     "H",     "I",     "J",     "K",     "L",     "M",     "N",     "O", // 0x40
	    "P",     "Q",     "R",     "S",     "T",     "U",     "V",     "W",     "X",     "Y",     "Z",     "[",    "\\",     "]",     "^",     "_", // 0x50
	    "`",     "a",     "b",     "c",     "d",     "e",     "f",     "g",     "h",     "i",     "j",     "k",     "l",     "m",     "n",     "o", // 0x60
	    "p",     "q",     "r",     "s",     "t",     "u",     "v",     "w",     "x",     "y",     "z",     "{",     "|",     "}",     "~", "\\177", // 0x70
	    "€", "\\201",     "‚",     "ƒ",     "„",     "…",     "†",     "‡",     "ˆ",     "‰",     "Š",     "‹",     "Œ", "\\215",     "Ž", "\\217", // 0x80
	"\\220",     "‘",     "’",     "“",     "”",     "•",     "–",     "—",     "˜",     "™",     "š",     "›",     "œ", "\\235",     "ž",     "Ÿ", // 0x90
	"\\240",     "¡",     "¢",     "£",     "¤",     "¥",     "¦",     "§",     "¨",     "©",     "ª",     "«",     "¬", "\\255",     "®",     "¯", // 0xA0
	    "°",     "±",     "²",     "³",     "´",     "µ",     "¶",     "·",     "¸",     "¹",     "º",     "»",     "¼",     "½",     "¾",     "¿", // 0xB0
	    "À",     "Á",     "Â",     "Ã",     "Ä",     "Å",     "Æ",     "Ç",     "È",     "É",     "Ê",     "Ë",     "Ì",     "Í",     "Î",     "Ï", // 0xC0
	    "Ð",     "Ñ",     "Ò",     "Ó",     "Ô",     "Õ",     "Ö",     "×",     "Ø",     "Ù",     "Ú",     "Û",     "Ü",     "Ý",     "Þ",     "ß", // 0xD0
	    "à",     "á",     "â",     "ã",     "ä",     "å",     "æ",     "ç",     "è",     "é",     "ê",     "ë",     "ì",     "í",     "î",     "ï", // 0xE0
	    "ð",     "ñ",     "ò",     "ó",     "ô",     "õ",     "ö",     "÷",     "ø",     "ù",     "ú",     "û",     "ü",     "ý",     "þ",     "ÿ", // 0xF0
	};


//========
// Common
//========

Application* Application::Current=nullptr;

VOID Application::Open(Handle<String> path)
{
auto result_box=m_Window->ResultBox;
result_box->ReadOnly=true;
result_box->Clear();
if(!path)
	return;
LPCTSTR ext=PathHelper::GetExtension(path->Begin());
if(!ext)
	return;
if(StringHelper::Compare(ext, "ico", 0, false)==0)
	{
	OpenIcon(path);
	}
else
	{
	OpenBinary(path);
	}
result_box->ReadOnly=false;
result_box->SetFocus();
result_box->SelectAll();
}


//==========================
// Con-/Destructors Private
//==========================

Application::Application():
Desktop::Application(STR_APP_TITLE)
{
Current=this;
m_Window=AppWindow::Create();
}


//================
// Common Private
//================

VOID Application::DoParse(Handle<Intermediate> stream)
{
WriteLock lock(m_Mutex);
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
}

VOID Application::OpenBinary(Handle<String> path)
{
auto file=Filesystem::File::Create(path);
if(file->Create()!=Status::Success)
	return;
auto name=PathHelper::GetName(path->Begin());
auto var=String::Create("BIN_%S", name->Begin());
Stringify(var, file);
}

VOID Application::OpenIcon(Handle<String> path)
{
auto icon=Icon::Create(path);
auto name=PathHelper::GetName(path);
for(auto it=icon->cbegin(); it.has_current(); it.move_next())
	{
	auto ico=it.get_current();
	auto var=String::Create("BMP_%S_%u", name, ico.Width);
	Stringify(var, ico.Buffer);
	}
}

VOID Application::StretchOctal(LPSTR dst, LPCSTR src)
{
UINT len=StringHelper::Length(&src[1]);
UINT pos=0;
dst[pos++]='\\';
UINT stretch=3-len;
for(UINT u=0; u<stretch; u++)
	dst[pos++]='0';
for(UINT u=0; u<len; u++)
	dst[pos++]=src[u+1];
dst[4]=0;
}

VOID Application::Stringify(Handle<String> name, InputStream* src)
{
auto str=String::Create("constexpr char %s[]=", name);
DispatchedQueue::Append(this, [this, str]()
	{
	m_Window->ResultBox->AppendLine(str);
	});
CHAR buf[LINE_LEN+8];
auto dst=StaticBuffer::Create(buf, LINE_LEN+8);
dst->SetFormat(StreamFormat::Ansi);
StreamWriter writer(dst);
SIZE_T line_len=writer.Print("\"");
BYTE byte=0;
SIZE_T read=src->Read(&byte, 1);
while(1)
	{
	if(line_len>=LINE_LEN)
		{
		writer.Print("\"");
		writer.PrintChar('\0');
		auto str=String::Create(buf);
		DispatchedQueue::Append(this, [this, str]()
			{
			m_Window->ResultBox->AppendLine(str);
			});
		dst->Reset();
		line_len=writer.Print("\"");
		}
	LPCSTR write=StringTable[byte];
	if(CharHelper::IsDigit(write[1], 8))
		{
		BYTE next_byte=0;
		read=src->Read(&next_byte, 1);
		if(read)
			{
			CHAR c=(CHAR)next_byte;
			if(CharHelper::IsDigit(c, 8))
				{
				CHAR stretch[5];
				StretchOctal(stretch, write);
				SIZE_T written=writer.Print(stretch);
				if(written!=4)
					throw DeviceNotReadyException();
				line_len+=written;
				}
			else
				{
				SIZE_T written=writer.Print(write);
				if(!written)
					throw DeviceNotReadyException();
				line_len+=written;
				}
			byte=next_byte;
			continue;
			}
		}
	SIZE_T written=writer.Print(write);
	if(!written)
		throw DeviceNotReadyException();
	line_len+=written;
	read=src->Read(&byte, 1);
	if(!read)
		break;
	}
writer.Print("\";");
writer.PrintChar('\0');
str=String::Create(buf);
DispatchedQueue::Append(this, [this, str]()
	{
	m_Window->ResultBox->AppendLine(str);
	m_Window->ResultBox->AppendLine("");
	});
}

}
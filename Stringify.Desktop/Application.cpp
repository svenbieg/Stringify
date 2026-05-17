//=================
// Application.cpp
//=================

#include "Application.h"


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
INT status=app->Run();
return status;
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

const LPCSTR STR_TABLE[256]=
	{
	  "\\0","\\x01","\\x02","\\x03","\\x04","\\x05","\\x06",  "\\a",  "\\b",  "\\t",  "\\n",  "\\v",  "\\f",  "\\r","\\x0E","\\x0F",
	"\\x10","\\x11","\\x12","\\x13","\\x14","\\x15","\\x16","\\x17","\\x18","\\x19","\\x1A","\\x1B","\\x1C","\\x1D","\\x1E","\\x1F",
	    " ",    "!", "\\\"",    "#",    "$",    "%",    "&",    "'",    "(",    ")",    "*",    "+",    ",",    "-",    ".",    "/",
	    "0",    "1",    "2",    "3",    "4",    "5",    "6",    "7",    "8",    "9",    ":",    ";",    "<",    "=",    ">",    "?",
	    "@",    "A",    "B",    "C",    "D",    "E",    "F",    "G",    "H",    "I",    "J",    "K",    "L",    "M",    "N",    "O",
	    "P",    "Q",    "R",    "S",    "T",    "U",    "V",    "W",    "X",    "Y",    "Z",    "[", "\\\\",    "]",    "^",    "_",
	    "`",    "a",    "b",    "c",    "d",    "e",    "f",    "g",    "h",    "i",    "j",    "k",    "l",    "m",    "n",    "o",
	    "p",    "q",    "r",    "s",    "t",    "u",    "v",    "w",    "x",    "y",    "z",    "{",    "|",    "}",    "~","\\x7F",
	"\\x80","\\x81","\\x82","\\x83","\\x84","\\x85","\\x86","\\x87","\\x88","\\x89","\\x8A","\\x8B","\\x8C","\\x8D","\\x8E","\\x8F",
	"\\x90","\\x91","\\x92","\\x93","\\x94","\\x95","\\x96","\\x97","\\x98","\\x99","\\x9A","\\x9B","\\x9C","\\x9D","\\x9E","\\x9F",
	"\\xA0","\\xA1","\\xA2","\\xA3","\\xA4","\\xA5","\\xA6","\\xA7","\\xA8","\\xA9","\\xAA","\\xAB","\\xAC","\\xAD","\\xAE","\\xAF",
	"\\xB0","\\xB1","\\xB2","\\xB3","\\xB4","\\xB5","\\xB6","\\xB7","\\xB8","\\xB9","\\xBA","\\xBB","\\xBC","\\xBD","\\xBE","\\xBF",
	"\\xC0","\\xC1","\\xC2","\\xC3","\\xC4","\\xC5","\\xC6","\\xC7","\\xC8","\\xC9","\\xCA","\\xCB","\\xCC","\\xCD","\\xCE","\\xCF",
	"\\xD0","\\xD1","\\xD2","\\xD3","\\xD4","\\xD5","\\xD6","\\xD7","\\xD8","\\xD9","\\xDA","\\xDB","\\xDC","\\xDD","\\xDE","\\xDF",
	"\\xE0","\\xE1","\\xE2","\\xE3","\\xE4","\\xE5","\\xE6","\\xE7","\\xE8","\\xE9","\\xEA","\\xEB","\\xEC","\\xED","\\xEE","\\xEF",
	"\\xF0","\\xF1","\\xF2","\\xF3","\\xF4","\\xF5","\\xF6","\\xF7","\\xF8","\\xF9","\\xFA","\\xFB","\\xFC","\\xFD","\\xFE","\\xFF"
	};


//========
// Common
//========

Application* Application::Current=nullptr;

VOID Application::Open(Handle<String> path)
{
if(!path)
	return;
auto result_box=m_Window->ResultBox;
result_box->Enabled=false;
result_box->Clear();
auto task=Task::Create(this, [this, path]()
	{
	LPCTSTR ext=PathHelper::GetExtension(path->Begin());
	if(!ext)
		return;
	if(StringHelper::Compare(ext, "ico")==0)
		{
		OpenIcon(path);
		}
	else
		{
		OpenBinary(path);
		}
	});
task->Then(this, [this]()
	{
	auto result_box=m_Window->ResultBox;
	result_box->Enabled=true;
	result_box->SetFocus();
	result_box->SelectAll();
	});
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
auto result_box=m_Window->ResultBox;
auto str=String::Create("constexpr char %s[]=", name);
result_box->AppendLine(str);
CHAR buf[LINE_LEN+8];
auto dst=StaticBuffer::Create(buf, LINE_LEN+8);
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
		result_box->AppendLine(str);
		dst->Reset();
		line_len=writer.Print("\"");
		}
	LPCSTR write=STR_TABLE[byte];
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
result_box->AppendLine(str);
result_box->AppendLine("");
}

}
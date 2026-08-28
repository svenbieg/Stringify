//=================
// Application.cpp
//=================

#include "Application.h"


//=======
// Using
//=======

#include "Concurrency/DispatchedQueue.h"
#include "Concurrency/Task.h"
#include "Desktop/Dialogs/MessageBox.h"
#include "Graphics/Ocr/OcrScanner.h"
#include "Resources/Strings/Application.h"
#include "Resources/ResourceHelper.h"
#include "Storage/Filesystem/File.h"
#include "Storage/Streams/StreamReader.h"
#include "Storage/Streams/StreamWriter.h"
#include "Storage/Buffer.h"
#include "Storage/StaticBuffer.h"
#include "PathHelper.h"

using namespace Collections;
using namespace Concurrency;
using namespace Desktop::Dialogs;
using namespace Graphics::Ocr;
using namespace Resources;
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
	  "\\0",  "\\1",  "\\2",  "\\3",  "\\4",  "\\5",  "\\6",  "\\a",  "\\b",  "\\t",  "\\n",  "\\v",  "\\f",  "\\r", "\\16", "\\17",
	 "\\20", "\\21", "\\22", "\\23", "\\24", "\\25", "\\26", "\\27", "\\30", "\\31", "\\32", "\\33", "\\34", "\\35", "\\36", "\\37",
	    " ",    "!", "\\\"",    "#",    "$",    "%",    "&",    "'",    "(",    ")",    "*",    "+",    ",",    "-",    ".",    "/",
	 "\\60", "\\61", "\\62", "\\63", "\\64", "\\65", "\\66", "\\67", "\\70", "\\71",    ":",    ";",    "<",    "=",    ">",    "?",
	    "@",    "A",    "B",    "C",    "D",    "E",    "F",    "G",    "H",    "I",    "J",    "K",    "L",    "M",    "N",    "O",
	    "P",    "Q",    "R",    "S",    "T",    "U",    "V",    "W",    "X",    "Y",    "Z",    "[", "\\\\",    "]",    "^",    "_",
	    "`",    "a",    "b",    "c",    "d",    "e",    "f",    "g",    "h",    "i",    "j",    "k",    "l",    "m",    "n",    "o",
	    "p",    "q",    "r",    "s",    "t",    "u",    "v",    "w",    "x",    "y",    "z",    "{",    "|",    "}",    "~","\\177",
	"\\200","\\201","\\202","\\203","\\204","\\205","\\206","\\207","\\210","\\211","\\212","\\213","\\214","\\215","\\216","\\217",
	"\\220","\\221","\\222","\\223","\\224","\\225","\\226","\\227","\\230","\\231","\\232","\\233","\\234","\\235","\\236","\\237",
	"\\240","\\241","\\242","\\243","\\244","\\245","\\246","\\247","\\250","\\251","\\252","\\253","\\254","\\255","\\256","\\257",
	"\\260","\\261","\\262","\\263","\\264","\\265","\\266","\\267","\\270","\\271","\\272","\\273","\\274","\\275","\\276","\\277",
	"\\300","\\301","\\302","\\303","\\304","\\305","\\306","\\307","\\310","\\311","\\312","\\313","\\314","\\315","\\316","\\317",
	"\\320","\\321","\\322","\\323","\\324","\\325","\\326","\\327","\\330","\\331","\\332","\\333","\\334","\\335","\\336","\\337",
	"\\340","\\341","\\342","\\343","\\344","\\345","\\346","\\347","\\350","\\351","\\352","\\353","\\354","\\355","\\356","\\357",
	"\\360","\\361","\\362","\\363","\\364","\\365","\\366","\\367","\\370","\\371","\\372","\\373","\\374","\\375","\\376","\\377"
	};


//========
// Common
//========

Application* Application::Current=nullptr;

VOID Application::Open(Handle<String> path)
{
if(!path)
	return;
LPCTSTR ext=PathHelper::GetExtension(path);
if(StringHelper::Compare(ext, "bmp", 0, CompareMode::IgnoreCase)==0)
	{
	ScanImage(path);
	}
else if(StringHelper::Compare(ext, "jpg", 0, CompareMode::IgnoreCase)==0)
	{
	ScanImage(path);
	}
else if(StringHelper::Compare(ext, "png", 0, CompareMode::IgnoreCase)==0)
	{
	ScanImage(path);
	}
else if(StringHelper::Compare(ext, "ico", 0, CompareMode::IgnoreCase)==0)
	{
	OpenIcon(path);
	}
else
	{
	auto path_edit=m_Window->Path;
	path_edit->Clear();
	auto result_box=m_Window->Result;
	result_box->Clear();
	result_box->Enabled=false;
	MessageBox::Error(STR_APP_TITLE, STR_FILE_TYPE_NOT_SUPPORTED);
	}
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

VOID Application::OpenIcon(Handle<String> path)
{
auto result_box=m_Window->Result;
result_box->Clear();
result_box->Enabled=false;
auto task=Task::Create(this, [this, path]()
	{
	auto icon=ResourceHelper::CreateIcon(path);
	auto name=PathHelper::GetName(path->Begin());
	auto lines=StringList::Create();
	for(auto const& item: icon->Bitmaps)
		{
		auto size=item.get_key();
		auto bmp=item.get_value();
		auto var=String::Create("const char BMP_%S_%u[]=", name, size);
		lines->Append(var, EventNotification::None);
		auto buf=StaticBuffer::Create((BYTE*)bmp->Begin(), bmp->GetSize());
		Stringify(lines, buf);
		}
	auto task=Task::Get();
	task->Result=lines;
	});
task->Then(this, [this, task, result_box]()
	{
	auto lines=task->Result.As<StringList>();
	result_box->AppendLines(lines);
	result_box->Enabled=true;
	result_box->SetFocus();
	result_box->SelectAll();
	});
}

VOID Application::ScanImage(Handle<String> path)
{
auto result_box=m_Window->Result;
result_box->Clear();
result_box->Enabled=false;
auto task=Task::Create(this, [this, path]()
	{
	auto task=Task::Get();
	task->Result=OcrScanner::ScanImage(path);
	});
task->Then(this, [this, task, result_box]()
	{
	auto result=task->Result.As<OcrResult>();
	if(result)
		{
		for(auto const& line: result->Lines)
			result_box->AppendLine(line->Text);
		}
	result_box->Enabled=true;
	result_box->SetFocus();
	result_box->SelectAll();
	});
}

VOID Application::Stringify(Handle<StringList> lines, InputStream* src)
{
CHAR buf[LINE_LEN+8];
auto dst=StaticBuffer::Create(buf, LINE_LEN+8);
StreamWriter writer(dst);
SIZE_T line_len=writer.Print("\"");
BYTE byte=0;
SIZE_T read=src->Read(&byte, 1);
while(read>0)
	{
	if(line_len>=LINE_LEN)
		{
		writer.Print("\"");
		writer.PrintChar('\0');
		auto str=String::Create(buf);
		lines->Append(str, EventNotification::None);
		dst->Reset();
		line_len=writer.Print("\"");
		}
	SIZE_T written=writer.Print(STR_TABLE[byte]);
	if(!written)
		throw DeviceNotReadyException();
	line_len+=written;
	read=src->Read(&byte, 1);
	}
writer.Print("\";");
writer.PrintChar('\0');
auto str=String::Create(buf);
lines->Append(str, EventNotification::None);
lines->Append("", EventNotification::None);
}

}
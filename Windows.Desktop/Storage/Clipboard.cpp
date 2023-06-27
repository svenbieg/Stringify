//===============
// Clipboard.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#include "Desktop/AppWindow.h"
#include "Clipboard.h"

using namespace Desktop;


//===========
// Namespace
//===========

namespace Storage {


//========
// Common
//========

VOID Clipboard::Copy(Handle<String> text)
{
if(!text)
	return;
auto app_wnd=AppWindow::Current;
if(!app_wnd)
	return;
if(!OpenClipboard(app_wnd->GetHandle()))
	return;
EmptyClipboard();
UINT len=text->GetLength();
UINT size=(len+1)*sizeof(TCHAR);
HGLOBAL hglobal=GlobalAlloc(GMEM_MOVEABLE, size);
if(hglobal)
	{
	auto buf=GlobalLock(hglobal);
	CopyMemory(buf, text->Begin(), size);
	GlobalUnlock(hglobal);
	SetClipboardData(CF_UNICODETEXT, hglobal);
	}
CloseClipboard();
}

Handle<String> Clipboard::GetText()
{
auto app_wnd=AppWindow::Current;
if(!OpenClipboard(app_wnd->GetHandle()))
	return nullptr;
Handle<String> text;
HANDLE data=GetClipboardData(CF_UNICODETEXT);
if(data)
	{
	auto buf=(LPCWSTR)GlobalLock(data);
	text=new String(buf);
	GlobalUnlock(data);
	}
CloseClipboard();
return text;
}

BOOL Clipboard::HasText()
{
if(IsClipboardFormatAvailable(CF_UNICODETEXT))
	return true;
return false;
}

Handle<Clipboard> Clipboard::Open()
{
if(!hCurrent)
	hCurrent=new Clipboard();
return hCurrent;
}


//==========================
// Con-/Destructors Private
//==========================

Clipboard::Clipboard()
{}


//================
// Common Private
//================

Handle<Clipboard> Clipboard::hCurrent;

}
//===============
// Clipboard.cpp
//===============

#include "Clipboard.h"


//=======
// Using
//=======

#include "UI/AppWindow.h"

using namespace UI;


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
auto app_wnd=AppWindow::GetCurrent();
if(!app_wnd)
	return;
if(!OpenClipboard(app_wnd->GetHandle()))
	return;
EmptyClipboard();
UINT len=text->GetLength();
UINT size=(len+1)*sizeof(WCHAR);
HGLOBAL hglobal=GlobalAlloc(GMEM_MOVEABLE, size);
if(hglobal)
	{
	auto buf=(LPWSTR)GlobalLock(hglobal);
	StringHelper::Copy(buf, len+1, text->Begin());
	GlobalUnlock(hglobal);
	SetClipboardData(CF_UNICODETEXT, hglobal);
	}
CloseClipboard();
}

Handle<String> Clipboard::GetText()
{
auto app_wnd=AppWindow::GetCurrent();
if(!app_wnd)
	return nullptr;
if(!OpenClipboard(app_wnd->GetHandle()))
	return nullptr;
Handle<String> text;
HANDLE data=GetClipboardData(CF_UNICODETEXT);
if(data)
	{
	auto buf=(LPCWSTR)GlobalLock(data);
	text=String::Create(buf);
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
if(!m_Current)
	m_Current=new Clipboard();
return m_Current;
}


//==========================
// Con-/Destructors Private
//==========================

Clipboard::Clipboard()
{}


//================
// Common Private
//================

Handle<Clipboard> Clipboard::m_Current;

}
//================
// MessageBox.cpp
//================

#include "pch.h"


//=======
// Using
//=======

#include "Desktop/AppWindow.h"
#include "MessageBox.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Dialogs {


//========
// Common
//========

VOID MessageBox::Error(Handle<Sentence> title, Handle<Sentence> msg)
{
Show(msg->Begin(), title->Begin(), MB_OK|MB_ICONERROR);
}

VOID MessageBox::Info(Handle<Sentence> title, Handle<Sentence> msg)
{
Show(msg->Begin(), title->Begin(), MB_OK|MB_ICONINFORMATION);
}

BOOL MessageBox::Question(Handle<Sentence> title, Handle<Sentence> msg)
{
INT status=Show(msg->Begin(), title->Begin(), MB_YESNO|MB_ICONQUESTION);
return status==IDYES;
}

BOOL MessageBox::Warning(Handle<Sentence> title, Handle<Sentence> msg)
{
INT status=Show(msg->Begin(), title->Begin(), MB_OKCANCEL|MB_ICONWARNING);
return status==IDOK;
}


//================
// Common Private
//================

INT MessageBox::Show(LPCSTR title, LPCSTR msg, UINT type)
{
auto app_wnd=AppWindow::Current;
HWND hwnd=app_wnd? app_wnd->GetHandle(): NULL;
return MessageBoxA(hwnd, title, msg, type);
}

}}
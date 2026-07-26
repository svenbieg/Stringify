//=================
// Application.cpp
//=================

#include "Application.h"


//=======
// Using
//=======

#include "Storage/Clipboard.h"
#include "UI/Controls/Input.h"
#include "UI/Input/Shortcut.h"
#include "UI/Frame.h"

using namespace Concurrency;
using namespace Storage;
using namespace UI::Controls;
using namespace UI::Input;


//===========
// Namespace
//===========

namespace UI {


//==================
// Con-/Destructors
//==================

Application::~Application()
{
s_Current=nullptr;
}


//========
// Common
//========

BOOL Application::Shortcut(Handle<KeyEventArgs> args)
{
BOOL alt=FlagHelper::Get(args->Flags, KeyEventFlags::Alt);
BOOL ctrl=FlagHelper::Get(args->Flags, KeyEventFlags::Ctrl);
if(ctrl|alt)
	{
	BOOL shift=FlagHelper::Get(args->Flags, KeyEventFlags::Shift);
	ShortcutFlags shortcut=(ShortcutFlags)args->Key;
	FlagHelper::Set(shortcut, ShortcutFlags::Alt, alt);
	FlagHelper::Set(shortcut, ShortcutFlags::Ctrl, ctrl);
	FlagHelper::Set(shortcut, ShortcutFlags::Shift, shift);
	Interactive* control=nullptr;
	Shortcuts->TryGet((UINT)shortcut, &control);
	if(control)
		{
		control->Clicked(control, nullptr);
		args->Handled=true;
		return true;
		}
	}
return false;
}


//======
// Edit
//======

VOID Application::EditCopy()
{
auto input=Controls::Input::GetCurrent();
if(!input)
	return;
auto clipboard=Clipboard::Open();
auto text=input->GetSelection();
clipboard->Copy(text);
}

VOID Application::EditCut()
{
auto input=Controls::Input::GetCurrent();
if(!input)
	return;
auto clipboard=Clipboard::Open();
auto text=input->GetSelection();
clipboard->Copy(text);
input->ReplaceSelection(nullptr);
}

VOID Application::EditDelete()
{
auto input=Controls::Input::GetCurrent();
if(!input)
	return;
input->ReplaceSelection(nullptr);
}

VOID Application::EditPaste()
{
auto input=Controls::Input::GetCurrent();
if(!input)
	return;
auto clipboard=Clipboard::Open();
auto text=clipboard->GetText();
if(!text)
	return;
input->ReplaceSelection(text->Begin());
}

VOID Application::EditSelectAll()
{
auto input=Controls::Input::GetCurrent();
if(!input)
	return;
input->SelectAll();
}


//============================
// Con-/Destructors Protected
//============================

Application::Application(Handle<Sentence> name):
m_Name(name)
{
s_Current=this;
Shortcuts=ShortcutMap::Create();
}


//================
// Common Private
//================

Application* Application::s_Current=nullptr;

}
//=================
// Application.cpp
//=================

#include "UI/Application.h"


//=======
// Using
//=======

#include "Concurrency/DispatchedQueue.h"
#include "Storage/Clipboard.h"
#include "UI/Controls/Input.h"
#include "UI/Input/Shortcut.h"
#include "UI/Frame.h"

using namespace Concurrency;
using namespace Storage;
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

VOID Application::ExitMenu()
{
if(m_CurrentMenu)
	{
	m_CurrentMenu->Exit();
	m_CurrentMenu=nullptr;
	}
}

VOID Application::SetCurrentInput(Input* input)
{
if(m_CurrentInput==input)
	return;
if(m_CurrentInput)
	m_CurrentInput->SelectNone();
m_CurrentInput=input;
}

VOID Application::SetPointerFocus(Interactive* focus)
{
if(m_PointerFocus==focus)
	return;
if(m_PointerFocus)
	m_PointerFocus->PointerLeft(m_PointerFocus);
m_PointerFocus=focus;
if(m_PointerFocus)
	m_PointerFocus->PointerEntered(m_PointerFocus);
}

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
if(!m_CurrentInput)
	return;
auto clipboard=Clipboard::Open();
auto text=m_CurrentInput->GetSelection();
clipboard->Copy(text);
}

VOID Application::EditCut()
{
if(!m_CurrentInput)
	return;
auto clipboard=Clipboard::Open();
auto text=m_CurrentInput->GetSelection();
clipboard->Copy(text);
m_CurrentInput->ReplaceSelection(nullptr);
}

VOID Application::EditDelete()
{
if(!m_CurrentInput)
	return;
m_CurrentInput->ReplaceSelection(nullptr);
}

VOID Application::EditPaste()
{
if(!m_CurrentInput)
	return;
auto clipboard=Clipboard::Open();
auto text=clipboard->GetText();
if(!text)
	return;
m_CurrentInput->ReplaceSelection(text->Begin());
}

VOID Application::EditSelectAll()
{
if(!m_CurrentInput)
	return;
m_CurrentInput->SelectAll();
}


//============================
// Con-/Destructors Protected
//============================

Application::Application(Handle<Sentence> name):
m_CurrentInput(nullptr),
m_CurrentMenu(nullptr),
m_Name(name),
m_PointerFocus(nullptr)
{
s_Current=this;
Shortcuts=ShortcutMap::Create();
}


//================
// Common Private
//================

Application* Application::s_Current=nullptr;

}
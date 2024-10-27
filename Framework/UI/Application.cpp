//=================
// Application.cpp
//=================

#include "pch.h"


//=======
// Using
//=======

#include "Storage/Clipboard.h"
#include "UI/Controls/Input.h"
#include "UI/Input/Shortcut.h"
#include "Application.h"
#include "Frame.h"

using namespace Storage;
using namespace UI::Input;


//===========
// Namespace
//===========

namespace UI {


//========
// Common
//========

Application* Application::Current=nullptr;

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
BOOL alt=GetFlag(args->Flags, KeyEventFlags::Alt);
BOOL ctrl=GetFlag(args->Flags, KeyEventFlags::Ctrl);
if(ctrl|alt)
	{
	BOOL shift=GetFlag(args->Flags, KeyEventFlags::Shift);
	ShortcutFlags shortcut=(ShortcutFlags)args->Key;
	SetFlag(shortcut, ShortcutFlags::Alt, alt);
	SetFlag(shortcut, ShortcutFlags::Ctrl, ctrl);
	SetFlag(shortcut, ShortcutFlags::Shift, shift);
	auto control=Shortcuts->Get((UINT)shortcut);
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
BOOL has=clipboard->HasText();
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

Application::Application(LPCSTR name, LPCSTR version):
Core::Application(name, version),
m_CurrentInput(nullptr),
m_CurrentMenu(nullptr),
m_PointerFocus(nullptr)
{
Current=this;
Shortcuts=new ShortcutMap();
}

}
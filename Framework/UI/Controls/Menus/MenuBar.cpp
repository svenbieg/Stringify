//=============
// MenuBar.cpp
//=============

#include "MenuBar.h"


//=======
// Using
//=======

#include "UI/Controls/Menus/PopupMenu.h"
#include "UI/Frame.h"

using namespace Graphics;
using namespace UI::Input;


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {
		namespace Menus {


//==================
// Con-/Destructors
//==================

MenuBar::~MenuBar()
{
if(m_Frame)
	m_Frame->KeyEvent.Remove(this);
}


//========
// Common
//========

Handle<MenuBarItem> MenuBar::Add(Handle<Sentence> label)
{
return MenuBarItem::Create(this, label);
}


//==========================
// Con-/Destructors Private
//==========================

MenuBar::MenuBar(Window* parent):
WrapPanel(parent),
Menu(nullptr, this)
{
Padding.Set(2, 0, 2, 0);
m_Frame->KeyEvent.Add(this, &MenuBar::OnFrameKeyEvent);
}


//================
// Common Private
//================

VOID MenuBar::OnFrameKeyDown(Handle<KeyEventArgs> args)
{
if(args->Key==VirtualKey::Alt)
	{
	if(!FlagHelper::Get(m_MenuFlags, MenuFlags::KeyboardAccess))
		{
		FlagHelper::Set(m_MenuFlags, MenuFlags::KeyboardAccess);
		s_Current=this;
		WrapPanel::Invalidate();
		}
	args->Handled=true;
	return;
	}
}

VOID MenuBar::OnFrameKeyEvent(KeyEventType type, Handle<KeyEventArgs> args)
{
switch(type)
	{
	case KeyEventType::KeyDown:
		{
		OnFrameKeyDown(args);
		break;
		}
	case KeyEventType::KeyUp:
		{
		OnFrameKeyUp(args);
		break;
		}
	}
}

VOID MenuBar::OnFrameKeyUp(Handle<KeyEventArgs> args)
{
if(args->Key!=VirtualKey::Alt)
	return;
if(FlagHelper::Get(m_MenuFlags, MenuFlags::KeyboardNavigation))
	{
	Exit(FocusReason::Keyboard);
	}
else
	{
	if(FlagHelper::Get(m_MenuFlags, MenuFlags::KeyboardAccess))
		Select(FocusReason::Keyboard);
	}
args->Handled=true;
}

}}}